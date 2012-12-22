/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\GameOfDeathInc.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GAMEOFDEATHINC_H__Included_2004995344
#define _GAMEOFDEATHINC_H__Included_2004995344


namespace ASC
{
#define FIELD_TEXTURE_WIDTH 120
#define FIELD_TEXTURE_HEIGHT 210
#define FIELD_SCREEN_WIDTH_RATIO (SC_FLOAT(Ascension::Width()) / FIELD_TEXTURE_WIDTH)
#define FIELD_SCREEN_HEIGHT_RATIO (SC_FLOAT(Ascension::Height()) / FIELD_TEXTURE_HEIGHT)
#define GUI_TYPE_ZONE (FIELD_TEXTURE_HEIGHT / 8)
#define GUI_INFO_ZONE (FIELD_TEXTURE_HEIGHT / 12)
#define GUI_BOARDER_WIDTH 2
#define GUI_GAP_WIDTH 3
#define GUI_BUTTON_INTERVAL 0.05f
#define GUI_BUTTON_ACTIVE_TIME 0.4f
	
#define GUI_PRIMARY_COLOUR 0xFF939FAA
#define GUI_SECONDARY_COLOUR 0xFF4D5862
#define GUI_FONT_COLOUR 0xFF000000

#define GUI_ATTACK_PATTERN_UNACTIVE 0xFF000000
#define GUI_ATTACK_PATTERN_ACTIVE 0xFF777777

#define FIELD_PLACE_ZONES 10
#define FIELD_MOVE_UPDATE 0.33f
#define FIELD_RULES_UPDATE 0.25f

#define FIELD_EMPTY 0
#define FIELD_BASE 1
#define FIELD_ID_START FIELD_BASE+1

#define FIELD_KILL_CELL 100
	
#define MENU_BACKGROUND_UPDATE 0.5f
#define MENU_BACKGROUND_RESET 180.0f
#define MENU_BACKGROUND_NEW_PATTERN 2.0f
	
#define MENU_SPACING 4

#define MENU_TITLE_STEPS 9
#define MENU_TITLE_STEP_TIME 0.1f
#define MENU_START_STEPS 9
#define MENU_START_STEP_TIME 0.11f

#define TRANSITION_TIME 0.5f

#define SINGLE_PLAYER_SHOTS 100

	enum ESTATE
	{
		S_SINGLE_PLAYER,
		S_MAIN_MENU,
		S_COUNT,
	};

	struct SCoordinates
	{
		SINT32 m_sX;
		SINT32 m_sY;

		SCoordinates()
		: m_sX ( 0 )
		, m_sY ( 0 )
		{
		}

		SCoordinates(SINT32 sX, SINT32 sY)
		: m_sX ( sX )
		, m_sY ( sY )
		{
		}
	};

	struct SPatternTemplate
	{
		UINT32 m_uWidth;
		UINT32 m_uHeight;
		UINT32** m_pPattern;

		SPatternTemplate()
		: m_uWidth ( 0 )
		, m_uHeight ( 0 )
		, m_pPattern ( NULL )
		{
		}
		~SPatternTemplate()
		{
			if(NULL != m_pPattern)
			{
				for(UINT32 i = 0; i < m_uHeight; ++i)
				{
					SafeDeleteArray( m_pPattern[i] );
				}
				SafeDeleteArray( m_pPattern );
			}
		}
		void Initialise(UINT32 uWidth, UINT32 uHeight)
		{
			if(NULL != m_pPattern)
			{
				for(UINT32 i = 0; i < m_uHeight; ++i)
				{
					SafeDeleteArray( m_pPattern[i] );
				}
				SafeDeleteArray( m_pPattern );
			}
			m_uHeight = uHeight;
			m_uWidth = uWidth;
			m_pPattern = new UINT32*[m_uHeight];
			for(UINT32 i = 0; i < m_uHeight; ++i)
			{
				m_pPattern[i] = new UINT32[m_uWidth];
				memset(m_pPattern[i], 0, sizeof(UINT32)*m_uWidth);
			}
		}
	};
}

#endif //_GAMEOFDEATHINC_H__Included_2004995344

