#include "ASCGenInc.h"
#include "ASCAnimEvent.h"


ASCAnimEvent::ASCAnimEvent(void)
{
}


ASCAnimEvent::~ASCAnimEvent(void)
{
}

void 
ASCAnimEvent::SetAnimEventID(ASCString* pstrString)
{
	m_pData = pstrString;
}
ASCString 
ASCAnimEvent::GetAnimEventID()
{
	return *reinterpret_cast<ASCString*>(m_pData);
}