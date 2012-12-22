#include "ASCGenInc.h"
#include "ASCGUIManager.h"
#include "ASCGUIButton.h"
#include "ASCGUIStaticImage.h"
#include "ASCGUIInvisButton.h"
#include "ASCInputDevice.h"
#include "ASCEventManager.h"
#include "ASCInputEvent.h"
#include "ASCGUIDebugElement.h"
#include "../Ascension/Ascension.h"


ASCGUIManager::ASCGUIManager(void)
: m_uEventCount ( 0 )
#ifdef ASC_WINDOWS
, m_bClicked ( false )
, m_bHeld ( false )
, m_fMouseX ( 0.0f )
, m_fMouseY ( 0.0f )
, m_fScaleX ( 0.0f )
, m_fScaleY ( 0.0f )
#endif
{
}


ASCGUIManager::~ASCGUIManager(void)
{
	for(UINT32 i = 0; i < m_pGUIElements.size(); ++i)
	{
		SafeDelete( m_pGUIElements[i] );
	}
	m_pGUIDebugElements.Clear();
}

void 
ASCGUIManager::Init(FLOAT32 fScaleX, FLOAT32 fScaleY)
{
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
#ifdef ASC_IOS
    for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
    {
        Ascension::Touch().RegisterInput(m_uTouch[i], m_strDescTouch[i]);
        
        m_bUse[i] = false;
        m_bClicked[i] = false;
        m_bHeld[i] = false;
		m_fMouseX[i] = 0.0f;
		m_fMouseY[i] = 0.0f;
    }
#else
	Ascension::Mouse().RegisterInput(m_uMouseLeftClick, "MouseLButton");
	Ascension::Mouse().RegisterInput(m_uMousePos, "MouseXY");
#endif
	
	Ascension::EventSystem().AddEventListener(this, ET_INPUT);
}
FLOAT32 
ASCGUIManager::ScaleX()
{
	return m_fScaleX;
}
FLOAT32 
ASCGUIManager::ScaleY()
{
	return m_fScaleY;
}

ASCGUIButton* 
ASCGUIManager::AddGUIButton(ASCString strNorm, ASCString strOver, ASCString strDown, FLOAT32 fPosX, FLOAT32 fPosY, UINT32& uGUIEventID)
{
	ASCGUIButton* pNewButton = new ASCGUIButton();
	if(pNewButton->Init(strNorm, strOver, strDown, fPosX, fPosY, m_uEventCount))
	{
		uGUIEventID = m_uEventCount;
		++m_uEventCount;
		m_pGUIElements.push_back(pNewButton);
		return pNewButton;
	}
	SafeDelete( pNewButton );
	return NULL;
}
ASCGUIButton* 
ASCGUIManager::AddGUIButton(ASCTexture* pTexture, UINT32& uGUIEventID, ASCString strTechnique)
{
	ASCGUIButton* pNewButton = new ASCGUIButton();
	if(pNewButton->Init(pTexture, m_uEventCount, strTechnique))
	{
		uGUIEventID = m_uEventCount;
		++m_uEventCount;
		m_pGUIElements.push_back(pNewButton);
		return pNewButton;
	}
	SafeDelete( pNewButton );
	return NULL;
}
ASCGUIStaticImage* 
ASCGUIManager::AddGUIStaticImage(ASCString strImage, FLOAT32 fPosX, FLOAT32 fPosY, ASCString strTechnique)
{
	ASCGUIStaticImage* pNewImage = new ASCGUIStaticImage();
	if(pNewImage->Init(strImage, fPosX, fPosY, strTechnique))
	{
		m_pGUIElements.push_back(pNewImage);
		return pNewImage;
	}
	SafeDelete( pNewImage );
	return NULL;
}
ASCGUIStaticImage* 
ASCGUIManager::AddGUIStaticImage(ASCTexture* pTexture, FLOAT32 fPosX, FLOAT32 fPosY, ASCString strTechnique)
{
	ASCGUIStaticImage* pNewImage = new ASCGUIStaticImage();
	if(pNewImage->Init(pTexture, fPosX, fPosY, strTechnique))
	{
		m_pGUIElements.push_back(pNewImage);
		return pNewImage;
	}
	SafeDelete( pNewImage );
	return NULL;
}
ASCGUIInvisButton* 
ASCGUIManager::AddGUIInvisButton(FLOAT32 fPosX, FLOAT32 fPosY, FLOAT32 fWidth, FLOAT32 fHeight, UINT32& uGUIEventID)
{
	ASCGUIInvisButton* pNewButton = new ASCGUIInvisButton();
	if(pNewButton->Init(fPosX, fPosY, fWidth, fHeight, m_uEventCount))
	{
		uGUIEventID = m_uEventCount;
		++m_uEventCount;
		m_pGUIElements.push_back(pNewButton);
		return pNewButton;
	}
	SafeDelete( pNewButton );
	return NULL;
}

