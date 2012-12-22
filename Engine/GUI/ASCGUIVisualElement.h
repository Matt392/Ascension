/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIVisualElement.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUIVISUALELEMENT_H__Included_1431692897
#define _ASCGUIVISUALELEMENT_H__Included_1431692897


namespace ASC
{
	class ASCSprite;
	class ASCAnimSprite;

	class ASCGUIVisualElement
	{
	public:
		ASCGUIVisualElement(void);
		virtual ~ASCGUIVisualElement(void);

		virtual ASCSprite* Sprite();
		virtual	ASCAnimSprite* AnimSprite();

		virtual void SetPos(FLOAT32 fX, FLOAT32 fY);
		virtual void IncPosition(FLOAT32 fX, FLOAT32 fY);
		virtual void Rotate(FLOAT32 m_fDegrees);

		virtual void SetActive( bool bVal );
		virtual void Disable();
		virtual void Enable();

	protected:		
		ASCSprite* m_pSprite;
		ASCAnimSprite* m_pAnimSprite;

		FLOAT32 m_fRotation;
	};

}

#endif //_ASCGUIVISUALELEMENT_H__Included_1431692897

