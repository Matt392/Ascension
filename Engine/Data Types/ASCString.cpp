#include "ASCGenInc.h"
#include "ASCString.h"
#include <fstream>
#include <stdio.h>

//#include "ASCMemoryManagement.h"
#include "ASCStringRecyleBin.h"
#include "ASCFileIO.h"

#include "ASCStringDebugInfo.h"
#include "ASCFinalShutdownDelay.h"

//Constructors
ASCString::ASCString(void)
: m_strString ( NULL )
, m_uMaxSize ( m_kiInitialStringSize )
, m_iLetterCount ( 0 )
{
	ASCFinalShutdownDelay::AddDelay();
}
ASCString::ASCString(const CHAR8* str)
: m_strString ( NULL )
, m_uMaxSize ( m_kiInitialStringSize )
, m_iLetterCount ( 0 )
{
	*this = str;
	ASCFinalShutdownDelay::AddDelay();
}
ASCString::ASCString(const ASCString& str)
: m_strString ( NULL )
, m_uMaxSize ( m_kiInitialStringSize )
, m_iLetterCount ( 0 )
{
	*this = str;
	ASCFinalShutdownDelay::AddDelay();
}


//Destructors
ASCString::~ASCString(void)
{
	ASCStringRecyleBin::RecyleString(m_strString, m_uMaxSize);
	ASCFinalShutdownDelay::RemoveDelay();
}


//Info
UINT32 
ASCString::length()
{
	return m_iLetterCount;
}
SINT32 
ASCString::find( const CHAR8* str )
{
	SINT32 iStart = find(str[0]);
	if(iStart < 0)
	{
		return -1;
	}

	UINT32 i = 1;
	while(str[i] != 0)
	{
		if(find(str[i]) < 0)
		{
			return -1;
		}
		++i;
	}
	return iStart;
}
SINT32 
ASCString::find( const CHAR8 cLetter )
{
	for(UINT32 i = 0; i < m_iLetterCount;++i)
	{
		if(m_strString[i] == cLetter)
		{
			return i;
		}
	}
	return -1;
}
SINT32 
ASCString::findlast( const CHAR8 cLetter )
{
	for(SINT32 i = m_iLetterCount-1; i >= 0;--i)
	{
		if(m_strString[i] == cLetter)
		{
			return i;
		}
	}
	return -1;
}

bool 
ASCString::Compare(ASCString& str)
{
	if(m_iLetterCount != str.m_iLetterCount)
	{
		return false;
	}
	for(UINT32 i = 0; i < m_iLetterCount; ++i)
	{
		if(ks_ilutLetterValues[m_strString[i]] != ks_ilutLetterValues[str.m_strString[i]])
		{
			return false;
		}
	}
	return true;
}


//Gets
UINT32 
ASCString::UINT()
{
	return atoi(m_strString);
}
SINT32 
ASCString::SINT()
{
	return atoi(m_strString);
}
FLOAT32 
ASCString::FLOAT()
{
	return SC_FLOAT(atof(m_strString));
}
bool 
ASCString::BOOL()
{
	static CHAR8 sklutTrues[4][8] = { "true", "t", "T", "True" };
	ASCString strComp; 
	for(UINT32 i = 0; i < 4; ++i)
	{
		strComp = sklutTrues[i];
		if(Compare(strComp))
		{
			return true;
		}
	}
	return false;
}

CHAR8* 
ASCString::c_str()
{
	return m_strString;
}
bool 
ASCString::Valid()
{
	return m_iLetterCount > 0;
}
UINT32 
ASCString::Max()
{
	return m_uMaxSize;
}
void 
ASCString::Clear()
{
	for(UINT32 i = 0; i < m_iLetterCount; ++i)
	{
		m_strString[i] = 0;
	}
	m_iLetterCount = 0;
}


//Operations
void 
ASCString::substr( UINT32 iAt, ASCString& strReturn )
{
	strReturn = "";
	for(UINT32 i = iAt; i < m_iLetterCount; ++i)
	{
		strReturn += m_strString[i];
	}
}
void 
ASCString::substrc( const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset)
{
	substr(find(cLetter)+uOffset, strReturn);
}
void 
ASCString::sublaststrc( const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset)
{
	substr(findlast(cLetter)+uOffset, strReturn);
}

