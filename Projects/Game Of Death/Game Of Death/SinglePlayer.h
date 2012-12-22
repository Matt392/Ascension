/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\SinglePlayer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SINGLEPLAYER_H__Included_1444541913
#define _SINGLEPLAYER_H__Included_1444541913

#include "ASCIState.h"
#include "ASCIEventListener.h"

#include "GameOfDeathInc.h"

namespace ASC
{
	class ASCGUIStaticImage;
	class ASCGUIInvisButton;
	class ASCTexture;

	class CField;
	class CBase;
	class CGOLFont;

	class CGameOfLifePattern;

	enum EGUIElementID
	{
		GEID_LEFT_ARROW,
		GEID_RIGHT_ARROW,
		GEID_PATTERN_ONE,
		GEID_PATTERN_TWO,
		GEID_PATTERN_THREE,
		GEID_COUNT,
		GEID_ARROW_COUNT = GEID_PATTERN_ONE,
		GEID_PATTERN_COUNT = GEID_COUNT - GEID_PATTERN_ONE,
	};

	class CSinglePlayer : public ASCIState, ASCIEventListener
	{
	public:
		CSinglePlayer(void);
		~CSinglePlayer(void);
		
		bool Initialise();
		void CreateGUI();
		void LoadAttackPatterns();

		bool Update();
		
		bool SetUp(UINT32 uShotsAllowed);

		bool ActivateState();
		bool DeactivateState();
		
		void IncrementAttackPattern();
		void DecrementAttackPattern();

		void GameOver();

		//Events System
		void OnEvent(ASCIEvent* pEvent);
	
	protected:
		CField* m_pField;
		CGOLFont* m_pGOLFont;
		SCoordinates m_uFontCoordsL1;
		SCoordinates m_uFontCoordsL2;

		ASCTexture* m_pBackgroundTexture;
		ASCGUIStaticImage* m_pBackgroundDisplay;

		ASCTexture* m_pGUITexture;
		ASCGUIStaticImage* m_pGUIDisplay;

		CBase* m_pEnemyBase;

		CGameOfLifePattern* m_pNullPattern;
		CGameOfLifePattern* m_pGUIPattern[GEID_COUNT];
		ASCGUIInvisButton* m_pGUIButton[GEID_COUNT];
		UINT32 m_uGUIButtonID[GEID_COUNT];
		bool m_bGUIHasChanged[GEID_COUNT];
		bool m_bGUIUpdatePattern[GEID_COUNT];
		FLOAT32 m_fButtonRunning[GEID_COUNT];
		SCoordinates m_sGUICoords[GEID_COUNT];

		UINT32 m_uActiveAttackPattern;
		UINT32 m_uAttackPatternStartIndex;
		ASCVector<CGameOfLifePattern*> m_vecAttackPatterns;

		
		ASCGUIInvisButton* m_pFieldPlaceZone[FIELD_PLACE_ZONES];
		SCoordinates m_sFieldPlaceZoneCoords[FIELD_PLACE_ZONES];
		UINT32 m_uFieldPlaceZoneID[FIELD_PLACE_ZONES];

		bool m_bNewPattern;

		UINT32 m_uShotsAllowed;

	};

}

#endif //_SINGLEPLAYER_H__Included_1444541913

