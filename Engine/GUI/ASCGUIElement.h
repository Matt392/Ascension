/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIElement.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUIELEMENT_H__Included_815512342
#define _ASCGUIELEMENT_H__Included_815512342

#include "ASCGenInc.h"
#include "ASCGUIDebugElement.h"

#ifdef ASC_NEW_GUI_SYSTEM
namespace ASC
{
	class ASCGUIElement : public ASCGUIDebugElement
	{
	public:
		ASCGUIElement();
		virtual ~ASCGUIElement();

		virtual void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);

		virtual void SetPos(FLOAT32 fX, FLOAT32 fY);
		virtual void IncPosition(FLOAT32 fX, FLOAT32 fY);

		virtual FLOAT32 Width();
		virtual FLOAT32 HalfWidth();
		virtual FLOAT32 Height();
		virtual FLOAT32 HalfHeight();
		
		virtual FLOAT32 PosX();
		virtual FLOAT32 PosY();

		virtual void Disable();
		virtual void Enable();

		virtual bool IsActive();


	protected:
		FLOAT32 m_fHalfWidth;
		FLOAT32 m_fHalfHeight;

		FLOAT32 m_fPosX;
		FLOAT32 m_fPosY;

		bool m_bActive;
		bool m_bNoChange;
	};
}
#else
namespace ASC
{
	class ASCSprite;
	class ASCAnimSprite;

	class ASCGUIElement
	{
	public:
		ASCGUIElement();
		virtual ~ASCGUIElement();

		virtual void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);

		virtual void SetPos(FLOAT32 fX, FLOAT32 fY);
		virtual void IncPosition(FLOAT32 fX, FLOAT32 fY);
		virtual void SetActive( bool bVal );

		virtual FLOAT32 Width();
		virtual FLOAT32 HalfWidth();
		virtual FLOAT32 Height();
		virtual FLOAT32 HalfHeight();

		virtual ASCSprite* Sprite();
		virtual	ASCAnimSprite* AnimSprite();

		virtual void Disable();
		virtual void Enable();

		virtual void Rotate(FLOAT32 m_fDegrees);


	protected:
		ASCSprite* m_pSprite;
		ASCAnimSprite* m_pAnimSprite;

		FLOAT32 m_fHalfWidth;
		FLOAT32 m_fHalfHeight;

		FLOAT32 m_fPosX;
		FLOAT32 m_fPosY;

		FLOAT32 m_fRotation;

		bool m_bActive;

		bool m_bNoChange;
	};
}
#endif
#endif //_ASCGUIELEMENT_H__Included_815512342

