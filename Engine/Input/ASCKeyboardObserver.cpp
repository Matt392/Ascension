#include "ASCGenInc.h"
#include "ASCKeyboardObserver.h"
#include "../Ascension/Ascension.h"
#include "ASCInputDevice.h"
#include "ASCInputEvent.h"
#include "ASCKeyboardEvent.h"
#include "ASCInputNames.h"

#ifdef ASC_WINDOWS
#include <Windows.h>
#endif

ASCKeyboardObserver::ASCKeyboardObserver(void)
: m_bCaps ( false )
, m_bLShift ( false )
, m_bRShift ( false )
{
}


ASCKeyboardObserver::~ASCKeyboardObserver(void)
{
}
bool 
ASCKeyboardObserver::Init()
{
	Ascension::EventSystem().AddEventListener(this, ET_INPUT);

#ifdef ASC_WINDOWS
	for(UINT32 i = 0; i < sk_uKeyCount; ++i)
	{
		if(m_uKeyValueIndex[i][EKCS_LOWER] != 0 && m_uKeyValueIndex[i][EKCS_UPPER] != 0 && m_uKeyValueIndex[i][EKCS_SHIFT_LOWER] != 0 && m_uKeyValueIndex[i][EKCS_SHIFT_UPPER] != 0)
		{
			Ascension::KeyBoard().RegisterInput(m_uKeys[i], m_strDesc[i]);
		}
		else
		{
			m_uKeys[i] = 0;
		}
	}
	

	m_bCaps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

	Ascension::KeyBoard().RegisterInput(m_uCaps, "Key_Caps");
	Ascension::KeyBoard().RegisterInput(m_uRShift, "Key_R_Shift");
	Ascension::KeyBoard().RegisterInput(m_uLShift, "Key_L_Shift");
#endif
	return false;
}

void 
ASCKeyboardObserver::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_INPUT:
		{
			ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
			if(pInputEvent->GetState() == KS_Pressed)
			{
				for(UINT32 i = 0; i < sk_uKeyCount; ++i)
				{
					if(pInputEvent->ID() == m_uKeys[i])
					{
						ASCKeyboardEvent Event;
						Event.SetEventTypeID(ET_KEYBOARD);
						if(m_bCaps)
						{
							if(m_bLShift || m_bRShift)
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_SHIFT_UPPER]]);
							}
							else
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_UPPER]]);
							}
						}
						else
						{
							if(m_bLShift || m_bRShift)
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_SHIFT_LOWER]]);
							}
							else
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_LOWER]]);
							}
						}
						Ascension::EventSystem().AddEvent(Event);
						break;
					}
				}
				if(pInputEvent->ID() == m_uLShift)
				{
					m_bLShift = true;
				}
				if(pInputEvent->ID() == m_uRShift)
				{
					m_bRShift = true;
				}
				if(pInputEvent->ID() == m_uCaps)
				{
					m_bCaps = !m_bCaps;
				}
			}
			else if(pInputEvent->GetState() == KS_DoublePressed)
			{
				for(UINT32 i = 0; i < sk_uKeyCount; ++i)
				{
					if(pInputEvent->ID() == m_uKeys[i])
					{
						ASCKeyboardEvent Event;
						Event.SetEventTypeID(ET_KEYBOARD);
						if(m_bCaps)
						{
							if(m_bLShift || m_bRShift)
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_SHIFT_UPPER]]);
							}
							else
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_UPPER]]);
							}
						}
						else
						{
							if(m_bLShift || m_bRShift)
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_SHIFT_LOWER]]);
							}
							else
							{
								Event.SetCharVal(&m_strKeyValues[m_uKeyValueIndex[i][EKCS_LOWER]]);
							}
						}
						Ascension::EventSystem().AddEvent(Event);
						break;
					}
				}
			}
			else if(pInputEvent->GetState() == KS_Released)
			{
				if(pInputEvent->ID() == m_uLShift)
				{
					m_bLShift = false;
				}
				if(pInputEvent->ID() == m_uRShift)
				{
					m_bRShift = false;
				}
			}
			break;
		}
	default:break;
	}
}
