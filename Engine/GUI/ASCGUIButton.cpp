#include "ASCGenInc.h"
#include "ASCGUIButton.h"

#ifdef ASC_NEW_GUI_SYSTEM
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCSpriteManager.h"
#include "ASCSprite.h"
#include "ASCEventManager.h"
#include "ASCINIFile.h"
#include "ASCGUIManager.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "../Ascension/Ascension.h"
#endif

ASCGUIButton::ASCGUIButton(void)
: m_uStageIndex ( 0 )
{
}


ASCGUIButton::~ASCGUIButton(void)
{
}
bool 
ASCGUIButton::Init(ASCString strNorm, ASCString strOver, ASCString strDown, FLOAT32 fPosX, FLOAT32 fPosY, UINT32 uGUIEventID)
{
	ASCTextureAtlas* pTextureAtlas1 = NULL;
	ASCTextureAtlas* pTextureAtlas2 = NULL;
	ASCTextureAtlas* pTextureAtlas3 = NULL;

	ASCString strFirstTexture = strNorm;
	if(strNorm.find('.') >= 0)
	{
		ASCINIFile INIFile;
		if(INIFile.Load(strNorm))
		{
			strFirstTexture = INIFile.GetString( "General", "Texture");
		}
	}

	if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strFirstTexture, pTextureAtlas1))
	{
		if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strOver, pTextureAtlas2))
		{
			if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strDown, pTextureAtlas3))
			{
				if(pTextureAtlas1 == pTextureAtlas2 && pTextureAtlas2 == pTextureAtlas3)
				{
					m_sTexureInfo[0] = pTextureAtlas1->GetAtlasUVs(strFirstTexture);
					m_sTexureInfo[1] = pTextureAtlas2->GetAtlasUVs(strOver);
					m_sTexureInfo[2] = pTextureAtlas3->GetAtlasUVs(strDown);
					
					if(strNorm.find('.') >= 0)
					{
						m_pSprite = Ascension::SpriteManager().CreateSpriteFile(strNorm, "GUI");
					}
					else
					{
						m_pSprite = Ascension::SpriteManager().CreateSprite(strNorm, "GUI");
					}
					m_pSprite->SetAgainstScreen(true);

					m_fPosX = fPosX * Ascension::GUI().ScaleX();
					m_fPosY = fPosY * Ascension::GUI().ScaleY();
					m_pSprite->SetPosition(m_fPosX, m_fPosY);

					m_pSprite->SetSize(m_pSprite->GetWidth() * Ascension::GUI().ScaleX(),  m_pSprite->GetHeight() * Ascension::GUI().ScaleX());

					m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
					m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;


					m_PressedEvent.SetID(uGUIEventID);
					m_PressedEvent.SetEventTypeID(ET_GUI);
					
					m_bNoChange = false;

					m_pSprite->SetGUISprite( true );
					return true;
				}
			}
		}
	}
	if(strNorm == strOver && strOver == strDown)
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(strNorm, "GUI");
		if(m_pSprite)
		{
			m_pSprite->SetAgainstScreen(true);
			m_pSprite->SetPosition(fPosX, fPosY);

			m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
			m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;

			m_fPosX = fPosX;
			m_fPosY = fPosY;

			m_PressedEvent.SetID(uGUIEventID);
			m_PressedEvent.SetEventTypeID(ET_GUI);
			m_bNoChange = true;
			
			m_pSprite->SetGUISprite( true );
			return true;
		}
	}
	assert_now("All textures not in same atlas");
	return false;
}
bool 
ASCGUIButton::Init(ASCTexture* pTexture, UINT32 uGUIEventID, ASCString& strTechnique)
{
	if(pTexture)
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(pTexture, strTechnique);
		if(m_pSprite)
		{
			m_pSprite->SetAgainstScreen(true);
			m_pSprite->SetPosition(0.0f, 0.0f);

			m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
			m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;

			m_fPosX = 0.0f;
			m_fPosY = 0.0f;

			m_PressedEvent.SetID(uGUIEventID);
			m_PressedEvent.SetEventTypeID(ET_GUI);
			m_bNoChange = true;
			
			m_pSprite->SetGUISprite( true );
			return true;
		}
	}
	assert_now("All textures not in same atlas");
	return false;
}
void 
ASCGUIButton::SetSize(FLOAT32 fX, FLOAT32 fY)
{
	m_pSprite->SetSize(fX, fY);
	m_fHalfWidth = fX * 0.5f;
	m_fHalfHeight = fY * 0.5f;
}
void 
ASCGUIButton::Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld)
{
	if(m_bActive)
	{
		if(WithinBounds(fPosX, fPosY, m_fPosX, m_fPosY, m_fHalfWidth, m_fHalfHeight, m_fRotation))
		{
			if(bClick)
			{
				Ascension::EventSystem().AddEvent(m_PressedEvent);
			}
			if(bClick || bHeld)
			{
				if(m_uStageIndex != 2)
				{
					m_uStageIndex = 2;
					SetUVs();
				}
			}
			else
			{
				if(m_uStageIndex != 1)
				{
					m_uStageIndex = 1;
					SetUVs();
				}
			}
		}
		else
		{
			if(m_uStageIndex != 0)
			{
				m_uStageIndex = 0;
				SetUVs();
			}
		}
	}
}

