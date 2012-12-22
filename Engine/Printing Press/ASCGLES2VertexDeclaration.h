/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2VertexDeclaration.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2VERTEXDECLARATION_H__Included_83722330
#define _ASCGLES2VERTEXDECLARATION_H__Included_83722330

#ifdef ASC_RENDERER_GLES2

#include "ASCVertexDeclaration.h"

namespace ASC
{
	class ASCGLES2VertexDeclaration : public ASCVertexDeclaration
	{
	public:
		ASCGLES2VertexDeclaration(SVertElement VertElements[], UINT32 iElementCount);
		~ASCGLES2VertexDeclaration(void);
		bool Apply();
		UINT32 GetStride();
		
	protected:
		UINT32							m_iElementCount;
		UINT32							m_iStride;
	};

}

#endif 

#endif //_ASCGLES2VERTEXDECLARATION_H__Included_83722330

