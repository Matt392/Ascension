/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9VertexDeclaration.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9VERTEXDECLARATION_H__Included_1743015320
#define _ASCDX9VERTEXDECLARATION_H__Included_1743015320

#ifdef ASC_RENDERER_DX9

#include "ASCVertexDeclaration.h"
#include <d3dx9.h>

namespace ASC
{
	class ASCDX9VertexDeclaration : public ASCVertexDeclaration
	{
	public:
		ASCDX9VertexDeclaration(SVertElement VertElements[], UINT32 iElementCount, void* pDevice);
		~ASCDX9VertexDeclaration(void);
		bool Apply();
		UINT32 GetStride();
		
	protected:
		IDirect3DVertexDeclaration9*	m_pDecl;
		IDirect3DDevice9*				m_pDevice;
		D3DVERTEXELEMENT9*				m_pVertElem;

		UINT32							m_iElementCount;
		UINT32							m_iStride;
	};

}

#endif

#endif //_ASCDX9VERTEXDECLARATION_H__Included_1743015320

