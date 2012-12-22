#include "ASCGenInc.h"
#include "ASCStringDebugInfo.h"
#include "../Ascension/Ascension.h"

#include "ASCFinalShutdownDelay.h"
#include "ASCStringRecyleBin.h"

void 
ASCStringDebugInfo::DisplayDebugInfo()
{
	Ascension::Debugging().AddScreenLine("### String Info ###");
	Ascension::Debugging().AddScreenLine("");
	
	ASCStringRecyleBin::DispalyInfo();
}
