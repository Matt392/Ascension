/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCClock.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCCLOCK_H__Included_328912003
#define _ASCCLOCK_H__Included_328912003


namespace ASC
{
	const UINT32 m_kiDeltaSampleSize = 50;

	class ASCClock
	{
	public:
		ASCClock(bool bSetDelta = false);
		~ASCClock(void);

		FLOAT32 GetDeltaTick();
		FLOAT32 GetAverageFPS() const;
		FLOAT32 GetFPS() const;

		void Update();

		void DisplayInfo();
        
#ifdef ASC_IOS
        void LinkDisplayLink(void* pLink);
#endif

	private:
		void Initialise();

		FLOAT32 GetAverageTime() const;

	private:
		UINT64 m_liTicksPerSecond;
#ifdef ASC_IOS
		FLOAT64 m_liStartTime;
		FLOAT64 m_liLastTime;
		FLOAT64 m_liThisTime;
#else
		UINT64 m_liStartTime;
		UINT64 m_liLastTime;
		UINT64 m_liThisTime;
#endif

		FLOAT32 m_fDeltaSamples[m_kiDeltaSampleSize];
		UINT32 m_ilutNextDelta[m_kiDeltaSampleSize];

		FLOAT32 m_fAverageTotal;
		UINT32 m_iCurrentDelta;

		bool m_bSetDelta;
        
#ifdef ASC_IOS
        void* m_pDisplayLink;
#endif
	};

}

#endif //_ASCCLOCK_H__Included_328912003

