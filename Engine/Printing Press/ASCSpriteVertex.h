/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCSpriteVertex.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSPRITEVERTEX_H__Included_1008652079
#define _ASCSPRITEVERTEX_H__Included_1008652079

#define ASC_MAX_SPRITE_DEPTH 10

namespace ASC
{
	class ASCTexture;
	class ASCTextureAtlas;
	class ASCShader;
	class ASCVertexBuffer;


	class ASCSpriteVertex
	{
	public:
		ASCSpriteVertex(void);
		~ASCSpriteVertex(void);

		//Initialise
		bool Init(ASCString& strTechnique, ASCTexture* pTexture, ASCShader* pShader);
		bool Init(ASCString& strTechnique, ASCTextureAtlas* pTextureAtlas, ASCShader* pShader);

		//General
		bool IsCompatable(ASCTexture* pTexture, ASCString& strTechnique);
		bool IsCompatable(ASCTextureAtlas* pTextureAtlas, ASCString& strTechnique);

		//Rendering
		void Render();

		void OrderSpritesForDepth();

		void Prepare();

		void AddVertex(SVertex sVertex, UINT32 uDepth);

		void SetTexture(ASCTexture* pTexture);

		ASCString TextureID();
		ASCString TechniqeID();

	protected:
		const void* m_TechniqueHandle;
		const void* m_CameraPosHandle;
		ASCTexture* m_pTexture;
		ASCTextureAtlas* m_pTextureAtlas;
		ASCShader* m_pShader;

		ASCString m_strTechnique;
		
		ASCVector<SVertex> m_vecVertex;
		ASCVector<SVertex> m_vecVertexPreDepthSort[ASC_MAX_SPRITE_DEPTH];
		

		UINT32 m_uAvaliableVetexCount;

		ASCVertexBuffer* m_pVertexBuffer;

		bool m_bHardwareVerticies;

		UINT32 m_uCount;

		ASCString m_strID;
	};

}

#endif //_ASCSPRITEVERTEX_H__Included_1008652079

