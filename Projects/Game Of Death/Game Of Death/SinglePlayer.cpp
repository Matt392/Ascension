#include "ASCGenInc.h"
#include "SinglePlayer.h"

#include "Ascension.h"
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#include "ASCGUIManager.h"
#include "ASCGUIStaticImage.h"
#include "ASCGUIInvisButton.h"
#include "ASCSprite.h"
#include "ASCFolder.h"

#include "GameOfDeathContext.h"
#include "Field.h"
#include "Base.h"
#include "GameOfLifePattern.h"
#include "GOLFont.h"


CSinglePlayer::CSinglePlayer(void)
: m_pField ( NULL )
, m_pBackgroundTexture ( NULL )
, m_pEnemyBase ( NULL )
, m_pGUITexture ( NULL )
, m_pGUIDisplay ( NULL )
, m_pNullPattern ( NULL )
, m_pGOLFont ( NULL )
, m_uAttackPatternStartIndex ( 0 )
, m_uShotsAllowed ( 0 )
, m_bNewPattern ( false )
{
	for(UINT32 i = 0; i < GEID_COUNT; ++i)
	{
		m_pGUIPattern[i] = NULL;
		m_pGUIButton[i] = NULL;
		m_uGUIButtonID[i] = 0;
		m_bGUIUpdatePattern[i] = false;
		m_bGUIHasChanged[i] = false;
		m_fButtonRunning[i] = 0.0f;
	}
	for(UINT32 i = 0; i < FIELD_PLACE_ZONES; ++i)
	{
		m_pFieldPlaceZone[i] = NULL;
		m_uFieldPlaceZoneID[i] = 0;
	}
}

CSinglePlayer::~CSinglePlayer(void)
{
	SafeDelete( m_pField );
	SafeDelete( m_pEnemyBase );
	SafeDelete( m_pNullPattern );
	SafeDelete( m_pGOLFont );
	
	for(UINT32 i = 0; i < GEID_ARROW_COUNT; ++i)
	{
		SafeDelete( m_pGUIPattern[i] );
	}
	for(UINT32 i = 0; i < m_vecAttackPatterns.size(); ++i)
	{
		SafeDelete(m_vecAttackPatterns[i]);
	}
}

