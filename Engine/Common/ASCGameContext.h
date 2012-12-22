/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCGameContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGAMECONTEXT_H__Included_2015300773
#define _ASCGAMECONTEXT_H__Included_2015300773

#include "ASCGenInc.h"

namespace ASC
{
	class ASCIState;
	class ASCGameContext
	{
	public:
		virtual ~ASCGameContext()
		{
		}

		//Creation
		virtual bool Initialise() = 0;
		
		//Game logic
		virtual bool Update() = 0;
        
		//Optional
		virtual void Draw() = 0;
		virtual void Disable() = 0;
		virtual ASCIState* GetState(UINT32 uStateID) = 0;
		virtual void SetState(UINT32 uStateID) = 0;


	protected:
		bool m_bQuit;
	};
}

#endif //_ASCGAMECONTEXT_H__Included_2015300773

