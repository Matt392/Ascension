/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCIEvent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCIEVENT_H__Included_36266848
#define _ASCIEVENT_H__Included_36266848


namespace ASC
{
	class ASCIEvent
	{
	public:
		UINT32 ID(){ return m_uEventID; }
		UINT32 TypeID(){ return m_uEventTypeID; }
		void SetID(UINT32 uID){ m_uEventID = uID; }
		void SetEventTypeID(UINT32 uID){ m_uEventTypeID = uID; }
		void SetData(void* pData){ m_pData = pData; }

	protected:
		UINT32 m_uEventTypeID;

		UINT32 m_uEventID;

		void* m_pData;


		friend class ASCEventManager;
	};
}

#endif //_ASCIEVENT_H__Included_36266848

