#include "ASCGenInc.h"
#include "GameOfLifePattern.h"

#include "Ascension.h"
#include "ASCFileIO.h"

CGameOfLifePattern::CGameOfLifePattern(void)
: m_uAliveColour ( 0 )
, m_uDeadColour ( 0 )
, m_uWidth ( 0 )
, m_uHeight ( 0 )
, m_fUpdateInterval ( 0.0f )
, m_fInterval ( 0.0f )
, m_pOriginal ( NULL )
, m_pCurrent ( NULL )
, m_pNeighbours ( NULL )
{
}


CGameOfLifePattern::~CGameOfLifePattern(void)
{
	if(NULL != m_pOriginal)
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			if(NULL != m_pOriginal[y])
			{
				SafeDeleteArray( m_pOriginal[y] );
			}
		}
		SafeDeleteArray( m_pOriginal );
	}
	if(NULL != m_pCurrent)
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			if(NULL != m_pCurrent[y])
			{
				SafeDeleteArray( m_pCurrent[y] );
			}
		}
		SafeDeleteArray( m_pCurrent );
	}
	if(NULL != m_pNeighbours)
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			if(NULL != m_pNeighbours[y])
			{
				SafeDeleteArray( m_pNeighbours[y] );
			}
		}
		SafeDeleteArray( m_pNeighbours );
	}
}
bool 
CGameOfLifePattern::Initialise(CHAR8* strPatternFile, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	ASCString strFile = strPatternFile;
	return Initialise(strFile, uWidth, uHeight, uAliveColour, uDeadColour, fUpdateInterval);
}
bool 
CGameOfLifePattern::Initialise(ASCString& strPatternFile, UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	m_uAliveColour = uAliveColour;
	m_uDeadColour = uDeadColour;
	m_uWidth = uWidth;
	m_uHeight = uHeight;
	m_fUpdateInterval = fUpdateInterval;

	ASCFileIO File;

	if(File.Open(strPatternFile, IO_IN) && (m_pOriginal == NULL && m_pCurrent == NULL && m_pNeighbours == NULL))
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

		
		m_pOriginal = new bool*[m_uHeight];
		m_pCurrent = new bool*[m_uHeight];
		m_pNeighbours = new UINT32*[m_uHeight];

		for(UINT32 i = 0; i < m_uHeight; ++i)
		{
			m_pOriginal[i] = new bool[m_uWidth];
			m_pCurrent[i] = new bool[m_uWidth];
			m_pNeighbours[i] = new UINT32[m_uWidth];
			
			memset(m_pOriginal[i], false, sizeof(bool)*m_uWidth);
			memset(m_pCurrent[i], false, sizeof(bool)*m_uWidth);
			memset(m_pNeighbours[i], 0, sizeof(UINT32)*m_uWidth);
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
						m_pOriginal[uYIndex][uXIndex] = true;
						m_pCurrent[uYIndex][uXIndex] = true;
					}
				}
			}
		}

		return true;
	}
	return false;
}
		
