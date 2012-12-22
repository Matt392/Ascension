/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCVideoManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCVIDEOMANAGER_H__Included_1832487096
#define _ASCVIDEOMANAGER_H__Included_1832487096


namespace ASC
{
	class ASCVideo;
	class ASCSprite;

	class ASCVideoManager
	{
	public:
		ASCVideoManager(void);
		~ASCVideoManager(void);

		ASCVideo* LoadVideo(ASCString strVideoFile, ASCString strName, ASCSprite*& pVideoSprite, ASCString strTechnique, bool bScreenSpace = true);

		ASCSprite* CreateVideoSprite(ASCVideo* pVideo, ASCString strTechnique, bool bScreenSpace);

		void Update();

	protected:
		ASCVector<ASCVideo*> m_Videos;
	};

}

#endif //_ASCVIDEOMANAGER_H__Included_1832487096

