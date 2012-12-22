/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\GameOfLifePattern.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GAMEOFLIFEPATTERN_H__Included_393359154
#define _GAMEOFLIFEPATTERN_H__Included_393359154

#include "GameOfDeathInc.h"

namespace ASC
{
	class CGameOfLifePattern
	{
	public:
		CGameOfLifePattern(void);
		~CGameOfLifePattern(void);
		
		bool Initialise(CHAR8* strPatternFile, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);
		bool Initialise(ASCString& strPatternFile, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);
		
		bool Initialise(CHAR8* strPatternFile, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);
		bool Initialise(ASCString& strPatternFile, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);

		bool InitialiseNULL(UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);

		void Reset();

		bool Update();

		void DrawInto(UINT32* pTexels, UINT32 uPitch, UINT32 uWidth, UINT32 uHeight, SCoordinates sStart);

		void AddInto(bool** pPattern, UINT32 uWidth, UINT32 uHeight, SCoordinates sStart);
		
		void SetAliveColour(UINT32 uColour);
		void SetDeadColour(UINT32 uColour);
		void SetColours(UINT32 uAlive, UINT32 uDead);
		
		UINT32 AliveColour();
		UINT32 DeadColour();
		
		UINT32 Width();
		UINT32 Height();
		
		bool** Pattern();
		bool** CurrentPattern();

	protected:
		UINT32 m_uAliveColour;
		UINT32 m_uDeadColour;
		
		UINT32 m_uWidth;
		UINT32 m_uHeight;

		FLOAT32 m_fUpdateInterval;
		FLOAT32 m_fInterval;

		bool** m_pOriginal;
		bool** m_pCurrent;
		UINT32** m_pNeighbours;


	};

}

#endif //_GAMEOFLIFEPATTERN_H__Included_393359154