void 
ASCGUIManager::Update(FLOAT32 fDelta)
{
#ifdef ASC_IOS
    bool bTest = false;
	for(UINT32 i = 0; i < m_pGUIElements.size(); ++i)
	{
        bTest = false;
        for(UINT32 j = 0; j < ASC_MAX_TOUCH_INPUT; ++j)
        {
            if(m_bUse[j])
            {
                m_pGUIElements[i]->Update(m_fMouseX[j], m_fMouseY[j], m_bClicked[j], m_bHeld[j]);
                bTest = true;
            }
        }
        if(false == bTest)
        {
            m_pGUIElements[i]->Update(-100000.0f, -100000.0f, false, false);
        }
	}
    //DebugInfo();
#else
	for(UINT32 i = 0; i < m_pGUIElements.size(); ++i)
	{
		m_pGUIElements[i]->Update(m_fMouseX, m_fMouseY, m_bClicked, m_bHeld);
	}
#endif
}
void 
ASCGUIManager::DebugInfo()
{
    Ascension::Debugging().AddScreenLine("## GUI Info ##");
#ifdef ASC_IOS
    ASCString strTemp;
    for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
    {
        if(m_bUse[i])
        {
            strTemp = "";
            strTemp.AddSmallInt(i+1);
            strTemp += ASCString(") Pos X: ");
            Ascension::Debugging().AddScreenLine(strTemp, m_fMouseX[i]);
            
            strTemp = "";
            strTemp.AddSmallInt(i+1);
            strTemp += ASCString(") Pos Y: ");
            Ascension::Debugging().AddScreenLine(strTemp, m_fMouseY[i]);
            
            strTemp = "";
            strTemp.AddSmallInt(i+1);
            strTemp += ASCString(") Held: ");
            if(m_bHeld[i])
            {
                strTemp += ASCString("true");
            }
            else
            {
                strTemp += ASCString("false");
            }
            Ascension::Debugging().AddScreenLine(strTemp);
            
            strTemp = "";
            strTemp.AddSmallInt(i+1);
            strTemp += ASCString(") Clicked: ");;
            if(m_bClicked[i])
            {
                strTemp += ASCString("true");
            }
            else
            {
                strTemp += ASCString("false");
            }
            Ascension::Debugging().AddScreenLine(strTemp);
        }
    }
#endif
}
void 
ASCGUIManager::OnEvent(ASCIEvent* pEvent)
{
	if(pEvent->TypeID() == ET_INPUT)
	{
#ifdef ASC_IOS
        for(UINT32 i = 0; i < ASC_MAX_TOUCH_INPUT; ++i)
        {
            if(pEvent->ID() == m_uTouch[i])
            {
                ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
                SInputData sInputData = pInputEvent->GetData();
                EKeyState eState = sInputData.KeyState;
                if(eState != KS_Inactive)
                {
                    if(eState == KS_Released)
                    {
                        m_bClicked[i] = true;
                    }
                    else
                    {
                        m_bClicked[i] = false;
                    }
                    if(eState == KS_Held || eState == KS_Pressed)
                    {
                        m_bHeld[i] = true;
                    }
                    else
                    {
                        m_bHeld[i] = false;
                    }
                    m_bUse[i] = true;
                
                    m_fMouseX[i] = sInputData.m_fX;
                    m_fMouseY[i] = sInputData.m_fY;
                }
                else
                {
                    m_bUse[i] = false;
                }
            }
        }
#else
		if(pEvent->ID() == m_uMouseLeftClick)
		{
	m_bClicked = false;
	m_bHeld = false;
			ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
			if(pInputEvent->GetState() == KS_Pressed)
			{
				m_bClicked = true;
			}
			else if(pInputEvent->GetState() == KS_Held)
			{
				m_bHeld = true;
			}
		}
		if(pEvent->ID() == m_uMousePos)
		{
			ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
			m_fMouseX = pInputEvent->GetPos().m_fX;
			m_fMouseY = pInputEvent->GetPos().m_fY;
		}
#endif
	}
}
void 
ASCGUIManager::AddDebugElement(ASCGUIDebugElement* pElement)
{
	for(UINT32 i = 0; i < m_pGUIDebugElements.size(); ++i)
	{
		if(NULL == m_pGUIDebugElements[i])
		{
			m_pGUIDebugElements[i] = pElement;
			return;
		}
	}
	m_pGUIDebugElements.push_back( pElement );
}
void 
ASCGUIManager::RemoveDebugElement(ASCGUIDebugElement* pElement)
{
	for(UINT32 i = 0; i < m_pGUIDebugElements.size(); ++i)
	{
		if(pElement == m_pGUIDebugElements[i])
		{
			m_pGUIDebugElements[i] = NULL;
		}
	}
}
void 
ASCGUIManager::DebugBounds()
{
	for(UINT32 i = 0; i < m_pGUIDebugElements.size(); ++i)
	{
		if(m_pGUIDebugElements[i])
		{
			m_pGUIDebugElements[i]->DrawBounds();
		}
	}
}