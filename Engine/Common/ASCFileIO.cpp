#include "ASCGenInc.h"
#include "ASCFileIO.h"

#ifdef ASC_WINDOWS
#include <Windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#endif
#include <iostream>

ASCFileIO::ASCFileIO( void )
: m_bOpen ( false )
{
}

ASCFileIO::~ASCFileIO( void )
{
}
bool 
ASCFileIO::Open(ASCString strFile, EInOut eInOut, bool bFromBundle )
{
    bFromBundle;
#ifdef ASC_IOS
    if(bFromBundle)
    {
        strFile = ASCGeneralFunctions::ConvertFilePathForIOS(strFile);
    }
    else 
    {
        strFile = ASCGeneralFunctions::ConvertDocumentsPathForIOS(strFile);
    }
    //ASCString strFile
#endif
	m_eInOut = eInOut;
	switch(m_eInOut)
	{
	case IO_IN:
		{
			return FileIn(strFile);
		}
	case IO_OUT:
		{
			return FileOut(strFile);
		}
	case IO_OUT_BINARY:
		{
			return FileOutBinary(strFile);
		}
	case IO_IN_BINARY:
		{
			return FileInBinary(strFile);
		}
	case IO_BOTH:
	default:
		{
			return false;
		}
	}
}
bool 
ASCFileIO::Close()
{
	m_InputStream.close();
	m_OutputStream.close();
	return true;
}
bool 
ASCFileIO::FileIn( ASCString& strFile )
{
	m_InputStream.open( strFile.c_str(), std::fstream::in );
	m_bOpen = m_InputStream.is_open();
	return m_bOpen;
}
bool 
ASCFileIO::FileOut( ASCString& strFile )
{
	m_OutputStream.open( strFile.c_str(), std::fstream::out );
	m_bOpen = m_OutputStream.is_open();
	return m_bOpen;
}
bool 
ASCFileIO::FileInBinary( ASCString& strFile )
{
	m_InputStream.open( strFile.c_str(), std::fstream::binary );
	m_bOpen = m_InputStream.is_open();
	return m_bOpen;
}
bool 
ASCFileIO::FileOutBinary( ASCString& strFile )
{
	m_OutputStream.open( strFile.c_str(), std::fstream::binary );
	m_bOpen = m_OutputStream.is_open();
	return m_bOpen;
}
bool
ASCFileIO::GetNextLine( ASCString& strReturn )
{
	bool bReturn = false;
	if(m_bOpen == true /*&& m_InputStream.eof() == false*/)
	{
		char strBuffer[256];
		bReturn = m_InputStream.getline(strBuffer, 256) != '\0';
		strReturn = strBuffer;
	}
	return bReturn;
}
void* 
ASCFileIO::ReadBinary()
{
	if(m_bOpen)
	{
		UINT32 uSize = GetSize(BS_B);
		CHAR8* pData = new CHAR8[uSize];
		m_InputStream.read(pData, uSize);
		return pData;
	}
	return NULL;
}
UINT32 
ASCFileIO::GetSize(EByteSize eByteSize)
{
	if(m_eInOut == IO_IN || m_eInOut == IO_IN_BINARY)
	{
		m_InputStream.seekg(0, std::ios::end);
		UINT32 uSize = SC_UINT(m_InputStream.tellg());
		for(UINT32 i = 0; i < SC_UINT(eByteSize); ++i)
		{
			uSize /= 1024;
		}
		m_InputStream.seekg(0, std::ios::beg);
		return  uSize;
	}
	return 0;
}
ASCString 
ASCFileIO::GetSize()
{
	if(m_eInOut == IO_IN)
	{
		ASCString strReturn;
		m_InputStream.seekg(0, std::ios::end);
		FLOAT32 fSize = SC_FLOAT(m_InputStream.tellg());
		UINT32 i = 0;
		for(i = 0; i < BS_COUNT && fSize <= 1024.0f; ++i)
		{
			fSize /= 1024.0f;
		}
		static const ASCString strUnits[BS_COUNT] = { " B", " KB", " MB", " GB", " TB" };
		strReturn = SC_FLOAT(fSize);
		strReturn += strUnits[i];
		return strReturn;
	}
	return "Invalid File";
}
void 
ASCFileIO::PrintLine(ASCString& strLine)
{
	if(m_bOpen == true)
	{
		m_OutputStream << strLine.c_str() << "\n";
	}
}
void 
ASCFileIO::PrintLine(const CHAR8* strFile)
{
	if(m_bOpen == true)
	{
		m_OutputStream << strFile << "\n";
	}
}
void 
ASCFileIO::Print(ASCString& str)
{
	if(m_bOpen == true)
	{
		m_OutputStream << str.c_str();
	}
}
void 
ASCFileIO::OutputBianry( void* pData, UINT32 uCount )
{
	UINT32 uConvertedSize = uCount / sizeof(CHAR8);
	m_OutputStream.write (reinterpret_cast<CHAR8*>(pData), uConvertedSize);
}
void 
ASCFileIO::Clear()
{
	if(m_bOpen == true)
	{
		m_OutputStream.clear();
	}
}
bool 
ASCFileIO::FileExists(ASCString strFile, bool bFromBundle)
{
	ASCFileIO File;
	if(File.Open(strFile, IO_IN, bFromBundle))
	{
		File.Close();
		return true;
	}
	return false;
//#ifdef ASC_WINDOWS
//	
//	FILE* pFile = NULL;
//	fopen_s(&pFile, strFile.c_str(), "r");
//
//	if(pFile)
//	{
//		fclose(pFile);
//		return true;
//	}
//	return false;
//    
//#else
//#ifdef ASC_IOS
//    return false;
//#else
//    return false;
//#endif
//#endif
}
bool 
ASCFileIO::IsNewerThan(ASCString strFirst, ASCString strOther)
{
#ifdef ASC_WINDOWS
	struct tm pFirstTime;
	struct tm pOtherTime;
	FILE* pFirstFile = NULL;
	fopen_s(&pFirstFile, strFirst.c_str(), "r");
	if(pFirstFile)
	{
		struct stat attrib;
		stat(strFirst.c_str(), &attrib);

		gmtime_s(&pFirstTime, &(attrib.st_mtime));

		fclose(pFirstFile);
		
		FILE* pOtherFile = NULL;
		fopen_s(&pOtherFile, strOther.c_str(), "r");
		if(pOtherFile)
		{
			struct stat attrib;
			stat(strOther.c_str(), &attrib);

			gmtime_s(&pOtherTime, &(attrib.st_mtime));

			fclose(pOtherFile);
									
			if(pFirstTime.tm_year > pOtherTime.tm_year)
			{
				return true;
			}
			else if(pFirstTime.tm_year == pOtherTime.tm_year)
			{
				if(pFirstTime.tm_yday > pOtherTime.tm_yday)
				{
					return true;
				}
				else if(pFirstTime.tm_yday == pOtherTime.tm_yday)
				{
					if(pFirstTime.tm_hour > pOtherTime.tm_hour)
					{
						return true;
					}
					else if(pFirstTime.tm_hour == pOtherTime.tm_hour)
					{
						if(pFirstTime.tm_min > pOtherTime.tm_min)
						{
							return true;
						}
						else if(pFirstTime.tm_min == pOtherTime.tm_min)
						{
							if(pFirstTime.tm_sec >= pOtherTime.tm_sec)
							{
								return true;
							}
						}
					}
				}
			}
			return false;
		}
		else
		{
			assert_now("Guts, Failed to open file");
		}
	}
	else
	{
		assert_now("Guts, Failed to open file");
	}

	return false;

#else
	return false;
#endif
}