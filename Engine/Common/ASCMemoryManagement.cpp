#include "ASCGenInc.h"
#include "ASCFileIO.h"
#include "ASCDebugString.h"
#include "ASCMemoryManagement.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif

#include "ASCRenderer.h"
#include "ASCTextureManager.h"

#ifdef ASC_IOS
#include <malloc/malloc.h>
#include <stdio.h>

#else
#include <crtdbg.h>
#include <malloc.h>
#include <stdio.h>
#endif

#ifdef ASC_IOS
void* operator new(unsigned long s)
{
	void* pMem = malloc(s);
	ASCMemoryManagement::Increment(pMem, SC_UINT(s));
	return pMem;
}

void* operator new[](unsigned long s)
{
	void* pMem = malloc(s);
	ASCMemoryManagement::Increment(pMem, SC_UINT(s));
	return pMem;
}
#else
void* operator new(size_t s)
{
	void* pMem = malloc(s);
	ASCMemoryManagement::Increment(pMem, SC_UINT(s));
	return pMem;
}

void* operator new[](size_t s)
{
	void* pMem = malloc(s);
	ASCMemoryManagement::Increment(pMem, SC_UINT(s));
	return pMem;
}
#endif

void operator delete(void* p)
{
	if(0 != p)
	{
		ASCMemoryManagement::Decrement(p);
		free(p);
		p = NULL;
	}
}

void operator delete[](void* p)
{
	if(0 != p)
	{
		ASCMemoryManagement::Decrement(p);
		free(p);
		p = NULL;
	}
}
UINT32 ASCMemoryManagement::sm_uMemoryAlloced = 0;
UINT32 ASCMemoryManagement::sm_uMostMemoryAlloced = 0;
UINT32 ASCMemoryManagement::sm_uTotalMemoryAlloced = 0;
UINT32 ASCMemoryManagement::sm_uNewCalls = 0;
UINT32 ASCMemoryManagement::sm_uDeleteCalls = 0;
UINT32 ASCMemoryManagement::sm_uMaxMemStored = 0;
UINT32 ASCMemoryManagement::sm_uAdditionalStorageNeeded = 0;
UINT32 ASCMemoryManagement::sm_uMemoryAllocatedForInfo = 0;
UINT32 ASCMemoryManagement::sm_uMaxTraceDepth = 0;
UINT32 ASCMemoryManagement::sm_uNewCallsThisFrame = 0;
UINT32 ASCMemoryManagement::sm_uDeleteCallsThisFrame = 0;
UINT32 ASCMemoryManagement::sm_uStoredNewCalls = 0;

UINT32* ASCMemoryManagement::sm_pMemorySize = NULL;
ASCDebugString* ASCMemoryManagement::sm_pMemStackTrace = NULL;
void** ASCMemoryManagement::sm_pMemoryAddress = NULL;
bool ASCMemoryManagement::sm_bInitialised = false;
bool ASCMemoryManagement::sm_bEnded = true;
bool ASCMemoryManagement::sm_bConfirmWithWindows = false;

ASCListNode<UINT32>* ASCMemoryManagement::sm_lstMemoryIndices = NULL;
ASCListNode<UINT32>* ASCMemoryManagement::sm_pUnusedMemIndex = NULL;
ASCListNode<UINT32>* ASCMemoryManagement::sm_pUsedMemIndex = NULL;
ASCListNode<UINT32>* ASCMemoryManagement::sm_pUsedMemIndexLast = NULL;

