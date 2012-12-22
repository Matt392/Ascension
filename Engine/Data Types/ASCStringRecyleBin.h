/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Data Types\ASCStringRecyleBin.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSTRINGRECYLEBIN_H__Included_535381587
#define _ASCSTRINGRECYLEBIN_H__Included_535381587

#define FASTER_STRING_RECYCLING

#define ASC_STRING_START_SIZE 2
#define ASC_RECYCLER_RESIZE 256

#define MAX_RECYCLERS 12


namespace ASC
{
	class ASCStingRecycler
	{
	public:
		ASCStingRecycler();
		~ASCStingRecycler();

		void Initialise(UINT32 uSizeOfStrings);
		
		CHAR8* GetString();
		void RecyleString(CHAR8* pString);

		UINT32 SizeOfStrings();

		void DebugInfo();

		bool InUse();

		void CleanUp();
		
		UINT32 TotalActive();
		UINT32 TotalInactive();

	protected:
		UINT32 m_uSizeOfStrings;
		UINT32 m_uAddedThisFrame;
		UINT32 m_uRemovedThisFrame;

		ASCList<CHAR8*> m_lstStrings;
		UINT32 m_uStringsInUse;
	};


	class ASCStringRecyleBin
	{
	public:
		static UINT32 GetString(UINT32 uWantedLength, CHAR8*& pString, bool bExactLength = false);
		static void RecyleString(CHAR8* pString, UINT32 uLength);
		static void Initialise();

		static void DispalyInfo();

		static void CleanUp();

		static void Disable();

	protected:
		static ASCStingRecycler sm_vecStringRecyclers[MAX_RECYCLERS];
		static bool sm_bInitialised;
		static bool sm_bDeinitialised;
	};


}

#endif //_ASCSTRINGRECYLEBIN_H__Included_535381587

