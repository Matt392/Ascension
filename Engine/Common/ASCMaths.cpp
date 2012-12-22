#include "ASCGenInc.h"
#include "ASCMaths.h"

static const FLOAT32 g_sfPI = 3.14159265f;

FLOAT32 
ASCMaths::RadToDeg(FLOAT32 fVal)
{
	static const FLOAT32 fRatio = 180.0f / g_sfPI;
	return fVal * fRatio;
}
FLOAT32 
ASCMaths::DegToRad(FLOAT32 fVal)
{
	static const FLOAT32 fRatio = g_sfPI / 180.0f;
	return fVal * fRatio;
}
FLOAT32 
ASCMaths::PI()
{
	return g_sfPI;
}
