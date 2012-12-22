#include "ASCGenInc.h"
#include "ASCPhyBody.h"
#include "../Ascension/Ascension.h"
#include "ASCPhysicsScene.h"
#include "ASCPhysicsWorld.h"

ASCPhyBody::ASCPhyBody(b2Body* pBody)
: m_pBody ( pBody )
{
}


ASCPhyBody::~ASCPhyBody(void)
{
}

void 
ASCPhyBody::Release()
{
	m_pBody->SetActive( false );
}
void 
ASCPhyBody::Activate()
{
	m_pBody->SetActive( true );
}