void 
ASCGUIButton::SetUVs()
{
	if(false == m_bNoChange)
	{
		m_pSprite->SetUVs(m_sTexureInfo[m_uStageIndex]);
	}
}
void 
ASCGUIButton::SetPos(FLOAT32 fX, FLOAT32 fY)
{
	ASCGUIElement::SetPos(fX, fY);
	ASCGUIVisualElement::SetPos(fX, fY);
}
void 
ASCGUIButton::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	ASCGUIElement::IncPosition(fX, fY);
	ASCGUIVisualElement::IncPosition(fX, fY);
}
void 
ASCGUIButton::Disable()
{
	ASCGUIElement::Disable();
	ASCGUIVisualElement::Disable();
}
void 
ASCGUIButton::Enable()
{
	ASCGUIElement::Enable();
	ASCGUIVisualElement::Enable();
}

#else
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCSpriteManager.h"
#include "ASCSprite.h"
#include "ASCEventManager.h"
#include "ASCINIFile.h"
#include "ASCGUIManager.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "../Ascension/Ascension.h"
#endif

ASCGUIButton::ASCGUIButton(void)
: m_fDetectScaleX ( 1.0f )
, m_fDetectScaleY ( 1.0f )
, m_fDetectOffsetX ( 0.0f )
, m_fDetectOffsetY ( 0.0f )
{
}


