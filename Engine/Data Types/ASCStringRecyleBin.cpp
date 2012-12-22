#include "ASCGenInc.h"
#include "ASCStringRecyleBin.h"

#include "ASCFinalShutdownDelay.h"
#include "../Ascension/Ascension.h"


ASCStingRecycler::ASCStingRecycler()
: m_uSizeOfStrings ( 0 )
, m_uAddedThisFrame ( 0 )
, m_uRemovedThisFrame ( 0 )
, m_uStringsInUse ( 0 )
{
	ASCFinalShutdownDelay::AddDelay();
}
ASCStingRecycler::~ASCStingRecycler()
{
	ASCStringRecyleBin::Disable();
	CHAR8* pVal = NULL;
	while(m_lstStrings.size() > 0)
	{
		pVal = m_lstStrings.FirstValue();
		SafeDelete( pVal );
		m_lstStrings.RemoveFirst();
	}
	ASCFinalShutdownDelay::RemoveDelay();
}

void 
ASCStingRecycler::Initialise(UINT32 uSizeOfStrings)
{
	m_uSizeOfStrings = uSizeOfStrings;
	m_lstStrings.SetResizeSize(ASC_RECYCLER_RESIZE);
}
		
CHAR8* 
ASCStingRecycler::GetString()
{
	++m_uAddedThisFrame;
	++m_uStringsInUse;
	if(m_lstStrings.size() > 0)
	{
		CHAR8* pStr = m_lstStrings.FirstValue();
		m_lstStrings.RemoveFirst();
		return pStr;
	}
	CHAR8* pStr = new CHAR8[m_uSizeOfStrings];
	return pStr;
}
void 
ASCStingRecycler::RecyleString(CHAR8* pString)
{
	++m_uRemovedThisFrame;
	--m_uStringsInUse;
	pString[0] = NULL;
	m_lstStrings.AddToListEnd(pString);
}

UINT32 
ASCStingRecycler::SizeOfStrings()
{
	return m_uSizeOfStrings;
}
void 
ASCStingRecycler::DebugInfo()
{
	ASCString strInfo = "Size: ";
	strInfo.AddSmallInt(m_uSizeOfStrings);
	strInfo += ASCString(", Storage Size: ");
	strInfo.AddSmallInt(m_lstStrings.size_avaliable() );
	strInfo += ASCString(", Total: ");
	strInfo.AddSmallInt(m_lstStrings.size() + m_uStringsInUse);
	strInfo += ASCString(", Added: ");
	strInfo.AddSmallInt(m_uAddedThisFrame);
	strInfo += ASCString(", Removed: ");
	strInfo.AddSmallInt(m_uRemovedThisFrame);
	strInfo += ASCString(", In Use: ");
	strInfo.AddSmallInt(m_uStringsInUse);
	strInfo += ASCString(", Inactive: ");
	strInfo.AddSmallInt(m_lstStrings.size());
	Ascension::Debugging().AddScreenLine(strInfo);
	m_uAddedThisFrame = 0;
	m_uRemovedThisFrame = 0;
}
bool 
ASCStingRecycler::InUse()
{
	return m_lstStrings.size() > 0 || m_uStringsInUse > 0;
}
UINT32 
ASCStingRecycler::TotalActive()
{
	return m_uStringsInUse;
}
UINT32 
ASCStingRecycler::TotalInactive()
{
	return m_lstStrings.size();
}

void 
ASCStingRecycler::CleanUp()
{
	CHAR8* pVal = NULL;
	while(m_lstStrings.size() > 0)
	{
		pVal = m_lstStrings.FirstValue();
		SafeDelete( pVal );
		m_lstStrings.RemoveFirst();
	}
}


ASCStingRecycler ASCStringRecyleBin::sm_vecStringRecyclers[MAX_RECYCLERS] = {};
bool ASCStringRecyleBin::sm_bInitialised = false;
bool ASCStringRecyleBin::sm_bDeinitialised = false;

void 
ASCStringRecyleBin::Initialise()
{
	sm_bInitialised = true;
	UINT32 uStringSize = ASC_STRING_START_SIZE;
	for(UINT32 i = 0; i < MAX_RECYCLERS; ++i)
	{
		sm_vecStringRecyclers[i].Initialise(uStringSize);

		uStringSize *= 2;
	}
}

UINT32 
ASCStringRecyleBin::GetString(UINT32 uWantedLength, CHAR8*& pString, bool bExactLength)
{
	if(sm_bDeinitialised)
	{
		assert_now("Guts, Recylers have been deleted, problems will follow");
	}
	if(bExactLength)
	{
		assert_now("Guts, Using exact length is deprecated, defaulting to non exact");
	}
	if(false == sm_bInitialised || sm_vecStringRecyclers[0].SizeOfStrings() == 0)
	{
		Initialise();
	}
	
	UINT32 uSizeFound = 0;
	UINT32 uIndex = 0;
	for(UINT32 i = 0; i < MAX_RECYCLERS; ++i)
	{
		if(sm_vecStringRecyclers[i].SizeOfStrings() >= uWantedLength)
		{
			if(uSizeFound == 0 || sm_vecStringRecyclers[i].SizeOfStrings() < uSizeFound)
			{
				uIndex = i;
				uSizeFound = sm_vecStringRecyclers[i].SizeOfStrings();
				break;
			}
		}
	}
	if(uSizeFound != 0)
	{
		pString = sm_vecStringRecyclers[uIndex].GetString();
		return uSizeFound;
	}
	else
	{
		assert_now("Guts, No valid string recycler could be found");
		return 0;
	}
}
void 
ASCStringRecyleBin::RecyleString(CHAR8* pString, UINT32 uLength)
{
	if(sm_bDeinitialised)
	{
		SafeDelete( pString );
		return;
	}
	if(false == sm_bInitialised || sm_vecStringRecyclers[0].SizeOfStrings() == 0)
	{
		Initialise();
	}
	if(pString)
	{
		for(UINT32 i = 0; i < MAX_RECYCLERS; ++i)
		{
			if(sm_vecStringRecyclers[i].SizeOfStrings() == uLength)
			{
				sm_vecStringRecyclers[i].RecyleString(pString);
				return;
			}
		}
		assert_now("Guts, Where the hell did this string come from");
	}

}
void 
ASCStringRecyleBin::DispalyInfo()
{
	Ascension::Debugging().AddScreenLine("");
	
	Ascension::Debugging().AddScreenLine("Max Recyclers: ", MAX_RECYCLERS);
	UINT32 uTotalActive = 0;
	UINT32 uTotalInactive = 0;
	for(UINT32 i = 0; i < MAX_RECYCLERS; ++i)
	{
		if(sm_vecStringRecyclers[i].InUse())
		{
			Ascension::Debugging().AddScreenLine("Recycler #", i+1);
			sm_vecStringRecyclers[i].DebugInfo();
			uTotalActive += sm_vecStringRecyclers[i].TotalActive();
			uTotalInactive += sm_vecStringRecyclers[i].TotalInactive();
		}
	}
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Total Active: ", uTotalActive);
	Ascension::Debugging().AddScreenLine("Total Inactive: ", uTotalInactive);
}
void 
ASCStringRecyleBin::CleanUp()
{
	for(UINT32 i = 0; i < MAX_RECYCLERS; ++i)
	{
		sm_vecStringRecyclers[i].CleanUp();
	}
}
void 
ASCStringRecyleBin::Disable()
{
	sm_bDeinitialised = true;
}