/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Data Types\ASCTree.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCTREE_H__Included_1474668698
#define _ASCTREE_H__Included_1474668698

#include "ASCGenInc.h"
#include "ASCFileIO.h"
#include "ASCAssert.h"

namespace ASC
{
	template<typename T> class ASCTreeNode
	{
	public:
		ASCTreeNode(T tDefault)
		: m_tBadIndex ( tDefault )
		, m_bPossibleEnd ( false )
		{
		}
		ASCTreeNode()
		: m_bPossibleEnd ( false )
		{
		}
		~ASCTreeNode(void)
		{
			for(UINT32 i = 0; i < m_vecChildern.size(); ++i)
			{
				SafeDelete( m_vecChildern[i] );
			}
		}
		void Add(ASCString strID, T val, ASCVector<T*>& rRaw)
		{
			CHAR8 cID = strID[0];
			if(strID.length() > 0)
			{
				ASCString strTemp;
				strID.substr(1, strTemp);
				strID = strTemp;
				for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
				{
					if(m_vecIDs[i] == cID)
					{
						m_vecChildern[i]->Add(strID, val, rRaw);
						return;
					}
				}
				ASCTreeNode<T>* pTreeNode = new ASCTreeNode<T>( m_tBadIndex );
				pTreeNode->Add(strID, val, rRaw);

				m_vecIDs.push_back(cID);
				m_vecChildern.push_back(pTreeNode);
			}
			else
			{
				m_bPossibleEnd = true;
				m_tVal = val;
				rRaw.push_back(&m_tVal);
			}
		}
		void Remove(ASCString strID)
		{
			CHAR8 cID = strID[0];
			if(strID.length() > 0)
			{
				ASCString strTemp;
				strID.substr(1, strTemp);
				strID = strTemp;
				for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
				{
					if(m_vecIDs[i] == cID)
					{
						m_vecChildern[i]->Remove(strID);
						return;
					}
				}
			}
			else
			{
				m_bPossibleEnd = false;
				m_tVal = m_tBadIndex;
			}
		}
		T At(ASCString strID)
		{
			CHAR8 cID = strID[0];

			if(strID.length() > 0)
			{
				ASCString strTemp;
				strID.substr(1, strTemp);
				strID = strTemp;
				for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
				{
					if(m_vecIDs[i] == cID)
					{
						return m_vecChildern[i]->At(strID);
					}
				}
			}
			else
			{
				if(m_bPossibleEnd)
				{
					return m_tVal;
				}
			}
			return m_tBadIndex;
		}
		
		bool Contains(ASCString strID)
		{
			CHAR8 cID = strID[0];

			if(strID.length() > 0)
			{
				ASCString strTemp;
				strID.substr(1, strTemp);
				strID = strTemp;
				for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
				{
					if(m_vecIDs[i] == cID)
					{
						return m_vecChildern[i]->Contains(strID);
					}
				}
			}
			else
			{
				if(m_bPossibleEnd)
				{
					return true;
				}
			}
			return false;
		}
		void LoadBinary( void*& pData, ASCVector<T*>& rRaw )
		{
			ASCString strType = typeid(m_tVal).name();
			if(strType == strStringComp)
			{
				UINT32* pUintData = NULL;
				UINT32 uChildCount = 0;

				ASCString* pString = NULL;

				pUintData = reinterpret_cast<UINT32*>( pData );

				//Get Child Count
				uChildCount = pUintData[0];
				++pUintData;
				
				bool* pBoolData = reinterpret_cast<bool*>( pUintData );

				m_bPossibleEnd = pBoolData[0];
				++pBoolData;
				
				pData = reinterpret_cast<UINT32*>( pBoolData );

				if(m_bPossibleEnd)
				{
					//Load Value String
					pString = reinterpret_cast<ASCString*>(&m_tVal);
					
					pString->LoadBinary(pData);
					
					if(pString->length() > 0)
					{
						rRaw.push_back(&m_tVal);
					}
				}
				
				m_vecIDs.LoadBinary(pData);

				for(UINT32 i = 0; i < uChildCount; ++i)
				{
					ASCTreeNode<T>* tNewNode = new ASCTreeNode<T>();
					tNewNode->LoadBinary(pData, rRaw);
					m_vecChildern.push_back(tNewNode);
				}
				if(m_vecChildern.size() != uChildCount)
				{
					assert_now("Guts, Incorrect amount of children");
				}

			}
			else if(strType.find('*') >= 0)
			{
				assert_now("Guts, Non string tree serialization not implemented");
			}
			else
			{
				assert_now("Guts, Non string tree serialization not implemented");
			}
		}
		void Serialize( ASCFileIO* pFile )
		{
			ASCString strType = typeid(m_tVal).name();
			if(strType == strStringComp)
			{
				ASCString* pString = NULL;
				UINT32 uChildCount = m_vecChildern.size();
				
				pFile->OutputBianry(&uChildCount, sizeof(UINT32));// Child count
				
				pFile->OutputBianry(&m_bPossibleEnd, sizeof(bool));// Possible end

				if(m_bPossibleEnd)
				{
					pString = reinterpret_cast<ASCString*>(&m_tVal);
					
					pString->Serialize( pFile );
				}
				
				m_vecIDs.Serialize( pFile );

				for(UINT32 i = 0; i < m_vecChildern.size(); ++i)
				{
					m_vecChildern[i]->Serialize( pFile );
				}
			}
			else if(strType.find('*') >= 0)
			{
				assert_now("Guts, Non string tree serialization not implemented");
			}
			else
			{
				assert_now("Guts, Non string tree serialization not implemented");
			}
		}
	protected:
		T m_tVal;
		T m_tBadIndex;
		bool m_bPossibleEnd;
		ASCVector<CHAR8> m_vecIDs;
		ASCVector<ASCTreeNode<T>*> m_vecChildern;
	};
	template<typename T> class ASCTree
	{
	public:
		ASCTree(T tDefault)
		: m_tBadIndex ( tDefault )
		{
		}
		ASCTree()
		{
		}
		~ASCTree(void)
		{
			for(UINT32 i = 0; i < m_Roots.size(); ++i)
			{
				SafeDelete( m_Roots[i] );
			}
		}
		void Add(ASCString strID, T val)
		{
			CHAR8 cID = strID[0];
			ASCString strTemp;
			strID.substr(1, strTemp);
			strID = strTemp;
			//m_veRaw.push_back(val);

			for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
			{
				if(m_vecIDs[i] == cID)
				{
					m_Roots[i]->Add(strID, val, m_veRaw);
					return;
				}
			}
			ASCTreeNode<T>* pTreeNode = new ASCTreeNode<T>( m_tBadIndex );
			pTreeNode->Add(strID, val, m_veRaw);

			m_vecIDs.push_back(cID);
			m_Roots.push_back(pTreeNode);
		}
		void Remove(ASCString strID)
		{
			CHAR8 cID = strID[0];
			ASCString strTemp;
			strID.substr(1, strTemp);
			strID = strTemp;

			for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
			{
				if(m_vecIDs[i] == cID)
				{
					return m_Roots[i]->Remove(strID);
				}
			}
		}
		T At(ASCString strID)
		{
			CHAR8 cID = strID[0];
			ASCString strTemp;
			strID.substr(1, strTemp);
			strID = strTemp;

			for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
			{
				if(m_vecIDs[i] == cID)
				{
					return m_Roots[i]->At(strID);
				}
			}
			return m_tBadIndex;
		}
		bool Contains(ASCString strID)
		{
			CHAR8 cID = strID[0];
			ASCString strTemp;
			strID.substr(1, strTemp);
			strID = strTemp;

			for(UINT32 i = 0; i < m_vecIDs.size(); ++i)
			{
				if(m_vecIDs[i] == cID)
				{
					return m_Roots[i]->Contains(strID);
				}
			}
			return false;
		}
		T& operator[](const UINT32& _iIndex)
		{
			return *(m_veRaw[_iIndex]);
		}
		UINT32 size()
		{
			return m_veRaw.size();
		}
		void LoadBinary( void*& pData )
		{
			ASCString strType = typeid(m_tBadIndex).name();
			if(strType == strStringComp)
			{
				UINT32 uRootCount = 0;
				
				UINT32* pUintData = NULL;

				pUintData = reinterpret_cast<UINT32*>( pData );

				uRootCount = pUintData[0];
				++pUintData;

				pData = pUintData;

				m_vecIDs.LoadBinary(pData);
				
				for(UINT32 i = 0; i < uRootCount; ++i)
				{
					ASCTreeNode<T>* tNewNode = new ASCTreeNode<T>();
					tNewNode->LoadBinary(pData, m_veRaw);
					m_Roots.push_back(tNewNode);
				}
				if(m_Roots.size() != uRootCount)
				{
					assert_now("Guts, Incorrect amount of children");
				}
			}
			else if(strType.find('*') >= 0)
			{
				assert_now("Guts, Non string tree serialization not implemented");
			}
			else
			{
				assert_now("Guts, Non string tree serialization not implemented");
			}
		}
		void Serialize( ASCFileIO* pFile )
		{
			ASCString strType = typeid(m_tBadIndex).name();
			if(strType == strStringComp)
			{
				UINT32 uRootCount = m_Roots.size();
				
				pFile->OutputBianry(&uRootCount, sizeof(UINT32));

				m_vecIDs.Serialize( pFile );

				//Size of roots
				for(UINT32 i = 0; i < m_Roots.size(); ++i)
				{
					m_Roots[i]->Serialize( pFile );
				}
			}
			else if(strType.find('*') >= 0)
			{
			}
			else
			{
				assert_now ("Guts, Non string tree serialization not implemented");
			}
		}
		void BadIndex(T strBad)
		{
			m_tBadIndex = strBad;
		}
	protected:
		ASCVector<CHAR8> m_vecIDs;
		ASCVector<ASCTreeNode<T>*> m_Roots;

		ASCVector<T*> m_veRaw;
		T m_tBadIndex;

	};

}

#endif //_ASCTREE_H__Included_1474668698

