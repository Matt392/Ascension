#include "ASCGenInc.h"
#include "ASCGeneralFunctions.h"

#ifdef ASC_IOS
#include <malloc/malloc.h>
#include <stdio.h>

#else
#include <crtdbg.h>
#include <malloc.h>
#include <stdio.h>

#include <windows.h>
#include <Dbghelp.h>
#endif

UINT32 
ASCGeneralFunctions::GenUniqueID()
{
	static UINT32 suID = 0;
	++suID;
	if(suID >= ASC_MAX_UINT)
	{
		assert_now("Guts, To many unique IDs");
	}
	return suID - 1;
}

#ifndef ASC_RELEASE
bool 
ASCGeneralFunctions::StackTrace(UINT32 uDepth, ASCDebugString& rReturn, SINT32 uDepthOffset)
{
	void* pTraceData[63];
	SYMBOL_INFO* symbol;
	HANDLE process;  
	process = GetCurrentProcess();  
	SymInitialize( process, NULL, TRUE );

	symbol = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof( SYMBOL_INFO ); 

	UINT32 uTraces = CaptureStackBackTrace(0, 63, pTraceData, NULL);

	ASCDebugString strTemp;

	bool bFirstPass = true;

	rReturn = "Called from: ";
	if(uDepth <= uTraces-6)
	{
		rReturn = "Called from(Partial trace): ";
	}
	UINT32 uCount = 0;
	for(SINT32 i = uTraces-6; i >= uDepthOffset; --i)
	{
		SymFromAddr( process, ( DWORD64 )( pTraceData[ i ] ), 0, symbol );

		strTemp = symbol->Name;
		if(bFirstPass)
		{
			bFirstPass = false;
			rReturn += strTemp;
		}
		else
		{
			rReturn += ASCDebugString("()->");
			rReturn += strTemp;
		}
		++uCount;
		if(uCount > uDepth)
		{
			break;
		}
	}
	
	free( symbol );
	return true;
}
bool 
ASCGeneralFunctions::StackTrace(UINT32 uDepth, ASCString& rReturn, SINT32 uDepthOffset)
{
	void* pTraceData[63];
	SYMBOL_INFO* symbol;
	HANDLE process;  
	process = GetCurrentProcess();  
	SymInitialize( process, NULL, TRUE );

	symbol = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof( SYMBOL_INFO ); 

	UINT32 uTraces = CaptureStackBackTrace(0, 63, pTraceData, NULL);

	ASCString strTemp;

	bool bFirstPass = true;

	rReturn = "Called from: ";
	if(uDepth <= uTraces-6)
	{
		rReturn = "Called from(Partial trace): ";
	}
	UINT32 uCount = 0;
	for(SINT32 i = uTraces-6; i >= uDepthOffset; --i)
	{
		SymFromAddr( process, ( DWORD64 )( pTraceData[ i ] ), 0, symbol );

		strTemp = symbol->Name;
		if(bFirstPass)
		{
			bFirstPass = false;
			rReturn += strTemp;
		}
		else
		{
			rReturn += ASCString("()->");
			rReturn += strTemp;
		}
		++uCount;
		if(uCount > uDepth)
		{
			break;
		}
	}
	
	free( symbol );
	return true;
}
#else
bool 
ASCGeneralFunctions::StackTrace(UINT32 uDepth, ASCDebugString& rReturn, SINT32 uDepthOffset)
{
	uDepth;
	rReturn;
	uDepthOffset;
	return false;
}
bool 
ASCGeneralFunctions::StackTrace(UINT32 uDepth, ASCString& rReturn, SINT32 uDepthOffset)
{
	uDepth;
	rReturn;
	uDepthOffset;
	return false;
}
#endif