ASCGUIButton::~ASCGUIButton(void)
{
}
bool 
ASCGUIButton::Init(ASCString strNorm, ASCString strOver, ASCString strDown, FLOAT32 fPosX, FLOAT32 fPosY, UINT32 uGUIEventID)
{
	ASCTextureAtlas* pTextureAtlas1 = NULL;
	ASCTextureAtlas* pTextureAtlas2 = NULL;
	ASCTextureAtlas* pTextureAtlas3 = NULL;

	ASCString strFirstTexture = strNorm;
	if(strNorm.find('.') >= 0)
	{
		ASCINIFile INIFile;
		if(INIFile.Load(strNorm))
		{
			strFirstTexture = INIFile.GetString( "General", "Texture");
		}
	}

	if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strFirstTexture, pTextureAtlas1))
	{
		if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strOver, pTextureAtlas2))
		{
			if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strDown, pTextureAtlas3))
			{
				if(pTextureAtlas1 == pTextureAtlas2 && pTextureAtlas2 == pTextureAtlas3)
				{
					m_sTexureInfo[0] = pTextureAtlas1->GetAtlasUVs(strFirstTexture);
					m_sTexureInfo[1] = pTextureAtlas2->GetAtlasUVs(strOver);
					m_sTexureInfo[2] = pTextureAtlas3->GetAtlasUVs(strDown);
					
					if(strNorm.find('.') >= 0)
					{
						m_pSprite = Ascension::SpriteManager().CreateSpriteFile(strNorm, "GUI");
					}
					else
					{
						m_pSprite = Ascension::SpriteManager().CreateSprite(strNorm, "GUI");
					}
					m_pSprite->SetAgainstScreen(true);

					m_fPosX = fPosX * Ascension::GUI().ScaleX();
					m_fPosY = fPosY * Ascension::GUI().ScaleY();
					m_pSprite->SetPosition(m_fPosX, m_fPosY);

					m_pSprite->SetSize(m_pSprite->GetWidth() * Ascension::GUI().ScaleX(),  m_pSprite->GetHeight() * Ascension::GUI().ScaleX());

					m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
					m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;


					m_PressedEvent.SetID(uGUIEventID);
					m_PressedEvent.SetEventTypeID(ET_GUI);
					
					m_bNoChange = false;

					m_pSprite->SetGUISprite( true );
					return true;
				}
			}
		}
	}
	if(strNorm == strOver && strOver == strDown)
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(strNorm, "GUI");
		if(m_pSprite)
		{
			m_pSprite->SetAgainstScreen(true);
			m_pSprite->SetPosition(fPosX, fPosY);

			m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
			m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;

			m_fPosX = fPosX;
			m_fPosY = fPosY;

			m_PressedEvent.SetID(uGUIEventID);
			m_PressedEvent.SetEventTypeID(ET_GUI);
			m_bNoChange = true;
			
			m_pSprite->SetGUISprite( true );
			return true;
		}
	}
	assert_now("All textures not in same atlas");
	return false;
}
bool 
ASCGUIButton::Init(ASCTexture* pTexture, UINT32 uGUIEventID, ASCString& strTechnique)
{
	if(pTexture)
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(pTexture, strTechnique);
		if(m_pSprite)
		{
			m_pSprite->SetAgainstScreen(true);
			m_pSprite->SetPosition(0.0f, 0.0f);

			m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
			m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;

			m_fPosX = 0.0f;
			m_fPosY = 0.0f;

			m_PressedEvent.SetID(uGUIEventID);
			m_PressedEvent.SetEventTypeID(ET_GUI);
			m_bNoChange = true;
			
			m_pSprite->SetGUISprite( true );
			return true;
		}
	}
	assert_now("All textures not in same atlas");
	return false;
}
void 
ASCGUIButton::SetDetectionScale(FLOAT32 fX, FLOAT32 fY)
{
	m_fDetectScaleX = fX;
	m_fDetectScaleY = fY;
}
void 
ASCGUIButton::SetDetectionOffset(FLOAT32 fX, FLOAT32 fY)
{
	m_fDetectOffsetX = fX;
	m_fDetectOffsetY = fY;
}
void 
ASCGUIButton::SetSize(FLOAT32 fX, FLOAT32 fY)
{
	m_pSprite->SetSize(fX, fY);
	m_fHalfWidth = fX * 0.5f;
	m_fHalfHeight = fY * 0.5f;
}
void 
ASCGUIButton::Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld)
{
	if(m_bActive)
	{
#ifdef ASC_IOS
        FLOAT32 fHeight;
        if(Ascension::Device() == ED_IPHONE)
        {
            fHeight = Ascension::Width();
        }
        else if(Ascension::Device() == ED_IPAD)
        {
            fHeight = Ascension::Height();
        }
        static const FLOAT32 skfOffset = 13.0f;
        FLOAT32 fLeft = m_fPosX - (m_fHalfWidth * m_fDetectScaleX);
        FLOAT32 fRight = m_fPosX + (m_fHalfWidth * m_fDetectScaleX);
        FLOAT32 fTop = fHeight - (m_fPosY + (m_fHalfHeight * m_fDetectScaleY)) - skfOffset;
        FLOAT32 fBottom = fHeight - (m_fPosY - (m_fHalfHeight * m_fDetectScaleY)) - skfOffset;
		//if((fPosX >= fLeft && fPosX <= fRight) && (fPosY >= fTop && fPosY <= fBottom))
#else
		FLOAT32 fLeft = m_fPosX - (m_fHalfWidth * m_fDetectScaleX);
        FLOAT32 fRight = m_fPosX + (m_fHalfWidth * m_fDetectScaleX);
        FLOAT32 fTop = m_fPosY - (m_fHalfHeight * m_fDetectScaleY);
        FLOAT32 fBottom = m_fPosY + (m_fHalfHeight * m_fDetectScaleY);

		//if((fPosX >= fLeft && fPosX <= fRight) && (fPosY >= fTop && fPosY <= fBottom))
#endif
		FLOAT32 fOffsetPosX = m_fPosX + m_fDetectOffsetX;
		FLOAT32 fOffsetPosY = m_fPosY + m_fDetectOffsetY;

		fLeft -= fOffsetPosX;
		fRight -= fOffsetPosX;
		fTop -= fOffsetPosY;
		fBottom -= fOffsetPosY;

		FLOAT32 fTempX = fPosX - fOffsetPosX;
		FLOAT32 fTempY = fPosY - fOffsetPosY;

		FLOAT32 fUseRot = -m_fRotation;

		fPosX = (fTempX * cos(fUseRot)) - (fTempY * sin(fUseRot));
		fPosY = (fTempY * cos(fUseRot)) + (fTempX * sin(fUseRot));

		if((fPosX >= fLeft && fPosX <= fRight) && (fPosY >= fTop && fPosY <= fBottom))
		{
			if(bClick)
			{
				Ascension::EventSystem().AddEvent(m_PressedEvent);
			}
			if(bClick || bHeld)
			{
				if(m_uStageIndex != 2)
				{
					m_uStageIndex = 2;
					SetUVs();
				}
			}
			else
			{
				if(m_uStageIndex != 1)
				{
					m_uStageIndex = 1;
					SetUVs();
				}
			}
		}
		else
		{
			if(m_uStageIndex != 0)
			{
				m_uStageIndex = 0;
				SetUVs();
			}
		}
	}
}

void 
ASCGUIButton::SetUVs()
{
	if(false == m_bNoChange)
	{
		m_pSprite->SetUVs(m_sTexureInfo[m_uStageIndex]);
	}
}
void 
ASCGUIButton::Disable()
{
	ASCGUIElement::Disable();
	m_bActive = false;
}
void 
ASCGUIButton::Enable()
{
	ASCGUIElement::Enable();
	m_bActive = true;
}
#endif