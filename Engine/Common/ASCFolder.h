/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCFolder.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCFOLDER_H__Included_106874712
#define _ASCFOLDER_H__Included_106874712


namespace ASC
{
	class ASCFolder
	{
	public:
		ASCFolder(void);
		~ASCFolder(void);

		bool Load(ASCString strDirectory);

		ASCString FolderPath();
		
		ASCVector<ASCString>* Files();
		ASCVector<ASCString>* Folders();

		void SaveDirectoryInfo();

	protected:
		ASCVector<ASCString> m_strFiles;
		ASCVector<ASCString> m_strFolders;

		ASCString m_strPath;
	};

}

#endif //_ASCFOLDER_H__Included_106874712

