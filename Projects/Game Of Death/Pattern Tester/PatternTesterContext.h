/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Pattern Tester\PatternTesterContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PATTERNTESTERCONTEXT_H__Included_1428172368
#define _PATTERNTESTERCONTEXT_H__Included_1428172368

#include "ASCGameContext.h"
#include "ASCIEventListener.h"

namespace ASC
{
#define PATTERN_BOARD_SIZE 128
#define PATTERN_BOARD_SCALE 8.5f
#define PATTERN_UPDATE_GAP 0.2f

	class ASCGUIStaticImage;
	class ASCTexture;

	class CPatternTesterContext : public ASCGameContext, ASCIEventListener
	{
	public:
		CPatternTesterContext(void);
		~CPatternTesterContext(void);

		//Creation
		bool Initialise();
		
		//Game logic
		bool Update();

		//Optional
		void Draw();
        void Disable();
		ASCIState* GetState(UINT32 uStateID)
		{
			uStateID;
			return NULL;
		}
		void SetState(UINT32 uStateID)
		{
			uStateID;
		}

		//Functionality
		void ReloadPattern();
		void UpdatePattern();

		//Events System
		void OnEvent(ASCIEvent* pEvent);

	protected:
		ASCGUIStaticImage* m_pPatternDisplay;
		ASCTexture* m_pPatternBoard;

		bool m_bAlive[PATTERN_BOARD_SIZE][PATTERN_BOARD_SIZE];
		UINT32 m_uNeighbourCount[PATTERN_BOARD_SIZE][PATTERN_BOARD_SIZE];

		FLOAT32 m_fCounter;

		UINT32 m_uReload;
	};

}

#endif //_PATTERNTESTERCONTEXT_H__Included_1428172368

