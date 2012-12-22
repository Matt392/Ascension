#include "ASCGenInc.h"
#include "PreCalcGameOfLifePattern.h"

#include "Ascension.h"

CPreCalcGameOfLifePattern::CPreCalcGameOfLifePattern(void)
: m_uAliveColour ( 0 )
, m_uDeadColour ( 0 )
, m_uWidth ( 0 )
, m_uHeight ( 0 )
, m_uMaxSteps ( 0 )
, m_uIndex ( 0 )
, m_fUpdateInterval ( 0.0f )
, m_fInterval ( 0.0f )
, m_pPattern ( NULL )
, m_bPingPongMode ( false )
, m_bPlay ( true )
{
}


CPreCalcGameOfLifePattern::~CPreCalcGameOfLifePattern(void)
{
	if(NULL != m_pPattern)
	{
		for(UINT32 z = 0; z < m_uMaxSteps; ++z)
		{
			for(UINT32 y = 0; y < m_uHeight; ++y)
			{
				if(NULL != m_pPattern[z][y])
				{
					SafeDeleteArray(m_pPattern[z][y]);
				}
			}
			SafeDeleteArray(m_pPattern[z]);
		}
		SafeDeleteArray(m_pPattern);
	}
}
		
bool 
CPreCalcGameOfLifePattern::Initialise(CHAR8* strPatternFile, UINT32 uSteps, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	ASCString strFile = strPatternFile;
	return Initialise(strFile, uSteps, uWidth, uHeight, uAliveColour, uDeadColour, fUpdateInterval);
}
bool 
CPreCalcGameOfLifePattern::Initialise(ASCString& strPatternFile, UINT32 uSteps, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	m_uAliveColour = uAliveColour;
	m_uDeadColour = uDeadColour;
	m_uWidth = uWidth;
	m_uHeight = uHeight;
	m_uMaxSteps = uSteps;
	m_fUpdateInterval = fUpdateInterval;
	m_uIndex = 0;

	ASCFileIO File;

	if(File.Open(strPatternFile, IO_IN) && m_pPattern == NULL)
	{
		ASCVector<ASCString> vecLines;
		ASCString strLine;
		UINT32 uLongestLine = 0;

		while(File.GetNextLine(strLine))
		{
			if(strLine.length() > uLongestLine)
			{
				uLongestLine = strLine.length();
			}
			vecLines.push_back( strLine );
		}
		File.Close();

		
		m_pPattern = new bool**[m_uMaxSteps];

		for(UINT32 z = 0; z < m_uMaxSteps; ++z)
		{
			m_pPattern[z] = new bool*[m_uHeight];

			for(UINT32 i = 0; i < m_uHeight; ++i)
			{
				m_pPattern[z][i] = new bool[m_uWidth];
			
				memset(m_pPattern[z][i], false, sizeof(bool)*m_uWidth);
			}
		}
		
		UINT32 uOffsetX = (m_uWidth/2) - (uLongestLine/2);
		UINT32 uOffsetY = (m_uHeight/2) - (vecLines.size()/2);
		
		UINT32 uYIndex = 0;
		UINT32 uXIndex = 0;

		for(UINT32 y = 0; y < vecLines.size(); ++y)
		{
			for(UINT32 x = 0; x < vecLines[y].length(); ++x)
			{
				if(vecLines[y][x] == 'x' || vecLines[y][x] == 'X')
				{
					uYIndex = y+uOffsetY;
					uXIndex = x+uOffsetX;
					if((uYIndex >= 0 && uYIndex < m_uHeight) && (uXIndex >= 0 && uXIndex < m_uWidth))
					{
						m_pPattern[m_uIndex][uYIndex][uXIndex] = true;
					}
				}
			}
		}
		CalculateSteps();
		return true;
	}
	return false;
}
		
