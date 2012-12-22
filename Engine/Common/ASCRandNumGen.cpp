#include "ASCGenInc.h"
#include "ASCRandNumGen.h"

#include <time.h>
#include <math.h>

UINT32 ASCRandNumGen::sm_uMaxRand = 100000;
UINT32 ASCRandNumGen::sm_uSeed = 0;


void 
ASCRandNumGen::Initialise(UINT32 uMaxRand)
{
	sm_uMaxRand = uMaxRand;
}
void 
ASCRandNumGen::Seed(UINT32 uSeed)
{
	sm_uSeed = uSeed;
}
void 
ASCRandNumGen::Seed()
{
	Seed(SC_UINT(time(0)));
}
UINT32 
ASCRandNumGen::Rand(UINT32 uUpper)
{
	return Rand() % uUpper;
}
SINT32 
ASCRandNumGen::Rand(SINT32 uLower, SINT32 uUpper)
{
	return uLower + (Rand() % (uUpper - uLower));
}
FLOAT32 
ASCRandNumGen::Rand(FLOAT32 fUpper, FLOAT32 fDecimalSupport)
{
	FLOAT32 fDivisor = powf(10.0f, fDecimalSupport);
	UINT32 uUpper = SC_UINT(fUpper * fDivisor);
	return SC_FLOAT(Rand(uUpper)) / fDivisor;
}
FLOAT32 
ASCRandNumGen::Rand(FLOAT32 fLower, FLOAT32 fUpper, FLOAT32 fDecimalSupport)
{
	FLOAT32 fDivisor = powf(10.0f, fDecimalSupport);
	SINT32 uUpper = SC_SINT(fUpper * fDivisor);
	SINT32 uLower = SC_SINT(fLower * fDivisor);
	return SC_FLOAT(Rand(uLower, uUpper)) / fDivisor;
}
UINT32 
ASCRandNumGen::Rand()
{
    sm_uSeed = sm_uSeed * 1103515245 + 12345;
	return sm_uSeed;
}
