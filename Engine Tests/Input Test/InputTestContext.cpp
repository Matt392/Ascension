#include "ASCGenInc.h"
#include "InputTestContext.h"

#include "Ascension.h"
#include "ASCInputEvent.h"
#include "ASCInputDevice.h"
#include "ASCInputNames.h"


CInputTestContext::CInputTestContext(void)
: m_uExitButton ( 0 )
, m_bQuit ( false )
{
}


CInputTestContext::~CInputTestContext(void)
{
}

//Creation
bool 
CInputTestContext::Initialise()
{
	Ascension::EventSystem().AddEventListener(this, ET_INPUT);
	
	Ascension::KeyBoard().RegisterInput(m_uExitButton, "Key_Tilde");

	UINT32 uTemp;
	for(UINT32 i = 0; i < m_kiInputCount; ++i)
	{
		Ascension::Mouse().RegisterInput(uTemp, m_strDescMouse[i]);
	}
	
	for(UINT32 i = 0; i < sk_uKeyCount; ++i)
	{
		Ascension::KeyBoard().RegisterInput(uTemp, m_strDesc[i]);
	}

	return true;
}
		
//Game logic
bool 
CInputTestContext::Update()
{
	Ascension::Mouse().DisplayDebugInfo();
	Ascension::KeyBoard().DisplayDebugInfo();

	return m_bQuit == false;
}

//Optional
void 
CInputTestContext::Draw()
{
	//do nothing
}
void
CInputTestContext::Disable()
{
	//do nothing
}

//Events System
void 
CInputTestContext::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_GUI:
		{
			break;
		}
	case ET_INPUT:
		{
			if(pEvent->ID() == m_uExitButton)
			{
				ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
				if(pInputEvent->GetState() == KS_Pressed)
				{
					//m_bQuit = true;
				}
			}
		}
	default:break;
	}
}
