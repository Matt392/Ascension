/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Data Types\ASCDebugString.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDebugString_H__Included_669415313
#define _ASCDebugString_H__Included_669415313

//#define STRING_VARIABLE_STORAGE
//#define NEW_STRING

namespace ASC
{
#define MY_STR_CMP

	const UINT32 m_kiMaxStrLength = 1024;

	//static const SINT32 ks_ilutLetterValues[128] = 
	//{
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//0
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//10
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//20
	//	-1, -1,  0,  1,  2,  3,  4,  5,  6,  7,//30
	//	 8,  9, 10, 11, 12, 13, 14, 15, 16, 17,//40
	//	18, 19, 20, 21, 22, 23, 24, 25, 26, 27,//50
	//	28, 29, 30, 31, 32, 33, 34, 35, 36, 37,//60
	//	38, 39, 40, 41, 42, 43, 44, 45, 46, 47,//70
	//	48, 49, 50, 51, 52, 53, 54, 55, 56, 57,//80
	//	58, 59, 60, 61, 62, 63, 64, 33, 34, 35,//90
	//	36, 37, 38, 39, 40, 41, 42, 43, 44, 45,//100
	//	46, 47, 48, 49, 50, 51, 52, 53, 54, 55,//110
	//	56, 57, 58, 65, 66, 67, 68, 69//120
	//};


	class ASCDebugString
	{
	public:
		ASCDebugString(void);
		ASCDebugString(const CHAR8* _str);
		~ASCDebugString(void);
		UINT32 length();
		CHAR8* c_str();
		SINT32 find( const CHAR8* str );
		SINT32 find( const CHAR8 cLetter );
		ASCDebugString substr( UINT32 iAt );
		void split( SINT32 iAt, ASCDebugString& str1, ASCDebugString& str2 );
		ASCDebugString substrc( const CHAR8 cLetter, SINT32 uOffset = 0);
		void splitc( const CHAR8 cLetter, ASCDebugString& str1, ASCDebugString& str2, SINT32 uOffset = 0 );
		ASCDebugString prestr( UINT32 iAt );
		ASCDebugString prestrc( const CHAR8 cLetter, SINT32 uOffset = 0 );
		ASCDebugString extract( UINT32 uStart, UINT32 uEnd);
		ASCDebugString extractc( CHAR8 cStart, CHAR8 cEnd, SINT32 iOffsetStart = 0, SINT32 iOffsetEnd = 0);
		UINT32 UINT();
		FLOAT32 FLOAT();
		bool BOOL();
		SINT32 SINT();

		bool Compare(ASCDebugString& _str);


		UINT32 Max()
		{
			return m_kiMaxStrLength; 
		}

		bool Valid() { return m_iLetterCount > 0; }

		//Operators
		CHAR8 operator[](const UINT32& _iIndex)
		{
			return m_strString[_iIndex];
		}
		ASCDebugString operator+(ASCDebugString rString);
		void operator=(const CHAR8*& _str);
		//void operator=(const ASCDebugString& str);
		void operator=(CHAR8 cLetter)
		{
			m_iLetterCount = 1;
			m_strString[0] = cLetter;
			m_strString[m_iLetterCount] = 0;
		}
		void operator=(FLOAT32 fNumber);
		void operator+=(ASCDebugString rString);
		void operator+=(FLOAT32& rVal);
		void AddFloatHP(FLOAT32& rVal);
		void operator+=(UINT32& rVal);
		void operator+=(SINT32& rVal);
		void operator+=(void* rVal);
		void operator+=(CHAR8 cLetter);

		void AddSmallInt(UINT32 uVal);
		bool operator==(ASCDebugString& rString)
		{
			#ifdef MY_STR_CMP
			return Compare(rString);
			#else
			return _stricmp(c_str(), rString.c_str()) == 0;
			#endif
		}
		bool operator==(CHAR8* rString)
		{
			#ifdef MY_STR_CMP
			ASCDebugString strTemp = rString;
			return Compare(strTemp);
			#else
			return _stricmp(c_str(), rString) == 0;
			#endif
		}	
		bool operator!=(ASCDebugString& rString)
		{
			return !(*(this)==rString);
		}
		bool operator!=(CHAR8* rString)
		{
			return !(*(this)==rString);
		}	
		bool operator>(ASCDebugString& rString);
		bool operator<(ASCDebugString& rString);
		bool operator>=(ASCDebugString& rString)
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
		bool operator<=(ASCDebugString& rString)
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


	protected:
		CHAR8 m_strString[m_kiMaxStrLength];

		UINT32 m_iLetterCount;
	};
	

	//const ASCDebugString g_kstrInvalid = "Invalid";
}

#endif //_ASCDebugString_H__Included_669415313

 