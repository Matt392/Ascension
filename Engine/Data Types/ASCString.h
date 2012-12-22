/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Data Types\ASCString.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSTRING_H__Included_669415313
#define _ASCSTRING_H__Included_669415313

namespace ASC
{

	static const UINT32 m_kiInitialStringSize = 2;
	static const UINT32 m_kiIncreaseStringSize = 4;

	static const SINT32 ks_ilutLetterValues[128] = 
	{
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//0
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//10
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//20
		-1, -1,  0,  1,  2,  3,  4,  5,  6,  7,//30
		 8,  9, 10, 11, 12, 13, 14, 15, 16, 17,//40
		18, 19, 20, 21, 22, 23, 24, 25, 26, 27,//50
		28, 29, 30, 31, 32, 33, 34, 35, 36, 37,//60
		38, 39, 40, 41, 42, 43, 44, 45, 46, 47,//70
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57,//80
		58, 59, 60, 61, 62, 63, 64, 33, 34, 35,//90
		36, 37, 38, 39, 40, 41, 42, 43, 44, 45,//100
		46, 47, 48, 49, 50, 51, 52, 53, 54, 55,//110
		56, 57, 58, 65, 66, 67, 68, 69//120
	};
	
	class ASCFileIO;

	class ASCString
	{
	public:
		//Constructors
		ASCString(void);
		ASCString(const CHAR8* str);
		ASCString(const ASCString& str);


		//Destructors
		~ASCString(void);


		//Info
		UINT32 length();
		
		SINT32 find( const CHAR8* str );
		SINT32 find( const CHAR8 cLetter );
		SINT32 findlast( const CHAR8 cLetter );

		bool Compare(ASCString& str);


		//Gets 
		UINT32 UINT();
		SINT32 SINT();
		FLOAT32 FLOAT();
		bool BOOL();

		CHAR8* c_str();
		bool Valid();
		UINT32 Max();
		void Clear();


		//Operations
		void substr( UINT32 iAt, ASCString& strReturn );
		void substrc( const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset = 0);
		void sublaststrc( const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset = 0);

		void split( SINT32 iAt, ASCString& str1, ASCString& str2 );
		void splitc( const CHAR8 cLetter, ASCString& str1, ASCString& str2, SINT32 uOffset = 0 );

		void prestr( UINT32 iAt, ASCString& strReturn );
		void prestrc( const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset = 0 );

		void poststrc (const CHAR8 cLetter, ASCString& strReturn, SINT32 uOffset = 0 );

		void extract( UINT32 uStart, UINT32 uEnd, ASCString& strReturn);
		void extractc( CHAR8 cStart, CHAR8 cEnd, ASCString& strReturn, SINT32 iOffsetStart = 0, SINT32 iOffsetEnd = 0);


		//Other
		void Resize();
		void AddFloatHP(FLOAT32& rVal);
		void AddFloatLP(FLOAT32& rVal);
		void AddFloatOneDP(FLOAT32& rVal);
		void AddSmallInt(UINT32 uVal);
		void RemoveLast();
		
		//Serialization
		void LoadBinary(void*& pData);
		void Serialize(ASCFileIO* pFile);

		//Operators
		CHAR8& operator[](const UINT32& iIndex);
		ASCString operator+(ASCString rString);
		void operator=(const CHAR8*& str);
		void operator=(const ASCString& str);
		void operator=(CHAR8 cLetter);
		void operator=(FLOAT32 fNumber);
		void operator+=(ASCString rString);
		void operator+=(FLOAT32& rVal);
		void operator+=(UINT32& rVal);
		void operator+=(SINT32& rVal);
		void operator+=(void* rVal);
		void operator+=(CHAR8 cLetter);

		bool operator==(ASCString& rString);
		bool operator==(CHAR8* rString);
		bool operator!=(ASCString& rString);
		bool operator!=(CHAR8* rString);
		bool operator>(ASCString& rString);
		bool operator<(ASCString& rString);
		bool operator>=(ASCString& rString);
		bool operator<=(ASCString& rString);


	protected:
		UINT32 m_uMaxSize;
		UINT32 m_iLetterCount;
		CHAR8* m_strString;

		friend class ASCBinaryString;
	};
	

	static ASCString g_kstrInvalid = "Invalid";
}

#endif //_ASCSTRING_H__Included_669415313

 