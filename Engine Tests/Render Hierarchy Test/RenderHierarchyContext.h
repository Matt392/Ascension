/**
*	@Ascension Engine
 *	@file Ascension\Engine Tests\Render Hierarchy Test\RenderHierarchyContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RENDERHIERARCHYCONTEXT_H__Included_490730986
#define _RENDERHIERARCHYCONTEXT_H__Included_490730986

#include "ASCGameContext.h"
#include "ASCIEventListener.h"

namespace ASC
{
	class RenderHierarchyContext :	public ASCGameContext
	{
	public:
		RenderHierarchyContext(void);
		~RenderHierarchyContext(void);

		//Creation
		bool Initialise();
		
		//Game logic
		bool Update();

		//Optional
		void Draw(){}
		void Disable(){}
		ASCIState* GetState(UINT32 uStateID){ uStateID; return 0;}
		void SetState(UINT32 uStateID){uStateID;}
	};

}

#endif //_RENDERHIERARCHYCONTEXT_H__Included_490730986

