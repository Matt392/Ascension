/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Data Types\ASCVector.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCVECTOR_H__Included_1040089712
#define _ASCVECTOR_H__Included_1040089712

#include "ASCGenInc.h"

#ifdef ASC_IOS
#include <typeinfo>
#endif

namespace ASC
{
    
#ifdef ASC_IOS
    static ASCString strStringComp = "N3ASC9ASCStringE";
#else
    static ASCString strStringComp = "class ASC::ASCString";
    
#endif
    
	const UINT32 m_kiInitialSize = 10;
	const UINT32 m_kiIncereaseSize = 10;

	template<typename T> class ASCVector
	{
	public:
		ASCVector(void)
		{
			m_bFreed = false;
			m_iIntSize = m_kiInitialSize;
			m_iIncreaseSize = m_kiIncereaseSize;
			m_tValues = NULL;
			m_iArraySize = 0;
			m_iTotalSize = m_iIntSize;
			m_tValues = new T[m_iTotalSize];
		}
		ASCVector(UINT32 _iIntSize)
		{
			m_bFreed = false;
			m_iIntSize = _iIntSize;
			m_iIncreaseSize = m_kiIncereaseSize;
			m_tValues = NULL;
			m_iArraySize = 0;
			m_iTotalSize = m_iIntSize;
			m_tValues = new T[m_iTotalSize];
		}
		ASCVector(UINT32 _iIntSize, UINT32 _iIncreaseSize)
		{
			m_bFreed = false;
			m_iIntSize = _iIntSize;
			m_iIncreaseSize = _iIncreaseSize;
			m_tValues = NULL;
			m_iArraySize = 0;
			m_iTotalSize = m_iIntSize;
			m_tValues = new T[m_iTotalSize];
		}
		~ASCVector(void)
		{
			if(m_tValues && false == m_bFreed)
			{
				delete[] m_tValues;
				m_tValues = NULL;
				//m_bFreed = true;
			}
		}
		bool push_back(T _tVal)
		{
			//m_bFreed = false;
			if(m_iArraySize >= m_iTotalSize)// || m_iArraySize == 0)
			{
				IncreaseVectorSize();
				m_tValues[m_iArraySize] = _tVal;
				++m_iArraySize;
				return true;
			}
			m_tValues[m_iArraySize] = _tVal;
			++m_iArraySize;
			return false;
		}
		void push_back_bulk(T* _tVal, UINT32 uCount)
		{
			//m_bFreed = false;
			while(m_iArraySize+uCount >= m_iTotalSize)// || m_iArraySize == 0)
			{
				IncreaseVectorSize();
			}
			memcpy(&m_tValues[m_iArraySize], _tVal, sizeof(T) * uCount);
			m_iArraySize += uCount;
		}
		T pop_back()
		{
			T tVal = m_tValues[m_iArraySize-1];
			--m_iArraySize;
			return tVal;
		}
		UINT32 size()
		{
			return m_iArraySize;
		}
		UINT32 size_avaliable()
		{
			return m_iTotalSize;
		}

		void remove(const UINT32& _iIndex)
		{
			for(UINT32 i = _iIndex; i < m_iArraySize; ++i)
			{
				if(i < m_iArraySize-1)
				{
					m_tValues[i] = m_tValues[i+1];
				}
			}
			--m_iArraySize;
		}
		void swap(const UINT32& _iIndexA, const UINT32& _iIndexB)
		{
			Temp = m_tValues[_iIndexA];
			m_tValues[_iIndexA] = m_tValues[_iIndexB];
			m_tValues[_iIndexB] = Temp;
		}
		void insert(const UINT32& _iIndexA, const T tVal)
		{
			++m_iArraySize;
			if(m_iArraySize >= m_iTotalSize)
			{
				IncreaseVectorSize();
			}
			
			for( UINT32 uiCount = m_iArraySize; _iIndexA < uiCount; --uiCount )
			{
				m_tValues[uiCount] = m_tValues[( uiCount - 1 )];
			}
			m_tValues[_iIndexA] = tVal;

			//++m_uiSize;
		}


		T& at(const UINT32& _iIndex)
		{
			return m_tValues[_iIndex];
		}
		void setat(const UINT32& _iIndex, const T& _tVal)
		{
			m_tValues[_iIndex] = _tVal;
		}
		T& operator[](const UINT32& _iIndex)
		{
			return m_tValues[_iIndex];
		}
		T* raw()
		{
			return m_tValues;
		}
		void Clear()
		{
			m_iArraySize = 0;
		}
		//_stricmp
		UINT32 TotalSize()
		{
			return m_iTotalSize;
		}
		void SetDefault(T tValue)
		{
			m_tDefault = tValue;
			for(UINT32 i = m_iArraySize; i < m_iTotalSize; ++i)
			{
				m_tValues[i] = m_tDefault;
			}
		}
		void SetResizeSize(UINT32 uSize)
		{
			m_iIncreaseSize = uSize;
		}

		bool Valid()
		{
			return m_tValues != NULL;
		}
		void operator=(ASCVector<T>& rVector)
		{
			m_iArraySize = rVector.m_iArraySize;
			m_iTotalSize = rVector.m_iTotalSize;
			m_iIntSize = rVector.m_iIntSize;
			m_iIncreaseSize = rVector.m_iIncreaseSize;
			m_bFreed = rVector.m_bFreed;
			m_tDefault = rVector.m_tDefault;

			SafeDeleteArray ( m_tValues );
			m_tValues = new T[m_iTotalSize];
			
			for(UINT32 i = m_iArraySize; i < m_iTotalSize; ++i)
			{
				m_tValues[i] = m_tDefault;
			}
			
			for(UINT32 i = 0; i < m_iArraySize; ++i)
			{
				m_tValues[i] = rVector.m_tValues[i];
			}
		}

		UINT32 SerializedSize()
		{
			UINT32 uSize = 0;
			uSize += sizeof(UINT32);//Array size
			uSize += sizeof(UINT32);//Total size
			uSize += sizeof(UINT32);//Inc size
			ASCString strType = typeid(m_tValues[0]).name();
			if( strStringComp == strType)
			{
				for(UINT32 i = 0; i < m_iArraySize; ++i)
				{
					uSize += sizeof(UINT32);
					uSize += sizeof(CHAR8) * reinterpret_cast<ASCString*>(&m_tValues[i])->length()+1;
				}
			}
			else
			{
				uSize += sizeof(T) * m_iArraySize;//Values
			}
			return uSize;
		}

		void LoadBinary(void*& pData)
		{
			UINT32* pUintData = reinterpret_cast<UINT32*>(pData);
	
			//Assignment must be same order as decleration
			m_iArraySize = pUintData[0];
			//Move to next value
			++pUintData;

			m_iTotalSize = pUintData[0];
			//Move to next value
			++pUintData;

			m_iIncreaseSize = pUintData[0];
			//Move to next value
			++pUintData;

			m_bFreed = false;
			
			pData = pUintData;
	
			//The remaining data is the vector itself
			ASCString strType = typeid(m_tValues[0]).name();
			if( strStringComp == strType)
			{
				if(m_tValues)
				{
					SafeDelete( m_tValues );
				}
				m_tValues = new T[m_iArraySize];

				for(UINT32 i = 0; i < m_iArraySize; ++i)
				{
					reinterpret_cast<ASCString*>(m_tValues)[i].LoadBinary( pData );
				}
			}
			else
			{
				T* pTemplateData = reinterpret_cast<T*>(pData);
				if(m_tValues)
				{
					SafeDelete( m_tValues );
				}
				m_tValues = new T[m_iArraySize];
				
				for(UINT32 i = 0; i < m_iArraySize; ++i)
				{
					m_tValues[i] = *pTemplateData;
					++pTemplateData;
				}
				pData = pTemplateData;
			}
		}

		void Serialize(ASCFileIO* pFile)
		{
			pFile->OutputBianry(&m_iArraySize, sizeof(UINT32));
			pFile->OutputBianry(&m_iTotalSize, sizeof(UINT32));
			pFile->OutputBianry(&m_iIncreaseSize, sizeof(UINT32));
			
			ASCString strType = typeid(m_tValues[0]).name();
			if( strStringComp == strType)
			{
				for(UINT32 i = 0; i < m_iArraySize; ++i)
				{
					reinterpret_cast<ASCString*>(m_tValues)[i].Serialize( pFile );
				}
			}
			else
			{
				pFile->OutputBianry(m_tValues, sizeof(T) * m_iArraySize);
			}
		}

	//protected:
		UINT32 m_iArraySize;
		UINT32 m_iTotalSize;
		UINT32 m_iIntSize;
		UINT32 m_iIncreaseSize;
		bool m_bFreed;

		T m_tDefault;
		T* m_tValues;

	private:
		T Temp;
		void IncreaseVectorSize()
		{
			if(m_iIncreaseSize <= 0)
			{
				m_iIncreaseSize = m_kiIncereaseSize;
			}
			T* tTemp = new T[m_iTotalSize+m_iIncreaseSize];
			for(UINT32 i = 0; i < m_iArraySize; ++i)
			{
				tTemp[i] = m_tValues[i];
			}
			if(m_tValues)// && m_iTotalSize != 0)
			{
				delete[] m_tValues;
				m_tValues = NULL;
			}
			m_iTotalSize += m_iIncreaseSize;
			m_tValues = tTemp;
			for(UINT32 i = m_iArraySize; i < m_iTotalSize; ++i)
			{
				m_tValues[i] = m_tDefault;
			}
		}

		//friend class ASCBinaryVector;
	};

}

#endif //_ASCVECTOR_H__Included_1040089712