bool 
CPreCalcGameOfLifePattern::Initialise(CHAR8* strPatternFile, UINT32 uSteps, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	ASCString strFile = strPatternFile;
	return Initialise(strFile, uSteps, uAliveColour, uDeadColour, fUpdateInterval);
}
bool 
CPreCalcGameOfLifePattern::Initialise(ASCString& strPatternFile, UINT32 uSteps, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	m_uAliveColour = uAliveColour;
	m_uDeadColour = uDeadColour;
	m_uMaxSteps = uSteps;
	m_fUpdateInterval = fUpdateInterval;
	m_uIndex = 0;

	ASCFileIO File;

	if(File.Open(strPatternFile, IO_IN) && m_pPattern == NULL)
	{
		ASCVector<ASCString> vecLines;
		ASCString strLine;
		UINT32 uLongestLine = 0;

		while(File.GetNextLine(strLine))
		{
			if(strLine.length() > uLongestLine)
			{
				uLongestLine = strLine.length();
			}
			vecLines.push_back( strLine );
		}
		File.Close();

		m_uWidth = uLongestLine;
		m_uHeight = vecLines.size();

		
		m_pPattern = new bool**[m_uMaxSteps];

		for(UINT32 z = 0; z < m_uMaxSteps; ++z)
		{
			m_pPattern[z] = new bool*[m_uHeight];

			for(UINT32 i = 0; i < m_uHeight; ++i)
			{
				m_pPattern[z][i] = new bool[m_uWidth];
			
				memset(m_pPattern[z][i], false, sizeof(bool)*m_uWidth);
			}
		}
		
		UINT32 uOffsetX = (m_uWidth/2) - (uLongestLine/2);
		UINT32 uOffsetY = (m_uHeight/2) - (vecLines.size()/2);
		
		UINT32 uYIndex = 0;
		UINT32 uXIndex = 0;

		for(UINT32 y = 0; y < vecLines.size(); ++y)
		{
			for(UINT32 x = 0; x < vecLines[y].length(); ++x)
			{
				if(vecLines[y][x] == 'x' || vecLines[y][x] == 'X')
				{
					uYIndex = y+uOffsetY;
					uXIndex = x+uOffsetX;
					if((uYIndex >= 0 && uYIndex < m_uHeight) && (uXIndex >= 0 && uXIndex < m_uWidth))
					{
						m_pPattern[m_uIndex][uYIndex][uXIndex] = true;
					}
				}
			}
		}
		CalculateSteps();
		return true;
	}
	return false;
}
bool 
CPreCalcGameOfLifePattern::CalculateSteps()
{
	UINT32** pNeighbours;
	pNeighbours = new UINT32*[m_uHeight];

	for(UINT32 i = 0; i < m_uHeight; ++i)
	{
		pNeighbours[i] = new UINT32[m_uWidth];
	}

	for(UINT32 z = 1; z < m_uMaxSteps; ++z)
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				pNeighbours[y][x] = 0;
				if(y > 0)// Top
				{
					if(m_pPattern[z-1][y-1][x])
					{
						++pNeighbours[y][x];
					}
				}
				if(y < m_uHeight-1)// Bottom
				{
					if(m_pPattern[z-1][y+1][x])
					{
						++pNeighbours[y][x];
					}
				}
				if(x > 0)// Left
				{
					if(m_pPattern[z-1][y][x-1])
					{
						++pNeighbours[y][x];
					}
				}
				if(x < m_uWidth-1)// Right
				{
					if(m_pPattern[z-1][y][x+1])
					{
						++pNeighbours[y][x];
					}
				}
				if(y != 0 && x != 0)// Top Left
				{
					if(m_pPattern[z-1][y-1][x-1])
					{
						++pNeighbours[y][x];
					}
				}
				if(y != 0 && x != m_uWidth-1)// Top Right
				{
					if(m_pPattern[z-1][y-1][x+1])
					{
						++pNeighbours[y][x];
					}
				}
				if(y != m_uHeight-1 && x != 0)// Bottom Left
				{
					if(m_pPattern[z-1][y+1][x-1])
					{
						++pNeighbours[y][x];
					}
				}
				if(y != m_uHeight-1 && x != m_uWidth-1)// Bottom Right
				{
					if(m_pPattern[z-1][y+1][x+1])
					{
						++pNeighbours[y][x];
					}
				}
			}
		}
		
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				if(m_pPattern[z-1][y][x] == false && pNeighbours[y][x] == 3)
				{
					m_pPattern[z][y][x] = true;
				}
				else if(m_pPattern[z-1][y][x] == true && pNeighbours[y][x] == 3 || pNeighbours[y][x] == 2)
				{
				}
				else
				{
					m_pPattern[z][y][x] = false;
				}
			}
		}
	}
	

	for(UINT32 i = 0; i < m_uHeight; ++i)
	{
		SafeDeleteArray( pNeighbours[i] );
	}
	SafeDeleteArray( pNeighbours );

	return true;
}
void 
CPreCalcGameOfLifePattern::Reset()
{
	m_uIndex = 0;
}
void 
CPreCalcGameOfLifePattern::SetToEnd()
{
	m_uIndex = m_uMaxSteps-1;
}
void 
CPreCalcGameOfLifePattern::PingPongMode(bool bVal)
{
	m_bPingPongMode = bVal;
}
void 
CPreCalcGameOfLifePattern::IncrementForward(bool bVal)
{
	m_bForward = bVal;
}
void 
CPreCalcGameOfLifePattern::Loop(bool bVal)
{
	m_bLoop = bVal;
}
void 
CPreCalcGameOfLifePattern::Play(bool bVal)
{
	m_bPlay = bVal;
}

