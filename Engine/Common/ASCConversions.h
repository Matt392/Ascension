/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCConversions.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCCONVERSIONS_H__Included_705302293
#define _ASCCONVERSIONS_H__Included_705302293


namespace ASC
{
	const static FLOAT32 fPI = 3.141592653589793f;

#ifdef ASC_WINDOWS
	LPCWSTR CharStrToLPCWSTR(const char* str)
	{
		WCHAR strTemp[20]; 
		MultiByteToWideChar( 0,0, str, -1, strTemp, 20); 
		LPCWSTR strOut = strTemp;
		return strOut;
	}
#endif
	FLOAT32 DegsToRad( FLOAT32 fDegs )
	{
		return (fDegs/180.0f)*fPI;
	}

}

#endif //_ASCCONVERSIONS_H__Included_705302293

