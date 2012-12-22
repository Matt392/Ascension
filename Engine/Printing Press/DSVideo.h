/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\DSVideo.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DSVIDEO_H__Included_734833474
#define _DSVIDEO_H__Included_734833474

#include "ASCVideo.h"

#include <dshow.h>
#include <streams.h>
#include "renbase.h"


struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

namespace ASC
{
	class ASCTexture;

	class CDSVideo : public ASCVideo, CBaseVideoRenderer
	{
	public:
		CDSVideo(LPUNKNOWN pUnk,HRESULT *phr);
		~CDSVideo(void);
		
		virtual bool Initialise(ASCString strFile);
		virtual ASCTexture* GetTexture();
		virtual void Update();
		virtual void PlayVideo();
		virtual void PauseVideo();
		virtual void StopVideo();
		virtual bool IsPlaying();

		
		HRESULT SetMediaType(const CMediaType *pmt);
		virtual HRESULT DoRenderSample(IMediaSample *pMediaSample);
		virtual HRESULT CheckMediaType(const CMediaType *);
		
		void AttachRendererToSourceFilter(  );

	protected:
		IBaseFilter* m_pSourceFilter;
		IGraphBuilder* m_pGraphBuilder;
		IMediaControl*   m_pMediaCntr;
		IMediaPosition*  m_pMediaPosition;

		ASCTexture* m_pTexture;
		WCHAR m_pwcName[256];

		bool m_bNewFrame;
		
		BYTE* m_pVideoBuffer;
		VIDEOINFO* m_pVideoInfo;
		UINT32 m_lVideoWidth;
		UINT32 m_lVideoHeight;
		UINT32 m_lVideoPitch;

		
		IPin* m_pRendererInPin;
		IPin* m_pSourceOutPin;


	};

}

#endif //_DSVIDEO_H__Included_734833474

