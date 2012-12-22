#include "ASCGUIElement.h"

#ifdef ASC_NEW_GUI_SYSTEM
#include "ASCsprite.h"

#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif
#include "ASCGUIManager.cpp"
#include "ASCMaths.h"


ASCGUIElement::ASCGUIElement(void)
: m_fHalfWidth ( 0.0f )
, m_fHalfHeight ( 0.0f )
, m_fPosX ( 0.0f )
, m_fPosY ( 0.0f )
, m_bActive ( true )
{
}


ASCGUIElement::~ASCGUIElement(void)
{
}

void 
ASCGUIElement::Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld)
{
	fPosX = fPosX;
	fPosY = fPosY;
	bClick = bClick;
	bHeld = bHeld;
	assert_now("This must be overriden in the derived class");
}

void 
ASCGUIElement::SetPos(FLOAT32 fX, FLOAT32 fY)
{
    m_fPosX = fX * Ascension::GUI().ScaleX();
    m_fPosY = fY * Ascension::GUI().ScaleY();
}
void 
ASCGUIElement::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	m_fPosX += fX;
	m_fPosY += fY;
}
FLOAT32 
ASCGUIElement::Width()
{
	return m_fHalfWidth+m_fHalfWidth;
}
FLOAT32 
ASCGUIElement::HalfWidth()
{
	return m_fHalfWidth;
}
FLOAT32 
ASCGUIElement::Height()
{
	return m_fHalfHeight+m_fHalfHeight;
}
FLOAT32 
ASCGUIElement::HalfHeight()
{
	return m_fHalfHeight;
}
FLOAT32 
ASCGUIElement::PosX()
{
	return m_fPosX;
}
FLOAT32 
ASCGUIElement::PosY()
{
	return m_fPosY;
}

void 
ASCGUIElement::Disable()
{
	m_bActive = false;
}
void 
ASCGUIElement::Enable()
{
	m_bActive = true;
}
bool 
ASCGUIElement::IsActive()
{
	return m_bActive;
}

#else

#include "ASCsprite.h"

#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif
#include "ASCGUIManager.cpp"
#include "ASCMaths.h"


ASCGUIElement::ASCGUIElement(void)
: m_pSprite ( NULL )
, m_pAnimSprite ( NULL )
, m_fHalfWidth ( 0.0f )
, m_fHalfHeight ( 0.0f )
, m_fPosX ( 0.0f )
, m_fPosY ( 0.0f )
, m_fRotation ( 0.0f )
, m_bActive ( true )
{
}


ASCGUIElement::~ASCGUIElement(void)
{
}

void 
ASCGUIElement::Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld)
{
	fPosX = fPosX;
	fPosY = fPosY;
	bClick = bClick;
	bHeld = bHeld;
	assert_now("This must be overriden in the derived class");
}

void 
ASCGUIElement::SetPos(FLOAT32 fX, FLOAT32 fY)
{
    m_fPosX = fX * Ascension::GUI().ScaleX();
    m_fPosY = fY * Ascension::GUI().ScaleY();
    m_pSprite->SetPosition(m_fPosX, m_fPosY);
}
void 
ASCGUIElement::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	m_pSprite->IncPosition(fX, fY);
	m_fPosX += fX;
	m_fPosY += fY;
}
void 
ASCGUIElement::SetActive( bool bVal )
{
	m_bActive = bVal;
	m_pSprite->SetActive(m_bActive);
}
void 
ASCGUIElement::Rotate(FLOAT32 m_fDegrees)
{
	m_fRotation = ASCMaths::DegToRad(m_fDegrees);
	m_pSprite->SetRotation( m_fRotation );
	//m_fRotation -= ASCMaths::DegToRad(180.0f);
}
FLOAT32 
ASCGUIElement::Width()
{
	return m_fHalfWidth+m_fHalfWidth;
}
FLOAT32 
ASCGUIElement::HalfWidth()
{
	return m_fHalfWidth;
}
FLOAT32 
ASCGUIElement::Height()
{
	return m_fHalfHeight+m_fHalfHeight;
}
FLOAT32 
ASCGUIElement::HalfHeight()
{
	return m_fHalfHeight;
}

ASCSprite* 
ASCGUIElement::Sprite()
{
	return m_pSprite;
}
void 
ASCGUIElement::Disable()
{
	m_pSprite->Release();
}
void 
ASCGUIElement::Enable()
{
	m_pSprite->Create();
}
ASCAnimSprite* 
ASCGUIElement::AnimSprite()
{
	return m_pAnimSprite;
}
#endif