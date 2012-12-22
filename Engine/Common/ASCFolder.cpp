#include "ASCGenInc.h"
#include "ASCFolder.h"
#include "ASCFileIO.h"


#ifdef ASC_IOS
#include "Ascension.h"
#else
#include <windows.h>
#include "../Ascension/Ascension.h"
#endif

ASCFolder::ASCFolder(void)
{
}


ASCFolder::~ASCFolder(void)
{
}

#ifdef ASC_IOS
bool 
ASCFolder::Load(ASCString strDirectory)
{
	m_strPath = strDirectory;
	while(strDirectory.find('/') >= 0)
	{
		strDirectory[strDirectory.find('/')] = '_';
	}

	strDirectory += ASCString(".folder");

	ASCFileIO File;
	if( File.Open(strDirectory, IO_IN) )
	{
		ASCString strLine;

		while(File.GetNextLine(strLine))
		{
			ASCString strTemp = m_strPath;
			ASCString strTemp2;
			strTemp += ASCString("/");
			strTemp += strLine;
			if(strTemp.find('.') > 0)
			{
                if(strTemp.find('\r') > strTemp.find('.'))
                {
                    strTemp.poststrc('\r', strTemp2);
                    strTemp = strTemp2;
                }
                //for(UINT32 i = strTemp.length(); i < strTemp.Max(); ++i)
                //{
                //    strTemp[i] = 0;
                //}
				m_strFiles.push_back(strTemp);
			}
			else
			{
                if(strLine.find('\r') > 0)
                {
                    strLine.poststrc('\r', strTemp2);
                    strLine = strTemp2;
                }
                //strTemp2 = strLine;
                //strTemp2 += ASCString("Test");
                //Ascension::Debugging().OutputWindowLine("Slash R Test: ", ASCDebugString( strTemp2.c_str()));
				m_strFolders.push_back(strLine);
			}
		}

		File.Close();
		return true;
	}
	return false;
}
#else
bool 
ASCFolder::Load(ASCString strDirectory)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	m_strPath = strDirectory;
	ASCString strTempDirectory = strDirectory;
	strDirectory += ASCString("/*");

	hFind = FindFirstFileA(strDirectory.c_str(), &FindData);

	while(hFind != NULL)
	{
		ASCString strFile = ASCString(FindData.cFileName);
		ASCString strTemp = strTempDirectory;
		strTemp += ASCString("/");
		strTemp += strFile;
		if(strFile.find('.') > 0)
		{
			m_strFiles.push_back(strTemp);
		}
		else if(strFile.find('.') < 0)
		{
			m_strFolders.push_back(strTemp);
		}

		if(false == FindNextFileA(hFind, &FindData))
		{
			return true;
		}
	}
	return false;
}
#endif

ASCString 
ASCFolder::FolderPath()
{
	return m_strPath;
}
		
ASCVector<ASCString>* 
ASCFolder::Files()
{
	return &m_strFiles;
}
ASCVector<ASCString>* 
ASCFolder::Folders()
{
	return &m_strFolders;
}
void 
ASCFolder::SaveDirectoryInfo()
{
	ASCString strTemp;
	ASCString strFileName = m_strPath;
	while(strFileName.find('/') >= 0)
	{
		strFileName[strFileName.find('/')] = '_';
	}
	strFileName += ASCString(".folder");
	ASCFileIO File;
	if(File.Open(strFileName, IO_OUT))
	{
		for(UINT32 i = 0; i < m_strFiles.size(); ++i)
		{
			strTemp = m_strFiles[i];
			File.PrintLine(strTemp);
		}
		for(UINT32 i = 0; i < m_strFolders.size(); ++i)
		{
			strTemp = m_strFolders[i];
			File.PrintLine(strTemp);
		}
		File.Close();
	}

	
	for(UINT32 i = 0; i < m_strFolders.size(); ++i)
	{
		ASCFolder Folder;
		Folder.Load(m_strFolders[i]);
		Folder.SaveDirectoryInfo();
	}
}