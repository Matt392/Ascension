#include "ASCGenInc.h"
#include "PatternTesterContext.h"

#include "Ascension.h"

#include "ASCRenderer.h"

#include "ASCTexture.h"
#include "ASCTextureManager.h"

#include "ASCGUIStaticImage.h"
#include "ASCGUIManager.h"

#include "ASCSprite.h"

#include "ASCInputDevice.h"
#include "ASCInputEvent.h"

#include "ASCFileIO.h"

CPatternTesterContext::CPatternTesterContext(void)
: m_pPatternDisplay ( NULL )
, m_pPatternBoard ( NULL )
, m_uReload ( 0 )
, m_fCounter ( 0.0f )
{
	m_bQuit = false;
}


CPatternTesterContext::~CPatternTesterContext(void)
{
}

bool 
CPatternTesterContext::Initialise()
{
	ASCString strTemp = "";
	m_pPatternBoard = Ascension::Renderer().GetTextureManager()->CreateTexture(PATTERN_BOARD_SIZE, PATTERN_BOARD_SIZE, strTemp);

	m_pPatternDisplay = Ascension::GUI().AddGUIStaticImage(m_pPatternBoard, Ascension::Width() * 0.5f, Ascension::Height() * 0.5f, "AsIs");
	m_pPatternDisplay->Sprite()->SetSize(PATTERN_BOARD_SIZE * PATTERN_BOARD_SCALE, PATTERN_BOARD_SIZE * PATTERN_BOARD_SCALE);

	UINT32 uPitch;
	UINT32* pTexels;

	if(m_pPatternBoard->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 i = 0; i < PATTERN_BOARD_SIZE* PATTERN_BOARD_SIZE; ++i)
		{
			pTexels[i] = 0xFF000000;
		}

		m_pPatternBoard->UnlockTexels();
	}

	Ascension::KeyBoard().RegisterInput(m_uReload, "Key_F1");

	Ascension::EventSystem().AddEventListener(this, ET_INPUT);
	return true;
}
		
//Game logic
bool 
CPatternTesterContext::Update()
{
	m_fCounter += Ascension::Delta();
	if(m_fCounter > PATTERN_UPDATE_GAP)
	{
		m_fCounter -= PATTERN_UPDATE_GAP;
		UpdatePattern();
	}
	return m_bQuit == false;
}
void 
CPatternTesterContext::UpdatePattern()
{
	for(UINT32 y = 0; y < PATTERN_BOARD_SIZE; ++y)
	{
		for(UINT32 x = 0; x < PATTERN_BOARD_SIZE; ++x)
		{
			m_uNeighbourCount[y][x] = 0;
			if(y != 0 && y != PATTERN_BOARD_SIZE-1 && x != 0 && x != PATTERN_BOARD_SIZE-1)
			{
				if(m_bAlive[y-1][x])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y][x-1])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y+1][x])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y][x+1])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y-1][x-1])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y+1][x-1])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y+1][x+1])
				{
					++m_uNeighbourCount[y][x];
				}
				if(m_bAlive[y-1][x+1])
				{
					++m_uNeighbourCount[y][x];
				}
			}
		}
	}

	for(UINT32 y = 0; y < PATTERN_BOARD_SIZE; ++y)
	{
		for(UINT32 x = 0; x < PATTERN_BOARD_SIZE; ++x)
		{
			if(m_bAlive[y][x] == false && m_uNeighbourCount[y][x] == 3)
			{
				m_bAlive[y][x] = true;
			}
			else if(m_bAlive[y][x] == true && m_uNeighbourCount[y][x] == 3 || m_uNeighbourCount[y][x] == 2)
			{
			}
			else
			{
				m_bAlive[y][x] = false;
			}
		}
	}

	UINT32 uPitch;
	UINT32* pTexels;

	if(m_pPatternBoard->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 i = 0; i < PATTERN_BOARD_SIZE* PATTERN_BOARD_SIZE; ++i)
		{
			pTexels[i] = 0xFF000000;
		}

		for(UINT32 y = 0; y < PATTERN_BOARD_SIZE; ++y)
		{
			for(UINT32 x = 0; x < PATTERN_BOARD_SIZE; ++x)
			{
				if(m_bAlive[PATTERN_BOARD_SIZE-1-y][x])
				{
					pTexels[(PATTERN_BOARD_SIZE * (y))+x] = 0xFFFFFFFF;
				}
			}
		}

		m_pPatternBoard->UnlockTexels();
	}
}
//Optional
void 
CPatternTesterContext::Draw()
{
	//do nothing
}
void
CPatternTesterContext::Disable()
{
	//do nothing
}
void 
CPatternTesterContext::ReloadPattern()
{
	ASCFileIO File;
	if(File.Open("Pattern.txt", IO_IN))
	{
		bool bArray[PATTERN_BOARD_SIZE][PATTERN_BOARD_SIZE];
		memset(bArray, false, PATTERN_BOARD_SIZE*PATTERN_BOARD_SIZE);
		memset(m_bAlive, false, PATTERN_BOARD_SIZE*PATTERN_BOARD_SIZE);
		UINT32 uLine = 0;
		UINT32 uLongestLine = 0;
		ASCString strLine;
		while(File.GetNextLine(strLine))
		{
			if(strLine.length() > uLongestLine)
			{
				uLongestLine = strLine.length();
			}
			for(UINT32 i = 0; i < strLine.length(); ++i)
			{
				bArray[uLine][i] = (strLine[i] == 'x' || strLine[i] == 'X');
			}
			++uLine;
		}
		File.Close();

		UINT32 uPitch;
		UINT32* pTexels;
		UINT32 uOffsetX = (PATTERN_BOARD_SIZE/2) - (uLongestLine/2);
		UINT32 uOffsetY = (PATTERN_BOARD_SIZE/2) - (uLine/2);

		if(m_pPatternBoard->LockTexels(pTexels, uPitch, false))
		{
			for(UINT32 i = 0; i < PATTERN_BOARD_SIZE* PATTERN_BOARD_SIZE; ++i)
			{
				pTexels[i] = 0xFF000000;
			}

			for(UINT32 y = 0; y < PATTERN_BOARD_SIZE; ++y)
			{
				for(UINT32 x = 0; x < PATTERN_BOARD_SIZE; ++x)
				{
					if(bArray[PATTERN_BOARD_SIZE-1-y][x])
					{
						pTexels[(PATTERN_BOARD_SIZE * (y-uOffsetY))+x+uOffsetX] = 0xFFFFFFFF;
						m_bAlive[y-uOffsetY+1][x+uOffsetX] = true;
					}
				}
			}

			m_pPatternBoard->UnlockTexels();
		}

	}
}
void 
CPatternTesterContext::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_INPUT:
		{
			ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
			EKeyState eState = pInputEvent->GetState();
			UINT32 uID = pInputEvent->ID();
			if(uID == m_uReload && eState == KS_Pressed)
			{
				ReloadPattern();
			}
			break;
		}
	default:break;
	}
}
