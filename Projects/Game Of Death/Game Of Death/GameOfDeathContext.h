/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\GameOfDeathContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GAMEOFDEATHCONTEXT_H__Included_1464540130
#define _GAMEOFDEATHCONTEXT_H__Included_1464540130

#include "ASCGameContext.h"
#include "ASCIEventListener.h"
#include "GameOfDeathInc.h"

namespace ASC
{
	class CTemplates;
	class CMenuBackground;

	class CGameOfDeathContext : public ASCGameContext, ASCIEventListener
	{
	public:
		CGameOfDeathContext(void);
		~CGameOfDeathContext(void);

		//Creation
		bool Initialise();
		
		//Game logic
		bool Update();

		//Optional
		void Draw();
        void Disable();
		ASCIState* GetState(UINT32 uStateID);
		void SetState(UINT32 uStateID);

		//Functionality
		CTemplates* Templates();

		//Events System
		void OnEvent(ASCIEvent* pEvent);

	protected:
		ASCIState* m_pStates[S_COUNT];
		ESTATE m_eCurrentState;

		CTemplates* m_pTemplates;
		CMenuBackground* m_pMenuBackground;

	};

}

#endif //_GAMEOFDEATHCONTEXT_H__Included_1464540130

