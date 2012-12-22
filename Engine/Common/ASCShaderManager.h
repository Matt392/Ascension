/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCShaderManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSHADERMANAGER_H__Included_2026619839
#define _ASCSHADERMANAGER_H__Included_2026619839


namespace ASC
{
	class ASCShader;

	class ASCShaderManager
	{
	public:		
		virtual ~ASCShaderManager()
		{
		}
		//Loading
		virtual ASCShader* LoadShader(ASCString strFileName) = 0;
		virtual void Reload() = 0;
	};
}

#endif //_ASCSHADERMANAGER_H__Included_2026619839

