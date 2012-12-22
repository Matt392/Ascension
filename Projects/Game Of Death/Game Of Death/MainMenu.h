/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\MainMenu.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MAINMENU_H__Included_65303283
#define _MAINMENU_H__Included_65303283


#include "ASCIState.h"
#include "ASCIEventListener.h"

#include "GameOfDeathInc.h"

namespace ASC
{
	class CMenuBackground;
	class CPreCalcGameOfLifePattern;

	class ASCGUIStaticImage;
	class ASCTexture;
	class ASCGUIInvisButton;

	class CMainMenu : public ASCIState, ASCIEventListener
	{
	public:
		CMainMenu(void);
		~CMainMenu(void);
		
		bool Initialise(CMenuBackground* pBackground);

		bool Update();

		bool UpdateTransitioning();

		bool ActivateState();
		bool DeactivateState();

		//Events System
		void OnEvent(ASCIEvent* pEvent);

	protected:
		CMenuBackground* m_pBackground;

		ASCGUIStaticImage* m_pMainMenuImage;
		ASCTexture* m_pMainMenuTexture;
		
		CPreCalcGameOfLifePattern* m_pTitle;
		SCoordinates m_sTitleCoords;

		CPreCalcGameOfLifePattern* m_pNewGame;
		SCoordinates m_sNewGameCoords;
		ASCGUIInvisButton* m_pNewGameButton;
		UINT32 m_uNewGame;

		bool m_bTransitioning;
		FLOAT32 m_fTransitionTimer;
		UINT32 m_uTransitionID;
		
	};

}

#endif //_MAINMENU_H__Included_65303283

