/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCINIManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCINIMANAGER_H__Included_1004139603
#define _ASCINIMANAGER_H__Included_1004139603

#define ASC_BINARY_INI_FILES

namespace ASC
{
	class ASCINIFile;
	class ASCINIManager
	{
	public:
		ASCINIManager(void);
		~ASCINIManager(void);
		
		bool LoadFile(ASCString strFile);
		ASCINIFile* File(ASCString strFile);
		
		ASCString GetString(ASCString strFile, ASCString strSegment, ASCString strValueID);
		UINT32 GetUInt(ASCString strFile, ASCString strSegment, ASCString strValueID);
		SINT32 GetSInt(ASCString strFile, ASCString strSegment, ASCString strValueID);
		CHAR8 GetChar(ASCString strFile, ASCString strSegment, ASCString strValueID);
		FLOAT32 GetFloat(ASCString strFile, ASCString strSegment, ASCString strValueID);
		bool GetBool(ASCString strFile, ASCString strSegment, ASCString strValueID);
		void SetValue(ASCString strFile, ASCString strSegment, ASCString strValueID, ASCString strValue);

		void DisplayDebugInfo();

	protected:
		ASCTree<ASCINIFile*> m_trFiles;
		ASCVector<ASCString> m_veFilesToSave; 
	};

}

#endif //_ASCINIMANAGER_H__Included_1004139603

