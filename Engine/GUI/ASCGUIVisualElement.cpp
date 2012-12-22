#include "ASCGenInc.h"
#include "ASCGUIVisualElement.h"

#include "ASCsprite.h"

#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif

#include "ASCGUIManager.h"
#include "ASCMaths.h"


ASCGUIVisualElement::ASCGUIVisualElement(void)
{
}


ASCGUIVisualElement::~ASCGUIVisualElement(void)
{
}

void 
ASCGUIVisualElement::SetPos(FLOAT32 fX, FLOAT32 fY)
{
    m_pSprite->SetPosition(fX * Ascension::GUI().ScaleX(), fY * Ascension::GUI().ScaleY());
}
void 
ASCGUIVisualElement::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	m_pSprite->IncPosition(fX, fY);
}
void 
ASCGUIVisualElement::Rotate(FLOAT32 m_fDegrees)
{
	m_fRotation = ASCMaths::DegToRad(m_fDegrees);
	m_pSprite->SetRotation( m_fRotation );
}
void 
ASCGUIVisualElement::SetActive( bool bVal )
{
	m_pSprite->SetActive(bVal);
}
ASCSprite* 
ASCGUIVisualElement::Sprite()
{
	return m_pSprite;
}
ASCAnimSprite* 
ASCGUIVisualElement::AnimSprite()
{
	return m_pAnimSprite;
}
void 
ASCGUIVisualElement::Disable()
{
	m_pSprite->Release();
}
void 
ASCGUIVisualElement::Enable()
{
	m_pSprite->Create();
}