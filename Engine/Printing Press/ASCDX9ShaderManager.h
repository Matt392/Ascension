/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9ShaderManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9SHADERMANAGER_H__Included_1242907924
#define _ASCDX9SHADERMANAGER_H__Included_1242907924

#ifdef ASC_RENDERER_DX9

#include "ASCShaderManager.h"
#include <d3dx9.h>

namespace ASC
{
	class ASCDX9ShaderManager : public ASCShaderManager
	{
	public:
		ASCDX9ShaderManager( IDirect3DDevice9* pDevice );
		~ASCDX9ShaderManager(void);
		
		//Loading
		ASCShader* LoadShader(ASCString strFileName);
		void Reload();

	protected:
		IDirect3DDevice9* m_pDevice;
		UINT32 m_iShaderCount;

		ASCVector<ASCShader*> m_vecShaders;

	};

}

#endif

#endif //_ASCDX9SHADERMANAGER_H__Included_1242907924

