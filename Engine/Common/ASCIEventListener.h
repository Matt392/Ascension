/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCIEventListener.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCIEVENTLISTENER_H__Included_1485140575
#define _ASCIEVENTLISTENER_H__Included_1485140575


namespace ASC
{
	class ASCIEvent;

	class ASCIEventListener
	{
	public:
		ASCIEventListener()
		: m_bActive ( true )
		{
		}
		virtual void OnEvent(ASCIEvent* pEvent) = 0;
		virtual void Activate()
		{
			m_bActive = true;
		}
		virtual void Deactivate()
		{
			m_bActive = false;
		}
		
	protected:
		UINT32 m_uEventTypeID;
		
		bool m_bActive;

		friend class ASCEventManager;
	};
}

#endif //_ASCIEVENTLISTENER_H__Included_1485140575

