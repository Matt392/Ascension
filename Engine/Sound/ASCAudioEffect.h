/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Sound\ASCAudioEffect.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCAUDIOEFFECT_H__Included_935252202
#define _ASCAUDIOEFFECT_H__Included_935252202

#include "ASCSound.h"

namespace ASC
{
	class ASCAudioEffect : public ASCSound
	{
	public:
		ASCAudioEffect(void);
		~ASCAudioEffect(void);

		bool Play(ASCString strFile);
	};

}

#endif //_ASCAUDIOEFFECT_H__Included_935252202

