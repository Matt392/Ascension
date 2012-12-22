/**
*	@Ascension Engine
 *	@file Ascension 2D\Tools\Texture Atlas Creator\TextureAtlasCreatorContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTUREATLASCREATORCONTEXT_H__Included_2031587636
#define _TEXTUREATLASCREATORCONTEXT_H__Included_2031587636

#include "ASCGameContext.h"
#include "ASCIEventListener.h"


namespace ASC
{
#define ATLAS_SIZE 2048
#define ALTAS_MAX_LAYERS 8
	enum EMainButtons
	{
		EMB_NEW,
		EMB_LOAD,
		EMB_SAVE,
		EMB_ADD,
		EMB_REMOVE,
		EMB_COUNT,
	};
	enum EBigButtons
	{
		EBB_UPDATE_ALL,
		EBB_UPDATE,
		EBB_COUNT,
	};

	class ASCGUIStaticImage;
	class ASCGUIButton;
	class CAtlasLayer;

	class CTextureAtlasCreatorContext : public ASCGameContext, ASCIEventListener
	{
	public:
		CTextureAtlasCreatorContext(void);
		~CTextureAtlasCreatorContext(void);

		//Creation
		bool Initialise();
		
		//Game logic
		bool Update();

		//Optional
		void Draw();
        void Disable();
		ASCIState* GetState(UINT32 uStateID)
		{
			uStateID;
			return NULL;
		}
		void SetState(UINT32 uStateID)
		{
			uStateID;
		}

		//Functionality
		bool New(bool bValidCheck = true);
		void AddTexture(ASCString strFile);
		void RemoveTexture();
		void LoadTexture();
		void Save();
		void SaveAtlas(ASCString strFile);

		void Load();
		void LoadAtlas(ASCString strFile);
		//Events System
		void OnEvent(ASCIEvent* pEvent);
	
	protected:
		ASCGUIStaticImage* m_pBackground;

		ASCGUIStaticImage* m_pMainButtonBanner;
		ASCGUIStaticImage* m_pInfoZoneBanner;
		ASCGUIStaticImage* m_pMessageZoneBanner;
		
		ASCGUIStaticImage* m_pTextureListZone;
		ASCGUIStaticImage* m_pLayerZone;

		ASCGUIButton* m_pMainButtons[EMB_COUNT];
		UINT32 m_uMainButtons[EMB_COUNT];
		ASCGUIStaticImage* m_pMainButtonName[EMB_COUNT];

		ASCGUIButton* m_pBigButtons[EBB_COUNT];
		UINT32 m_uBigButtons[EBB_COUNT];
		ASCGUIStaticImage* m_pBigButtonName[EBB_COUNT];

		CAtlasLayer* m_pAtlasLayers[ALTAS_MAX_LAYERS];
		UINT32 m_uLayerButtons[ALTAS_MAX_LAYERS];

		UINT32 m_uActiveLayer;
		
		UINT32 m_uMouseLeftButton;
		UINT32 m_uMouseXY;
		FLOAT32 m_fMouseX;
		FLOAT32 m_fMouseY;
		bool m_bMouseDown;
		
		UINT32 m_uMouseRightButton;
		bool m_bMouseRightDown;
		
		UINT32 m_uSroll;
		FLOAT32 m_fMouseScrollWheel;

		bool m_bQuit;
	};

}

#endif //_TEXTUREATLASCREATORCONTEXT_H__Included_2031587636

