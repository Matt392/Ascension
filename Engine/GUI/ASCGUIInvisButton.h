/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIInvisButton.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUIINVISBUTTON_H__Included_2027884632
#define _ASCGUIINVISBUTTON_H__Included_2027884632

#include "ASCGUIElement.h"
#include "ASCGUIButtonElement.h"

namespace ASC
{
	class ASCGUIInvisButton : public ASCGUIElement, public ASCGUIButtonElement
	{
	public:
		ASCGUIInvisButton(void);
		virtual ~ASCGUIInvisButton(void);
		
		bool Init(FLOAT32 fPosX, FLOAT32 fPosY, FLOAT32 fWidth, FLOAT32 fHeight, UINT32 uGUIEventID);

		void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);
		
		void SetSize(FLOAT32 fX, FLOAT32 fY);
		virtual void SetPos(FLOAT32 fX, FLOAT32 fY);
		virtual void IncPosition(FLOAT32 fX, FLOAT32 fY);

		void SetRotation(FLOAT32 fRotation);

	protected:
		ASCIEvent m_PressedEvent;
		FLOAT32 m_fRotation;
	};

}

#endif //_ASCGUIINVISBUTTON_H__Included_2027884632

