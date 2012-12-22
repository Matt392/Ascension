/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9VertexBuffer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9VERTEXBUFFER_H__Included_1912060952
#define _ASCDX9VERTEXBUFFER_H__Included_1912060952

#ifdef ASC_RENDERER_DX9

#include "ASCVertexBuffer.h"
#include <d3dx9.h>

namespace ASC
{
	class ASCDX9VertexBuffer : public ASCVertexBuffer
	{
	public:
		ASCDX9VertexBuffer(void);
		~ASCDX9VertexBuffer(void);
		virtual bool Initialise(UINT32 uSize, UINT32 uStride);

		virtual bool Resize(UINT32 uSize);

		virtual void* Lock();
		virtual void UnLock();

		virtual UINT32 Size();

		virtual void SetSource();

		virtual void Unbind();

		virtual UINT32 MemSize();

	protected:
		IDirect3DVertexBuffer9* m_pVertexBuffer;
		UINT32 m_uSize;
		UINT32 m_uStride;
	};

}

#endif

#endif //_ASCDX9VERTEXBUFFER_H__Included_1912060952

