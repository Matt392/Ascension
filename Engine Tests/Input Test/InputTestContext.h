/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine Tests\Input Test\InputTestContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _INPUTTESTCONTEXT_H__Included_2013450520
#define _INPUTTESTCONTEXT_H__Included_2013450520

#include "ASCGameContext.h"
#include "ASCIEventListener.h"

namespace ASC
{
	class CInputTestContext : public ASCGameContext, ASCIEventListener
	{
	public:
		CInputTestContext(void);
		~CInputTestContext(void);

		//Creation
		bool Initialise();
		
		//Game logic
		bool Update();

		//Optional
		void Draw();
        void Disable();
		ASCIState* GetState(UINT32 uStateID)
		{
			uStateID;
			return NULL;
		}
		void SetState(UINT32 uStateID)
		{
			uStateID;
		}

		//Events System
		void OnEvent(ASCIEvent* pEvent);
	
	protected:
		UINT32 m_uExitButton;
		bool m_bQuit;
	};

}

#endif //_INPUTTESTCONTEXT_H__Included_2013450520

