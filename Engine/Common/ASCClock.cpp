#include "ASCGenInc.h"
#include "../../Engine/Ascension/Ascension.h"
#include "ASCClock.h"
#ifdef ASC_IOS
//#include "CADisplayLink.h"
#import "/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.sdk/System/Library/Frameworks/QuartzCore.framework/Headers/CADisplayLink.h"
#else
#include <Windows.h>
#endif

#ifdef ASC_IOS
ASCClock::ASCClock(bool bSetDelta)
: m_liTicksPerSecond(0)
, m_liStartTime( 0 )
, m_liLastTime( 0 )
, m_liThisTime( 0 )
, m_iCurrentDelta ( 0 )
, m_fAverageTotal ( 0.0f )
, m_bSetDelta ( bSetDelta )
, m_pDisplayLink ( NULL )
{
	m_liTicksPerSecond = 0;
    
    m_liStartTime = CFAbsoluteTimeGetCurrent();
    
	for(UINT32 i = 0; i < m_kiDeltaSampleSize; ++i)
	{
		m_fDeltaSamples[i] = 0.0f;
		m_ilutNextDelta[i] = i+1;
	}
	m_ilutNextDelta[m_kiDeltaSampleSize-1] = 0;
}
#else
ASCClock::ASCClock(bool bSetDelta)
: m_liTicksPerSecond(0)
, m_liStartTime( 0 )
, m_liLastTime( 0 )
, m_liThisTime( 0 )
, m_iCurrentDelta ( 0 )
, m_fAverageTotal ( 0.0f )
, m_bSetDelta ( bSetDelta )
{
	LARGE_INTEGER Time;
	QueryPerformanceFrequency(&Time);
	m_liTicksPerSecond = Time.QuadPart;

	QueryPerformanceCounter(&Time);
	m_liStartTime = Time.QuadPart;

	for(UINT32 i = 0; i < m_kiDeltaSampleSize; ++i)
	{
		m_fDeltaSamples[i] = 0.0f;
		m_ilutNextDelta[i] = i+1;
	}
	m_ilutNextDelta[m_kiDeltaSampleSize-1] = 0;
}
#endif

ASCClock::~ASCClock(void)
{
}

FLOAT32 
ASCClock::GetDeltaTick()
{
	return m_fDeltaSamples[m_iCurrentDelta];
}
#ifdef ASC_IOS
void 
ASCClock::Update()
{
    /*
    if(m_pDisplayLink)
    {
        CADisplayLink* pDisLink = reinterpret_cast<CADisplayLink*>(m_pDisplayLink);
        m_liLastTime = m_liThisTime;
        m_liThisTime = pDisLink.duration;//; / pDisLink.frameInterval; 
    }
     */
    m_liLastTime = m_liThisTime;
   // m_liThisTime = mach_absolute_time();
    m_liThisTime = CACurrentMediaTime();
    
	m_iCurrentDelta = m_ilutNextDelta[m_iCurrentDelta];
    
	m_fAverageTotal -= m_fDeltaSamples[m_iCurrentDelta];
    
	m_fDeltaSamples[m_iCurrentDelta] = static_cast<FLOAT32>((m_liThisTime - m_liLastTime));
	
	m_fAverageTotal += m_fDeltaSamples[m_iCurrentDelta];
    
	if(m_bSetDelta)
	{
		Ascension::SetDelta(m_fDeltaSamples[m_iCurrentDelta]);
	}
    
}
#else
void 
ASCClock::Update()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	
	m_liLastTime = m_liThisTime;
	m_liThisTime = Time.QuadPart;

	m_iCurrentDelta = m_ilutNextDelta[m_iCurrentDelta];

	m_fAverageTotal -= m_fDeltaSamples[m_iCurrentDelta];

	m_fDeltaSamples[m_iCurrentDelta] = static_cast<FLOAT32>((m_liThisTime - m_liLastTime)) / static_cast<FLOAT32>(m_liTicksPerSecond);
	
	m_fAverageTotal += m_fDeltaSamples[m_iCurrentDelta];

	if(m_bSetDelta)
	{
		Ascension::SetDelta(m_fDeltaSamples[m_iCurrentDelta]);
	}

}
#endif
FLOAT32 
ASCClock::GetAverageTime() const
{
	return 0.0f;
}
FLOAT32 
ASCClock::GetAverageFPS() const
{
	FLOAT32 fAverageDelta = m_fAverageTotal / m_kiDeltaSampleSize;
	assert_msg(fAverageDelta != 0.0f, "Average delta is ZERO!!!");
	if(fAverageDelta == 0.0f)
	{
		return 0.0f;
	}
	return(1.0f / m_fDeltaSamples[m_iCurrentDelta]);
}
FLOAT32 
ASCClock::GetFPS() const
{
	assert_msg(m_fDeltaSamples[m_iCurrentDelta] != 0.0f, "Delta is ZERO!!!");
	if(m_fDeltaSamples[m_iCurrentDelta] == 0.0f)
	{
		return 0.0f;
	}
	return(1.0f / m_fDeltaSamples[m_iCurrentDelta]);
}
#ifdef ASC_IOS
void 
ASCClock::LinkDisplayLink(void* pLink)
{
    m_pDisplayLink = pLink;
    
    CADisplayLink* pDisLink = reinterpret_cast<CADisplayLink*>(m_pDisplayLink);
    
    
    m_iCurrentDelta = 0;
	m_liTicksPerSecond = 0;
    
    m_liStartTime = pDisLink.duration;
    
	for(UINT32 i = 0; i < m_kiDeltaSampleSize; ++i)
	{
		m_fDeltaSamples[i] = 0.0f;
		m_ilutNextDelta[i] = i+1;
	}
	m_ilutNextDelta[m_kiDeltaSampleSize-1] = 0;
}   
#endif
void 
ASCClock::DisplayInfo()
{
	FLOAT32 fFPS = GetFPS();
	static FLOAT32 fHeighestFPS = fFPS;
	static FLOAT32 fLowestFPS = fFPS;
	static FLOAT32 fTimeElapsed = 0.0f;
	static FLOAT32 fAverageFPS = GetAverageFPS();
	if(fFPS > fHeighestFPS)
	{
		fHeighestFPS = fFPS;
	}
	if(fFPS < fLowestFPS)
	{
		fLowestFPS = fFPS;
	}
	Ascension::Debugging().AddScreenLine("### Clock Info ###");
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Average FPS: ", fAverageFPS);
	Ascension::Debugging().AddScreenLine("Heighest FPS: ", fHeighestFPS);
	Ascension::Debugging().AddScreenLine("Lowest FPS: ", fLowestFPS);
	Ascension::Debugging().AddScreenLine("Actual FPS: ", fFPS);

	fTimeElapsed += m_fDeltaSamples[m_iCurrentDelta];
	if(fTimeElapsed >= 1.0f)
	{
		fAverageFPS = GetAverageFPS();
		fTimeElapsed -= 2.0f;
		fHeighestFPS = fFPS;
		fLowestFPS = fFPS;
	}
}