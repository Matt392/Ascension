#ifdef ASC_IOS

#include "ASCGenInc.h"
#include "ASCTouchInput.h"

#include "Ascension.h"
#include "ASCEventManager.h"
#include "ASCInputEvent.h"


ASCTouchInput::ASCTouchInput(void)
{
	for(UINT32 i = 0; i < m_kuTouchInputs; ++i)
	{
		m_bObserved[i] = false;
	
        m_sInputDesc[i].m_eType = IST_Button;

		m_sInputDesc[i].m_strName = m_strDescTouch[i];
		m_sInputDesc[i].m_strDescription = m_strDescTouch[i];
		m_sInputDesc[i].m_fMin = 0.0f;
		m_sInputDesc[i].m_fMax = 0.0f;
        
        
		m_sTouchData[i].ID = m_strDescTouch[i];
		m_sTouchData[i].FloatVal = 0.0f;
		m_sTouchData[i].KeyState = KS_Inactive;
		m_sTouchData[i].m_fX = 0.0f;
		m_sTouchData[i].m_fY = 0.0f;
        
        if(i < ASC_MAX_TOUCH_INPUT)
        {
            m_pTouches[i] = &m_sTouchData[i];
        }
	}
}


ASCTouchInput::~ASCTouchInput(void)
{
}


void 
ASCTouchInput::Update( FLOAT32 fTimeDelta )
{
    for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
    {
        if(m_pTouches[i]->KeyState == KS_Released)
        {
            m_pTouches[i]->KeyState = KS_Inactive;
        }
        else if(m_pTouches[i]->KeyState == KS_Pressed)
        {
            m_pTouches[i]->KeyState = KS_Held;
        }
    }
    
    for (UINT32 j = 0; j < m_vecTouchInfo.size(); ++j) 
    {
        bool bTest = false;
        ASCTouchInfo& rTouch = m_vecTouchInfo[j];
        if(rTouch.State() == ETS_STARTED)
        {
            for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
            {
                if(m_pTouches[i]->KeyState == KS_Inactive)
                {
                    m_pTouches[i]->KeyState = KS_Pressed;
                    m_pTouches[i]->m_fX = rTouch.Position().X();
                    m_pTouches[i]->m_fY = rTouch.Position().Y();
                    m_pTouchIDs[i] = rTouch.ID();
                    bTest = true;
                    break;
                }
            }
            if(false == bTest)
            {
                assert_now("Guts, Failed to find avaliable touch slot");
            }
        }
        else if(rTouch.State() == ETS_MOVED)
        {
            for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
            {
                if(m_pTouches[i]->KeyState == KS_Held || m_pTouches[i]->KeyState == KS_Pressed)
                {
                    //if(m_pTouches[i]->m_fX == rTouch.PreviousPosition().X() && m_pTouches[i]->m_fY == rTouch.PreviousPosition().Y())
                    if(m_pTouchIDs[i] == rTouch.ID())
                    {
                        m_pTouches[i]->KeyState = KS_Held;
                        m_pTouches[i]->m_fX = rTouch.Position().X();
                        m_pTouches[i]->m_fY = rTouch.Position().Y();
                        bTest = true;
                        break;
                    }
                }
            }
            if(false == bTest)
            {
                assert_now("Guts, Failed to find its touch slot");
            }
        }
        else if(rTouch.State() == ETS_ENDED)
        {
            for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
            {
                if(m_pTouches[i]->KeyState == KS_Held || m_pTouches[i]->KeyState == KS_Pressed)
                {
                    if(m_pTouchIDs[i] == rTouch.ID())
                    {
                        m_pTouches[i]->KeyState = KS_Released;
                        m_pTouches[i]->m_fX = rTouch.Position().X();
                        m_pTouches[i]->m_fY = rTouch.Position().Y();
                        bTest = true;
                        break;
                    }
                }
            }
            if(false == bTest)
            {
                assert_now("Guts, Failed to find its touch slot");
            }
        }
        else
        {
            assert_now("Guts, Unkow touch type");
        }
    }
    m_vecTouchInfo.Clear();
    
    static ASCEventManager* pEventManager = &Ascension::EventSystem();
	ASCInputEvent Event;
	Event.SetEventTypeID(ET_INPUT);
    
    for(UINT32 i = 0; i < m_kuTouchInputs; ++i)
    {
        Event.SetID(m_uEventIds[i]);
        Event.SetData(&m_sTouchData[i]);
        
        pEventManager->AddEvent(Event);
    }
    
    fTimeDelta;
}

bool 
ASCTouchInput::RegisterInput( UINT32& uEventID, ASCString strID )
{
	for(UINT32 i = 0; i < m_kiInputCount; ++i)
	{
		if(m_sInputDesc[i].m_strName == strID)
		{
			if(false == m_bObserved[i])
			{
				uEventID = Ascension::GetNextInputEventID();
				m_uEventIds[i] = uEventID;
			}
			else
			{
				uEventID = m_uEventIds[i];
			}
			m_bObserved[i] = true;
			return true;
		}
	}
	return false;
}

void 
ASCTouchInput::SetFullScreen(bool bVal)
{
    bVal;
}

void 
ASCTouchInput::AddTouchInfo(ASCTouchInfo sInfo)
{
    m_vecTouchInfo.push_back( sInfo );
}

//ASCTouchInfo

ASCTouchInfo::ASCTouchInfo(void)
: m_eState ( ETS_COUNT )
{
    m_v2Pos.Set(0.0f, 0.0f);
    m_v2PrevPos.Set(0.0f, 0.0f);
}
ASCTouchInfo::~ASCTouchInfo(void)
{
    
}
ETouch_State 
ASCTouchInfo::State()
{
    return m_eState;
}
void  
ASCTouchInfo::State(ETouch_State eState)
{
    m_eState = eState;
}

ASCVector2&  
ASCTouchInfo::Position()
{
    return m_v2Pos;
}
void  
ASCTouchInfo::Position(ASCVector2 v2Pos)
{
    m_v2Pos = v2Pos;
}
ASCVector2& 
ASCTouchInfo::PreviousPosition()
{
    return m_v2PrevPos;
}
void 
ASCTouchInfo::PreviousPosition(ASCVector2 v2Pos)
{
    m_v2PrevPos = v2Pos;
}

void* 
ASCTouchInfo::ID()
{
    return m_pID;
}
void 
ASCTouchInfo::ID(void* pID)
{
    m_pID = pID;
}

#endif