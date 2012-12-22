#include "ASCGenInc.h"
#include "ASCPhysicsListeners.h"
#include "../Ascension/Ascension.h"


ASCPhysicsContactListener::ASCPhysicsContactListener(void)
{
}

ASCPhysicsContactListener::~ASCPhysicsContactListener(void)
{
}
void 
ASCPhysicsContactListener::SendContactEvent(SPhysicsEventData sPhysData)
{
	m_vecPhysEvents.push_back(sPhysData);

	ASCPhysicsEvent PhyEvent;
	//PhyEvent.m_uEventTypeID = ET_PHYSICS;
	PhyEvent.SetPhysicsData(&m_vecPhysEvents[m_vecPhysEvents.size()-1]);

	Ascension::EventSystem().AddEvent(PhyEvent);

}
void 
ASCPhysicsContactListener::Update()
{
	m_vecPhysEvents.Clear();
}
void 
ASCPhysicsContactListener::BeginContact(b2Contact* contact)
{
	if(contact && Ascension::Running())
	{
		SPhysicsEventData sPhysEventData;
		sPhysEventData.m_ePhyEventType = PET_CONTACT;
		sPhysEventData.m_bBeginContact = true;

		b2Fixture* pFixture = NULL;
		b2Body* pBody = NULL;
		pFixture = contact->GetFixtureA();
		if(pFixture)
		{
			pBody = pFixture->GetBody();
			if(pBody)
			{
				sPhysEventData.m_pObjects[0] = reinterpret_cast<ASCPhysicsObject*>(pBody->GetUserData());
			}
		}
		pFixture = contact->GetFixtureB();
		if(pFixture)
		{
			pBody = pFixture->GetBody();
			if(pBody)
			{
				sPhysEventData.m_pObjects[1] = reinterpret_cast<ASCPhysicsObject*>(pBody->GetUserData());
			}
		}

		SendContactEvent(sPhysEventData);
	}
}
void 
ASCPhysicsContactListener::EndContact(b2Contact* contact)
{
	if(contact && Ascension::Running())
	{
		SPhysicsEventData sPhysEventData;
		sPhysEventData.m_ePhyEventType = PET_CONTACT;
		sPhysEventData.m_bBeginContact = false;

		b2Fixture* pFixture = NULL;
		b2Body* pBody = NULL;
		pFixture = contact->GetFixtureA();
		if(pFixture)
		{
			pBody = pFixture->GetBody();
			if(pBody)
			{
				sPhysEventData.m_pObjects[0] = reinterpret_cast<ASCPhysicsObject*>(pBody->GetUserData());
			}
		}
		pFixture = contact->GetFixtureB();
		if(pFixture)
		{
			pBody = pFixture->GetBody();
			if(pBody)
			{
				sPhysEventData.m_pObjects[1] = reinterpret_cast<ASCPhysicsObject*>(pBody->GetUserData());
			}
		}

		SendContactEvent(sPhysEventData);
	}
}
void 
ASCPhysicsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	contact = contact;
	oldManifold = oldManifold;
}
void 
ASCPhysicsContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	contact = contact;
	impulse = impulse;
}



ASCPhysicsDestructionListener::ASCPhysicsDestructionListener(void)
{
}

ASCPhysicsDestructionListener::~ASCPhysicsDestructionListener(void)
{
	UINT32 uTest;
	uTest = 0;
}
void 
ASCPhysicsDestructionListener::SendContactEvent(SPhysicsEventData sPhysData)
{
	m_vecPhysEvents.push_back(sPhysData);

	ASCPhysicsEvent PhyEvent;
	PhyEvent.SetPhysicsData(&m_vecPhysEvents[m_vecPhysEvents.size()-1]);

	Ascension::EventSystem().AddEvent(PhyEvent);

}
void 
ASCPhysicsDestructionListener::Update()
{
	m_vecPhysEvents.Clear();
}
void 
ASCPhysicsDestructionListener::SayGoodbye(b2Fixture* fixture)
{
	if(fixture && Ascension::Running())
	{
		SPhysicsEventData sPhysEventData;
		sPhysEventData.m_ePhyEventType = PET_DESTRUCTION;

		b2Body* pBody = NULL;
		pBody = fixture->GetBody();
		if(pBody)
		{
			sPhysEventData.m_pObjects[0] = reinterpret_cast<ASCPhysicsObject*>(pBody->GetUserData());
		}

		SendContactEvent(sPhysEventData);
	}
}
void 
ASCPhysicsDestructionListener::SayGoodbye(b2Joint* joint)
{
	joint;
}