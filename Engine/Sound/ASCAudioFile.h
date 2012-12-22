/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Sound\ASCAudioFile.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCAUDIOFILE_H__Included_1361884367
#define _ASCAUDIOFILE_H__Included_1361884367

#include "ASCSound.h"

namespace ASC
{
#ifdef ASC_IOS
    void LoadOggFile(ASCString strFile, UINT32 uiBuffer);
#endif
    
	class ASCAudioFile : public ASCSound
	{
	public:
		ASCAudioFile(void);
		~ASCAudioFile(void);

		bool Load(ASCString strFile);
		bool Play();
		bool Pause();
		void Rewind();

		FLOAT32 Duration();
		FLOAT32 CurrentTime();

		void Volume(FLOAT32 fVal);
		FLOAT32 Volume();

		bool Owned();
		
		bool Take();
		bool Return();

	protected:
		bool m_bOwned;
		FLOAT32 m_fDuration;
		FLOAT32 m_fVolume;
	};

}

#endif //_ASCAUDIOFILE_H__Included_1361884367

