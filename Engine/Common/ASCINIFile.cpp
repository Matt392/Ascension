#include "ASCGenInc.h"
#include "ASCFileIO.h"
#include "ASCINIFile.h"

#include "../Ascension/Ascension.h"

//ASCINISegment

ASCINISegment::ASCINISegment(void)
: m_strValues()
, m_strSegmentID ( "" )
{
	m_strValues.BadIndex("");
}
ASCINISegment::~ASCINISegment(void)
{
}
void 
ASCINISegment::SetID(ASCString strID)
{
	m_strSegmentID = strID;
}
ASCString& 
ASCINISegment::SegmentID()
{
	return m_strSegmentID;
}
void 
ASCINISegment::SetValue(ASCString& strValueID, ASCString& strValue)
{
	m_strValues.Add(strValueID, strValue);
}
ASCString 
ASCINISegment::GetValue(ASCString& strValueID)
{
	return m_strValues.At(strValueID);
}
void 
ASCINISegment::LoadBinary( void*& pData )
{
	m_strSegmentID.LoadBinary( pData );

	m_strValues.LoadBinary( pData );
}
void 
ASCINISegment::Serialize( ASCFileIO* pFile )
{
	m_strSegmentID.Serialize( pFile );

	m_strValues.Serialize( pFile );
}


//ASCINIFile
ASCINIFile::ASCINIFile()
: m_trSegments()
{
}


