/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\MenuBackground.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MENUBACKGROUND_H__Included_2117227322
#define _MENUBACKGROUND_H__Included_2117227322


namespace ASC
{
	class CGameOfLifePattern;
	
	class ASCTexture;
	class ASCGUIStaticImage;

	class CMenuBackground
	{
	public:
		CMenuBackground(void);
		~CMenuBackground(void);

		bool Initialise();

		bool Activate();
		bool Deactivate();

		bool Update();

	protected:
		ASCVector<CGameOfLifePattern*> m_vecPatterns;

		CGameOfLifePattern* m_pGameOfLfie;
		ASCGUIStaticImage* m_pGUIImage;
		ASCTexture* m_pGUITexture;
		
		FLOAT32 m_fNewPatternTimer;
		FLOAT32 m_fResetTimer;

	};

}

#endif //_MENUBACKGROUND_H__Included_2117227322

