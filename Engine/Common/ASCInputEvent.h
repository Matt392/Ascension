/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCInputEvent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCINPUTEVENT_H__Included_480016958
#define _ASCINPUTEVENT_H__Included_480016958

#include "ASCIEvent.h"

namespace ASC
{
	class ASCInputEvent : public ASCIEvent
	{
	public:
		ASCInputEvent(void);
		~ASCInputEvent(void);

		void SetState(EKeyState* eState);
		EKeyState GetState();

		void SetScrollWheel(FLOAT32* fVal);
		FLOAT32 GetScrollWheel();

		void SetPos(SDoubleFloat* ffVal);
		SDoubleFloat GetPos();
        
        void SetData(SInputData* sData);
        SInputData GetData();
	};

}

#endif //_ASCINPUTEVENT_H__Included_480016958

