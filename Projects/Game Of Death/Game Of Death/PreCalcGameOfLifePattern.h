/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\PreCalcGameOfLifePattern.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PRECALCGAMEOFLIFEPATTERN_H__Included_1487342218
#define _PRECALCGAMEOFLIFEPATTERN_H__Included_1487342218

#include "GameOfDeathInc.h"

namespace ASC
{
	class CPreCalcGameOfLifePattern
	{
	public:
		CPreCalcGameOfLifePattern(void);
		~CPreCalcGameOfLifePattern(void);
		
		bool Initialise(CHAR8* strPatternFile, UINT32 uSteps, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);
		bool Initialise(ASCString& strPatternFile, UINT32 uSteps, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);
		
		bool Initialise(CHAR8* strPatternFile, UINT32 uSteps, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);
		bool Initialise(ASCString& strPatternFile, UINT32 uSteps, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval);

		bool CalculateSteps();


		void Reset();
		void SetToEnd();
		void PingPongMode(bool bVal);
		void IncrementForward(bool bVal);
		void Loop(bool bVal);
		void Play(bool bVal);

		bool Update();

		void DrawInto(UINT32* pTexels, UINT32 uPitch, UINT32 uWidth, UINT32 uHeight, SCoordinates sStart);
		
		void SetAliveColour(UINT32 uColour);
		void SetDeadColour(UINT32 uColour);
		void SetColours(UINT32 uAlive, UINT32 uDead);
		
		UINT32 AliveColour();
		UINT32 DeadColour();
		
		UINT32 Width();
		UINT32 Height();
		
		bool** CurrentPattern();

	protected:
		UINT32 m_uAliveColour;
		UINT32 m_uDeadColour;
		
		UINT32 m_uWidth;
		UINT32 m_uHeight;

		UINT32 m_uMaxSteps;
		UINT32 m_uIndex;

		FLOAT32 m_fUpdateInterval;
		FLOAT32 m_fInterval;

		bool m_bPingPongMode;
		bool m_bForward;
		bool m_bLoop;
		bool m_bPlay;

		bool*** m_pPattern;
	};

}

#endif //_PRECALCGAMEOFLIFEPATTERN_H__Included_1487342218

