#include "ASCGenInc.h"
#include "ASCDebugString.h"
#include <fstream>
#include <stdio.h>

#include "ASCMemoryManagement.h"



ASCDebugString::ASCDebugString(void)
: m_iLetterCount ( 0 )
{
	for(UINT32 i = 0; i < m_kiMaxStrLength; ++i)
	{
		m_strString[i] = 0;
	}
}
ASCDebugString::ASCDebugString(const CHAR8* _str)
: m_iLetterCount ( 0 )
{
	for(UINT32 i = 0; i < m_kiMaxStrLength; ++i)
	{
		m_strString[i] = 0;
	}
	*this = _str;
}

ASCDebugString::~ASCDebugString(void)
{
}

CHAR8* 
ASCDebugString::c_str()
{
	return m_strString;
}
UINT32 
ASCDebugString::length()
{
	return m_iLetterCount;
}
SINT32 
ASCDebugString::find( const CHAR8* str )
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
ASCDebugString::find( const CHAR8 cLetter )
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
ASCDebugString 
ASCDebugString::extract( UINT32 uStart, UINT32 uEnd)
{
	if(uStart < uEnd && uEnd <= length())
	{
		ASCDebugString strTemp = prestr(uEnd);
		strTemp = strTemp.substr(uStart);
		return strTemp;
	}
	//assert_now("Guts, Bad indexes");
	return "";
}
ASCDebugString 
ASCDebugString::extractc( CHAR8 cStart, CHAR8 cEnd, SINT32 iOffsetStart, SINT32 iOffsetEnd)
{
	return extract(find(cStart)+iOffsetStart, find(cEnd)+iOffsetEnd);
}
ASCDebugString 
ASCDebugString::substr( UINT32 iAt )
{
	ASCDebugString strReturn;
	for(UINT32 i = iAt; i < m_iLetterCount; ++i)
	{
		strReturn += m_strString[i];
	}
	return strReturn;
}
ASCDebugString 
ASCDebugString::prestr( UINT32 iAt )
{
	ASCDebugString strReturn;
	for(UINT32 i = 0; i < iAt; ++i)
	{
		strReturn += m_strString[i];
	}
	return strReturn;
}
ASCDebugString 
ASCDebugString::prestrc( const CHAR8 cLetter, SINT32 uOffset )
{
	return prestr(find(cLetter)+uOffset);
}
void 
ASCDebugString::split( SINT32 iAt, ASCDebugString& str1, ASCDebugString& str2 )
{
	ASCDebugString strTemp1;
	for(SINT32 i = 0; i < iAt; ++i)
	{
		strTemp1 += m_strString[i];
	}
	str1 = strTemp1;

	ASCDebugString strTemp2;
	for(UINT32 i = iAt; i < m_iLetterCount; ++i)
	{
		strTemp2 += m_strString[i];
	}
	str2 = strTemp2;
}
ASCDebugString 
ASCDebugString::substrc( const CHAR8 cLetter, SINT32 uOffset)
{
	return substr(find(cLetter)+uOffset);
}
void 
ASCDebugString::splitc( const CHAR8 cLetter, ASCDebugString& str1, ASCDebugString& str2, SINT32 uOffset)
{
	return split(find(cLetter)+uOffset, str1, str2);
}
UINT32 
ASCDebugString::UINT()
{
	return atoi(m_strString);
}
FLOAT32 
ASCDebugString::FLOAT()
{
	return SC_FLOAT(atof(m_strString));
}
bool 
ASCDebugString::BOOL()
{
	static ASCDebugString sklutTrues[4] = { "true", "t", "T", "True" };
	for(UINT32 i = 0; i < 4; ++i)
	{
		if(Compare(sklutTrues[i]))
		{
			return true;
		}
	}
	return false;
}
SINT32 
ASCDebugString::SINT()
{
	return atoi(m_strString);
}
bool 
ASCDebugString::Compare(ASCDebugString& _str)
{
	if(m_iLetterCount != _str.m_iLetterCount)
	{
		return false;
	}
	for(UINT32 i = 0; i < m_iLetterCount; ++i)
	{
		if(ks_ilutLetterValues[m_strString[i]] != ks_ilutLetterValues[_str.m_strString[i]])
		{
			return false;
		}
	}
	return true;
}
bool 
ASCDebugString::operator>(ASCDebugString& rString)
{
#ifdef ASC_IOS
	return strcmp(c_str(), rString.c_str()) > 0;
#else
	return _stricmp(c_str(), rString.c_str()) > 0;
#endif
}
bool 
ASCDebugString::operator<(ASCDebugString& rString)
{
#ifdef ASC_IOS
	return strcmp(c_str(), rString.c_str()) < 0;
#else
	return _stricmp(c_str(), rString.c_str()) < 0;
#endif
}
ASCDebugString 
ASCDebugString::operator+(ASCDebugString rString)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
	ASCDebugString strReturn;
	
	if(length() + rString.length() < m_kiMaxStrLength)
	{
#ifdef ASC_IOS
        sprintf(cStrVal, "%s%s", c_str(), rString.c_str());
#else
		sprintf_s(cStrVal, "%s%s", c_str(), rString.c_str());
#endif
		strReturn = cStrVal;
		return strReturn;
	}
	ASCDebugString strTemp1 = "STRING TO LONG TO BE COMPLETLY STORED: ";
	//ASCDebugString strTemp2 = prestr(length() - strTemp1.length());
	strReturn = strTemp1 + rString;
	
	return strReturn;
}
void 
ASCDebugString::operator+=(ASCDebugString rString)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
	if(length() + rString.length() < m_kiMaxStrLength)
	{
#ifdef ASC_IOS
        sprintf(cStrVal, "%s%s", c_str(), rString.c_str());
#else
		sprintf_s(cStrVal, "%s%s", c_str(), rString.c_str());
#endif
		*this = cStrVal;
		return;
	}
	
	ASCDebugString strTemp1 = "STRING TO LONG TO BE COMPLETLY STORED: ";
	//ASCDebugString strTemp2 = prestr(length() - strTemp1.length());
	ASCDebugString strTemp3 = rString;
	*this = strTemp3;
}
void 
ASCDebugString::operator+=(FLOAT32& rVal)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%0.3f", c_str(), rVal);
#else
	sprintf_s(cStrVal, "%s%0.3f", c_str(), rVal);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::operator=(FLOAT32 fNumber)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
