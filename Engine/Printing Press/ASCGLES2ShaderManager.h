/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2ShaderManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2SHADERMANAGER_H__Included_1905850670
#define _ASCGLES2SHADERMANAGER_H__Included_1905850670

#ifdef ASC_RENDERER_GLES2

#include "ASCShaderManager.h"

namespace ASC
{

	class ASCGLES2ShaderManager : public ASCShaderManager
	{
	public:
		ASCGLES2ShaderManager(void);
		~ASCGLES2ShaderManager(void);
		
		//Loading
		ASCShader* LoadShader(ASCString strFileName);
		void Reload();

	protected:
		ASCVector<ASCShader*> m_vecShaders;
	};

}

#endif

#endif //_ASCGLES2SHADERMANAGER_H__Included_1905850670

