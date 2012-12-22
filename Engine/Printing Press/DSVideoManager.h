/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\DSVideoManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DSVIDEOMANAGER_H__Included_2010967650
#define _DSVIDEOMANAGER_H__Included_2010967650

#include "ASCVideoManager.h"

namespace ASC
{
	class ASCVideo;

	class CDSVideoManager : public ASCVideoManager
	{
	public:
		CDSVideoManager(void);
		~CDSVideoManager(void);

		virtual ASCVideo* LoadVideo(ASCString strFile);


	protected:
		ASCVector<ASCVideo*> m_Videos;
	};

}

#endif //_DSVIDEOMANAGER_H__Included_2010967650

