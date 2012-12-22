/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCSpriteManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSPRITEMANAGER_H__Included_125633254
#define _ASCSPRITEMANAGER_H__Included_125633254


namespace ASC
{
	class ASCSprite;
	class ASCAnimSprite;
	class ASCRenderer;
	class ASCShader;
	class ASCSpriteVertex;
	class ASCTexture;
	class ASCTextureAtlas;

	enum ESpriteRenderOrderMode
	{
		ESROM_FORWARDS,
		ESROM_BACKWARDS,
		ESROM_PRE_POST_FORWARD,
		ESROM_PRE_POST_BACKWARD,
		ESROM_COUNT,
	};

	class ASCSpriteManager
	{
	public:
		ASCSpriteManager(void);
		~ASCSpriteManager(void);
		
		//Initialise
		bool Initialise();

		//Create
		ASCSprite* CreateSprite(ASCString strSpriteName, ASCString strTechnique = "BasicSprite");
		ASCSprite* CreateSprite(FLOAT32 fWidth, FLOAT32 fHeight, UINT32 uTextureWidth, UINT32 uTextureHeight, ASCString strName, ASCString strTechnique = "BasicSprite");
		ASCSprite* CreateSprite(ASCTexture* pTexture, ASCString strTechnique = "BasicSprite", bool bVideoSprite = false);
		ASCSprite* CreateSpriteFile(ASCString strSpriteName, ASCString strTechnique = "BasicSprite");
		ASCAnimSprite* CreateAnimSprite(ASCString strSpriteName, ASCString strTechnique = "BasicSprite");

		//General
		ASCSpriteVertex* GetSpriteVertex(ASCTexture* pTexture, ASCString& strTechnique, bool bVideoSprite = false);
		ASCSpriteVertex* GetSpriteVertex(ASCTextureAtlas* pTextureAtlas, ASCString& strTechnique, bool bVideoSprite = false);
		void Render();
		void PostFontRender();
		void Update();
		ASCShader* GetSpriteShader();

		void AddSprite(ASCSprite* pSprite);
		void RemoveSprite(ASCSprite* pSprite);
		
		ASCVector<ASCSpriteVertex*>& SpriteVerticies();
		ASCVector<ASCSpriteVertex*>& SpriteVerticiesRenderOrderPre();
		ASCVector<ASCSpriteVertex*>& SpriteVerticiesRenderOrder();
		ASCVector<ASCSpriteVertex*>& SpriteVerticiesRenderOrderPost();
		ASCVector<ASCSpriteVertex*>& SpriteVerticiesPostFontRenderOrder();

		void SetRenderOrderMode(ESpriteRenderOrderMode eMode);

	protected:
		ASCRenderer* m_pRenderer;
		ASCShader* m_pSpriteShader;
		
		UINT32 m_uEmptySlots;
		ASCVector<ASCSprite*> m_SpriteList;
		ASCVector<ASCSpriteVertex*> m_SpriteVertexes;
		
		ASCVector<ASCSpriteVertex*> m_SpriteVertexesRenderOrderPre;
		ASCVector<ASCSpriteVertex*> m_SpriteVertexesRenderOrder;
		ASCVector<ASCSpriteVertex*> m_SpriteVertexesRenderOrderPost;

		ASCVector<ASCSpriteVertex*> m_SpriteVertexesPostFontRenderOrder;

		ESpriteRenderOrderMode m_OrderMode;
	};

}

#endif //_ASCSPRITEMANAGER_H__Included_125633254

