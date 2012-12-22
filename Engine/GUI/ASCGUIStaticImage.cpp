#include "ASCGenInc.h"
#include "ASCGUIStaticImage.h"
#include "ASCSpriteManager.h"
#include "ASCSprite.h"
#include "ASCAnimSprite.h"
#include "ASCGUIManager.h"
#include "../Ascension/Ascension.h"

ASCGUIStaticImage::ASCGUIStaticImage(void)
{
}


ASCGUIStaticImage::~ASCGUIStaticImage(void)
{
}

bool 
ASCGUIStaticImage::Init(ASCString strImage, FLOAT32 fPosX, FLOAT32 fPosY, ASCString& strTechnique)
{
	if(strImage.find('.') >= 0)
	{
		ASCString strExtension;
		strImage.substrc('.', strExtension);
		static ASCString s_strAnimSpriteExt = ".animspr";
		if(strExtension == s_strAnimSpriteExt)
		{
			m_pAnimSprite = Ascension::SpriteManager().CreateAnimSprite(strImage, strTechnique);
			m_pSprite = m_pAnimSprite;
		}
		else
		{
			m_pSprite = Ascension::SpriteManager().CreateSpriteFile(strImage, strTechnique);
		}
		
	}
	else
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(strImage, strTechnique);
	}

	m_pSprite->SetAgainstScreen( true );
	m_pSprite->SetPosition(fPosX * Ascension::GUI().ScaleX(), fPosY * Ascension::GUI().ScaleY());
	m_pSprite->SetSize(m_pSprite->GetWidth() * Ascension::GUI().ScaleX(), m_pSprite->GetHeight() * Ascension::GUI().ScaleY());
	m_pSprite->SetGUISprite( true );
    
    m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
    m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;

	return NULL != m_pSprite;
}
void 
ASCGUIStaticImage::ResetFile(ASCString strFile)
{
	ASCString strExtension;
	strFile.substrc('.', strExtension);
	static ASCString s_strAnimSpriteExt = ".animspr";
	if(strExtension == s_strAnimSpriteExt)
	{
		m_pAnimSprite->FileReset( strFile );
	}
	else
	{
		m_pSprite->FileReset( strFile );
	}

	m_pSprite->SetAgainstScreen( true );
	//m_pSprite->SetPosition(fPosX * Ascension::GUI().ScaleX(), fPosY * Ascension::GUI().ScaleY());
	m_pSprite->SetSize(m_pSprite->GetWidth() * Ascension::GUI().ScaleX(), m_pSprite->GetHeight() * Ascension::GUI().ScaleY());
	m_pSprite->SetGUISprite( true );
    
    m_fHalfWidth = m_pSprite->GetWidth() * 0.5f;
    m_fHalfHeight = m_pSprite->GetHeight() * 0.5f;
}
bool 
ASCGUIStaticImage::Init(ASCTexture* pTexture, FLOAT32 fPosX, FLOAT32 fPosY, ASCString& strTechnique)
{
	m_pSprite = Ascension::SpriteManager().CreateSprite(pTexture, strTechnique);
	if(m_pSprite)
	{
		m_pSprite->SetAgainstScreen( true );
		m_pSprite->SetPosition(fPosX, fPosY);
		m_pSprite->SetGUISprite( true );
	}
	return NULL != m_pSprite;
}
void 
ASCGUIStaticImage::SetPos(FLOAT32 fX, FLOAT32 fY)
{
	ASCGUIElement::SetPos(fX, fY);
	ASCGUIVisualElement::SetPos(fX, fY);
}
void 
ASCGUIStaticImage::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	ASCGUIElement::IncPosition(fX, fY);
	ASCGUIVisualElement::IncPosition(fX, fY);
}
void 
ASCGUIStaticImage::Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld)
{
	fPosX;
	fPosY;
	bClick;
	bHeld;
}
void 
ASCGUIStaticImage::Disable()
{
	ASCGUIElement::Disable();
	ASCGUIVisualElement::Disable();
}
void 
ASCGUIStaticImage::Enable()
{
	ASCGUIElement::Enable();
	ASCGUIVisualElement::Enable();
}