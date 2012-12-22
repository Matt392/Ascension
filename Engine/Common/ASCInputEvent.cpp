#include "ASCGenInc.h"
#include "ASCInputEvent.h"


ASCInputEvent::ASCInputEvent(void)
{
}


ASCInputEvent::~ASCInputEvent(void)
{
}

void 
ASCInputEvent::SetState(EKeyState* eState)
{
	m_pData = eState;
}
EKeyState 
ASCInputEvent::GetState()
{
	return *reinterpret_cast<EKeyState*>(m_pData);
}
void 
ASCInputEvent::SetScrollWheel(FLOAT32* fVal)
{
	m_pData = fVal;
}
FLOAT32 
ASCInputEvent::GetScrollWheel()
{
	return *reinterpret_cast<FLOAT32*>(m_pData);
}
void 
ASCInputEvent::SetPos(SDoubleFloat* ffVal)
{
	m_pData = ffVal;
}
SDoubleFloat 
ASCInputEvent::GetPos()
{
	return *reinterpret_cast<SDoubleFloat*>(m_pData);
}        
void 
ASCInputEvent::SetData(SInputData* sData)
{
	m_pData = sData;
}
SInputData 
ASCInputEvent::GetData()
{
	return *reinterpret_cast<SInputData*>(m_pData);
}