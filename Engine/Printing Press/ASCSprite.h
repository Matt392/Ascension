/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCSprite.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSPRITE_H__Included_1123685473
#define _ASCSPRITE_H__Included_1123685473

#include "ASCVector2.h"
#include "ASCTextureAtlas.h"

namespace ASC
{
	class ASCShader;
	class ASCTexture;
	class ASCSpriteVertex;
	class ASCTextureAtlas;

	struct SVertexValues
	{
		FLOAT32 m_fX;
		FLOAT32 m_fY;
		FLOAT32 m_fZ;
		
		FLOAT32 m_fU;
		FLOAT32 m_fV;
		SVertexValues()
		{
			m_fX = 0.0f;
			m_fY = 0.0f;
			m_fZ = 0.0f;
			
			m_fU = 0.0f;
			m_fV = 0.0f;
		}
	};


	class ASCSprite
	{
	public:
		ASCSprite(void);
		virtual ~ASCSprite(void);

		//Initialisation
		virtual bool Init(ASCString& strName, ASCShader* pShader, ASCString& strTechnique);
		virtual bool InitFile(ASCString& strName, ASCShader* pShader, ASCString& strTechnique);

		virtual bool FileReset(ASCString strName);

		virtual bool Init(ASCTexture* pTexture, ASCShader* pShader, SAtlasInfo sInfo, ASCString& strTechnique);
		virtual bool Init(ASCTextureAtlas* pTextureAtlas, ASCShader* pShader, SAtlasInfo sInfo, ASCString& strTechnique);
		virtual bool Init(ASCTexture* pTexture, ASCShader* pShader, ASCString& strTechnique, bool bVideoSprite);

		//General
		FLOAT32 GetWidth( void ) const;
		FLOAT32 GetHeight( void ) const;
		FLOAT32 PosX();
		FLOAT32 PosY();

		void SetSize(FLOAT32 fWidth, FLOAT32 fHeight, bool bUpdateHotSpot = true);
		void SetHotSpot(FLOAT32 fX, FLOAT32 fY);
		void SetTexels(UINT32* pColour, UINT32 uSize);

		void SetPosition(FLOAT32 fX, FLOAT32 fY);
		void IncPosition(FLOAT32 fX, FLOAT32 fY);
		void SetPosition(ASCVector2& v2Pos);

		void SetRotation(FLOAT32 fRot);

		void SetColour(UINT32 uColour);

		void SetActive(bool bVal);
		void SetDepth(FLOAT32 fVal);

		void SetUVs(SAtlasInfo& sUVInfo);

		void SetAgainstScreen(bool bVal);

		ASCTexture* Texture();
		void SetTexture(ASCTexture* pTexture);

		void SetScale(FLOAT32 fXScale, FLOAT32 fYScale);
		void IncScale(FLOAT32 fXScale, FLOAT32 fYScale);
		
		FLOAT32 GetScaleX();
		FLOAT32 GetScaleY();

		void SetOffset(FLOAT32 fOffsetX, FLOAT32 fOffsetY);

		void SetDepthLayer(UINT32 uDepth);

		void SetAlpha(FLOAT32 fAlpha);


		//Updating
		virtual bool Update( );

		virtual void Release();
		virtual void Create();

		virtual void Render();

		SVertex GetVertex(UINT32 uVal);

		void SetGUISprite(bool bVal);
		bool IsGUISprite();

	protected:
		ASCTexture* m_pTexture;
		ASCTextureAtlas* m_pTextureAtlas;
		ASCShader* m_pShader;
		ASCSpriteVertex* m_pSpriteVertex;
		SVertex m_pSpriteVerts[6];

		ASCString m_strTechnique;

		ASC_Handle m_TechniqueHandle;
		
		UINT32 m_uDepth;
		UINT32 m_uColour;

		FLOAT32 m_fHotSpotX;
		FLOAT32 m_fHotSpotY;
		FLOAT32 m_fWidth;
		FLOAT32 m_fHeight;
		FLOAT32 m_fRotation;
		
		FLOAT32 m_fOffsetX;
		FLOAT32 m_fOffsetY;

		FLOAT32 m_fNonAgainstScreenDepth;

		ASCVector2 m_v2Pos;

		bool m_bPositionChanged;
		bool m_bRotationChanged;
		bool m_bActive;
		bool m_bAgainstScreen;

		bool m_bGUISprite;
	};

}

#endif //_ASCSPRITE_H__Included_1123685473

