/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Ascension\ASCVisualDebugging.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCVISUALDEBUGGING_H__Included_596747809
#define _ASCVISUALDEBUGGING_H__Included_596747809


namespace ASC
{
	class ASCFont;

	class ASCVisualDebugging
	{
	public:
		ASCVisualDebugging(void);
		~ASCVisualDebugging(void);

		void Render();

	protected:
		ASCFont* m_pFont;

		FLOAT32 m_fLineGap;

		UINT32 m_uTextColour;

		UINT32 m_uMaxLinesToDraw;

		UINT32 m_uOffset;

		ASCVector<ASCString> strFinal;
		ASCVector<UINT32> uLines;

	};

}

#endif //_ASCVISUALDEBUGGING_H__Included_596747809

