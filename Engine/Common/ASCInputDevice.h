/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCInputDevice.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCINPUTDEVICE_H__Included_1966481752
#define _ASCINPUTDEVICE_H__Included_1966481752

#include "ASCGenInc.h"

namespace ASC
{
	class ASCInputDevice
	{
	public:
		virtual ~ASCInputDevice()
		{
		}
		virtual void Update( FLOAT32 fTimeDelta ) = 0;

		virtual bool RegisterInput( UINT32& uEventID, ASCString strID ) = 0;
			
		virtual void SetFullScreen(bool bVal) = 0;

		virtual void DisplayDebugInfo() = 0;
	};
}

#endif //_ASCINPUTDEVICE_H__Included_1966481752

