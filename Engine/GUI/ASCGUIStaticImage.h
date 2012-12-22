/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIStaticImage.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUISTATICIMAGE_H__Included_1489012800
#define _ASCGUISTATICIMAGE_H__Included_1489012800


#ifdef ASC_NEW_GUI_SYSTEM

#include "ASCGUIElement.h"
#include "ASCGUIVisualElement.h"

namespace ASC
{
	class ASCTexture;
	class ASCAnimSprite;

	class ASCGUIStaticImage : public ASCGUIElement, public ASCGUIVisualElement
	{
	public:
		ASCGUIStaticImage(void);
		~ASCGUIStaticImage(void);
		
		bool Init(ASCString strImage, FLOAT32 fPosX, FLOAT32 fPosY, ASCString& strTechnique);
		bool Init(ASCTexture* pTexture, FLOAT32 fPosX, FLOAT32 fPosY, ASCString& strTechnique);
		void ResetFile(ASCString strFile);
		
		virtual void SetPos(FLOAT32 fX, FLOAT32 fY);
		virtual void IncPosition(FLOAT32 fX, FLOAT32 fY);
		
		virtual void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);
		
		virtual void Disable();
		virtual void Enable();


	protected:
	};

}
#else
namespace ASC
{
	class ASCTexture;
	class ASCAnimSprite;

	class ASCGUIStaticImage : public ASCGUIElement
	{
	public:
		ASCGUIStaticImage(void);
		~ASCGUIStaticImage(void);
		
		bool Init(ASCString strImage, FLOAT32 fPosX, FLOAT32 fPosY, ASCString& strTechnique);
		bool Init(ASCTexture* pTexture, FLOAT32 fPosX, FLOAT32 fPosY, ASCString& strTechnique);
		void ResetFile(ASCString strFile);
		
		virtual void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);


	protected:
	};

}
#endif
#endif //_ASCGUISTATICIMAGE_H__Included_1489012800