void 
ASCString::split( SINT32 iAt, ASCString& str1, ASCString& str2 )
{
	ASCString strTemp1;
	for(SINT32 i = 0; i < iAt; ++i)
	{
		strTemp1 += m_strString[i];
	}
	str1 = strTemp1;

	ASCString strTemp2;
	for(UINT32 i = iAt; i < m_iLetterCount; ++i)
	{
		strTemp2 += m_strString[i];
	}
	str2 = strTemp2;
}
void 
ASCString::splitc( const CHAR8 cLetter, ASCString& str1, ASCString& str2, SINT32 uOffset )
{
	return split(find(cLetter)+uOffset, str1, str2);
}

void 
ASCString::prestr( UINT32 iAt, ASCString& strReturn )
{
	strReturn = "";
	for(UINT32 i = 0; i < iAt && i < m_iLetterCount; ++i)
	{
		strReturn += m_strString[i];
	}
}
void 
ASCString::prestrc( const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset )
{
	prestr(find(cLetter)+uOffset, strReturn);
}
void 
ASCString::poststrc (const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset )
{
	prestr(findlast(cLetter)+uOffset, strReturn);
}

void 
ASCString::extract( UINT32 uStart, UINT32 uEnd, ASCString& strReturn)
{
	if(uStart < uEnd && uEnd <= length())
	{
		ASCString strTemp;
		strReturn = "";
		prestr(uEnd, strTemp);
		strTemp.substr(uStart, strReturn);
		return;
	}
	assert_now("Guts, Bad indexes");
}
void 
ASCString::extractc( CHAR8 cStart, CHAR8 cEnd, ASCString& strReturn, SINT32 iOffsetStart, SINT32 iOffsetEnd)
{
	return extract(find(cStart)+iOffsetStart, find(cEnd)+iOffsetEnd, strReturn);
}