bool 
CSinglePlayer::Initialise()
{
	m_pEnemyBase = new CBase();
	m_pEnemyBase->Initialise(false);

	m_pGOLFont = new CGOLFont();
	m_pGOLFont->Initialise("Fonts/Pattern", GUI_FONT_COLOUR, GUI_PRIMARY_COLOUR);
	
	ASCString strTemp = "Background";
	m_pBackgroundTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, strTemp);
	strTemp = "GUI";
	m_pGUITexture = Ascension::Renderer().GetTextureManager()->CreateTexture(FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, strTemp);
	
	UINT32* pTexels;
	UINT32 uPitch;

	if(m_pBackgroundTexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < FIELD_TEXTURE_HEIGHT; ++y)
		{
			for(UINT32 x = 0; x < FIELD_TEXTURE_WIDTH; ++x)
			{
				pTexels[(uPitch*y)+x] = 0XFF000000;
			}
		}
		m_pBackgroundTexture->UnlockTexels();
	}
	
	
	m_pBackgroundDisplay = Ascension::GUI().AddGUIStaticImage(m_pBackgroundTexture, Ascension::Width()*0.5f, Ascension::Height()*0.5f, "GUI_No_Blend");
	m_pBackgroundDisplay->Sprite()->SetSize(Ascension::Width()*1.0f, Ascension::Height()*1.0f);

	m_pField = new CField();
	m_pField->Initialise();

	m_pField->FillField(m_pEnemyBase);
	
	
	m_pGUIDisplay = Ascension::GUI().AddGUIStaticImage(m_pGUITexture, Ascension::Width()*0.5f, Ascension::Height()*0.5f, "GUI_No_Blend");
	m_pGUIDisplay->Sprite()->SetSize(Ascension::Width()*1.0f, Ascension::Height()*1.0f);

	LoadAttackPatterns();
	CreateGUI();

	Ascension::EventSystem().AddEventListener(this, ET_GUI);
	DeactivateState();
	return true;
}
void 
CSinglePlayer::LoadAttackPatterns()
{
	m_pNullPattern = new CGameOfLifePattern();
	m_pNullPattern->InitialiseNULL(GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), 0xFFFFFFFF, 0xFF000000, GUI_BUTTON_INTERVAL);

	ASCFolder Folder;
	if(Folder.Load("Templates/Patterns"))
	{
		ASCVector<ASCString>& rFiles = *Folder.Files();

		for(UINT32 i = 0; i < rFiles.size(); ++i)
		{
			CGameOfLifePattern* pPattern = new CGameOfLifePattern();
			if(pPattern->Initialise(rFiles[i], GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), 0xFFFFFFFF, GUI_ATTACK_PATTERN_UNACTIVE, GUI_BUTTON_INTERVAL))
			{
				m_vecAttackPatterns.push_back( pPattern );
			}
			else
			{
				SafeDelete( pPattern );
			}
		}
	}

	m_uActiveAttackPattern = 0;
	m_vecAttackPatterns[m_uActiveAttackPattern]->SetDeadColour(GUI_ATTACK_PATTERN_ACTIVE);
	m_uAttackPatternStartIndex = 0;
}
void 
CSinglePlayer::CreateGUI()
{
	m_pGUIPattern[GEID_LEFT_ARROW] = new CGameOfLifePattern();
	m_pGUIPattern[GEID_LEFT_ARROW]->Initialise("Templates/GUI/Left Arrow.txt", GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), 0xFFFFFFFF, 0xFF000000, GUI_BUTTON_INTERVAL);

	m_pGUIPattern[GEID_RIGHT_ARROW] = new CGameOfLifePattern();
	m_pGUIPattern[GEID_RIGHT_ARROW]->Initialise("Templates/GUI/Right Arrow.txt", GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2), 0xFFFFFFFF, 0xFF000000, GUI_BUTTON_INTERVAL);
	
	FLOAT32 fGUIWidth = (GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2)) * FIELD_SCREEN_WIDTH_RATIO;
	FLOAT32 fGUIHeight = (GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2)) * FIELD_SCREEN_HEIGHT_RATIO;
	FLOAT32 fGUIPosX = ((GUI_BOARDER_WIDTH+1) * FIELD_SCREEN_WIDTH_RATIO) + (fGUIWidth * 0.5f);
	FLOAT32 fGUIPosY = ((GUI_BOARDER_WIDTH+1)* FIELD_SCREEN_HEIGHT_RATIO) + (fGUIHeight * 0.5f);
	
	m_pGUIButton[GEID_LEFT_ARROW] = Ascension::GUI().AddGUIInvisButton(fGUIPosX, fGUIPosY, fGUIWidth, fGUIHeight, m_uGUIButtonID[GEID_LEFT_ARROW]);
	
	fGUIPosX = ((FIELD_TEXTURE_WIDTH-(GUI_BOARDER_WIDTH+1)-(GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2))) * FIELD_SCREEN_WIDTH_RATIO) + (fGUIWidth * 0.5f);
	m_pGUIButton[GEID_RIGHT_ARROW] = Ascension::GUI().AddGUIInvisButton(fGUIPosX, fGUIPosY, fGUIWidth, fGUIHeight, m_uGUIButtonID[GEID_RIGHT_ARROW]);

	FLOAT32 fMiddleX = (FIELD_TEXTURE_WIDTH * 0.5f) * FIELD_SCREEN_WIDTH_RATIO;
	FLOAT32 fWidth = GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2) + GUI_GAP_WIDTH;
	FLOAT32 fOffset = (fWidth * GEID_PATTERN_COUNT * 0.5f) - (fWidth * 0.5f);

	for(UINT32 i = GEID_PATTERN_ONE; i < GEID_COUNT; ++i)
	{
		fGUIPosX = fMiddleX + ((fWidth * (i - GEID_PATTERN_ONE) - fOffset) * FIELD_SCREEN_WIDTH_RATIO);
		m_pGUIButton[i] = Ascension::GUI().AddGUIInvisButton(fGUIPosX, fGUIPosY, fGUIWidth, fGUIHeight, m_uGUIButtonID[i]);
	}
	
	m_uFontCoordsL1.m_sX = GUI_BOARDER_WIDTH+1;
	m_uFontCoordsL1.m_sY = GUI_TYPE_ZONE+GUI_BOARDER_WIDTH-GUI_BOARDER_WIDTH;

	m_uFontCoordsL2.m_sX = GUI_BOARDER_WIDTH+1;
	m_uFontCoordsL2.m_sY = m_uFontCoordsL1.m_sY+(GUI_INFO_ZONE/2);

	UINT32* pTexels;
	UINT32 uPitch;

	if(m_pGUITexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < GUI_TYPE_ZONE+GUI_INFO_ZONE; ++y)
		{
			for(UINT32 x = 0; x < FIELD_TEXTURE_WIDTH; ++x)
			{
				if((y < GUI_BOARDER_WIDTH-1 || y > (GUI_TYPE_ZONE-GUI_BOARDER_WIDTH+GUI_INFO_ZONE)) || (x < GUI_BOARDER_WIDTH-1 || x > (FIELD_TEXTURE_WIDTH-GUI_BOARDER_WIDTH)))
				{
					pTexels[(uPitch*y)+x] = GUI_SECONDARY_COLOUR;
				}
				else
				{
					pTexels[(uPitch*y)+x] = GUI_PRIMARY_COLOUR;
				}
			}
		}
		m_sGUICoords[GEID_LEFT_ARROW] = SCoordinates(GUI_BOARDER_WIDTH+1,GUI_BOARDER_WIDTH+1);
		m_pGUIPattern[GEID_LEFT_ARROW]->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, GUI_TYPE_ZONE, m_sGUICoords[GEID_LEFT_ARROW]);
		
		m_sGUICoords[GEID_RIGHT_ARROW] = SCoordinates(FIELD_TEXTURE_WIDTH-(GUI_BOARDER_WIDTH+1)-(GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2)),GUI_BOARDER_WIDTH+1);
		m_pGUIPattern[GEID_RIGHT_ARROW]->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, GUI_TYPE_ZONE, m_sGUICoords[GEID_RIGHT_ARROW]);
		
		FLOAT32 fBoardMiddleX = (FIELD_TEXTURE_WIDTH * 0.5f);
		FLOAT32 fBoardWidth = GUI_TYPE_ZONE-(GUI_BOARDER_WIDTH*2+2) + GUI_GAP_WIDTH;
		FLOAT32 fBoardOffset = (fWidth * GEID_PATTERN_COUNT * 0.5f);

		for(UINT32 i = GEID_PATTERN_ONE; i < GEID_COUNT; ++i)
		{
			SINT32 sPosX = SC_SINT( fBoardMiddleX + (fBoardWidth * (i - GEID_PATTERN_ONE) - fBoardOffset) ) + GUI_BOARDER_WIDTH;
			m_sGUICoords[i] = SCoordinates(sPosX,GUI_BOARDER_WIDTH+1);
			if((i-GEID_PATTERN_ONE) < m_vecAttackPatterns.size())
			{
				m_pGUIPattern[i] = m_vecAttackPatterns[i-GEID_PATTERN_ONE];
				m_pGUIPattern[i]->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, GUI_TYPE_ZONE, m_sGUICoords[i]);
			}
			else
			{
				m_pGUIPattern[i] = NULL;
			}
		}


		m_pGUITexture->UnlockTexels();
	}
	
	fWidth = (FIELD_TEXTURE_WIDTH / FIELD_PLACE_ZONES) * FIELD_SCREEN_WIDTH_RATIO;
	fGUIPosY = ((GUI_TYPE_ZONE+GUI_INFO_ZONE) * FIELD_SCREEN_HEIGHT_RATIO) + (fWidth * 0.5f);
	SINT32 sWidth = FIELD_TEXTURE_WIDTH / FIELD_PLACE_ZONES;
	SINT32 sYPos = (GUI_TYPE_ZONE+GUI_INFO_ZONE) + (sWidth / 2);
	SINT32 sXPos = 0;


	for(UINT32 i = 0; i < FIELD_PLACE_ZONES; ++i)
	{
		fGUIPosX = (fWidth * i) + (fWidth * 0.5f);
		sXPos = (sWidth * i);
		m_pFieldPlaceZone[i] = Ascension::GUI().AddGUIInvisButton(fGUIPosX, fGUIPosY, fWidth, fWidth, m_uFieldPlaceZoneID[i]);
		m_sFieldPlaceZoneCoords[i] = SCoordinates(sXPos, sYPos);
	}
}
bool 
CSinglePlayer::Update()
{
	for(UINT32 i = 0; i < GEID_COUNT; ++i)
	{
		if(m_bGUIUpdatePattern[i] && NULL != m_pGUIPattern[i])
		{
			if(m_pGUIPattern[i]->Update() && m_bGUIUpdatePattern[i])
			{
				m_bGUIHasChanged[i] = true;
				m_bNewPattern = true;
			}
			else
			{
				m_bGUIHasChanged[i] = false;
			}
			m_fButtonRunning[i] += Ascension::Delta();
			if(m_fButtonRunning[i] >= GUI_BUTTON_ACTIVE_TIME)
			{
				m_fButtonRunning[i] = 0.0f;
				m_bGUIUpdatePattern[i] = false;
				m_pGUIPattern[i]->Reset();
				m_bGUIHasChanged[i] = true;
				m_bNewPattern = true;
			}

		}
		else if(m_bGUIUpdatePattern[i])//NULL pattern
		{
			m_bGUIHasChanged[i] = true;
			m_bNewPattern = true;
			m_fButtonRunning[i] += Ascension::Delta();
			if(m_fButtonRunning[i] >= GUI_BUTTON_ACTIVE_TIME)
			{
				m_fButtonRunning[i] = 0.0f;
				m_bGUIUpdatePattern[i] = false;
				m_bGUIHasChanged[i] = true;
				m_bNewPattern = true;
			}
		}
		else
		{
			m_bGUIHasChanged[i] = false;
		}
	}
	if(m_bNewPattern || m_pField->BaseChanged())
	{
		UINT32* pTexels = NULL;
		UINT32 uPitch = 0;

		if(m_pGUITexture->LockTexels(pTexels, uPitch, false))
		{
			for(UINT32 i = 0; i < GEID_COUNT; ++i)
			{
				if(m_bGUIHasChanged[i] && NULL != m_pGUIPattern[i])
				{
					m_pGUIPattern[i]->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, GUI_TYPE_ZONE, m_sGUICoords[i]);
				}
				else if(m_bGUIHasChanged[i])
				{
					m_pNullPattern->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, GUI_TYPE_ZONE, m_sGUICoords[i]);
				}
			}
			UINT32 uLength;
			ASCString strLine = "ENEMY_BASE-_";
			uLength = strLine.length();
			strLine.AddSmallInt(m_pField->BaseTargets());
			while(uLength+5 > strLine.length())
			{
				strLine += ASCString("_");
			}
			
			m_pGOLFont->DrawInto(strLine, m_uFontCoordsL1, pTexels, uPitch, FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT);

			strLine = "SHOTS_,LEFT-_";
			uLength = strLine.length();
			strLine.AddSmallInt(m_uShotsAllowed);
			while(uLength+5 > strLine.length())
			{
				strLine += ASCString("_");
			}

			m_pGOLFont->DrawInto(strLine, m_uFontCoordsL2, pTexels, uPitch, FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT);

			m_pGUITexture->UnlockTexels();
		}
	}
	if(m_pField->Update())
	{
		if(m_uShotsAllowed == 0 && m_pField->AnyChange() == false)
		{
			GameOver();
		}
	}

	//m_pField->u
	
	m_bNewPattern = false;
	return true;
}
void 
CSinglePlayer::GameOver()
{
	Ascension::Context().SetState( S_MAIN_MENU );
}
bool 
CSinglePlayer::SetUp(UINT32 uShotsAllowed)
{
	m_uShotsAllowed = uShotsAllowed;

	return true;
}
bool 
CSinglePlayer::ActivateState()
{
	Ascension::EventSystem().AddEventListener(this, ET_GUI);
	m_pField->Activate();
	m_pBackgroundDisplay->Enable();
	m_pGUIDisplay->Enable();
	
	for(UINT32 i = 0; i < FIELD_PLACE_ZONES; ++i)
	{
		m_pFieldPlaceZone[i]->Enable();
	}
	for(UINT32 i = 0; i < GEID_COUNT; ++i)
	{
		m_pGUIButton[i]->Enable();
	}

	m_bNewPattern = true;
	return true;
}
bool 
CSinglePlayer::DeactivateState()
{
	Ascension::EventSystem().RemoveEventListener(this, ET_GUI);
	m_pField->Deactivcate();
	m_pBackgroundDisplay->Disable();
	m_pGUIDisplay->Disable();
	
	
	for(UINT32 i = 0; i < FIELD_PLACE_ZONES; ++i)
	{
		m_pFieldPlaceZone[i]->Disable();
	}
	for(UINT32 i = 0; i < GEID_COUNT; ++i)
	{
		m_pGUIButton[i]->Disable();
	}

	return true;
}

