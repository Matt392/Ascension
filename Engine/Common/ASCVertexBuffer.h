/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCVertexBuffer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCVERTEXBUFFER_H__Included_1393976919
#define _ASCVERTEXBUFFER_H__Included_1393976919

#include "ASCGenInc.h"

namespace ASC
{
	class ASCVertexBuffer
	{
	public:
		virtual ~ASCVertexBuffer()
		{
		}
		virtual bool Initialise(UINT32 uSize, UINT32 uStride) = 0;

		virtual bool Resize(UINT32 uSize) = 0;

		virtual void* Lock() = 0;
		virtual void UnLock() = 0;

		virtual UINT32 Size() = 0;

		virtual void SetSource() = 0;

		virtual void Unbind() = 0;

		virtual UINT32 MemSize() = 0;

	protected:
	};
}

#endif //_ASCVERTEXBUFFER_H__Included_1393976919

