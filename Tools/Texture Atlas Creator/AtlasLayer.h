/**
*	@Ascension Engine
 *	@file Ascension 2D\Tools\Texture Atlas Creator\AtlasLayer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ATLASLAYER_H__Included_2095765767
#define _ATLASLAYER_H__Included_2095765767

#include "ASCIEventListener.h"

namespace ASC
{
#define ATLAS_ICON_SIZE 102.0f
#define ATLAS_CENTER_X 586.0f
#define ATLAS_CENTER_Y 526.5f
#define ATLAS_AREA_H_WIDTH 438.0f
#define ATLAS_AREA_H_HEIGHT 468.5f
#define TEXTURE_LIST_X 1030.0f
#define TEXTURE_LIST_Y 58.0f
#define INFO_ZONE_X 170.0f
#define INFO_ZONE_Y 34.0f

	class ASCTexture;
	class ASCGUIStaticImage;
	class ASCGUIButton;
	class ASCFont;
	class ASCFileIO;
	class ASCINIFile;

	struct AtlasTexture
	{
		ASCGUIButton* m_pOnScreenImage;
		ASCTexture* m_pTexture;
		UINT32 m_uGUIID;
		UINT32* m_pTextureData;
		ASCString m_strFilePath;
		ASCString m_strName;

		AtlasTexture()
		{
			m_pTextureData = NULL;
		}
		~AtlasTexture()
		{
			SafeDeleteArray( m_pTextureData );
		}
	};

	class CAtlasLayer : public ASCIEventListener
	{
	public:
		CAtlasLayer(void);
		~CAtlasLayer(void);

		UINT32 Initialise(UINT32 uWidth, UINT32 uHeight, UINT32 uLayerNum);
		void Initialise();

		bool IsValid();

		void Reset();

		void Update();
		void UpdateCurrentTexture();
		void UpdateAllTextures();
		
		void Deactivate();
		void Activate();
		
		void AddTexture(ASCString& strTexture);
		void AddTexture(ASCString& strSection, ASCTexture* pSourceTexture, ASCINIFile& rFile);
		void RemoveCurrentTexture();
		void MoveCurrentTexture(FLOAT32 fX, FLOAT32 fY);
		void PanAtlas(FLOAT32 fX, FLOAT32 fY);

		void Zoom(FLOAT32 fVal);

		void Save(ASCFileIO& rGameFile, ASCFileIO& rEditFile, UINT32& ruTextureCount, UINT32 uAtlasNum, ASCString& strAtlasName);

		UINT32 TextureCount();

		void CalculateTexture();
		
		//Events System
		void OnEvent(ASCIEvent* pEvent);

	protected:
		ASCVector<AtlasTexture*> m_vecTextures;
		UINT32 m_uCurrentTexture;
		
		ASCTexture* m_pLayerSaveTexure;
		ASCTexture* m_pLayerTexure;
		ASCGUIButton* m_pLayerIcon;
		UINT32 m_uIconButton;

		
		ASCTexture* m_pDisplayTexture;
		ASCGUIStaticImage* m_pEditDisplay;

		ASCFont* m_pFont;

		UINT32 m_uLayerNumber;
		
		UINT32 m_uWidth;
		UINT32 m_uHeight;

		bool m_bActive;
	};

}

#endif //_ATLASLAYER_H__Included_2095765767

