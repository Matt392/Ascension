#include "ASCGenInc.h"
#include "ASCPhysicsEvent.h"


ASCPhysicsEvent::ASCPhysicsEvent(void)
{
	m_uEventTypeID = ET_PHYSICS;
}


ASCPhysicsEvent::~ASCPhysicsEvent(void)
{
}
void 
ASCPhysicsEvent::SetPhysicsData(SPhysicsEventData* pPhyData)
{
	m_pData = pPhyData;
}
SPhysicsEventData& 
ASCPhysicsEvent::GetPhysicsData()
{
	return *reinterpret_cast<SPhysicsEventData*>(m_pData);
}
