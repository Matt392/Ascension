#include "ASCGenInc.h"
#include "ASCMatrix4.h"


ASCMatrix4::ASCMatrix4(void)
{
	for(UINT32 i = 0; i < 4; ++i)
	{
		for(UINT32 j = 0; j < 4; ++j)
		{
			m_fValues[i][j] = 0.0f;
		}
	}
}


ASCMatrix4::~ASCMatrix4(void)
{
}