ASCINIFile::~ASCINIFile(void)
{
	for(UINT32 i = 0; i < m_trSegments.size(); ++i)
	{
		SafeDelete( m_trSegments[i] );
	}
}
void 
ASCINIFile::LoadBinary( void* pData )
{
	UINT32* pUintData = reinterpret_cast<UINT32*>(pData);
	UINT32 uSegmentCount = pUintData[0];
	++pUintData;
	pData = pUintData;

	m_strID.LoadBinary( pData );
	

	for(UINT32 i = 0; i < uSegmentCount; ++i)
	{
		ASCINISegment* pSegment = new ASCINISegment();
		pSegment->LoadBinary( pData );
		ASCString& strID = pSegment->SegmentID();

		m_trSegments.Add(strID, pSegment);
	}
}
void 
ASCINIFile::Serialize( ASCFileIO* pFile )
{
	UINT32 uSegmentCount = m_trSegments.size();
	pFile->OutputBianry( &uSegmentCount, sizeof(UINT32) );

	m_strID.Serialize( pFile );

	for(UINT32 i = 0; i < uSegmentCount; ++i)
	{
		m_trSegments[i]->Serialize( pFile );
	}
}
bool 
ASCINIFile::Load(CHAR8* strID)
{
	m_strID = strID;
	
	ASCString strBinaryFile;
	ASCString strExtension;
	m_strID.poststrc('.', strBinaryFile);
	m_strID.substrc('.', strExtension);
	strBinaryFile += strExtension;
	strBinaryFile += ASCString("b");
	
	//OUTPUT_WINDOW("INI File: ", ASCDebugString(m_strID.c_str()));
	//OUTPUT_WINDOW("INI Binary File: ", ASCDebugString(strBinaryFile.c_str()));

	if(ASCFileIO::FileExists(strBinaryFile))
	{
		//OUTPUT_WINDOW("Loading binary file");
		if(ASCFileIO::IsNewerThan(strBinaryFile, m_strID))
		{
			ASCFileIO BinaryFile;
			if(BinaryFile.Open(strBinaryFile, IO_IN_BINARY))
			{
				void* pData = BinaryFile.ReadBinary();
				LoadBinary(pData);
				SafeDelete( pData );
			}
		}
		else
		{
			//OUTPUT_WINDOW("Updating old binary file");
			Load();
			ASCFileIO BinaryFile;
			if(BinaryFile.Open(strBinaryFile, IO_OUT_BINARY))
			{
				BinaryFile.Clear();
				Serialize(&BinaryFile);
				BinaryFile.Close();
			}
		}
	}
	else
	{
		//OUTPUT_WINDOW("Loading source file");
		Load();
		//OUTPUT_WINDOW("Creating binary file");
		ASCFileIO BinaryFile;
		if(BinaryFile.Open(strBinaryFile, IO_OUT_BINARY))
		{
			BinaryFile.Clear();
			Serialize(&BinaryFile);
			BinaryFile.Close();
		}
	}

	return true;
}
bool 
ASCINIFile::Load(ASCString& strID)
{
	m_strID = strID;
	
	ASCString strBinaryFile;
	ASCString strExtension;
	m_strID.poststrc('.', strBinaryFile);
	m_strID.substrc('.', strExtension);
	strBinaryFile += strExtension;
	strBinaryFile += ASCString("b");
	
	//OUTPUT_WINDOW("INI File: ", ASCDebugString(m_strID.c_str()));
	//OUTPUT_WINDOW("INI Binary File: ", ASCDebugString(strBinaryFile.c_str()));

	if(ASCFileIO::FileExists(strBinaryFile))
	{
		//OUTPUT_WINDOW("Loading binary file");
		if(ASCFileIO::IsNewerThan(strBinaryFile, m_strID))
		{
			ASCFileIO BinaryFile;
			if(BinaryFile.Open(strBinaryFile, IO_IN_BINARY))
			{
				void* pData = BinaryFile.ReadBinary();
				LoadBinary(pData);
				SafeDelete( pData );
			}
		}
		else
		{
			//OUTPUT_WINDOW("Updating old binary file");
			Load();
			ASCFileIO BinaryFile;
			if(BinaryFile.Open(strBinaryFile, IO_OUT_BINARY))
			{
				BinaryFile.Clear();
				Serialize(&BinaryFile);
				BinaryFile.Close();
			}
		}
	}
	else
	{
		//OUTPUT_WINDOW("Loading source file");
		Load();
		//OUTPUT_WINDOW("Creating binary file");
		ASCFileIO BinaryFile;
		if(BinaryFile.Open(strBinaryFile, IO_OUT_BINARY))
		{
			BinaryFile.Clear();
			Serialize(&BinaryFile);
			BinaryFile.Close();
		}
	}

	return true;
}
void 
ASCINIFile::Initialise(ASCString& strID)
{
	m_strID = strID;
}
void 
ASCINIFile::Load()
{
	if(m_strID.Valid())
	{
		ASCFileIO File;
		if(File.Open(m_strID, IO_IN, false))
		{
			ASCString strLine;
			ASCString strSegment;
			while(File.GetNextLine(strLine))
			{
				SINT32 iStart = strLine.find('<');
				SINT32 iEnd = strLine.find('>');
				SINT32 iPair = strLine.find('/');
				if(iEnd >= 0 && iStart >= 0 && iPair < 0)
				{
					ASCString strTemp;
					strLine.extract(iStart+1, iEnd, strTemp);
					strSegment = strTemp;
					ProcessSegment(strSegment, File);
				}
				//if(strLine.find('<')
			}
			File.Close();
		}
	}
}
void 
ASCINIFile::ProcessSegment(ASCString& strSegment, ASCFileIO& rFile)
{
	ASCString strLine;
	while(rFile.GetNextLine(strLine))
	{
		if(strLine.find('/') < strLine.find('>') && strLine.find('/') > strLine.find('<'))
		{
			break;
		}
		ProcessLine(strSegment, strLine);
	}
}
void 
ASCINIFile::ProcessLine(ASCString& strSegment, ASCString& strLine)
{
	ASCString strInitialLine = strLine; //For debugging
	ASCString strValueName;
	ASCString strValue;

	ASCString strTemp;
	strLine.substrc('<', strTemp);
	strLine = strTemp;
	if(strLine[1] == '!' && strLine[2] == '-' && strLine[2] == '-')
	{
		//It is a comment ignore it
		return;
	}
	if(strLine.find('<') < 0 || strLine.find('>') < 0)
	{
		//Invalid value
		ASCString strDebugInfo = "Guts, Invalid value ( BAD FORMATTING IN LINE ";
		strDebugInfo += strInitialLine;
		strDebugInfo += ASCString(" )");
		assert_now( strDebugInfo.c_str() );
		return;
	}
	strLine.extractc('<', '>', strValueName, 1);
	strLine.substrc('>', strTemp, 1);
	strLine = strTemp;
	if(strLine.find('<') < 0 || strLine.find('>') < 0 || strLine.find('/') < 0)
	{
		//Invalid value
		ASCString strDebugInfo = "Guts, Invalid value ( BAD FORMATTING IN LINE ";
		strDebugInfo += strInitialLine;
		strDebugInfo += ASCString(" )");
		assert_now( strDebugInfo.c_str() );
		return;
	}
	strLine.prestrc('<', strValue);

	SetValue(strSegment, strValueName, strValue);
}
void 
ASCINIFile::SetValue(ASCString strSegmentID, ASCString strValueID, ASCString strValue)
{
	if(m_trSegments.Contains(strSegmentID))
	{
		m_trSegments.At(strSegmentID)->SetValue(strValueID, strValue);
	}
	else
	{
		ASCINISegment* pSegment = new ASCINISegment();
		pSegment->SetID( strSegmentID );
		m_trSegments.Add(strSegmentID, pSegment);
		m_trSegments.At(strSegmentID)->SetValue(strValueID, strValue);
	}
}
ASCString 
ASCINIFile::GetValue(ASCString& strSegmentID, ASCString& strValueID)
{
	if(m_trSegments.Contains(strSegmentID))
	{
		return m_trSegments.At(strSegmentID)->GetValue(strValueID);
	}
	else
	{
		return g_kstrInvalid;
	}
}
		
