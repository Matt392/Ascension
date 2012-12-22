#include "ASCGenInc.h"
#include "ASCGUIInvisButton.h"

#include "../Ascension/Ascension.h"
#include "ASCEventManager.h"


ASCGUIInvisButton::ASCGUIInvisButton(void)
: m_fRotation ( 0.0f )
{
}


ASCGUIInvisButton::~ASCGUIInvisButton(void)
{
}

		
bool 
ASCGUIInvisButton::Init(FLOAT32 fPosX, FLOAT32 fPosY, FLOAT32 fWidth, FLOAT32 fHeight, UINT32 uGUIEventID)
{
	m_fHalfWidth = fWidth * 0.5f;
	m_fHalfHeight = fHeight * 0.5f;

	m_fPosX = fPosX;
	m_fPosY = fPosY;

	m_PressedEvent.SetID(uGUIEventID);
	m_PressedEvent.SetEventTypeID(ET_GUI);
	m_bNoChange = true;

	return true;
}

void 
ASCGUIInvisButton::Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld)
{
	if(m_bActive && bClick)
	{
		if(WithinBounds(fPosX, fPosY, m_fPosX, m_fPosY, m_fHalfWidth, m_fHalfHeight, m_fRotation))
		{
			Ascension::EventSystem().AddEvent(m_PressedEvent);
		}
	}
}
void 
ASCGUIInvisButton::SetRotation(FLOAT32 fRotation)
{
	m_fRotation = fRotation;
}
void 
ASCGUIInvisButton::SetSize(FLOAT32 fX, FLOAT32 fY)
{
	m_fHalfWidth = fX * 0.5f;
	m_fHalfHeight = fY * 0.5f;
}
void 
ASCGUIInvisButton::SetPos(FLOAT32 fX, FLOAT32 fY)
{
	m_fPosX = fX;
	m_fPosY = fY;
}
void 
ASCGUIInvisButton::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	m_fPosX += fX;
	m_fPosY += fY;
}