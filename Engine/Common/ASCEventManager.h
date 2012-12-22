/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCEventManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCEVENTMANAGER_H__Included_885126845
#define _ASCEVENTMANAGER_H__Included_885126845

#include "ASCIEvent.h"
#include "ASCIEventListener.h"

namespace ASC
{
	static const UINT32 g_skMaxEventTypes = 32;
	
	enum EEventType
	{
		ET_INPUT,
		ET_GUI,
		ET_ANIM,
		ET_CMD_WND,
		ET_KEYBOARD,
		ET_PHYSICS,
		ET_APPLICATION,
		ET_COUNT,
	};

	class ASCEventManager
	{
	public:
		ASCEventManager(void);
		~ASCEventManager(void);
		void AddEvent(ASCIEvent pEvent);
		bool AddEventListener(ASCIEventListener* pListener, UINT32 uEventTypeID);
		void RemoveEventListener(ASCIEventListener* pListener, UINT32 uEventTypeID);

		void Process();

	protected:
		ASCVector<ASCIEventListener*> m_pEventListeners[g_skMaxEventTypes];
		UINT32 m_uListenerSlots[g_skMaxEventTypes];
		ASCVector<ASCIEvent> m_pEvents[g_skMaxEventTypes];
	};

}

#endif //_ASCEVENTMANAGER_H__Included_885126845

