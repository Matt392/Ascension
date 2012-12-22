/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCINIFile.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCINIFILE_H__Included_236736859
#define _ASCINIFILE_H__Included_236736859


namespace ASC
{
	class ASCFileIO;

	class ASCINISegment
	{
	public:
		ASCINISegment(void);
		~ASCINISegment(void);
		void SetID(ASCString strID);
		ASCString& SegmentID();
		void SetValue(ASCString& strValueID, ASCString& strValue);
		ASCString GetValue(ASCString& strValueID);

		void LoadBinary( void*& pData );
		void Serialize( ASCFileIO* pFile );

	protected:
		ASCString m_strSegmentID;
		ASCTree<ASCString> m_strValues;
	};

	class ASCINIFile
	{
	public:
		ASCINIFile();
		~ASCINIFile(void);
		bool Load(ASCString& strID);
		bool Load(CHAR8* strID);
		void Initialise(ASCString& strID);
		void Load();
		void SetValue(ASCString strSegmentID, ASCString strValueID, ASCString strValue);
		ASCString GetValue(ASCString& strSegmentID, ASCString& strValueID);
		
		ASCString GetString(ASCString strSegment, ASCString strValueID);
		UINT32 GetUInt(ASCString strSegment, ASCString strValueID);
		SINT32 GetSInt(ASCString strSegment, ASCString strValueID);
		CHAR8 GetChar(ASCString strSegment, ASCString strValueID);
		FLOAT32 GetFloat(ASCString strSegment, ASCString strValueID);
		bool GetBool(ASCString strSegment, ASCString strValueID);
		
		void LoadBinary( void* pData );
		void Serialize( ASCFileIO* pFile );

		void DisplayDebugInfo();
	protected:
		void ProcessSegment(ASCString& strSegment, ASCFileIO& rFile);
		void ProcessLine(ASCString& strSegment, ASCString& strLine);

	protected:
		ASCTree<ASCINISegment*> m_trSegments;
		ASCString m_strID;

		friend class ASCINIManager;

	protected:
	};

}

#endif //_ASCINIFILE_H__Included_236736859

