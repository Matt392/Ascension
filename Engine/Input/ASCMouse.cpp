#ifdef ASC_WINDOWS

#include "ASCGenInc.h"
#include "ASCMouse.h"
#include "../Ascension/Ascension.h"
#include "ASCEventManager.h"
#include "ASCInputEvent.h"
#include "ASCRenderer.h"
#include "ASCInputNames.h"


ASCMouse::ASCMouse(bool bShowMouse, bool bLockToWindow)
: m_bFullScreen ( false )
, m_bLockToWindow ( bLockToWindow )
{
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	// Create Direct Input.
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
	{
		// Assert if Direct Input failed to create.
		assert_now("DInput failed to create");
	}
	// Create Direct Input Device.
	if(FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL)))
	{
		// Assert if Direct Input Device failed to create.
		assert_now("DInput Mouse Device failed to create");
	}
	// Set Data Format on Device to Mouse.
	if(FAILED(m_pDIDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		// Assert if failed to set Data Format
		assert_now("Failed to set data format");
	}

	// Iterate through and initialise KeyData structure.
	UINT32 j = 0;
	for(UINT32 i = 0; i < m_kiRealInputCount; ++i)
	{
		m_sKeyData[i].ID = m_strDescMouse[j];
		++j;
		m_sKeyData[i].FloatVal = 0.0f;
		m_sKeyData[i].KeyState = KS_Inactive;
		m_sKeyData[i].m_fX = 0.0f;
		m_sKeyData[i].m_fY = 0.0f;
		m_bKeyObserved[i] = false;

	}

	// Check device by acquring it.
	if(FAILED(m_pDIDevice->Acquire()))
	{
		// Assert if failed to acquire the device.
		assert_now("Failed to Acquire device state");
	}

	// Get Foreground Window
	m_hWnd = reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle());
	//m_hWnd = NULL;

	// Set Cursor Position
	//SetCursorPos(100 , 100);

	// Get Cursor Position
	POINT pt;
	GetCursorPos( &pt );

	// Set real coordinates
	m_dfMousePos.m_fX = static_cast<FLOAT32>(pt.x);
	m_dfMousePos.m_fY = static_cast<FLOAT32>(pt.y);

	// Set KeyData to real coordinates.
	m_sKeyData[0].m_fX = m_dfMousePos.m_fX;
	m_sKeyData[0].m_fY = m_dfMousePos.m_fY;

	
	ShowCursor(bShowMouse);

	m_uEventIds[0] = Ascension::GetNextInputEventID();
	m_uEventIds[1] = Ascension::GetNextInputEventID();
	m_uEventIds[2] = Ascension::GetNextInputEventID();
	m_uEventIds[3] = Ascension::GetNextInputEventID();

	m_bKeyObserved[2] = true;
}


