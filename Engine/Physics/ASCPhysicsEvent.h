/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCPhysicsEvent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCPHYSICSEVENT_H__Included_440185184
#define _ASCPHYSICSEVENT_H__Included_440185184

#include "ASCIEvent.h"

namespace ASC
{
	class ASCPhysicsObject;

	enum EPhysicsEventType
	{
		PET_CONTACT,
		PET_DESTRUCTION,
		PET_COUNT,
	};

	struct SPhysicsEventData
	{
		EPhysicsEventType m_ePhyEventType;

		ASCPhysicsObject* m_pObjects[2];

		bool m_bBeginContact;

		SPhysicsEventData()
		{
			m_ePhyEventType = PET_COUNT;

			m_pObjects[0] = NULL;
			m_pObjects[1] = NULL;

			m_bBeginContact = false;

		}
	};

	class ASCPhysicsEvent : public ASCIEvent
	{
	public:
		ASCPhysicsEvent(void);
		~ASCPhysicsEvent(void);

		void SetPhysicsData(SPhysicsEventData* pPhyData);
		SPhysicsEventData& GetPhysicsData();
	};

}

#endif //_ASCPHYSICSEVENT_H__Included_440185184

