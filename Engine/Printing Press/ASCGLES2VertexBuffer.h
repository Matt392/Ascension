/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2VertexBuffer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2VERTEXBUFFER_H__Included_1456171144
#define _ASCGLES2VERTEXBUFFER_H__Included_1456171144

#ifdef ASC_RENDERER_GLES2

#include "ASCVertexBuffer.h"

namespace ASC
{
	class ASCGLES2VertexBuffer : public ASCVertexBuffer
	{
	public:
		ASCGLES2VertexBuffer(void);
		~ASCGLES2VertexBuffer(void);
		virtual bool Initialise(UINT32 uSize, UINT32 uStride);

		virtual bool Resize(UINT32 uSize);

		virtual void* Lock();
		virtual void UnLock();

		virtual UINT32 Size();

		virtual void SetSource();

		virtual void Unbind();

	protected:
		UINT32 m_uBuffer;
		UINT32 m_uSize;
		UINT32 m_uStride;

		void* m_pTempBuffer;
	};

}

#endif

#endif //_ASCGLES2VERTEXBUFFER_H__Included_1456171144

