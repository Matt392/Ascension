#include "ASCGenInc.h"
#include "MainMenu.h"

#include "Ascension.h"
#include "ASCGUIManager.h"
#include "ASCGUIStaticImage.h"
#include "ASCGUIInvisButton.h"
#include "ASCSprite.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#include "ASCRenderer.h"

#include "GameOfDeathContext.h"
#include "MenuBackground.h"
#include "SinglePlayer.h"
#include "PreCalcGameOfLifePattern.h"

CMainMenu::CMainMenu(void)
: m_pBackground ( NULL )
, m_pTitle ( NULL )
, m_pMainMenuImage ( NULL )
, m_pMainMenuTexture( NULL )
, m_pNewGame ( NULL )
, m_pNewGameButton ( NULL )
, m_bTransitioning ( false )
, m_uNewGame ( 0 )
{
}


CMainMenu::~CMainMenu(void)
{
	SafeDelete( m_pTitle );
	SafeDelete( m_pNewGame );

	m_pBackground = NULL;
}
		
bool 
CMainMenu::Initialise(CMenuBackground* pBackground)
{
	ASCString strTemp = "Main Menu";
	m_pMainMenuTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, strTemp);
	
	UINT32* pTexels;
	UINT32 uPitch;

	if(m_pMainMenuTexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < FIELD_TEXTURE_HEIGHT; ++y)
		{
			for(UINT32 x = 0; x < FIELD_TEXTURE_WIDTH; ++x)
			{
				pTexels[(uPitch*y)+x] = 0x00000000;
			}
		}
		m_pMainMenuTexture->UnlockTexels();
	}

	m_pTitle = new CPreCalcGameOfLifePattern();
	m_pTitle->Initialise("Templates/Menus/MM Title.txt", MENU_TITLE_STEPS, 0xFFFFFFFF, 0x00000000, MENU_TITLE_STEP_TIME);
	m_sTitleCoords.m_sY = MENU_SPACING;
	m_sTitleCoords.m_sX = (FIELD_TEXTURE_WIDTH / 2) - (m_pTitle->Width() / 2);
	if(m_sTitleCoords.m_sX < 0)
	{
		m_sTitleCoords.m_sX = 0;
		assert_now("Guts, Title too wide");
	}


	m_pNewGame = new CPreCalcGameOfLifePattern();
	m_pNewGame->Initialise("Templates/Menus/MM Start.txt", MENU_START_STEPS, 0xFFFFFFFF, 0x00000000, MENU_START_STEP_TIME);
	m_sNewGameCoords.m_sY = m_sTitleCoords.m_sY + m_pTitle->Height() + MENU_SPACING;
	m_sNewGameCoords.m_sX = MENU_SPACING;

	FLOAT32 fGUIWidth = (m_pNewGame->Width()) * FIELD_SCREEN_WIDTH_RATIO;
	FLOAT32 fGUIHeight = (m_pNewGame->Height()) * FIELD_SCREEN_HEIGHT_RATIO;
	FLOAT32 fGUIPosX = ((m_sNewGameCoords.m_sX) * FIELD_SCREEN_WIDTH_RATIO) + (fGUIWidth * 0.5f);
	FLOAT32 fGUIPosY = ((m_sNewGameCoords.m_sY)* FIELD_SCREEN_HEIGHT_RATIO) + (fGUIHeight * 0.5f);

	m_pNewGameButton = Ascension::GUI().AddGUIInvisButton(fGUIPosX, fGUIPosY, fGUIWidth, fGUIHeight, m_uNewGame);


	
	m_pMainMenuImage = Ascension::GUI().AddGUIStaticImage(m_pMainMenuTexture, Ascension::Width()*0.5f, Ascension::Height()*0.5f, "GUI_No_Blend");
	m_pMainMenuImage->Sprite()->SetSize(Ascension::Width()*1.0f, Ascension::Height()*1.0f);

	m_pBackground = pBackground;
	Ascension::EventSystem().AddEventListener(this, ET_GUI);
	DeactivateState();
	return true;
}

bool 
CMainMenu::Update()
{

	bool bChangeMade = false;
	if(m_pTitle->Update())
	{
		bChangeMade = true;
	}
	
	if(m_pNewGame->Update())
	{
		bChangeMade = true;
	}

	if(bChangeMade)
	{
		UINT32* pTexels;
		UINT32 uPitch;
		if(m_pMainMenuTexture->LockTexels(pTexels, uPitch, false))
		{
			m_pTitle->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, m_sTitleCoords);
			m_pNewGame->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, m_sNewGameCoords);

			m_pMainMenuTexture->UnlockTexels();
		}
	}
	
	if( m_bTransitioning )
	{
		UpdateTransitioning();
	}

	m_pBackground->Update();

	return true;
}
bool 
CMainMenu::UpdateTransitioning()
{
	m_fTransitionTimer += Ascension::Delta();
	if(m_fTransitionTimer >= TRANSITION_TIME)
	{
		if(m_uTransitionID == m_uNewGame)
		{
			m_bTransitioning = false;
			reinterpret_cast<CSinglePlayer*>(Ascension::Context().GetState(S_SINGLE_PLAYER))->SetUp(SINGLE_PLAYER_SHOTS);
			Ascension::Context().SetState(S_SINGLE_PLAYER);
		}
	}
	return true;
}

bool 
CMainMenu::ActivateState()
{
	m_bTransitioning = false;
	m_pBackground->Activate();
	m_pMainMenuImage->Enable();

	m_pTitle->SetToEnd();
	m_pTitle->IncrementForward( false );
	m_pTitle->Loop( false );
	m_pTitle->Play( true );

	m_pNewGame->SetToEnd();
	m_pNewGame->IncrementForward( false );
	m_pNewGame->Loop( false );
	m_pNewGame->Play( true );
	m_pNewGameButton->Enable();

	UINT32* pTexels;
	UINT32 uPitch;
	if(m_pMainMenuTexture->LockTexels(pTexels, uPitch, false))
	{
		m_pTitle->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, m_sTitleCoords);

		m_pMainMenuTexture->UnlockTexels();
	}

	Ascension::EventSystem().AddEventListener(this, ET_GUI);
	return true;
}
bool 
CMainMenu::DeactivateState()
{
	m_pBackground->Deactivate();
	m_pMainMenuImage->Disable();
	m_pNewGameButton->Disable();

	Ascension::EventSystem().RemoveEventListener(this, ET_GUI);
	return true;
}

//Events System
void 
CMainMenu::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_GUI:
		{
			UINT32 uEventID = pEvent->ID();
			if(false == m_bTransitioning)
			{
				if(uEventID == m_uNewGame)
				{
					m_fTransitionTimer = 0.0f;
					m_bTransitioning = true;
					m_uTransitionID = m_uNewGame;
					
					m_pNewGame->Reset();
					m_pNewGame->IncrementForward( true );
					m_pNewGame->Loop( false );
					m_pNewGame->Play( true );
				}
			}
			break;
		}
	default:break;
	}
}
