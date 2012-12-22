/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Sound\ASCSound.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSOUND_H__Included_772710984
#define _ASCSOUND_H__Included_772710984


namespace ASC
{
	class ASCSound
	{
	public:
		ASCSound(void);
		~ASCSound(void);
		
		bool Init();
		bool Active();

	protected:
		UINT32 uiBuffer;
		UINT32 uiSource;  
		SINT32 iState;

		ASCString m_strFileLoaded;
	};

}

#endif //_ASCSOUND_H__Included_772710984