#ifdef ASC_IOS
	sprintf(cStrVal, "%0.3f", fNumber);
#else
	sprintf_s(cStrVal, "%0.3f", fNumber);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::AddFloatHP(FLOAT32& rVal)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
    
#ifdef ASC_IOS
	sprintf(cStrVal, "%s%0.7f", c_str(), rVal);
#else
	sprintf_s(cStrVal, "%s%0.7f", c_str(), rVal);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::operator+=(UINT32& rVal)
{
	CHAR8 cStrVal[m_kiMaxStrLength];

#ifdef ASC_IOS
    sprintf(cStrVal, "%s%d", c_str(), rVal);
#else
	sprintf_s(cStrVal, "%s%d", c_str(), rVal);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::operator+=(SINT32& rVal)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
    
#ifdef ASC_IOS
    sprintf(cStrVal, "%s%d", c_str(), rVal);
#else
	sprintf_s(cStrVal, "%s%d", c_str(), rVal);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::operator+=(void* rVal)
{
	CHAR8 cStrVal[m_kiMaxStrLength];

#ifdef ASC_IOS
    UINT32 uVal = reinterpret_cast<UINT32>(rVal);
	sprintf(cStrVal, "%s%x", c_str(), uVal);
#else
	sprintf_s(cStrVal, "%s%x", c_str(), rVal);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::operator+=(CHAR8 cLetter)
{
	CHAR8 cStrVal[m_kiMaxStrLength];
	CHAR8 cStrTemp[2];
	cStrTemp[0] = cLetter;
	cStrTemp[1] = 0;
	CHAR8* strTemp = cStrTemp;

#ifdef ASC_IOS
    sprintf(cStrVal, "%s%s", c_str(), strTemp);
#else
	sprintf_s(cStrVal, "%s%s", c_str(), strTemp);
#endif
	*this = cStrVal;
}

void 
ASCDebugString::AddSmallInt(UINT32 uVal)
{
	CHAR8 cStrVal[m_kiMaxStrLength];

#ifdef ASC_IOS
    sprintf(cStrVal, "%s%d", c_str(), uVal);
#else
	sprintf_s(cStrVal, "%s%d", c_str(), uVal);
#endif
	*this = cStrVal;
}
void 
ASCDebugString::operator=(const CHAR8*& _str)
{
	m_iLetterCount = 0;
	while(_str[m_iLetterCount] != 0)
	{
		m_strString[m_iLetterCount] = _str[m_iLetterCount];
		++m_iLetterCount;
	}
	m_strString[m_iLetterCount] = 0;
}