#ifdef ASC_IOS
ASCString 
ASCGeneralFunctions::ConvertDocumentsPathForIOS( ASCString& strFilePath )
{
    //ASCDebuggingManager::OutputWindowLine("Original File Path: ", ASCDebugString ( strFilePath.c_str()));
    ASCString strAsset = strFilePath;
    ASCString strTemp;
    int iSlashIndex = strAsset.findlast( '/' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    iSlashIndex = strAsset.findlast( '\\' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    //ASCDebuggingManager::OutputWindowLine("No Folder Path: ", ASCDebugString ( strAsset.c_str()));
    
    static bool s_bDocumentsDir = false;
    static ASCString s_strDocuments = "";
    if(false == s_bDocumentsDir)
    {
        s_bDocumentsDir = true;
        
        NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
        s_strDocuments = ASCString( [documentsDirectory UTF8String] );
        s_strDocuments += ASCString("/");
        ASCDebuggingManager::OutputWindowLine("Bundle Directory: ", ASCDebugString ( s_strDocuments.c_str()));
    }
    
    ASCString strIOSResourcePath = s_strDocuments;
    strIOSResourcePath += strAsset;
    
    return strIOSResourcePath;
}
ASCString 
ASCGeneralFunctions::ConvertFilePathForIOS( ASCString& strFilePath )
{
    //ASCDebuggingManager::OutputWindowLine("Original File Path: ", ASCDebugString ( strFilePath.c_str()));
    ASCString strAsset = strFilePath;
    ASCString strTemp;
    int iSlashIndex = strAsset.findlast( '/' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    iSlashIndex = strAsset.findlast( '\\' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    //ASCDebuggingManager::OutputWindowLine("No Folder Path: ", ASCDebugString ( strAsset.c_str()));
    
    static bool s_bBundleDir = false;
    static ASCString s_strMainBundle = "";
    if(false == s_bBundleDir)
    {
        s_bBundleDir = true;
        NSString *bundleDirectory = [[NSBundle mainBundle] resourcePath];
        s_strMainBundle = ASCString( [bundleDirectory UTF8String] );
        s_strMainBundle += ASCString("/");
       //ASCDebuggingManager::OutputWindowLine("Bundle Directory: ", ASCDebugString ( s_strMainBundle.c_str()));
    }
    
    ASCString strIOSResourcePath = s_strMainBundle;
    strIOSResourcePath += strAsset;
    
    return strIOSResourcePath;
    
    /*
    //ASCDebuggingManager::OutputWindowLine("Original File Path: ", ASCDebugString ( strFilePath.c_str()));
    int iPeriodIndex = strFilePath.findlast( '.' );
    int iLength = strFilePath.length();
    ASCString strAsset;
    ASCString strTemp;
    strFilePath.prestr( iPeriodIndex, strAsset );
    int iSlashIndex = strAsset.findlast( '/' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    iSlashIndex = strAsset.findlast( '\\' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    ASCString strFileType;
    strFilePath.substr( iPeriodIndex + 1, strFileType );
    //ASCDebuggingManager::OutputWindowLine("Asset: ", ASCDebugString ( strAsset.c_str()));
    //ASCDebuggingManager::OutputWindowLine("Extension: ", ASCDebugString ( strFileType.c_str()));
    
    if(strFileType[strFileType.length()-1] == '\r')
    {
        strFileType.prestrc('\r', strTemp);
        strFileType = strTemp;
    }
    
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    ASCString strPath = ASCString( [documentsDirectory UTF8String] );
    strPath += ASCString("/");
    strPath += strAsset;
    strPath += ASCString(".");
    strPath += strFileType;
    
    NSString* Path = [[NSString alloc] initWithUTF8String: strPath.c_str()];
    
    
    NSFileManager* filemgr = [NSFileManager defaultManager];
    
    if ([filemgr fileExistsAtPath:Path]  == YES)
    {
        NSLog (@"q  aFile exists");
    }
    else
    {
        [Path release];
        
        NSString* AssetPath = [[NSString alloc] initWithUTF8String: strAsset.c_str()];
        NSString* FileType = [[NSString alloc] initWithUTF8String: strFileType.c_str()];
        Path = (NSString*)[[NSBundle mainBundle] pathForResource:AssetPath ofType:FileType];
        
        
        if ( Path == nil )
        {
            CHAR8 c1 = strFileType[strFileType.length()-1];
            CHAR8 c2 = strFileType[strFileType.length()-2];
            CHAR8 c3 = strFileType[strFileType.length()-3];
            CHAR8 c4 = strFileType[strFileType.length()-4];
            //Failed to find asset.
            assert_now("Guts, Could not convert to IOS Path");
        }
        
        ASCString strIOSResourcePath = ASCString( [Path UTF8String] );
        
        //Release strings.
        [AssetPath release];
        [FileType release];
        
        return strIOSResourcePath;
    }
    
    ASCString strIOSResourcePath = ASCString( [Path UTF8String] );
    
    [Path release];
    //ASCDebuggingManager::OutputWindowLine("New File Path: ", ASCDebugString( strIOSResourcePath.c_str()));
    
    return strIOSResourcePath;
     */
}
bool 
ASCGeneralFunctions::FileExist( ASCString& strFilePath )
{
    int iPeriodIndex = strFilePath.findlast( '.' );
    int iLength = strFilePath.length();
    ASCString strAsset;
    ASCString strTemp;
    strFilePath.prestr( iPeriodIndex, strAsset );
    int iSlashIndex = strAsset.findlast( '/' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    iSlashIndex = strAsset.findlast( '\\' );
    if ( -1 != iSlashIndex )
    {
        strAsset.substr( iSlashIndex + 1, strTemp );
        strAsset = strTemp;
    }
    ASCString strFileType;
    strFilePath.substr( iPeriodIndex + 1, strFileType );
    
    if(strFileType[strFileType.length()-1] == '\r')
    {
        strFileType.prestrc('\r', strTemp);
        strFileType = strTemp;
    }
    
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    ASCString strPath = ASCString( [documentsDirectory UTF8String] );
    strPath += ASCString("/");
    strPath += strAsset;
    strPath += ASCString(".");
    strPath += strFileType;
    
    NSString* Path = [[NSString alloc] initWithUTF8String: strPath.c_str()];
    
    
    NSFileManager* filemgr = [NSFileManager defaultManager];
    bool bPathExists = false;
    
    if ([filemgr fileExistsAtPath:Path]  == YES)
    {
        NSLog (@"q  aFile exists");
    }
    else
    {
        [Path release];
        
        NSString* AssetPath = [[NSString alloc] initWithUTF8String: strAsset.c_str()];
        NSString* FileType = [[NSString alloc] initWithUTF8String: strFileType.c_str()];
        Path = (NSString*)[[NSBundle mainBundle] pathForResource:AssetPath ofType:FileType];
        
        
        if ( Path == nil )
        {
            bPathExists = false;
        }
        else
        {
            bPathExists = true;
            [Path release];
        }
        
        //Release strings.
        [AssetPath release];
        [FileType release];
    }
    
    return bPathExists;
}
#endif