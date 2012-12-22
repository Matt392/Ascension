/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCShader.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSHADER_H__Included_1680912862
#define _ASCSHADER_H__Included_1680912862

#include "ASCGenInc.h"

namespace ASC
{	
	class ASCShader
	{
	public:
		virtual ~ASCShader()
		{
		}
		//Rendering
		virtual bool Begin ( UINT32& uPassCount ) = 0;
		virtual bool Begin ( void ) = 0;
		virtual bool BeginPass ( UINT32 uPassNum ) = 0;
		virtual bool EndPass ( void ) = 0;
		virtual bool End ( void ) = 0;

		//Get handles
		virtual ASC_Handle GetSemanticHandle( ASCString strSemantic ) = 0;
		virtual ASC_Handle GetNamedHandle( ASCString strName ) = 0;
		virtual bool SetFVectors( ASC_Handle hTarget, const void* pSrc, UINT32 uQuadCount ) = 0;
		virtual bool SetTechnique( ASC_Handle hTarget ) = 0;

		//Loading
		virtual void Reload() = 0;
		
		//Get
		virtual ASCString GetName() = 0;
	};
}

#endif //_ASCSHADER_H__Included_1680912862

