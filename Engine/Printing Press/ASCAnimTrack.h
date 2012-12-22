/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCAnimTrack.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCANIMTRACK_H__Included_1298413142
#define _ASCANIMTRACK_H__Included_1298413142

#include "ASCTextureAtlas.h"

namespace ASC
{
	class ASCINIFile;

	class ASCAnimTrack
	{
	public:
		ASCAnimTrack(void);
		~ASCAnimTrack(void);

		bool Init(ASCINIFile& rFile, ASCString strAnim, UINT32 uEventID, ASCTextureAtlas* pAtlas);

		bool Update( FLOAT32 fDelta, SAtlasInfo* &pNewUVs );

		bool Play(bool bLooped, bool bReset, FLOAT32 fTimeScale, FLOAT32 fABSLength, SAtlasInfo* &pNewUVs);

		ASCString GetID();

	protected:
		ASCVector<ASCString> m_strFrameEvent;
		ASCVector<SAtlasInfo> m_fFrameUVs;
		ASCVector<FLOAT32> m_fFrameTimes;

		ASCString m_strID;

		UINT32 m_uCurrentFrame;
		UINT32 m_uTotalFrames;
		UINT32 m_uEventID;


		FLOAT32 m_fTrackTimeElapsed;
		FLOAT32 m_fFrameTimeElapsed;
		FLOAT32 m_fTimeScale;
		FLOAT32 m_fTotalLength;

		bool m_bLooped;
	};

}

#endif //_ASCANIMTRACK_H__Included_1298413142

