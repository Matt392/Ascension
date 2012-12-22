#include "ASCGenInc.h"
#include "ASCPhysicsWorld.h"
#include "ASCPhysicsListeners.h"


ASCPhysicsWorld::ASCPhysicsWorld(void)
{
}


ASCPhysicsWorld::~ASCPhysicsWorld(void)
{
	SafeDelete( m_pPhysicsWorld );
	SafeDelete( m_pDestListener );
	SafeDelete( m_pContactListener );
}
bool 
ASCPhysicsWorld::Init( SPhyWorldInitParam rInitParams )
{
	b2Vec2 gravity;
	gravity.Set(rInitParams.m_fGravityX, rInitParams.m_fGravityY);
	bool doSleep = true;
	m_pPhysicsWorld = new b2World(gravity);

	
	m_pDestListener =  new ASCPhysicsDestructionListener();
	m_pContactListener =  new ASCPhysicsContactListener();

	m_pPhysicsWorld->SetDestructionListener(m_pDestListener);
	m_pPhysicsWorld->SetContactListener(m_pContactListener);

	m_pPhysicsWorld->SetContinuousPhysics(true);

	
	m_fFrameTimeMax = 1.0f/120.0f;
	m_fTimeElapsed = 0.0f;
	m_uFrameCount = 0;
	m_uVelIters = 8;
	m_uPosIters = 3;

	return false;
}
b2World& 
ASCPhysicsWorld::GetBox2DWorld()
{
	return *m_pPhysicsWorld;
}
void 
ASCPhysicsWorld::Update( FLOAT32 fDelta )
{
	if(fDelta > 1.0f)
	{
		fDelta = 1.0f;
	}
	m_pDestListener->Update();
	m_pContactListener->Update();
	m_fTimeElapsed += fDelta;
	++m_uFrameCount;
	while(m_fTimeElapsed >= m_fFrameTimeMax)
	{
		m_fTimeElapsed -= m_fFrameTimeMax;
		m_pPhysicsWorld->Step(m_fFrameTimeMax, m_uVelIters, m_uPosIters);
		m_uFrameCount = 0;
	}
}
