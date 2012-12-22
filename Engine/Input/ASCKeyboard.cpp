#ifdef ASC_WINDOWS

#include "ASCGenInc.h"
#include "ASCKeyboard.h"
#include "../Ascension/Ascension.h"
#include "ASCRenderer.h"
#include "ASCEventManager.h"
#include "ASCInputEvent.h"
#include "ASCInputNames.h"

#define KEYDOWN(name, key) (name[key] & 0x80)

ASCKeyboard::ASCKeyboard(void)
{
	// Create Direct Input.
	if(FAILED(DirectInput8Create(reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL)), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
	{
		// Assert if Direct Input failed to create.
		assert_now("DInput failed to create");
	}
	// Create Direct Input Device.
	if(FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL)))
	{
		// Assert if Direct Input Device failed to create.
		assert_now("DInput Device failed to create");
	}
	// Set Data Format on Device to Keyboard.
	if(FAILED(m_pDIDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		// Assert if failed to set Data Format
		assert_now("Failed to set data format");
	}

	// Create Lookup Tables and Input Descriptions
	CreateLookupTable();

	// Iterate through and initialise KeyData structure.
	for(UINT32 i = 0; i < sk_uKeyCount; ++i)
	{
		m_bKeyObserved[i] = false;
		m_sKeyData[i].ID = m_strDesc[i];
		m_sKeyData[i].FloatVal = 0.0f;
		m_sKeyData[i].KeyState = KS_Inactive;

	}
	// Check device by acquring it.
	if(FAILED(m_pDIDevice->Acquire()))
	{
		// Assert if failed to acquire the device.
		assert_now("Failed to Acquire device state");
	}
}


ASCKeyboard::~ASCKeyboard(void)
{
}
void 
ASCKeyboard::Update( FLOAT32 fTimeDelta )
{
	HWND hwnd = GetForegroundWindow();
	if(reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle()) == hwnd)
	{
		fTimeDelta;
		// Gets the current state of all keys on the keyboard.
		if (FAILED(m_pDIDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer))) 
		{ 
			// If failed to get the current key states, try to acquire the keyboard device.
			if FAILED(m_pDIDevice->Acquire())
			{
				// Unable to acquire the keyboard device. Assert.
				assert_now("Failed to acquire device state on update");
			}
			// Unable to process rest of function. Return.
			return; 
		}

		// Iterate through all keys, and update the stored states to the new states.
		static ASCEventManager* pEventManager = &Ascension::EventSystem();
		ASCInputEvent Event;
		Event.SetEventTypeID(ET_INPUT);
		for(UINT32 i = 0; i < sk_uKeyCount; ++i)
		{
			if(true == m_bKeyObserved[i])
			{
				Event.SetID(m_uEventIds[i]);
				if(m_bProcessDoublePress[i])
				{
					m_fTimeSinceLastPress[i] += fTimeDelta;
					if(m_fTimeSinceLastPress[i] >= sk_fDoublePressGap)
					{
						m_bProcessDoublePress[i] = false;
						m_fTimeSinceLastPress[i] = 0.0f;
					}
				}
				if (KEYDOWN(buffer, (sk_uKeyCodes[i])))
				{
					m_fTimeOnState[i] += fTimeDelta;
					if(m_bProcessDoublePress[i] && m_sKeyData[i].KeyState == KS_Inactive)
					{
						m_sKeyData[i].KeyState = KS_DoublePressed;
						Event.SetState(&m_sKeyData[i].KeyState);
						pEventManager->AddEvent(Event);
					}
					else if(m_fTimeOnState[i] >= sk_mTimeBetweenStates[m_sKeyData[i].KeyState])
					{
						m_fTimeOnState[i] = 0.0f;
						m_sKeyData[i].KeyState = KS_TO_NEXT_KS_DOWN[m_sKeyData[i].KeyState];
					
						Event.SetState(&m_sKeyData[i].KeyState);
						pEventManager->AddEvent(Event);
					}
					else if(m_sKeyData[i].KeyState == KS_Held)
					{
						Event.SetState(&m_sKeyData[i].KeyState);
						pEventManager->AddEvent(Event);
					}
				}
				else
				{
					if(m_sKeyData[i].KeyState == KS_Pressed)
					{
						m_bProcessDoublePress[i] = true;
					}
					m_sKeyData[i].KeyState = KS_TO_NEXT_KS_UP[m_sKeyData[i].KeyState];
					Event.SetState(&m_sKeyData[i].KeyState);
					pEventManager->AddEvent(Event);
				}
			}
		}
	}
}
bool 
ASCKeyboard::RegisterInput( UINT32& uEventID, ASCString strID )
{
	for(UINT32 i = 0; i < sk_uKeyCount; ++i)
	{
		if(m_strDesc[i] == strID)
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
	assert_now("Guts, Invalid keyboard code");
	return false;
}


// PROTECTED FUNCTIONS.

/**
 *	Create Lookup Table - Sets up the lookup table for keys to be processed.
 *	@author		Matt Dalzell
 */
void 
ASCKeyboard::CreateLookupTable()
{
	// Static bool to ensure that this function is only run once.
	static bool s_bCreated = false;

	if(false == s_bCreated)
	{
		// Set static bool to true.
		s_bCreated = true;

		// Iterate through and set all keys to false.
		for(UINT32 i = 0; i < sk_uKeyCount; ++i)
		{
			m_fTimeOnState[i] = 0.0f;
			m_fTimeSinceLastPress[i] = 0.0f;
			m_bProcessDoublePress[i] = false;
		}
	}
}
void 
ASCKeyboard::SetFullScreen(bool bVal)
{
	bVal;
}
void 
ASCKeyboard::DisplayDebugInfo()
{
	Ascension::Debugging().AddScreenLine("## Keyboard Info ##");
	Ascension::Debugging().AddScreenLine("");

	for(UINT32 i = 0; i < sk_uKeyCount; ++i)
	{
		if(m_bKeyObserved[i])
		{
			if(m_sKeyData[i].KeyState == KS_Pressed)
			{
				Ascension::Debugging().AddScreenLine(m_strDesc[i], ASCString(": "), "Pressed");
			}
			else  if(m_sKeyData[i].KeyState == KS_DoublePressed)
			{
				Ascension::Debugging().AddScreenLine(m_strDesc[i], ASCString(": "), "Double Pressed");
			}														
			else  if(m_sKeyData[i].KeyState == KS_Held)				
			{														
				Ascension::Debugging().AddScreenLine(m_strDesc[i], ASCString(": "), "Held");
			}															
			else  if(m_sKeyData[i].KeyState == KS_Released)				
			{															
				Ascension::Debugging().AddScreenLine(m_strDesc[i], ASCString(": "), "Released");
			}
		}
	}
}

#endif