void 
ASCMemoryManagement::Start(UINT32 uMaxMemStored, bool bConfirmWithWindows, UINT32 uMaxTraceDepth)
{
#ifdef ASC_DO_MEM_LEAK_DETECTION
	if(false == sm_bInitialised)
	{
		sm_bEnded = false;
		sm_bInitialised = true;
		sm_bConfirmWithWindows = bConfirmWithWindows;
		sm_uMemoryAlloced = 0;
		sm_uTotalMemoryAlloced = 0;
		sm_uMostMemoryAlloced = 0;
		sm_uNewCalls = 0;
		sm_uDeleteCalls = 0;
		sm_uMaxMemStored = uMaxMemStored;
		sm_uAdditionalStorageNeeded = 0;
		sm_uMemoryAllocatedForInfo = 0;
		sm_uMaxTraceDepth = uMaxTraceDepth;

		sm_pMemorySize = reinterpret_cast<UINT32*>(malloc(sizeof(UINT32) * sm_uMaxMemStored));
		sm_lstMemoryIndices = reinterpret_cast<ASCListNode<UINT32>*>(malloc(sizeof(ASCListNode<UINT32>) * sm_uMaxMemStored));
		sm_pMemoryAddress = reinterpret_cast<void**>(malloc(sizeof(void*) * sm_uMaxMemStored));

#ifdef ASC_DO_STACK_TRACE
		sm_pMemStackTrace = reinterpret_cast<ASCDebugString*>(malloc(sizeof(ASCDebugString) * sm_uMaxMemStored));

		sm_uMemoryAllocatedForInfo = (sizeof(ASCListNode<UINT32>) + sizeof(UINT32) + sizeof(ASCDebugString) + sizeof(void*)) * sm_uMaxMemStored;
#else
		sm_uMemoryAllocatedForInfo = (sizeof(ASCListNode<UINT32>) + sizeof(UINT32) + sizeof(void*)) * sm_uMaxMemStored;
#endif

		sm_lstMemoryIndices[0].Set(0, NULL, &sm_lstMemoryIndices[1]);
		for(UINT32 i = 1; i < sm_uMaxMemStored-1; ++i)
		{
			sm_lstMemoryIndices[i].Set(i, &sm_lstMemoryIndices[i-1], &sm_lstMemoryIndices[i+1]);
		}
		sm_lstMemoryIndices[sm_uMaxMemStored-1].Set(sm_uMaxMemStored-1, &sm_lstMemoryIndices[sm_uMaxMemStored-2], NULL);

		sm_pUnusedMemIndex = &sm_lstMemoryIndices[0];

		for(UINT32 i = 0; i < sm_uMaxMemStored; ++i)
		{
			sm_pMemoryAddress[i] = NULL;
			sm_pMemorySize[i] = 0;
		}
	}
#endif
}
void 
ASCMemoryManagement::Increment(void* pAddress, UINT32 uSize)
{
	++sm_uNewCalls;
	++sm_uNewCallsThisFrame;
	if(false == sm_bEnded)
	{
#ifdef ASC_DO_MEM_LEAK_DETECTION
		//Store memory information
		if(NULL != sm_pUnusedMemIndex)
		{
			UINT32 uIndex = sm_pUnusedMemIndex->Value();
			sm_pMemorySize[uIndex] = uSize;
			sm_pMemoryAddress[uIndex] = pAddress;
#ifdef ASC_DO_STACK_TRACE
			ASCGeneralFunctions::StackTrace(sm_uMaxTraceDepth, sm_pMemStackTrace[uIndex], 3);
#endif
			++sm_uStoredNewCalls;
			
			UseMemIndex(sm_pUnusedMemIndex);
		}
		else if (sm_bInitialised)
		{
			++sm_uAdditionalStorageNeeded;
			ASCDebuggingManager::OutputWindowLine("ASC WARNING - No space left for memory storing\n");
		}


#endif
		//Update Total Memory Used
		sm_uMemoryAlloced += uSize;
		if(sm_uMemoryAlloced > sm_uMostMemoryAlloced)
		{
			sm_uMostMemoryAlloced = sm_uMemoryAlloced;
		}
		sm_uTotalMemoryAlloced += uSize;
	}
}
void 
ASCMemoryManagement::Decrement(void* pAddress)
{
	++sm_uDeleteCallsThisFrame;
	++sm_uDeleteCalls;
#ifdef ASC_DO_MEM_LEAK_DETECTION
	if(sm_bInitialised)
	{
		for(UINT32 i = 0; i < sm_uMaxMemStored; ++i)
		{
 			if(pAddress == sm_pMemoryAddress[i])
			{
				sm_uMemoryAlloced -= sm_pMemorySize[i];
				sm_pMemorySize[i] = 0;
				sm_pMemoryAddress[i] = NULL;
				--sm_uStoredNewCalls;
				UnuseMemIndex(&sm_lstMemoryIndices[i]);
				return;
			}
		}
	ASCDebuggingManager::OutputWindowLine("ASC WARNING - Unknown memory adress being delete\n");
	}
#endif
}
void 
ASCMemoryManagement::End()
{
#ifdef ASC_DO_MEM_LEAK_DETECTION
	sm_bInitialised = false;
	sm_bEnded = true;
	
	ASCDebuggingManager::OutputWindowLine("\nASC_MEMORY_LEAK_DETECTION");
	
	ASCDebuggingManager::OutputWindowLine("\nMemory allocated for info: ", sm_uMemoryAllocatedForInfo, " bytes\n");
	ASCDebuggingManager::OutputWindowLine("Memory allocated: ", sm_uTotalMemoryAlloced, " bytes\n");
	ASCDebuggingManager::OutputWindowLine("Most memory allocated: ", sm_uMostMemoryAlloced, " bytes\n");
	ASCDebuggingManager::OutputWindowLine("Memory allocated remaining: ", sm_uMemoryAlloced, " bytes\n");
	ASCDebuggingManager::OutputWindowLine("New calls: ", sm_uNewCalls, "\n");
	ASCDebuggingManager::OutputWindowLine("Delete calls: ", sm_uDeleteCalls, "\n");

	if(sm_uMemoryAlloced > 0)
	{
		UINT32 uMemoryLeaks = 0;
		ASCDebugString strLine;
		ASCDebugString strTemp;
		ASCDebuggingManager::OutputWindowLine("\nASC WARNING - Memory leaks detected\n");
		for(UINT32 i = 0; i < sm_uMaxMemStored; ++i)
		{
			if(sm_pMemoryAddress[i])
			{
				ASCDebuggingManager::OutputWindowLine("ASC WARNING - MEMORY_LEAK_", uMemoryLeaks, "\n");
				ASCDebuggingManager::OutputWindowLine("	Memory Size: ", sm_pMemorySize[i], " bytes\n");
#ifdef ASC_DO_STACK_TRACE
				strTemp = sm_pMemStackTrace[i];
				strTemp.substrc(':', 2);
				ASCDebuggingManager::OutputWindowLine("	Stack Trace: ", strTemp, "\n");
#endif
				++uMemoryLeaks;
			}
		}
		ASCDebuggingManager::OutputWindowLine("\nASC WARNING - ", uMemoryLeaks, " Memory Leaks Detected\n");
	}
	
	if(sm_uAdditionalStorageNeeded > 0)
	{
		ASCDebuggingManager::OutputWindowLine("Additional storage needed: ", sm_uAdditionalStorageNeeded, "\n");
	}
	if(sm_uMemoryAlloced > 0)
	{
		ASCDebuggingManager::OutputWindowLine("\nASC_MEMORY_LEAK_DETECTION - LEAKS DETECTED\n\n");
	}
	else
	{
		ASCDebuggingManager::OutputWindowLine("\nASC_MEMORY_LEAK_DETECTION - NO LEAKS DETECTED\n\n");
	}
	
	free(sm_pMemorySize);
	free(sm_pMemoryAddress);
	free(sm_lstMemoryIndices);
#ifdef ASC_DO_STACK_TRACE
	free(sm_pMemStackTrace);
#endif

	if(sm_bConfirmWithWindows)
	{
		_CrtDumpMemoryLeaks();
	}
#endif
}
void 
ASCMemoryManagement::UseMemIndex(ASCListNode<UINT32>* pIndex)
{
	if(NULL == pIndex->Next())
	{
		ASCDebuggingManager::OutputWindowLine("ASC WARNING - No space left for memory storing\n");
	}
	else
	{
		sm_pUnusedMemIndex = pIndex->Next();
		sm_pUnusedMemIndex->Previous(NULL);

		if(NULL == sm_pUsedMemIndex)
		{
			pIndex->SetNeighbours(NULL, NULL);
			sm_pUsedMemIndex = pIndex;
			sm_pUsedMemIndexLast = pIndex;
		}
		else
		{
			pIndex->SetNeighbours(NULL, NULL);
			sm_pUsedMemIndexLast->Next(pIndex);
			pIndex->SetNeighbours(sm_pUsedMemIndexLast, NULL);
			sm_pUsedMemIndexLast = pIndex;
		}
	}
}
void 
ASCMemoryManagement::UnuseMemIndex(ASCListNode<UINT32>* pIndex)
{
	if(pIndex == sm_pUsedMemIndex && pIndex == sm_pUsedMemIndexLast)
	{
		sm_pUsedMemIndex = NULL;
	}
	else if(pIndex == sm_pUsedMemIndex)
	{
		sm_pUsedMemIndex = pIndex->Next();
		sm_pUsedMemIndex->Previous(NULL);
	}
	else if(pIndex == sm_pUsedMemIndexLast)
	{
		sm_pUsedMemIndexLast = pIndex->Previous();
		sm_pUsedMemIndexLast->Next(NULL);
	}
	else
	{
		ASCListNode<UINT32>* pPrevious = pIndex->Previous();
		ASCListNode<UINT32>* pNext = pIndex->Next();
		pPrevious->Next(pNext);
		pNext->Previous(pPrevious);
	}
	pIndex->SetNeighbours(NULL, sm_pUnusedMemIndex);
	sm_pUnusedMemIndex = pIndex;
}