ASCString 
ASCINIFile::GetString(ASCString strSegment, ASCString strValueID)
{
	if(m_trSegments.Contains(strSegment))
	{
		return m_trSegments.At(strSegment)->GetValue(strValueID);
	}
	else
	{
		return g_kstrInvalid;
	}
}
UINT32 
ASCINIFile::GetUInt(ASCString strSegment, ASCString strValueID)
{
	if(m_trSegments.Contains(strSegment))
	{
		return m_trSegments.At(strSegment)->GetValue(strValueID).UINT();
	}
	else
	{
		return 0;
	}
}
SINT32 
ASCINIFile::GetSInt(ASCString strSegment, ASCString strValueID)
{
	if(m_trSegments.Contains(strSegment))
	{
		return m_trSegments.At(strSegment)->GetValue(strValueID).SINT();
	}
	else
	{
		return 0;
	}
}
CHAR8
ASCINIFile::GetChar(ASCString strSegment, ASCString strValueID)
{
	if(m_trSegments.Contains(strSegment))
	{
		return m_trSegments.At(strSegment)->GetValue(strValueID)[0];
	}
	else
	{
		return 0;
	}
}
FLOAT32 
ASCINIFile::GetFloat(ASCString strSegment, ASCString strValueID)
{
	if(m_trSegments.Contains(strSegment))
	{
		return m_trSegments.At(strSegment)->GetValue(strValueID).FLOAT();
	}
	else
	{
		return 0;
	}
}
bool 
ASCINIFile::GetBool(ASCString strSegment, ASCString strValueID)
{
	if(m_trSegments.Contains(strSegment))
	{
		return m_trSegments.At(strSegment)->GetValue(strValueID).BOOL();
	}
	else
	{
		return 0;
	}
}
void 
ASCINIFile::DisplayDebugInfo()
{
	ASCString strInfo = m_strID;
	strInfo += ASCString(", Segment Count: ");
	strInfo.AddSmallInt( m_trSegments.size() );
	Ascension::Debugging().AddScreenLine(strInfo);
}