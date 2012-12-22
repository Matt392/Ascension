/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCFinalShutdownDelay.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCFINALSHUTDOWNDELAY_H__Included_2049731737
#define _ASCFINALSHUTDOWNDELAY_H__Included_2049731737

#include "ASCMemoryManagement.h"

namespace ASC
{
	class ASCFinalShutdownDelay
	{
	public:
		static void 
		ASCFinalShutdownDelay::AddDelay()
		{
			if(sm_uDelays <= 0 && false == sm_bCanEnd)
			{
				ASCMemoryManagement::Start(ASC_MAX_MEM_STORED, false, ASC_MAX_TRACE_DEPTH);
			}
			++sm_uDelays;
		}
		static void 
		ASCFinalShutdownDelay::RemoveDelay()
		{
			--sm_uDelays;
			if(sm_uDelays <= 0 && sm_bCanEnd)
			{
				ASCMemoryManagement::End();
			}
		}

		static void SetCanEnd(bool bVal);
		static bool CanEnd();

	protected:
		static SINT32 sm_uDelays;
		static bool sm_bCanEnd;
	};

}

#endif //_ASCFINALSHUTDOWNDELAY_H__Included_2049731737