bool 
CPreCalcGameOfLifePattern::Update()
{
	if(m_bPlay)
	{
		m_fInterval += Ascension::Delta();
		if(m_fInterval >= m_fUpdateInterval)
		{
			m_fInterval -= m_fUpdateInterval;
			if(m_bForward)
			{
				++m_uIndex;
				if(m_uIndex >= m_uMaxSteps )
				{
					if(m_bLoop)
					{
						if(m_bPingPongMode)
						{
							m_uIndex = m_uMaxSteps-1;
							m_bForward = false;
						}
						else
						{
							m_uIndex = 0;
						}
					}
					else
					{
						m_bPlay = false;
					}
				}
			}
			else
			{
				if(m_uIndex > 0)
				{
					--m_uIndex;
				
					if(m_uIndex == 0)
					{
						if(m_bLoop)
						{
							if(m_bPingPongMode)
							{
								m_bForward = true;
							}
							else
							{
								m_uIndex = m_uMaxSteps-1;
							}
						}
						else
						{
							m_bPlay = false;
						}
					}
				}
				else
				{
					assert_now("Need to do something here");
				}
			}

			return true;
		}
	}

	return false;
}

void 
CPreCalcGameOfLifePattern::DrawInto(UINT32* pTexels, UINT32 uPitch, UINT32 uWidth, UINT32 uHeight, SCoordinates sStart)
{
	if(pTexels)
	{
		for(UINT32 y = sStart.m_sY; y-sStart.m_sY < m_uHeight && y < uHeight; ++y)
		{
			for(UINT32 x = sStart.m_sX; x-sStart.m_sX < m_uWidth && x < uWidth; ++x)
			{
				if(m_pPattern[m_uIndex][y-sStart.m_sY][x-sStart.m_sX])
				{
					pTexels[(uPitch*y)+x] = m_uAliveColour;
				}
				else
				{
					pTexels[(uPitch*y)+x] = m_uDeadColour;
				}
			}
		}
	}
	else
	{
		assert_now("Guts, NULL texels");
	}
}
		
void 
CPreCalcGameOfLifePattern::SetAliveColour(UINT32 uColour)
{
	m_uAliveColour = uColour;
}
void 
CPreCalcGameOfLifePattern::SetDeadColour(UINT32 uColour)
{
	m_uDeadColour = uColour;
}
void 
CPreCalcGameOfLifePattern::SetColours(UINT32 uAlive, UINT32 uDead)
{
	m_uAliveColour = uAlive;
	m_uDeadColour = uDead;
}
		
UINT32 
CPreCalcGameOfLifePattern::AliveColour()
{
	return m_uAliveColour;
}
UINT32 
CPreCalcGameOfLifePattern::DeadColour()
{
	return m_uDeadColour;
}
		
UINT32 
CPreCalcGameOfLifePattern::Width()
{
	return m_uWidth;
}
UINT32 
CPreCalcGameOfLifePattern::Height()
{
	return m_uHeight;
}
		
bool** 
CPreCalcGameOfLifePattern::CurrentPattern()
{
	return m_pPattern[m_uIndex];
}
  		 
												  