/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCVideoClip.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCVIDEOCLIP_H__Included_1748296086
#define _ASCVIDEOCLIP_H__Included_1748296086


namespace ASC
{
	class ASCTexture;
	class ASCAudioFile;
	class ASCSprite;

	class ASCVideo
	{
	public:
		ASCVideo(void);
		~ASCVideo(void);

		bool Initialise(ASCString strFile, ASCString strName);

		void Play();
		void Pause();
		void Stop();

		void NextFrame();

		void Rewind();
		void FastFoward();

		void Update();

		bool Playing();

		ASCTexture* CurrentFrame();
		
		UINT32 Width();
		UINT32 Height();
		
		void LinkSprite(ASCSprite* pSprite);
		void UnLinkSprite(ASCSprite* pSprite);

	protected:
		UINT32 m_uFrameCount;
		UINT32 m_uCurrentFrame;
		
		UINT32 m_uPreBuffer;
		UINT32 m_uPostBuffer;
		
		UINT32 m_uWidth;
		UINT32 m_uHeight;

		FLOAT32 m_fFrameRate;
		FLOAT32 m_fTimeElapsed;

		bool m_bPlaying;
		bool m_bHasSound;
		
		ASCAudioFile* m_pSound;
		ASCTexture** m_pFrames;

		ASCVector<ASCSprite*> m_LinkedSprites;
	};

}

#endif //_ASCVIDEOCLIP_H__Included_1748296086