ASCMouse::~ASCMouse(void)
{
}
void 
ASCMouse::Update( FLOAT32 fTimeDelta )
{
	HWND hwnd = GetForegroundWindow();
	if(reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle()) == hwnd)
	{
		fTimeDelta;
	
		if(m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState) == DIERR_INPUTLOST)
		{
			m_pDIDevice->Acquire();
			return;
		}
		static ASCEventManager* pEventManager = &Ascension::EventSystem();
		ASCInputEvent Event;
		Event.SetEventTypeID(ET_INPUT);
		ProcessMousePos();
		if(true == m_bKeyObserved[3])
		{
			if(m_sKeyData[3].FloatVal != static_cast<FLOAT32>(m_MouseState.lZ))
			{
				m_sKeyData[3].FloatVal = static_cast<FLOAT32>(m_MouseState.lZ);
				Event.SetScrollWheel(&m_sKeyData[3].FloatVal);
				Event.SetID(m_uEventIds[3]);
				pEventManager->AddEvent(Event);
			}
		}

		for(UINT32 i = 0; i < 7; ++i)
		{
			if(m_bKeyObserved[4+i])
			{
				Event.SetID(m_uEventIds[4+i]);
				if((m_MouseState.rgbButtons[i]))
				{
					m_sKeyData[4+i].KeyState = KS_TO_NEXT_KS_DOWN[m_sKeyData[4+i].KeyState];
					Event.SetState(&m_sKeyData[4+i].KeyState);
					pEventManager->AddEvent(Event);
				}
				else
				{
					m_sKeyData[4+i].KeyState = KS_TO_NEXT_KS_UP[m_sKeyData[4+i].KeyState];
					Event.SetState(&m_sKeyData[4+i].KeyState);
					pEventManager->AddEvent(Event);
				}
			}
		}
	}
}
bool 
ASCMouse::RegisterInput( UINT32& uEventID, ASCString strID )
{
	for(UINT32 i = 0; i < m_kiInputCount; ++i)
	{
		if(m_strDescMouse[i] == strID)
		{
			if(false == m_bKeyObserved[i])
			{
				uEventID = Ascension::GetNextInputEventID();
				m_uEventIds[i] = uEventID;
			}
			else
			{
				uEventID = m_uEventIds[i];
			}
			m_bKeyObserved[i] = true;
			return true;
		}
	}
	return false;
}
void 
ASCMouse::SetFullScreen(bool bVal)
{
	m_bFullScreen = bVal;
	if(false == m_bFullScreen)
	{
		m_fXOffset = 4.0f;
		m_fYOffset = 24.0f;
	}
	else
	{
		m_fXOffset = 0.0f;
		m_fYOffset = 0.0f;
	}
}
void 
ASCMouse::ProcessMousePos()
{
	static ASCEventManager* pEventManager = &Ascension::EventSystem();
	ASCInputEvent Event;
	Event.SetEventTypeID(ET_INPUT);
	
	POINT pt;
	GetCursorPos( &pt );

	RECT m_Rect;
	GetWindowRect( m_hWnd, &m_Rect );

	FLOAT32 fPosX = pt.x - SC_FLOAT(m_Rect.left);
	FLOAT32 fPosY = pt.y - SC_FLOAT(m_Rect.top);

	static const FLOAT32 skfWidth = SC_FLOAT(Ascension::Width());
	static const FLOAT32 skfHeight = SC_FLOAT(Ascension::Height());


	if(m_bLockToWindow)
	{
		if(fPosX < 0.0f+m_fXOffset)
		{
			fPosX = 0.0f+m_fXOffset;
		}
		else if(fPosX > skfWidth+m_fXOffset)
		{
			fPosX = skfWidth+m_fXOffset;
		}
		if(fPosY < 0.0f+m_fYOffset)
		{
			fPosY = 0.0f+m_fYOffset;
		}
		else if(fPosY > skfHeight+m_fYOffset)
		{
			fPosY = skfHeight+m_fYOffset;
		}
	}

	if((m_dfMousePos.m_fX != fPosX-m_fXOffset) || (m_dfMousePos.m_fY != fPosY-m_fYOffset))
	{
		m_dfMousePos.m_fX = fPosX-m_fXOffset;
		m_dfMousePos.m_fY = fPosY-m_fYOffset;
		Event.SetID(m_uEventIds[2]);
		Event.SetPos(&m_dfMousePos);
		pEventManager->AddEvent(Event);
	}
}
void 
ASCMouse::DisplayDebugInfo()
{
	Ascension::Debugging().AddScreenLine("## Mouse Info ##");
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Mouse X: ", m_dfMousePos.m_fX);
	Ascension::Debugging().AddScreenLine("Mouse Y: ", m_dfMousePos.m_fY);
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Mouse Scroll Wheel: ", m_sKeyData[3].FloatVal);
	Ascension::Debugging().AddScreenLine("");

	
	for(UINT32 i = 4; i < m_kiInputCount; ++i)
	{
		if(m_bKeyObserved[i])
		{
			if(m_sKeyData[i].KeyState == KS_Pressed)
			{
				Ascension::Debugging().AddScreenLine(m_strDescMouse[i], ASCString(": "), "Pressed");
			}
			else  if(m_sKeyData[i].KeyState == KS_DoublePressed)
			{
				Ascension::Debugging().AddScreenLine(m_strDescMouse[i], ASCString(": "), "Double Pressed");
			}														
			else  if(m_sKeyData[i].KeyState == KS_Held)				
			{														
				Ascension::Debugging().AddScreenLine(m_strDescMouse[i], ASCString(": "), "Held");
			}															
			else  if(m_sKeyData[i].KeyState == KS_Released)				
			{															
				Ascension::Debugging().AddScreenLine(m_strDescMouse[i], ASCString(": "), "Released");
			}
		}
	}
}

#endif