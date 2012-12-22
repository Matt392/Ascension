/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9Shader.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9SHADER_H__Included_313353687
#define _ASCDX9SHADER_H__Included_313353687

#ifdef ASC_RENDERER_DX9

#include "ASCShader.h"
#include <d3dx9.h>

namespace ASC
{
	class ASCDX9Shader : public ASCShader
	{
	public:
		ASCDX9Shader( IDirect3DDevice9* pDevice, ASCString& strFileName );
		~ASCDX9Shader(void);

		//Rendering
		bool Begin ( UINT32& uPassCount );
		bool Begin ( void );
		bool BeginPass ( UINT32 uPassNum );
		bool EndPass ( void );
		bool End ( void );

		//Get handles
		ASC_Handle GetSemanticHandle( ASCString strSemantic );
		ASC_Handle GetNamedHandle( ASCString strName );
		bool SetFVectors( ASC_Handle hTarget, const void* pSrc, UINT32 uQuadCount );
		bool SetTechnique( ASC_Handle hTarget );

		//Loading
		void Reload();

		//Get
		ASCString GetName();

	protected:
		IDirect3DDevice9* m_pDevice;
		ID3DXEffect* m_pEffect;
		ASCString m_strFileName;
	};

}

#endif

#endif //_ASCDX9SHADER_H__Included_313353687

