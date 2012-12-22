/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\Building.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BUILDING_H__Included_1573642051
#define _BUILDING_H__Included_1573642051

#include "GameOfDeathInc.h"

namespace ASC
{
	class CBuilding
	{
	public:
		CBuilding(void);
		~CBuilding(void);

		bool Initialise(UINT32 uColour, SPatternTemplate& Template, SCoordinates sStartPos, bool bTop);

		void DrawOnField(UINT32* pTexels, UINT32 uPitch);

	protected:
		UINT32 m_uColour;
		ASCVector<SCoordinates> m_uCoordinates;

	};

}

#endif //_BUILDING_H__Included_1573642051

