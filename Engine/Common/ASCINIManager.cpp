#include "ASCGenInc.h"
#include "ASCINIFile.h"
#include "ASCINIManager.h"
#include "ASCFileIO.h"

#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif

ASCINIManager::ASCINIManager(void)
: m_trFiles ( NULL )
{
}


ASCINIManager::~ASCINIManager(void)
{
	for(UINT32 i = 0; i < m_trFiles.size(); ++i)
	{
		SafeDelete( m_trFiles[i] );
	}
}

bool 
ASCINIManager::LoadFile(ASCString strFile)
{
#ifdef ASC_NEW_INI_SYSTEM
	if(false == m_trFiles.Contains(strFile))
	{
		ASCINIFile* pFile = new ASCINIFile();
		if(pFile->Load(strFile))
		{
			m_trFiles.Add(strFile, pFile);
		}
		else
		{
			SafeDelete(pFile);
		}
	}
	return true;
#else
#ifdef ASC_BINARY_INI_FILES
    //Ascension::Debugging().OutputWindowLine("INI File: ", ASCDebugString( strFile.c_str()));
	ASCFileIO File;
	ASCString strBinaryFile;
	ASCString strExtension;
	ASCString strBinaryExtension;
	strFile.poststrc('.', strBinaryFile);
	strFile.substrc('.', strExtension);
	strBinaryExtension = strExtension;
	strBinaryExtension += ASCString("b");
	strBinaryFile += strBinaryExtension;
	if(false == m_trFiles.Contains(strFile))
	{
#ifdef ASC_IOS
        /*
        Ascension::Debugging().OutputWindowLine("INI File: ", ASCDebugString( strFile.c_str() ));
		ASCFileIO TestFile;
        if(TestFile.Open(strBinaryFile, IO_IN, false))
        {
            TestFile.Close();
            ASCFileIO File;
            Ascension::Debugging().OutputWindowLine("Loaded from binary");
            m_trFiles.Add(strFile, new ASCINIFile());
            if(File.Open(strBinaryFile, IO_IN_BINARY, false))
            {
                void* pData = File.ReadBinary();
                m_trFiles.At(strFile)->LoadBinary(pData);
                SafeDelete( pData );
                File.Close();
            }
        }
        else 
        {
            Ascension::Debugging().OutputWindowLine("Loaded from source");
            TestFile.Close();
            strFile = ASCGeneralFunctions::ConvertFilePathForIOS(strFile);
            
            m_trFiles.Add(strFile, new ASCINIFile());
            m_trFiles.At(strFile)->Initialise(strFile);
            m_trFiles.At(strFile)->Load();
            ASCFileIO File;
            if(File.Open(strBinaryFile, IO_OUT_BINARY, false))
            {
                File.Clear();
                m_trFiles.At(strFile)->Serialize(&File);
                File.Close();
            }
        }
         
        */
		//if(ASCFileIO::FileExists(strBinaryFile))
		//{
            m_trFiles.Add(strFile, new ASCINIFile());
            if(File.Open(strBinaryFile, IO_IN_BINARY))
            {
                //Ascension::Debugging().OutputWindowLine("INI File: ", ASCDebugString( strBinaryFile.c_str() ));
                //Ascension::Debugging().OutputWindowLine("File size: ", File.GetSize( BS_B ));
                void* pData = File.ReadBinary();
                //Ascension::Debugging().OutputWindowLine("Data address: ", pData);
                m_trFiles.At(strFile)->LoadBinary(pData);
                SafeDelete( pData );
                File.Close();
            }
		//}
		//else
		//{
        //    Ascension::Debugging().OutputWindowLine("INI Binary Version Does Not Exits");
		//	m_trFiles.Add(strFile, new ASCINIFile());
		//	m_trFiles.At(strFile)->Initialise(strFile);
		//	m_trFiles.At(strFile)->Load();
		//}
        
         
#else
		ASCFileIO File;
		if(ASCFileIO::FileExists(strBinaryFile))
		{
			if(ASCFileIO::IsNewerThan(strBinaryFile, strFile))
			{
				m_trFiles.Add(strFile, new ASCINIFile());
				if(File.Open(strBinaryFile, IO_IN_BINARY))
				{
					void* pData = File.ReadBinary();
					m_trFiles.At(strFile)->LoadBinary(pData);
					SafeDelete( pData );
					File.Close();
				}
			}
			else
			{
				m_trFiles.Add(strFile, new ASCINIFile());
				m_trFiles.At(strFile)->Initialise(strFile);
				m_trFiles.At(strFile)->Load();
				if(File.Open(strBinaryFile, IO_OUT_BINARY))
				{
					File.Clear();
					m_trFiles.At(strFile)->Serialize(&File);
					File.Close();
				}
			}
		}
		else
		{
			m_trFiles.Add(strFile, new ASCINIFile());
			m_trFiles.At(strFile)->Initialise(strFile);
			m_trFiles.At(strFile)->Load();
			if(File.Open(strBinaryFile, IO_OUT_BINARY))
			{
				File.Clear();
				m_trFiles.At(strFile)->Serialize(&File);
				File.Close();
			}
		}
#endif
	}
	if(bForceLoad)
	{
		m_trFiles.At(strFile)->Load();
	}

	if(bSaveChanges)
	{
		m_veFilesToSave.push_back(strFile);
	}
	return true;
#else
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	if(false == m_trFiles.Contains(strFile))
	{
		m_trFiles.Add(strFile, new ASCINIFile());
		m_trFiles.At(strFile)->Initialise(strFile);
		m_trFiles.At(strFile)->Load();
	}
	if(bForceLoad)
	{
		m_trFiles.At(strFile)->Load();
	}

	if(bSaveChanges)
	{
		m_veFilesToSave.push_back(strFile);
	}
	return true;
#endif
#endif
}
		