void 
CSinglePlayer::IncrementAttackPattern()
{
	if(m_uAttackPatternStartIndex < m_vecAttackPatterns.size() - GEID_PATTERN_COUNT)
	{
		++m_uAttackPatternStartIndex;
		for(UINT32 i = GEID_PATTERN_ONE; i < GEID_COUNT; ++i)
		{
			UINT32 uIndex = m_uAttackPatternStartIndex + (i-GEID_PATTERN_ONE);
			if(uIndex < m_vecAttackPatterns.size())
			{
				m_pGUIPattern[i] = m_vecAttackPatterns[uIndex];
			}
			else
			{
				m_pGUIPattern[i] = NULL;
			}
			m_bGUIUpdatePattern[i] = true;
		}
	}
}
void 
CSinglePlayer::DecrementAttackPattern()
{
	if(m_uAttackPatternStartIndex > 0)
	{
		--m_uAttackPatternStartIndex;
		for(UINT32 i = GEID_PATTERN_ONE; i < GEID_COUNT; ++i)
		{
			UINT32 uIndex = m_uAttackPatternStartIndex + (i-GEID_PATTERN_ONE);
			if(uIndex < m_vecAttackPatterns.size())
			{
				m_pGUIPattern[i] = m_vecAttackPatterns[uIndex];
			}
			else
			{
				m_pGUIPattern[i] = NULL;
			}
			m_bGUIUpdatePattern[i] = true;
		}
	}
}

