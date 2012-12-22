#include "ASCGenInc.h"
#include "ASCCMDEvent.h"


ASCCMDEvent::ASCCMDEvent(void)
{
}


ASCCMDEvent::~ASCCMDEvent(void)
{
}
		
void 
ASCCMDEvent::SetCMD(ASCString* pstrString)
{
	m_pData = pstrString;
}
ASCString& 
ASCCMDEvent::GetCMD()
{
	return *reinterpret_cast<ASCString*>(m_pData);
}
