/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIButtonElement.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUIBUTTONELEMENT_H__Included_351812488
#define _ASCGUIBUTTONELEMENT_H__Included_351812488


namespace ASC
{
	class ASCGUIButtonElement
	{
	public:
		ASCGUIButtonElement(void);
		~ASCGUIButtonElement(void);
		
		bool WithinBounds(FLOAT32 fPointerPosX, FLOAT32 fPointerPosY, FLOAT32 fPosX, FLOAT32 fPosY, FLOAT32 fHalfWidth, FLOAT32 fHalfHeight, FLOAT32 fRotation);

		void SetDetectionScale(FLOAT32 fX, FLOAT32 fY);
		void SetDetectionOffset(FLOAT32 fX, FLOAT32 fY);

	protected:
		FLOAT32 m_fDetectScaleX;
		FLOAT32 m_fDetectScaleY;
		
		FLOAT32 m_fDetectOffsetX;
		FLOAT32 m_fDetectOffsetY;
	};

}

#endif //_ASCGUIBUTTONELEMENT_H__Included_351812488

