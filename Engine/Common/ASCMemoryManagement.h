/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCMemoryManagement.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCMEMORYMANAGEMENT_H__Included_596255169
#define _ASCMEMORYMANAGEMENT_H__Included_596255169

#define ASC_LIST_NEXT_MEM
#ifdef ASC_LIST_NEXT_MEM
#include "ASCList.h"
#endif

namespace ASC
{
	class ASCMemoryManagement
	{
	public:
		static void Start(UINT32 uMaxMemStored, bool bConfirmWithWindows, UINT32 uMaxTraceDepth = 10);
		static void Increment(void* pAddress, UINT32 uSize);
		static void Decrement(void* pAddress);
		static void End();
		
		static void* GetMemory(UINT32 uSize, bool bTrackMemory = true);
		static void ReleaseMemory(void* pMem, bool bTrackMemory = true);

		static void DisplayMemoryInfo();

	protected:
		static UINT32 sm_uMemoryAlloced;
		static UINT32 sm_uMostMemoryAlloced;
		static UINT32 sm_uTotalMemoryAlloced;
		static UINT32 sm_uNewCalls;
		static UINT32 sm_uDeleteCalls;
		static UINT32 sm_uMaxMemStored;
		static UINT32 sm_uAdditionalStorageNeeded;
		static UINT32 sm_uMemoryAllocatedForInfo;
		static UINT32 sm_uMaxTraceDepth;
		
		static UINT32 sm_uStoredNewCalls;
		static UINT32 sm_uNewCallsThisFrame;
		static UINT32 sm_uDeleteCallsThisFrame;

		static UINT32* sm_pMemorySize;

		static ASCDebugString* sm_pMemStackTrace;

		static void** sm_pMemoryAddress;
#ifdef ASC_LIST_NEXT_MEM
		static ASCListNode<UINT32>* sm_lstMemoryIndices;
		static ASCListNode<UINT32>* sm_pUnusedMemIndex;
		static ASCListNode<UINT32>* sm_pUsedMemIndex;
		static ASCListNode<UINT32>* sm_pUsedMemIndexLast;
		
		static void UseMemIndex(ASCListNode<UINT32>* pIndex);
		static void UnuseMemIndex(ASCListNode<UINT32>* pIndex);
#endif

		static bool sm_bInitialised;
		static bool sm_bConfirmWithWindows;
		static bool sm_bEnded;
	};

}
#ifdef ASC_IOS
/*
void* operator new(unsigned long s);
void* operator new[](unsigned long s);

void operator delete(void* p);
void operator delete[](void* p);
 */
#else
void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void* p);
void operator delete[](void* p);
#endif

#endif //_ASCMEMORYMANAGEMENT_H__Included_596255169

