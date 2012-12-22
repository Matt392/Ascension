 #include "ASCGenInc.h"
#include "ASCAnimTrack.h"
#include "ASCAnimEvent.h"
#include "../Ascension/Ascension.h"
#include "ASCINIFile.h"


ASCAnimTrack::ASCAnimTrack(void)
: m_strID ( "" )
, m_uCurrentFrame ( 0 )
, m_uTotalFrames ( 0 )
, m_uEventID ( 0 )
, m_fTrackTimeElapsed ( 0.0f )
, m_fFrameTimeElapsed ( 0.0f )
, m_fTimeScale ( 1.0f )
, m_fTotalLength ( 0.0f )
, m_bLooped ( false )
{
}


ASCAnimTrack::~ASCAnimTrack(void)
{
}
bool 
ASCAnimTrack::Init(ASCINIFile& rFile, ASCString strAnim, UINT32 uEventID, ASCTextureAtlas* pAtlas)
{
	m_uEventID = uEventID;

	m_strID = rFile.GetString( strAnim, "Name");

	m_uTotalFrames = rFile.GetUInt( strAnim, "FrameCount");
    
   // Ascension::Debugging().OutputWindowLine(m_strID.c_str());

	ASCString strFrame;
	ASCString strFrameUse;
	for(UINT32 i = 0; i < m_uTotalFrames; ++i)
	{
		strFrame = "Frame";
		strFrame += i;
		strFrameUse = strFrame + "Image";
		m_fFrameUVs.push_back( pAtlas->GetAtlasUVs(rFile.GetString( strAnim, strFrameUse)));
		
		strFrameUse = strFrame + "Time";
		m_fFrameTimes.push_back(rFile.GetFloat( strAnim, strFrameUse));
		
		strFrameUse = strFrame + "Event";
		m_strFrameEvent.push_back(rFile.GetString( strAnim, strFrameUse));
        
  
       // Ascension::Debugging().OutputWindowLine("Event: ", ASCDebugString(m_strFrameEvent[m_strFrameEvent.size()-1].c_str()));
	}

	m_uCurrentFrame = 0;
	m_fTrackTimeElapsed = 0.0f;
	m_fFrameTimeElapsed = 0.0f;
	if(m_uTotalFrames > 0)
	{
		return true;
	}
	return false;
}
bool 
ASCAnimTrack::Update( FLOAT32 fDelta, SAtlasInfo* &pNewUVs )
{
	bool bReturn = false;
	//Increment time
	m_fTrackTimeElapsed += fDelta*m_fTimeScale;
	m_fFrameTimeElapsed += fDelta*m_fTimeScale;

	//Increment frame if required
	while(m_fFrameTimeElapsed >= m_fFrameTimes[m_uCurrentFrame])
	{
		m_fFrameTimeElapsed -= m_fFrameTimes[m_uCurrentFrame];
		++m_uCurrentFrame;
		//check for end;
		if(m_uCurrentFrame >= m_uTotalFrames)
		{
			m_uCurrentFrame = 0;
			// returns true if not looped
			bReturn = !m_bLooped;
		}
		pNewUVs = &m_fFrameUVs[m_uCurrentFrame];

		//Send the event if it exists
		if(m_strFrameEvent[m_uCurrentFrame].length() > 0)
		{
			ASCAnimEvent Event;
			Event.SetID(m_uEventID);
			Event.SetEventTypeID(ET_ANIM);
			Event.SetAnimEventID(&m_strFrameEvent[m_uCurrentFrame]);
			Ascension::EventSystem().AddEvent(Event);
		}
	}

	// returns true if not looped
	return bReturn;
}
bool 
ASCAnimTrack::Play(bool bLooped, bool bReset, FLOAT32 fTimeScale, FLOAT32 fABSLength, SAtlasInfo* &pNewUVs)
{
	m_bLooped = bLooped;
	if(bReset)
	{
		m_fTrackTimeElapsed = 0.0f;
		m_fFrameTimeElapsed = 0.0f;
		m_uCurrentFrame = 0;
	}
	pNewUVs = &m_fFrameUVs[m_uCurrentFrame];
	m_fTimeScale = fTimeScale;
	if(fABSLength != 0.0f)
	{
		m_fTimeScale = m_fTotalLength / fABSLength;
	}
	return true;
}

ASCString 
ASCAnimTrack::GetID()
{
	return m_strID;
}