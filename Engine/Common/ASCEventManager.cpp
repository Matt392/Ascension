#include "ASCGenInc.h"
#include "ASCEventManager.h"


ASCEventManager::ASCEventManager(void)
{
	for(UINT32 i = 0; i < g_skMaxEventTypes; ++i)
	{
		m_uListenerSlots[i] = 0;
	}
}


ASCEventManager::~ASCEventManager(void)
{
}

void 
ASCEventManager::AddEvent(ASCIEvent pEvent)
{
	if(this == NULL)
	{
		return;
	}
	if(pEvent.m_uEventTypeID >= g_skMaxEventTypes)
	{
		assert_now("Not supported index");
	}
	else
	{
		m_pEvents[pEvent.m_uEventTypeID].push_back(pEvent);
	}
}
bool 
ASCEventManager::AddEventListener(ASCIEventListener* pListener, UINT32 uEventTypeID)
{
	if(uEventTypeID >= g_skMaxEventTypes)
	{
		assert_now("Not supported index");
		return false;
	}
	else
	{
		if(m_uListenerSlots[uEventTypeID] > 0)
		{
			for(UINT32 i = 0; i < m_pEventListeners[uEventTypeID].size(); ++i)
			{
				if(m_pEventListeners[uEventTypeID][i] == NULL)
				{
					m_pEventListeners[uEventTypeID][i] = pListener;
					--m_uListenerSlots[uEventTypeID];
					break;
				}
			}
		}
		else
		{
			m_pEventListeners[uEventTypeID].push_back(pListener);
		}

	}
	return true;
}
void 
ASCEventManager::RemoveEventListener(ASCIEventListener* pListener, UINT32 uEventTypeID)
{
	for(UINT32 i = 0; i < m_pEventListeners[uEventTypeID].size(); ++i)
	{
		if(m_pEventListeners[uEventTypeID][i] == pListener)
		{
			m_pEventListeners[uEventTypeID][i] = NULL;
			++m_uListenerSlots[uEventTypeID];
			return;
		}
	}
}
void 
ASCEventManager::Process()
{
	for(UINT32 i = 0; i < g_skMaxEventTypes; ++i)
	{
		for(UINT32 k = 0; k < m_pEvents[i].size(); ++k)
		{
			for(UINT32 j = 0; j < m_pEventListeners[i].size(); ++j)
			{
				if(m_pEventListeners[i][j])
				{
					m_pEventListeners[i][j]->OnEvent(&m_pEvents[i][k]);
				}
			}
		}
		m_pEvents[i].Clear();
	}
}

