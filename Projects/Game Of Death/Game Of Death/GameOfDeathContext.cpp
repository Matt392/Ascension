#include "ASCGenInc.h"
#include "GameOfDeathContext.h"

#include "Ascension.h"
#include "ASCIState.h"

#include "SinglePlayer.h"
#include "MainMenu.h"
#include "MenuBackground.h"
#include "Templates.h"

CGameOfDeathContext::CGameOfDeathContext(void)
: m_eCurrentState ( S_COUNT )
, m_pTemplates ( NULL )
, m_pMenuBackground ( NULL )
{
	m_bQuit = false;
	
	for(UINT32 i = 0; i < S_COUNT; ++i)
	{
		m_pStates[i] = NULL;
	}
}


CGameOfDeathContext::~CGameOfDeathContext(void)
{
	for(UINT32 i = 0; i < S_COUNT; ++i)
	{
		if(NULL != m_pStates[i])
		{
			SafeDelete(m_pStates[i]);
		}
	}

	SafeDelete( m_pMenuBackground );
	SafeDelete( m_pTemplates );
}

bool 
CGameOfDeathContext::Initialise()
{
	m_pTemplates = new CTemplates();
	m_pTemplates->Initialise();

	m_pMenuBackground = new CMenuBackground();
	m_pMenuBackground->Initialise();

	CSinglePlayer* pSinglePlayerState = new CSinglePlayer();
	if(pSinglePlayerState->Initialise())
	{
		m_pStates[S_SINGLE_PLAYER] = pSinglePlayerState;
	}

	CMainMenu* pMainMenuState = new CMainMenu();
	if(pMainMenuState->Initialise( m_pMenuBackground ))
	{
		m_pStates[S_MAIN_MENU] = pMainMenuState;
	}

	SetState(S_MAIN_MENU);
	return true;
}
		
//Game logic
bool 
CGameOfDeathContext::Update()
{
	if(NULL != m_pStates[m_eCurrentState])
	{
		m_bQuit = !m_pStates[m_eCurrentState]->Update();
	}
	return m_bQuit == false;
}
//Optional
void 
CGameOfDeathContext::Draw()
{
	//do nothing
}
void
CGameOfDeathContext::Disable()
{
	//do nothing
}
ASCIState* 
CGameOfDeathContext::GetState(UINT32 uStateID)
{
	return m_pStates[uStateID];
}
void 
CGameOfDeathContext::SetState(UINT32 uStateID)
{
	if(static_cast<UINT32>(m_eCurrentState) != uStateID && NULL != m_pStates[uStateID])
	{
		if(NULL != m_pStates[m_eCurrentState] && m_eCurrentState != S_COUNT)
		{
			m_pStates[m_eCurrentState]->DeactivateState();
		}
		m_eCurrentState = static_cast<ESTATE>(uStateID);
		m_pStates[m_eCurrentState]->ActivateState();
	}
}

CTemplates* 
CGameOfDeathContext::Templates()
{
	return m_pTemplates;
}

void 
CGameOfDeathContext::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_INPUT:
		{
			//ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
			//EKeyState eState = pInputEvent->GetState();
			//UINT32 uID = pInputEvent->ID();
			break;
		}
	default:break;
	}
}
