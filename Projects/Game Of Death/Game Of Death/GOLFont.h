/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\GOLFont.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GOLFONT_H__Included_506030775
#define _GOLFONT_H__Included_506030775

#include "GameOfDeathInc.h"

namespace ASC
{
	class CGameOfLifePattern;

	class CGOLFont
	{
	public:
		CGOLFont(void);
		~CGOLFont(void);

		bool Initialise(CHAR8* strFile, UINT32 uAliveColour, UINT32 uDeadColour);
		bool Initialise(ASCString& strFile, UINT32 uAliveColour, UINT32 uDeadColour);

		void DrawInto(ASCString strLine, SCoordinates sCoord, UINT32* pTexels, UINT32 uPitch, UINT32 uWidth, UINT32 uHeight);

	protected:
		ASCTree<CGameOfLifePattern*> m_tLetters;
	};

}

#endif //_GOLFONT_H__Included_506030775