//Other
void 
ASCString::Resize()
{
	UINT32 uNewSize = m_uMaxSize + m_kiIncreaseStringSize;

	CHAR8* cTemp = NULL;
	uNewSize = ASCStringRecyleBin::GetString(uNewSize, cTemp);

	for(UINT32 i = 0; i < m_uMaxSize; ++i)
	{
		cTemp[i] = m_strString[i];
	}
	
	ASCStringRecyleBin::RecyleString(m_strString, uNewSize);

	m_uMaxSize = uNewSize;
	m_strString = cTemp;

	for(UINT32 i = m_iLetterCount; i < m_uMaxSize; ++i)
	{
		m_strString[i] = 0;
	}
}
void 
ASCString::AddFloatOneDP(FLOAT32& rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);

	ASCString strReturn;
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%0.1f", c_str(), rVal);
#else
	sprintf_s(cStrVal, uSize, "%s%0.1f", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::AddFloatHP(FLOAT32& rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);

	ASCString strReturn;
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%0.7f", c_str(), rVal);
#else
	sprintf_s(cStrVal, uSize, "%s%0.7f", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::AddFloatLP(FLOAT32& rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);

	ASCString strReturn;
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%0.1f", c_str(), rVal);
#else
	sprintf_s(cStrVal, uSize, "%s%0.1f", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::AddSmallInt(UINT32 uVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);
	
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%d", c_str(), uVal);
#else
	sprintf_s(cStrVal, uSize, "%s%d", c_str(), uVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::RemoveLast()
{
	--m_iLetterCount;
	m_strString[m_iLetterCount] = NULL;
}

//Operators
CHAR8& 
ASCString::operator[](const UINT32& iIndex)
{
	return m_strString[iIndex];
}
ASCString 
ASCString::operator+(ASCString rString)
{
	UINT32 uSize = 0;
	//if(rString.m_uMaxSize > m_uMaxSize)
	//{
	//	uSize = rString.m_uMaxSize;
	//}
	//else
	//{
	//	uSize = m_uMaxSize;
	//}
	uSize = m_uMaxSize + rString.m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);

	ASCString strReturn;

#ifdef ASC_IOS
    sprintf(cStrVal, "%s%s", c_str(), rString.c_str());
#else
	sprintf_s(cStrVal, uSize, "%s%s", c_str(), rString.c_str());
#endif
    strReturn = cStrVal;

	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return strReturn;
}
void 
ASCString::operator=(const CHAR8*& str)
{
	m_iLetterCount = 0;
	while(str[m_iLetterCount] != 0)
	{
		++m_iLetterCount;
	}
	if(m_iLetterCount >= m_uMaxSize || m_strString == NULL)
	{
		if(m_strString)
		{
			ASCStringRecyleBin::RecyleString(m_strString, m_uMaxSize);
		}
		m_uMaxSize = m_iLetterCount + m_kiIncreaseStringSize;
		m_uMaxSize = ASCStringRecyleBin::GetString(m_uMaxSize, m_strString);
	}

	for(UINT32 i = 0; i < m_iLetterCount; ++i)
	{
		m_strString[i] = str[i];
	}
	m_strString[m_iLetterCount] = 0;
}
void 
ASCString::operator=(const ASCString& str)
{
	if(str.m_iLetterCount >= m_uMaxSize || m_strString == NULL)
	{
		if(m_strString)
		{
			ASCStringRecyleBin::RecyleString(m_strString, m_uMaxSize);
		}
		m_uMaxSize = str.m_uMaxSize;
		m_uMaxSize = ASCStringRecyleBin::GetString(m_uMaxSize, m_strString);
	}
	m_iLetterCount = str.m_iLetterCount;
	
	
	for(UINT32 i = 0; i < m_iLetterCount; ++i)
	{
		m_strString[i] = str.m_strString[i];
	}
	for(UINT32 i = m_iLetterCount; i < m_uMaxSize; ++i)
	{
		m_strString[i] = 0;
	}
	m_strString[m_iLetterCount] = 0;
}
void 
ASCString::operator=(CHAR8 cLetter)
{
	if(m_uMaxSize > 2)
	{
		m_iLetterCount = 1;
		m_strString[0] = cLetter;
		m_strString[m_iLetterCount] = 0;
	}
	else
	{
		CHAR8 strTemp[2];
		strTemp[0] = cLetter;
		strTemp[1] = 0;
		*this = strTemp;
	}
}
void 
ASCString::operator=(FLOAT32 fNumber)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);

	ASCString strReturn;

#ifdef ASC_IOS
	sprintf(cStrVal, "%0.3f", fNumber);
#else
	sprintf_s(cStrVal, uSize, "%0.3f", fNumber);
#endif
    *this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::operator+=(ASCString rString)
{
	if(m_iLetterCount + rString.m_iLetterCount >= m_uMaxSize)
	{
		UINT32 uSize = m_uMaxSize + rString.m_uMaxSize;

		CHAR8* cStrVal = NULL;
		uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);
        
#ifdef ASC_IOS
		sprintf(cStrVal, "%s%s", c_str(), rString.c_str());
#else
		sprintf_s(cStrVal, uSize, "%s%s", c_str(), rString.c_str());
#endif
		*this = cStrVal;
	
		ASCStringRecyleBin::RecyleString(cStrVal, uSize);
	}
	else
	{
		for(UINT32 i = m_iLetterCount; i < m_iLetterCount + rString.m_iLetterCount; ++i)
		{
			m_strString[i] = rString.m_strString[i-m_iLetterCount];
		}
		m_iLetterCount = m_iLetterCount + rString.m_iLetterCount;
		m_strString[m_iLetterCount] = 0;
	}
	return;
}
void 
ASCString::operator+=(FLOAT32& rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);
	
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%0.3f", c_str(), rVal);
#else
	sprintf_s(cStrVal, uSize, "%s%0.3f", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::operator+=(UINT32& rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);
	
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%d", c_str(), rVal);
#else
	sprintf_s(cStrVal, uSize, "%s%d", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::operator+=(SINT32& rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);
	
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%d", c_str(), rVal);
#else
	sprintf_s(cStrVal, uSize, "%s%d", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::operator+=(void* rVal)
{
	UINT32 uSize = m_uMaxSize + m_uMaxSize;

	CHAR8* cStrVal = NULL;
	uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);
	
#ifdef ASC_IOS
    UINT32 uVal = reinterpret_cast<UINT32>(rVal);
	sprintf(cStrVal, "%s%x", c_str(), uVal);
#else
	sprintf_s(cStrVal, uSize, "%s%x", c_str(), rVal);
#endif
	*this = cStrVal;
	
	ASCStringRecyleBin::RecyleString(cStrVal, uSize);

	return;
}
void 
ASCString::operator+=(CHAR8 cLetter)
{
	if(m_uMaxSize > m_iLetterCount+2 && NULL != m_strString)
	{
		m_strString[m_iLetterCount] = cLetter;
		++m_iLetterCount;
		m_strString[m_iLetterCount] = 0;
	}
	else
	{
		if(NULL == m_strString)
		{
			m_uMaxSize = ASCStringRecyleBin::GetString(m_uMaxSize, m_strString);
			m_strString[0] = NULL;
		}
		UINT32 uSize = m_uMaxSize + m_uMaxSize;

		CHAR8* cStrVal = NULL;
		uSize = ASCStringRecyleBin::GetString(uSize, cStrVal);

		CHAR8 cStrTemp[2];	
		cStrTemp[0] = cLetter;
		cStrTemp[1] = 0;
		CHAR8* strTemp = cStrTemp;

#ifdef ASC_IOS
        sprintf(cStrVal, "%s%s", c_str(), strTemp);
#else
		sprintf_s(cStrVal, uSize, "%s%s", c_str(), strTemp);
#endif
		*this = cStrVal;
		
		ASCStringRecyleBin::RecyleString(cStrVal, uSize);
	}
	return;
}

bool 
ASCString::operator==(ASCString& rString)
{
	return Compare(rString);
}
bool 
ASCString::operator==(CHAR8* rString)
{
	ASCString strTemp = rString;
	return Compare(strTemp);
}
bool 
ASCString::operator!=(ASCString& rString)
{
	return !(*(this)==rString);
}
bool 
ASCString::operator!=(CHAR8* rString)
{
	return !(*(this)==rString);
}
bool 
ASCString::operator>(ASCString& rString)
{
#ifdef ASC_IOS
	return strcmp(c_str(), rString.c_str()) > 0;
#else
	return _stricmp(c_str(), rString.c_str()) > 0;
#endif
}
bool 
ASCString::operator<(ASCString& rString)
{
#ifdef ASC_IOS
	return strcmp(c_str(), rString.c_str()) < 0;
#else
	return _stricmp(c_str(), rString.c_str()) < 0;
#endif
}
bool 
ASCString::operator>=(ASCString& rString)
{
	UINT32 iLegth1 = length();
	UINT32 iStr1Add = 0;
	UINT32 iStr2Add = 0;
	for(UINT32 i = 0; i < iLegth1; ++i)//97  65
	{
		iStr1Add = 0;
		iStr2Add = 0;
		if(m_strString[i] < 97)
		{
			iStr1Add = 32;
		}
		if(rString[i] < 97)
		{
			iStr2Add = 32;
		}
		if(m_strString[i] == 0 || m_strString[i] + iStr1Add >= rString.m_strString[i] + iStr2Add)
		{
			return true;
		}
		if(rString[i] == 0)
		{
			return false;
		}
	}

	return false;
}
bool 
ASCString::operator<=(ASCString& rString)
{
	UINT32 iLegth1 = length();
	UINT32 iStr1Add = 0;
	UINT32 iStr2Add = 0;
	for(UINT32 i = 0; i < iLegth1; ++i)//97  65
	{
		iStr1Add = 0;
		iStr2Add = 0;
		if(m_strString[i] < 97)
		{
			iStr1Add = 32;
		}
		if(rString[i] < 97)
		{
			iStr2Add = 32;
		}
		if(m_strString[i] == 0 || m_strString[i] + iStr1Add >= rString.m_strString[i] + iStr2Add)
		{
			return false;
		}
		if(rString[i] == 0)
		{
			return true;
		}
	}

	return true;
}

void 
ASCString::LoadBinary(void*& pData)
{
	UINT32* pUintData = reinterpret_cast<UINT32*>(pData);

	m_iLetterCount = pUintData[0];
	//Move to next value
	++pUintData;
	
	//The remaining data is the string itself
	CHAR8* pCharData = reinterpret_cast<CHAR8*>(pUintData);
	*this = pCharData;
	for(UINT32 i = 0; i < m_iLetterCount+1; ++i)
	{
		++pCharData;
	}
	pData = pCharData;
}
void 
ASCString::Serialize(ASCFileIO* pFile)
{
	pFile->OutputBianry(&m_iLetterCount, sizeof(UINT32));
	if(NULL != m_strString)
	{
		pFile->OutputBianry(m_strString, sizeof(CHAR8) * m_iLetterCount+1);
	}
	else
	{
		CHAR8 cTemp = ' ';
		pFile->OutputBianry(&cTemp, sizeof(CHAR8));
	}
}