void 
CSinglePlayer::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_GUI:
		{
			UINT32 uEventID = pEvent->ID();
			if(uEventID == m_uGUIButtonID[GEID_LEFT_ARROW])
			{
				m_bGUIUpdatePattern[GEID_LEFT_ARROW] = true;
				DecrementAttackPattern();
			}
			else if(uEventID == m_uGUIButtonID[GEID_RIGHT_ARROW])
			{
				m_bGUIUpdatePattern[GEID_RIGHT_ARROW] = true;
				IncrementAttackPattern();
			}
			else
			{
				for(UINT32 i = GEID_PATTERN_ONE; i < GEID_COUNT; ++i)
				{
					if(uEventID == m_uGUIButtonID[i])
					{
						if(m_pGUIPattern[i] != NULL)
						{
							m_vecAttackPatterns[m_uActiveAttackPattern]->SetDeadColour(GUI_ATTACK_PATTERN_UNACTIVE);
							m_bGUIUpdatePattern[GEID_PATTERN_ONE+(m_uActiveAttackPattern-m_uAttackPatternStartIndex)] = true;

							m_uActiveAttackPattern = m_uAttackPatternStartIndex + (i-GEID_PATTERN_ONE);
							m_vecAttackPatterns[m_uActiveAttackPattern]->SetDeadColour(GUI_ATTACK_PATTERN_ACTIVE);

							m_bGUIUpdatePattern[i] = true;
						}
						break;
					}
				}
				if(NULL != m_vecAttackPatterns[m_uActiveAttackPattern] && m_uShotsAllowed > 0)
				{
					for(UINT32 i = 0; i < FIELD_PLACE_ZONES; ++i)
					{
						if(uEventID == m_uFieldPlaceZoneID[i])
						{
							SCoordinates sCoord = m_sFieldPlaceZoneCoords[i];
							sCoord.m_sY -= SC_SINT(m_vecAttackPatterns[m_uActiveAttackPattern]->Height() * 0.5f);
							m_pField->FillWithAttackPattern(m_vecAttackPatterns[m_uActiveAttackPattern], sCoord);
							--m_uShotsAllowed;
							m_bNewPattern = true;
							break;
						}
					}
				}
			}
			break;
		}
	default:break;
	}
}