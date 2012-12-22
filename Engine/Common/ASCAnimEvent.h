/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCAnimEvent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCANIMEVENT_H__Included_1789772632
#define _ASCANIMEVENT_H__Included_1789772632

#include "ASCIEvent.h"

namespace ASC
{
	class ASCAnimEvent : public ASCIEvent
	{
	public:
		ASCAnimEvent(void);
		~ASCAnimEvent(void);
		
		void SetAnimEventID(ASCString* pstrString);
		ASCString GetAnimEventID();
	};

}

#endif //_ASCANIMEVENT_H__Included_1789772632

