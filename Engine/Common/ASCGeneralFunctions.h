/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCGeneralFunctions.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGENERALFUNCTIONS_H__Included_22012756
#define _ASCGENERALFUNCTIONS_H__Included_22012756


namespace ASC
{
	class ASCGeneralFunctions
	{
	public:
		static UINT32 GenUniqueID();
		static bool StackTrace(UINT32 uDepth, ASCDebugString& rReturn, SINT32 uDepthOffset = 1);
		static bool StackTrace(UINT32 uDepth, ASCString& rReturn, SINT32 uDepthOffset = 1);

#ifdef ASC_IOS
        static ASCString ConvertFilePathForIOS( ASCString& strFilePath );
        static ASCString ConvertDocumentsPathForIOS( ASCString& strFilePath );
        static bool FileExist( ASCString& strFilePath );
#endif
	};
}

#endif //_ASCGENERALFUNCTIONS_H__Included_22012756

