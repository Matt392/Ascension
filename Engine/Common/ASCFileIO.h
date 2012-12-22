/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCFileIO.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCFILEIO_H__Included_1028773805
#define _ASCFILEIO_H__Included_1028773805

#include <fstream>

namespace ASC
{
    
	enum EByteSize
	{
		BS_B,
		BS_KB,
		BS_MB,
		BS_GB,
		BS_TB,
		BS_COUNT,
	};
	enum EInOut
	{
		IO_IN,
		IO_IN_BINARY,
		IO_OUT,
		IO_OUT_BINARY,
		IO_BOTH,
	};

	class ASCFileIO
	{
	public:
		ASCFileIO( void );
		~ASCFileIO( void );
		bool Open( ASCString strFile, EInOut eInOut, bool bFromBundle = true );
		bool Close();

		bool GetNextLine(ASCString& strReturn);
		void* ReadBinary();
		
		UINT32 GetSize(EByteSize eByteSize = BS_KB);
		ASCString GetSize();
		void PrintLine(const CHAR8* strFile);
		void PrintLine(ASCString& strLine);
		void Print(ASCString& str);\
		void OutputBianry( void* pData, UINT32 uCount );

		void Clear();
		
		static bool IsNewerThan(ASCString strFirst, ASCString strOther);
		static bool FileExists(ASCString strFile, bool bFromBundle = true);

	private:
		bool FileIn( ASCString& strFile );
		bool FileOut( ASCString& strFile );
		bool FileInBinary( ASCString& strFile );
		bool FileOutBinary( ASCString& strFile );


	protected:
		std::ifstream m_InputStream;
		std::ofstream m_OutputStream;

		bool m_bOpen;
	
		EInOut m_eInOut;
	};

}

#endif //_ASCFILEIO_H__Included_1028773805