void* 
ASCMemoryManagement::GetMemory(UINT32 uSize, bool bTrackMemory)
{
	void* pMem = malloc(uSize);
	if(bTrackMemory)
	{
		ASCMemoryManagement::Increment(pMem, uSize);
	}

	return pMem;
}

void 
ASCMemoryManagement::ReleaseMemory(void* pMem, bool bTrackMemory)
{
	if(0 != pMem)
	{
		if(bTrackMemory)
		{
			ASCMemoryManagement::Decrement(pMem);
		}
		free(pMem);
		pMem = NULL;
	}
}
void 
ASCMemoryManagement::DisplayMemoryInfo()
{
	UINT32 uTextureMem = Ascension::Renderer().GetTextureManager()->MemoryUsed();
	UINT32 uVertexMem = Ascension::Renderer().VertexMemUsed();
	Ascension::Debugging().AddScreenLine("### Memory Info ###");
	UINT32 uTotalMem = sm_uMemoryAlloced + uTextureMem + uVertexMem;
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Total Memory Allocated: ", uTotalMem / 1024, " kilobytes");
	Ascension::Debugging().AddScreenLine("Texture Memory Allocated: ", uTextureMem / 1024, " kilobytes");
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Total Memory Allocated: ", (uTotalMem / 1024) / 1024, " megabytes");
	Ascension::Debugging().AddScreenLine("Texture Memory Allocated: ", (uTextureMem / 1024) / 1024, " megabytes");
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Memory Allocated For Info: ", sm_uMemoryAllocatedForInfo / 1024, " kilobytes");
	
#ifdef ASC_DO_MEM_LEAK_DETECTION
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Storage Used: ", sm_uStoredNewCalls);
	Ascension::Debugging().AddScreenLine("Storage Avaliable: ", sm_uMaxMemStored - sm_uStoredNewCalls);
	Ascension::Debugging().AddScreenLine("Additional Storage Needed: ", sm_uAdditionalStorageNeeded);
#endif
	
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Total New Calls: ", sm_uNewCalls);
	Ascension::Debugging().AddScreenLine("Total Delete Calls: ", sm_uDeleteCalls);
	
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("New Calls This Frame: ", sm_uNewCallsThisFrame);
	Ascension::Debugging().AddScreenLine("Delete Calls This Frame: ", sm_uDeleteCallsThisFrame);
	
	sm_uNewCallsThisFrame = 0;
	sm_uDeleteCallsThisFrame = 0;
}