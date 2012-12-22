/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine Tests\Texture Atlas Test\TextureAtlasTest.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTUREATLASTEST_H__Included_824561365
#define _TEXTUREATLASTEST_H__Included_824561365

#include "ASCGameContext.h"
#include "ASCIEventListener.h"


namespace ASC
{
	class CTextureAtlasTest : public ASCGameContext, ASCIEventListener
	{
	public:
		CTextureAtlasTest(void);
		~CTextureAtlasTest(void);

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

#endif //_TEXTUREATLASTEST_H__Included_824561365

