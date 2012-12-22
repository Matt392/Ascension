#include "ASCGenInc.h"
#include "ASCFinalShutdownDelay.h"
#include "ASCMemoryManagement.h"

SINT32 ASCFinalShutdownDelay::sm_uDelays = 0;
bool ASCFinalShutdownDelay::sm_bCanEnd = false;

void 
ASCFinalShutdownDelay::SetCanEnd(bool bVal)
{
	sm_bCanEnd = bVal;
}
bool 
ASCFinalShutdownDelay::CanEnd()
{
	return sm_bCanEnd;
}