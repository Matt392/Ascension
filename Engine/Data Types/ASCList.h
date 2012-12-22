/**
*	@Danger Balls
 *	@file Ascension 2D\Engine\Data Types\ASCList.h
 *	@brief 
 *	
 *********************************************************************************************/
#ifndef _ASCLIST_H__Included_631192737
#define _ASCLIST_H__Included_631192737

#include "ASCVector.h"

namespace ASC
{
	template<typename T> class ASCListNode
	{
	public:
		ASCListNode(void)
		: m_pPrevious ( NULL )
		, m_pNext ( NULL )
		{
		}
		~ASCListNode(void)
		{
		}
		T Value()
		{
			return m_tValue;
		}
		void Value(T tValue)
		{
			m_tValue = tValue;
		}
		void Set(T tValue, ASCListNode<T>* pPrevious, ASCListNode<T>* pNext)
		{
			m_tValue = tValue;
			m_pPrevious = pPrevious;
			m_pNext = pNext;
			m_bActive = true;
		}
		void SetNeighbours(ASCListNode<T>* pPrevious, ASCListNode<T>* pNext)
		{
			m_pPrevious = pPrevious;
			m_pNext = pNext;
		}
		ASCListNode<T>* Next()
		{
			return m_pNext;
		}
		ASCListNode<T>* Previous()
		{
			return m_pPrevious;
		}
		void Next(ASCListNode<T>* pNode)
		{
			m_pNext = pNode;
		}
		void Previous(ASCListNode<T>* pNode)
		{
			m_pPrevious = pNode;
		}
		bool Active()
		{
			return m_bActive;
		}
		void Active(bool bValue)
		{
			m_bActive = bValue;
		}
	protected:
		T m_tValue;
		bool m_bActive;
		ASCListNode<T>* m_pPrevious;
		ASCListNode<T>* m_pNext;
	};

	template<typename T> class ASCList
	{
	public:
		ASCList(void)
		: m_uAvaliableNodes ( 0 )
		, m_pFirst ( NULL )
		, m_pLast ( NULL )
		{

		}
		~ASCList(void)
		{
			for(UINT32 i = 0; i < m_pNodes.size(); ++i)
			{
				SafeDelete( m_pNodes[i] );
			}
		}

		T FirstValue()
		{
			return m_pFirst->Value();
		}
		ASCListNode<T>* First()
		{
			return m_pFirst;
		}
		ASCListNode<T>* Last()
		{
			return m_pLast;
		}
		
		void AddToListEnd(T tValue)
		{
			ASCListNode<T>* pNode = AvaliableNode();
			if(m_pFirst == NULL)
			{
				pNode->Set(tValue, NULL, NULL);
				m_pLast = pNode;
				m_pFirst = pNode;
			}
			else
			{
				pNode->Set(tValue, m_pLast, NULL);
				m_pLast->Next( pNode );
				m_pLast = pNode;
			}
		}
		void AddToListStart(T tValue)
		{
			ASCListNode<T>* pNode = AvaliableNode();
			if(m_pFirst == NULL)
			{
				pNode->Set(tValue, NULL, NULL);
				m_pLast = pNode;
				m_pFirst = pNode;
			}
			else
			{
				pNode->Set(tValue, NULL, m_pFirst);
				m_pFirst->Previous( pNode );
				m_pFirst = pNode;
			}
		}
		void RemoveFirst()
		{
			m_pFirst->Active( false );
			++m_uAvaliableNodes;
			ASCListNode<T>* pNextNode = m_pFirst->Next();
			if(pNextNode)
			{
				pNextNode->Previous( NULL );
				m_pFirst = pNextNode;
			}
			else
			{
				m_pFirst = NULL;
				m_pLast = NULL;
			}
		}
		void RemoveLast()
		{
			m_pLast->Active( false );
			++m_uAvaliableNodes;
			ASCListNode<T>* pPrevNode = m_pLast->Previous();
			if(pPrevNode)
			{
				pPrevNode->Next( NULL );
				m_pLast = pPrevNode;
			}
			else
			{
				m_pFirst = NULL;
				m_pLast = NULL;
			}
		}

		ASCListNode<T>* AvaliableNode()
		{
			if( m_uAvaliableNodes > 0)
			{
				for(UINT32 i = 0; i < m_pNodes.size(); ++i)
				{
					if(false == m_pNodes[i]->Active())
					{
						--m_uAvaliableNodes;
						m_pNodes[i]->Active(true);
						return m_pNodes[i];
					}
				}
			}
			m_uAvaliableNodes = 0;
			ASCListNode<T>* pNewNode = new ASCListNode<T>();
			m_pNodes.push_back(pNewNode);
			return pNewNode;
		}

		void SetResizeSize(UINT32 uSize)
		{
			m_pNodes.SetResizeSize( uSize );
		}
		UINT32 size()
		{
			return m_pNodes.size() - m_uAvaliableNodes;
		}
		UINT32 size_avaliable()
		{
			return m_pNodes.size_avaliable();
		}
		void Clear()
		{
			for(UINT32 i = 0; i < m_pNodes.size(); ++i)
			{
				if(true == m_pNodes[i]->Active())
				{
					m_pNodes[i]->Active( false );
				}
			}
			m_uAvaliableNodes = m_pNodes.size();
			m_pFirst = NULL;
			m_pLast = NULL;
		}
		void FillListUINTSPECINC(UINT32 uCount)
		{
			ASCListNode<T>* pNodes = new ASCListNode<T>[uCount];
			//pNodes = reinterpret_cast<ASCListNode**>(ASCMemoryManagement::GetMemory(sizeof(ASCListNode<T>) * uCount, false));
			m_pNodes.SetResizeSize( uCount );
			//m_pNodes.push_back_bulk(&pNodes, uCount);
			for(UINT32 i = 0; i < uCount; ++i)
			{
				m_pNodes.push_back(&pNodes[i]);
			}

			m_pNodes[0]->Set(0, NULL, m_pNodes[1]);
			for(UINT32 i = 1; i < m_pNodes.size()-1; ++i)
			{
				m_pNodes[i]->Set(i, m_pNodes[i-1], m_pNodes[i+1]);
			}
			m_pNodes[uCount-1]->Set(uCount-1, m_pNodes[uCount-2], NULL);
		}
	protected:
		UINT32 m_uAvaliableNodes;
		ASCVector<ASCListNode<T>*> m_pNodes;
		ASCListNode<T>* m_pFirst;
		ASCListNode<T>* m_pLast;
	};

}

#endif //_ASCLIST_H__Included_631192737

