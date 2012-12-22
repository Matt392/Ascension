/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCCMDEvent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCCMDEVENT_H__Included_1373423233
#define _ASCCMDEVENT_H__Included_1373423233

#include "ASCIEvent.h"

namespace ASC
{
	class ASCCMDEvent : public ASCIEvent
	{
	public:
		ASCCMDEvent(void);
		~ASCCMDEvent(void);
		
		void SetCMD(ASCString* pstrString);
		ASCString& GetCMD();
	};

}

#endif //_ASCCMDEVENT_H__Included_1373423233

