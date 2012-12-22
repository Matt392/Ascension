/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCPhysicsListeners.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCPHYSICSLISTENERS_H__Included_2019396312
#define _ASCPHYSICSLISTENERS_H__Included_2019396312

#include "Box2D.h"
#include "ASCPhysicsEvent.h"

namespace ASC
{
	class ASCPhysicsContactListener : public b2ContactListener
	{
	public:
		ASCPhysicsContactListener(void);
		~ASCPhysicsContactListener(void);
		
		/// Called when two fixtures begin to touch.
		virtual void BeginContact(b2Contact* contact);

		/// Called when two fixtures cease to touch.
		virtual void EndContact(b2Contact* contact);

		/// This is called after a contact is updated. This allows you to inspect a
		/// contact before it goes to the solver. If you are careful, you can modify the
		/// contact manifold (e.g. disable contact).
		/// A copy of the old manifold is provided so that you can detect changes.
		/// Note: this is called only for awake bodies.
		/// Note: this is called even when the number of contact points is zero.
		/// Note: this is not called for sensors.
		/// Note: if you set the number of contact points to zero, you will not
		/// get an EndContact callback. However, you may get a BeginContact callback
		/// the next step.
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

		/// This lets you inspect a contact after the solver is finished. This is useful
		/// for inspecting impulses.
		/// Note: the contact manifold does not include time of impact impulses, which can be
		/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
		/// in a separate data structure.
		/// Note: this is only called for contacts that are touching, solid, and awake.
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

		void SendContactEvent(SPhysicsEventData sPhysData);
		void Update();

	protected:
		ASCVector<SPhysicsEventData> m_vecPhysEvents;
	};

	
	class ASCPhysicsDestructionListener : public b2DestructionListener
	{
	public:
		ASCPhysicsDestructionListener(void);
		~ASCPhysicsDestructionListener(void);
		
		/// Called when any joint is about to be destroyed due
		/// to the destruction of one of its attached bodies.
		virtual void SayGoodbye(b2Joint* joint);

		/// Called when any fixture is about to be destroyed due
		/// to the destruction of its parent body.
		virtual void SayGoodbye(b2Fixture* fixture);

		void SendContactEvent(SPhysicsEventData sPhysData);
		void Update();

	protected:
		ASCVector<SPhysicsEventData> m_vecPhysEvents;
	};

}

#endif //_ASCPHYSICSLISTENERS_H__Included_2019396312

