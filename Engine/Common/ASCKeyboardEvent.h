/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCKeyboardEvent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCKEYBOARDEVENT_H__Included_2091704082
#define _ASCKEYBOARDEVENT_H__Included_2091704082

#include "ASCIEvent.h"

namespace ASC
{
	class ASCKeyboardEvent : public ASCIEvent
	{
	public:
		ASCKeyboardEvent(void);
		~ASCKeyboardEvent(void);

		void SetCharVal(ASCString* strCharacter);
		ASCString GetCharVal();
	};

}

#endif //_ASCKEYBOARDEVENT_H__Included_2091704082