bool 
CGameOfLifePattern::Initialise(CHAR8* strPatternFile, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	ASCString strFile = strPatternFile;
	return Initialise(strFile, uAliveColour, uDeadColour, fUpdateInterval);
}
bool 
CGameOfLifePattern::Initialise(ASCString& strPatternFile, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	m_uAliveColour = uAliveColour;
	m_uDeadColour = uDeadColour;
	m_fUpdateInterval = fUpdateInterval;

	ASCFileIO File;

	if(File.Open(strPatternFile, IO_IN) && (m_pOriginal == NULL && m_pCurrent == NULL && m_pNeighbours == NULL))
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
		
		m_pOriginal = new bool*[m_uHeight];
		m_pCurrent = new bool*[m_uHeight];
		m_pNeighbours = new UINT32*[m_uHeight];

		for(UINT32 i = 0; i < m_uHeight; ++i)
		{
			m_pOriginal[i] = new bool[m_uWidth];
			m_pCurrent[i] = new bool[m_uWidth];
			m_pNeighbours[i] = new UINT32[m_uWidth];
			
			memset(m_pOriginal[i], false, sizeof(bool)*m_uWidth);
			memset(m_pCurrent[i], false, sizeof(bool)*m_uWidth);
			memset(m_pNeighbours[i], 0, sizeof(UINT32)*m_uWidth);
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
						m_pOriginal[uYIndex][uXIndex] = true;
						m_pCurrent[uYIndex][uXIndex] = true;
					}
				}
			}
		}

		return true;
	}
	return false;
}
bool 
CGameOfLifePattern::InitialiseNULL(UINT32 uWidth, UINT32 uHeight, UINT32 uAliveColour, UINT32 uDeadColour, FLOAT32 fUpdateInterval)
{
	m_uAliveColour = uAliveColour;
	m_uDeadColour = uDeadColour;
	m_uWidth = uWidth;
	m_uHeight = uHeight;
	m_fUpdateInterval = fUpdateInterval;

	ASCFileIO File;

	if(m_pOriginal == NULL && m_pCurrent == NULL && m_pNeighbours == NULL)
	{		
		m_pOriginal = new bool*[m_uHeight];
		m_pCurrent = new bool*[m_uHeight];
		m_pNeighbours = new UINT32*[m_uHeight];

		for(UINT32 i = 0; i < m_uHeight; ++i)
		{
			m_pOriginal[i] = new bool[m_uWidth];
			m_pCurrent[i] = new bool[m_uWidth];
			m_pNeighbours[i] = new UINT32[m_uWidth];
			
			memset(m_pOriginal[i], false, sizeof(bool)*m_uWidth);
			memset(m_pCurrent[i], false, sizeof(bool)*m_uWidth);
			memset(m_pNeighbours[i], 0, sizeof(UINT32)*m_uWidth);
		}
		
		//UINT32 uOffsetX = (m_uWidth/2) - (uLongestLine/2);
		//UINT32 uOffsetY = (m_uHeight/2) - (vecLines.size()/2);
		//
		//UINT32 uYIndex = 0;
		//UINT32 uXIndex = 0;

		//for(UINT32 y = 0; y < vecLines.size(); ++y)
		//{
		//	for(UINT32 x = 0; x < vecLines[y].length(); ++x)
		//	{
		//		if(vecLines[y][x] == 'x' || vecLines[y][x] == 'X')
		//		{
		//			uYIndex = y+uOffsetY;
		//			uXIndex = x+uOffsetX;
		//			if((uYIndex >= 0 && uYIndex < m_uHeight) && (uXIndex >= 0 && uXIndex < m_uWidth))
		//			{
		//				m_pOriginal[uYIndex][uXIndex] = true;
		//				m_pCurrent[uYIndex][uXIndex] = true;
		//			}
		//		}
		//	}
		//}

		return true;
	}
	return false;
}
void 
CGameOfLifePattern::Reset()
{
	if(NULL != m_pOriginal && NULL != m_pCurrent)
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				m_pCurrent[y][x] = m_pOriginal[y][x];
			}
		}
	}
}
void 
CGameOfLifePattern::SetAliveColour(UINT32 uColour)
{
	m_uAliveColour = uColour;
}
void 
CGameOfLifePattern::SetDeadColour(UINT32 uColour)
{
	m_uDeadColour = uColour;
}
void 
CGameOfLifePattern::SetColours(UINT32 uAlive, UINT32 uDead)
{
	m_uAliveColour = uAlive;
	m_uDeadColour = uDead;
}
UINT32 
CGameOfLifePattern::AliveColour()
{
	return m_uAliveColour;
}
UINT32 
CGameOfLifePattern::DeadColour()
{
	return m_uDeadColour;
}

