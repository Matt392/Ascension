/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCPhysicsWorld.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCPHYSICSWORLD_H__Included_2142069478
#define _ASCPHYSICSWORLD_H__Included_2142069478

#include "Box2D.h"

namespace ASC
{
	class ASCPhysicsDestructionListener;
	class ASCPhysicsContactListener;


	struct SPhyWorldInitParam
	{
		FLOAT32 m_fGravityX;
		FLOAT32 m_fGravityY;


		SPhyWorldInitParam()
		{
			m_fGravityX = 0.0f;
			m_fGravityX = 98.0f;
		}
	};

	class ASCPhysicsWorld
	{
	public:
		ASCPhysicsWorld(void);
		~ASCPhysicsWorld(void);
		bool Init( SPhyWorldInitParam rInitParams );

		b2World& GetBox2DWorld();

		void Update( FLOAT32 fDelta );

	protected:
		FLOAT32 m_fFrameTimeMax;
		FLOAT32 m_fTimeElapsed;
		UINT32 m_uFrameCount;
		UINT32 m_uVelIters;
		UINT32 m_uPosIters;
		
		b2World* m_pPhysicsWorld;
		ASCPhysicsDestructionListener* m_pDestListener;
		ASCPhysicsContactListener* m_pContactListener;
	};

}

#endif //_ASCPHYSICSWORLD_H__Included_2142069478

