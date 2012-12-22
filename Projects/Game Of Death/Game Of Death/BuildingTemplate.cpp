#include "ASCGenInc.h"
#include "BuildingTemplate.h"

#include "ASCFileIO.h"

CBuildingTemplate::CBuildingTemplate(void)
{
}


CBuildingTemplate::~CBuildingTemplate(void)
{
}

bool 
CBuildingTemplate::Initialise( ASCString& strFile, UINT32* pLookUp )
{
	ASCFileIO File;
	ASCVector<ASCString> vecLines;
	ASCString strLine;
	UINT32 uLongestLine = 0;

	if(File.Open(strFile, IO_IN))
	{
		while(File.GetNextLine(strLine))
		{
			if(strLine.length() > uLongestLine)
			{
				uLongestLine = strLine.length();
			}
			vecLines.push_back( strLine );
		}
		File.Close();
	}
	if(vecLines.size() > 0)
	{
		m_sPattern.Initialise(uLongestLine, vecLines.size());
		for(UINT32 i = 0; i < m_sPattern.m_uHeight; ++i)
		{
			for(UINT32 j = 0; j < vecLines[i].length(); ++j)
			{
				m_sPattern.m_pPattern[i][j] = pLookUp[vecLines[i][j]];
			}
		}

		return true;
	}

	return false;
}

SPatternTemplate& 
CBuildingTemplate::Template()
{
	return m_sPattern;
}