bool 
CGameOfLifePattern::Update()
{
	m_fInterval += Ascension::Delta();
	if(m_fInterval >= m_fUpdateInterval)
	{
		m_fInterval -= m_fUpdateInterval;
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				m_pNeighbours[y][x] = 0;
				if(y != 0 && y != m_uHeight-1 && x != 0 && x != m_uWidth-1)//Middle
				{
					if(m_pCurrent[y-1][x])//Top
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x-1])//Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x])//Bottom
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x+1])//Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x-1])//Top Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x-1])//Bottom Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x+1])//Bottom Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x+1])//Top Right
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(y == 0 && x == 0)//Top Left
				{
					if(m_pCurrent[y+1][x])//Bottom
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x+1])//Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x+1])//Bottom Right
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(y == 0 && x == m_uWidth-1)//Top Right
				{
					if(m_pCurrent[y][x-1])//Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x])//Bottom
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x-1])//Bottom Left
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(y == m_uHeight-1 && x == 0)//Bottom Left
				{
					if(m_pCurrent[y-1][x])//Top
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x+1])//Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x+1])//Top Right
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(y == m_uHeight-1 && x == m_uWidth-1)//Bottom Right
				{
					if(m_pCurrent[y-1][x])//Top
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x-1])//Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x-1])//Top Left
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(y ==  0)//Top
				{
					if(m_pCurrent[y][x-1])//Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x])//Bottom
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x+1])//Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x-1])//Bottom Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x+1])//Bottom Right
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(y ==  m_uHeight-1)//Bottom
				{
					if(m_pCurrent[y-1][x])//Top
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x-1])//Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x+1])//Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x-1])//Top Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x+1])//Top Right
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(x ==  0)//Left
				{
					if(m_pCurrent[y-1][x])//Top
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x])//Bottom
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x+1])//Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x+1])//Bottom Right
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x+1])//Top Right
					{
						++m_pNeighbours[y][x];
					}
				}
				else if(x ==  m_uWidth-1)//Right
				{
					if(m_pCurrent[y-1][x])//Top
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y][x-1])//Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x])//Bottom
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y-1][x-1])//Top Left
					{
						++m_pNeighbours[y][x];
					}
					if(m_pCurrent[y+1][x-1])//Bottom Left
					{
						++m_pNeighbours[y][x];
					}
				}
				else
				{
					assert_now("Guts, Unknown position");
				}
			}
		}

		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				if(m_pCurrent[y][x] == false && m_pNeighbours[y][x] == 3)
				{
					m_pCurrent[y][x] = true;
				}
				else if(m_pCurrent[y][x] == true && m_pNeighbours[y][x] == 3 || m_pNeighbours[y][x] == 2)
				{
				}
				else
				{
					m_pCurrent[y][x] = false;
				}
			}
		}

		return true;
	}
	return false;
}

void 
CGameOfLifePattern::DrawInto(UINT32* pTexels, UINT32 uPitch, UINT32 uWidth, UINT32 uHeight, SCoordinates sStart)
{
	if(pTexels)
	{
		for(UINT32 y = sStart.m_sY; y-sStart.m_sY < m_uHeight && y < uHeight; ++y)
		{
			for(UINT32 x = sStart.m_sX; x-sStart.m_sX < m_uWidth && x < uWidth; ++x)
			{
				if(m_pCurrent[y-sStart.m_sY][x-sStart.m_sX])
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
CGameOfLifePattern::AddInto(bool** pPattern, UINT32 uWidth, UINT32 uHeight, SCoordinates sStart)
{
	if(pPattern)
	{
		for(UINT32 y = sStart.m_sY; y < m_uHeight && y-sStart.m_sY < uHeight; ++y)
		{
			for(UINT32 x = sStart.m_sX; x < m_uWidth && x-sStart.m_sX < uWidth; ++x)
			{
				m_pCurrent[y][x] = pPattern[y-sStart.m_sY][x-sStart.m_sX];
			}
		}
	}
	else
	{
		assert_now("Guts, Invalid pattern");
	}
}
UINT32 
CGameOfLifePattern::Width()
{
	return m_uWidth;
}
UINT32 
CGameOfLifePattern::Height()
{
	return m_uHeight;
}

bool** 
CGameOfLifePattern::Pattern()
{
	return m_pOriginal;
}
bool** 
CGameOfLifePattern::CurrentPattern()
{
	return m_pCurrent;
}