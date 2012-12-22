/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUIMANAGER_H__Included_1792586604
#define _ASCGUIMANAGER_H__Included_1792586604

#include "ASCIEventListener.h"

namespace ASC
{
	class ASCGUIElement;
	class ASCGUIButton;
	class ASCGUIStaticImage;
	class ASCGUIDebugElement;
	class ASCGUIInvisButton;
	class ASCTexture;

	class ASCGUIManager : public ASCIEventListener
	{
	public:
		ASCGUIManager(void);
		~ASCGUIManager(void);

		void Init(FLOAT32 fScaleX, FLOAT32 fScaleY);
		
		ASCGUIButton* AddGUIButton(ASCString strNorm, ASCString strOver, ASCString strDown, FLOAT32 fPosX, FLOAT32 fPosY, UINT32& uGUIEventID);
		ASCGUIButton* AddGUIButton(ASCTexture* pTexture, UINT32& uGUIEventID, ASCString strTechnique = "GUI");
		ASCGUIStaticImage* AddGUIStaticImage(ASCString strImage, FLOAT32 fPosX, FLOAT32 fPosY, ASCString strTechnique = "GUI");
		ASCGUIStaticImage* AddGUIStaticImage(ASCTexture* pTexture, FLOAT32 fPosX, FLOAT32 fPosY, ASCString strTechnique = "GUI");

		ASCGUIInvisButton* AddGUIInvisButton(FLOAT32 fPosX, FLOAT32 fPosY, FLOAT32 fWidth, FLOAT32 fHeight, UINT32& uGUIEventID);

		void AddDebugElement(ASCGUIDebugElement* pElement);
		void RemoveDebugElement(ASCGUIDebugElement* pElement);

		void Update(FLOAT32 fDelta);

		virtual void OnEvent(ASCIEvent* pEvent);
        
		void DebugBounds();

        void DebugInfo();
		
		FLOAT32 ScaleX();
		FLOAT32 ScaleY();

	protected:
		ASCVector<ASCGUIElement*> m_pGUIElements;
		ASCVector<ASCGUIDebugElement*> m_pGUIDebugElements;
		
		UINT32 m_uEventCount;
        
		FLOAT32 m_fScaleX;
		FLOAT32 m_fScaleY;

#ifdef ASC_IOS
        UINT32 m_uTouch[ASC_MAX_TOUCH_INPUT];
        
        bool m_bUse[ASC_MAX_TOUCH_INPUT];
        bool m_bClicked[ASC_MAX_TOUCH_INPUT];
		bool m_bHeld[ASC_MAX_TOUCH_INPUT];
		FLOAT32 m_fMouseX[ASC_MAX_TOUCH_INPUT];
		FLOAT32 m_fMouseY[ASC_MAX_TOUCH_INPUT];
#else
		UINT32 m_uMouseLeftClick;
		UINT32 m_uMousePos;
        
		FLOAT32 m_fMouseX;
		FLOAT32 m_fMouseY;
        
		bool m_bClicked;
		bool m_bHeld;
#endif
		

	};

}

#endif //_ASCGUIMANAGER_H__Included_1792586604