ASCString 
ASCINIManager::GetString(ASCString strFile, ASCString strSegment, ASCString strValueID)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	ASCString strReturn = m_trFiles.At(strFile)->GetValue(strSegment, strValueID);
	return strReturn;
}
ASCINIFile* 
ASCINIManager::File(ASCString strFile)
{
	return m_trFiles.At(strFile);
}
UINT32 
ASCINIManager::GetUInt(ASCString strFile, ASCString strSegment, ASCString strValueID)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	ASCINIFile* pFile = m_trFiles.At(strFile);
	ASCString strReturn = pFile->GetValue(strSegment, strValueID);
	return strReturn.UINT();
}
SINT32 
ASCINIManager::GetSInt(ASCString strFile, ASCString strSegment, ASCString strValueID)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	ASCString strReturn = m_trFiles.At(strFile)->GetValue(strSegment, strValueID);
	return strReturn.SINT();
}
CHAR8 
ASCINIManager::GetChar(ASCString strFile, ASCString strSegment, ASCString strValueID)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	ASCString strReturn = m_trFiles.At(strFile)->GetValue(strSegment, strValueID);
	return strReturn[0];
}
FLOAT32 
ASCINIManager::GetFloat(ASCString strFile, ASCString strSegment, ASCString strValueID)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	ASCString strReturn = m_trFiles.At(strFile)->GetValue(strSegment, strValueID);
	return strReturn.FLOAT();
}
bool 
ASCINIManager::GetBool(ASCString strFile, ASCString strSegment, ASCString strValueID)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	ASCString strReturn = m_trFiles.At(strFile)->GetValue(strSegment, strValueID);
	return strReturn.BOOL();
}
		
void 
ASCINIManager::SetValue(ASCString strFile, ASCString strSegment, ASCString strValueID, ASCString strValue)
{
	if(strFile.find('.') < 0)
	{
		strFile += ASCString(".ascini");
	}
	m_trFiles.At(strFile)->SetValue(strSegment, strValueID, strValue);
}
void 
ASCINIManager::DisplayDebugInfo()
{
	Ascension::Debugging().AddScreenLine("INI Manager Info");
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Files in memory: ", m_trFiles.size());
	for(UINT32 i = 0; i < m_trFiles.size(); ++i)
	{
		m_trFiles[i]->DisplayDebugInfo();
	}
}