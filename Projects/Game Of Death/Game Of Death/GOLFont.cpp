#include "ASCGenInc.h"
#include "GOLFont.h"

#include "ASCFolder.h"

#include "GameOfLifePattern.h"

CGOLFont::CGOLFont(void)
{
}


CGOLFont::~CGOLFont(void)
{
	for(UINT32 i = 0; i < m_tLetters.size(); ++i)
	{
		SafeDelete( m_tLetters[i] );
	}
}

bool 
CGOLFont::Initialise(CHAR8* strFile, UINT32 uAliveColour, UINT32 uDeadColour)
{
	ASCString strString = strFile;
	return Initialise(strString, uAliveColour, uDeadColour);
}
bool 
CGOLFont::Initialise(ASCString& strFile, UINT32 uAliveColour, UINT32 uDeadColour)
{
	m_tLetters.BadIndex(NULL);
	ASCFolder Folder;
	if(Folder.Load(strFile))
	{
		ASCVector<ASCString>& rVecFiles = *Folder.Files();
		for(UINT32 i = 0; i < rVecFiles.size(); ++i)
		{
			CGameOfLifePattern* pPattern = new CGameOfLifePattern();
			if(pPattern->Initialise(rVecFiles[i], uAliveColour, uDeadColour, 1.0f))
			{
				ASCString strID;
				ASCString strTemp;
				rVecFiles[i].sublaststrc('/', strTemp, 1);
				strTemp.prestrc('.', strID);
				m_tLetters.Add(strID, pPattern);
			}
			else
			{
				SafeDelete( pPattern );
			}
		}

		return true;
	}

	return false;
}
void 
CGOLFont::DrawInto(ASCString strLine, SCoordinates sCoord, UINT32* pTexels, UINT32 uPitch, UINT32 uWidth, UINT32 uHeight)
{
	UINT32 uLength = strLine.length();

	for(UINT32 i = 0; i < uLength; ++i)
	{
		CHAR8 cVal[2];
		cVal[0] = strLine[i];
		cVal[1] = 0;
		ASCString strVal = &cVal[0];

		CGameOfLifePattern* pPattern = m_tLetters.At( strVal );
		if(pPattern != NULL)
		{
			UINT32 uPWidth = pPattern->Width();
			UINT32 uPHeight = pPattern->Height();
			
			UINT32 uAlive = pPattern->AliveColour();
			UINT32 uDead = pPattern-> DeadColour();

			bool** pCurPattern = pPattern->CurrentPattern();

			for(UINT32 y = sCoord.m_sY; y < uHeight && y-sCoord.m_sY < uPHeight; ++y)
			{
				for(UINT32 x = sCoord.m_sX; x < uWidth && x-sCoord.m_sX < uPWidth; ++x)
				{
					if(pCurPattern[y-sCoord.m_sY][x-sCoord.m_sX])
					{
						pTexels[(uPitch*y)+x] = uAlive;
					}
					else
					{
						pTexels[(uPitch*y)+x] = uDead;
					}
				}
			}
			sCoord.m_sX += uPWidth;
		}
		else
		{
			assert_now("Guts, Unknown pattern");
		}
	}
}