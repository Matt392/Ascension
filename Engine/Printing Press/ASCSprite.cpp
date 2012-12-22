#include "ASCGenInc.h"
#include "ASCSprite.h"
#include "ASCSpriteManager.h"
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "../Ascension/Ascension.h"
#include "ASCShader.h"
#include "ASCVertexDeclaration.h"
#include "ASCTexture.h"
#include "ASCTextureAtlas.h"
#include "ASCSpriteVertex.h"
#include "ASCINIFile.h"

#include <math.h>

ASCSprite::ASCSprite(void)
: m_pTexture ( NULL )
, m_pShader ( NULL )
, m_TechniqueHandle ( NULL )
, m_pSpriteVertex ( NULL )
, m_pTextureAtlas ( NULL )
, m_uDepth ( 0 )
, m_uColour ( 0xFFFFFFFF )
, m_fHotSpotX ( 0.0f )
, m_fHotSpotY ( 0.0f )
, m_fWidth ( 0.0f )
, m_fHeight ( 0.0f )
, m_fRotation ( 0.0f )
, m_fOffsetX ( 0.0f )
, m_fOffsetY ( 0.0f )
, m_bPositionChanged ( false )
, m_bRotationChanged ( false )
, m_bActive ( false )
, m_bAgainstScreen ( false )
, m_bGUISprite ( false )
{
	m_v2Pos.SetZero();
}


ASCSprite::~ASCSprite(void)
{
}

//Initialisation
bool 
ASCSprite::Init(ASCString& strName, ASCShader* pShader, ASCString& strTechnique)
{
	m_bActive = true;
	m_bPositionChanged = true;
	m_bRotationChanged = true;
	ASCTextureAtlas* pTextureAtlas = NULL;
	if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strName, pTextureAtlas))
	{
		SAtlasInfo sInfo = pTextureAtlas->GetAtlasUVs(strName);
		
		return Init(pTextureAtlas, pShader, sInfo, strTechnique);
	}
	m_pShader = pShader;
	m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(strName, true);
	
	Init(m_pTexture, pShader, strTechnique, false);

	return true;
}
bool 
ASCSprite::InitFile(ASCString& strName, ASCShader* pShader, ASCString& strTechnique)
{
	m_bActive = true;
	m_bPositionChanged = true;
	m_bRotationChanged = true;
	ASCINIFile INIFile;
	if(INIFile.Load(strName))
	{
		ASCString strAtlasFile = INIFile.GetString("General", "TextureAtlas");
		Ascension::Renderer().GetTextureManager()->LoadTextureAtlas(strAtlasFile);
	
		ASCString strTextureFile = INIFile.GetString("General", "Texture");

		m_strTechnique = strTechnique;

		ASCTextureAtlas* pTextureAtlas = NULL;
		if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strTextureFile, pTextureAtlas))
		{
			SAtlasInfo sInfo = pTextureAtlas->GetAtlasUVs(strTextureFile);
			if(Init(pTextureAtlas, pShader, sInfo, strTechnique))
			{
				SetSize(INIFile.GetFloat("General", "Width"), INIFile.GetFloat("General", "Height"));
				return true;
			}
			OUTPUT_WINDOW("Failed to load sprite file");
			return false;
		}
		m_pShader = pShader;
		m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(strTextureFile, true);
		if(Init(m_pTexture, pShader, strTechnique, false))
		{
			SetSize(INIFile.GetFloat("General", "Width"), INIFile.GetFloat("General", "Height"));
			return true;
		}
	}
	OUTPUT_WINDOW("Failed to load sprite file");
	return false;
}
bool 
ASCSprite::FileReset(ASCString strName)
{
	m_bActive = true;
	m_bPositionChanged = true;
	m_bRotationChanged = true;
	ASCINIFile INIFile;
	if(INIFile.Load(strName))
	{
		ASCString strAtlasFile = INIFile.GetString("General", "TextureAtlas");
		Ascension::Renderer().GetTextureManager()->LoadTextureAtlas(strAtlasFile);
	
		ASCString strTextureFile = INIFile.GetString("General", "Texture");

		ASCTextureAtlas* pTextureAtlas = NULL;
		if(Ascension::Renderer().GetTextureManager()->IsInTextureAtlas(strTextureFile, pTextureAtlas))
		{
			SAtlasInfo sInfo = pTextureAtlas->GetAtlasUVs(strTextureFile);
			if(Init(pTextureAtlas->GetTexture(), m_pShader, sInfo, m_strTechnique))
			{
				SetSize(INIFile.GetFloat("General", "Width"), INIFile.GetFloat("General", "Height"));
				return true;
			}
			OUTPUT_WINDOW("Failed to load sprite file");
			return false;
		}
		m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(strTextureFile, true);
		if(Init(m_pTexture, m_pShader, m_strTechnique, false))
		{
			SetSize(INIFile.GetFloat("General", "Width"), INIFile.GetFloat("General", "Height"));
			return true;
		}
	}
	OUTPUT_WINDOW("Failed to load sprite file");
	return false;
}
bool 
ASCSprite::Init(ASCTexture* pTexture, ASCShader* pShader, SAtlasInfo sInfo, ASCString& strTechnique)
{
	m_pShader = pShader;
	m_pTexture = pTexture;

	m_TechniqueHandle = m_pShader->GetNamedHandle(strTechnique.c_str());

#ifdef ASC_IOS
	m_fWidth = sInfo.m_fWidth;// / (960.0f / Ascension::Width());
	m_fHeight = sInfo.m_fHeight;// / (640.0f / Ascension::Height());
#else
	m_fWidth = sInfo.m_fWidth;
	m_fHeight = sInfo.m_fHeight;
#endif
	m_fHotSpotX = m_fWidth * 0.5f;
	m_fHotSpotY = m_fHeight * 0.5f;


	m_pSpriteVertex = Ascension::SpriteManager().GetSpriteVertex(m_pTexture, strTechnique);

	static const FLOAT32 sk_uvMod = 0.001f;
	
	m_pSpriteVerts[0].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);//UL
	m_pSpriteVerts[1].Init(356.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR-sk_uvMod, sInfo.m_fVL);//UR
	m_pSpriteVerts[2].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR-sk_uvMod, sInfo.m_fVR-sk_uvMod);//LR
	m_pSpriteVerts[3].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);//UL
	m_pSpriteVerts[4].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR-sk_uvMod, sInfo.m_fVR-sk_uvMod);//LR
	m_pSpriteVerts[5].Init(100.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVR-sk_uvMod);//LL

	
	return m_pTexture != NULL;
}
bool 
ASCSprite::Init(ASCTextureAtlas* pTextureAtlas, ASCShader* pShader, SAtlasInfo sInfo, ASCString& strTechnique)
{
	m_pShader = pShader;
	m_pTextureAtlas = pTextureAtlas;

	m_TechniqueHandle = m_pShader->GetNamedHandle(strTechnique.c_str());

#ifdef ASC_IOS
	m_fWidth = sInfo.m_fWidth;// / (960.0f / Ascension::Width());
	m_fHeight = sInfo.m_fHeight;// / (640.0f / Ascension::Height());
#else
	m_fWidth = sInfo.m_fWidth;
	m_fHeight = sInfo.m_fHeight;
#endif
	m_fHotSpotX = m_fWidth * 0.5f;
	m_fHotSpotY = m_fHeight * 0.5f;


	m_pSpriteVertex = Ascension::SpriteManager().GetSpriteVertex(m_pTextureAtlas, strTechnique);

	static const FLOAT32 sk_uvMod = 0.001f;
	
	m_pSpriteVerts[0].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);//UL
	m_pSpriteVerts[1].Init(356.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR-sk_uvMod, sInfo.m_fVL);//UR
	m_pSpriteVerts[2].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR-sk_uvMod, sInfo.m_fVR-sk_uvMod);//LR
	m_pSpriteVerts[3].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);//UL
	m_pSpriteVerts[4].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR-sk_uvMod, sInfo.m_fVR-sk_uvMod);//LR
	m_pSpriteVerts[5].Init(100.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVR-sk_uvMod);//LL

	for(UINT32 i = 0; i < 6; ++i)
	{
		m_pSpriteVerts[i].TextureIndex(sInfo.m_uTextureID);
	}
	
	return m_pTextureAtlas != NULL;
}
bool 
ASCSprite::Init(ASCTexture* pTexture, ASCShader* pShader, ASCString& strTechnique, bool bVideoSprite)
{
	m_pShader = pShader;
	m_pTexture = pTexture;

	m_TechniqueHandle = m_pShader->GetNamedHandle(strTechnique.c_str());

	m_fWidth = static_cast<FLOAT32>(m_pTexture->GetWidth());
	m_fHeight = static_cast<FLOAT32>(m_pTexture->GetHeight());

	m_fHotSpotX = m_fWidth * 0.5f;
	m_fHotSpotY = m_fHeight * 0.5f;


	m_pSpriteVertex = Ascension::SpriteManager().GetSpriteVertex(m_pTexture, strTechnique, bVideoSprite);

	m_pSpriteVerts[0].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	m_pSpriteVerts[1].Init(356.0f,	100.0f,	0.0f, 0xFFFFFFFF, 1.0f, 0.0f);
	m_pSpriteVerts[2].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, 1.0f, 1.0f);
	m_pSpriteVerts[3].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	m_pSpriteVerts[4].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, 1.0f, 1.0f);
	m_pSpriteVerts[5].Init(100.0f,	356.0f,	0.0f, 0xFFFFFFFF, 0.0f, 1.0f);

	m_bActive = true;
	
	return m_pTexture != NULL;
}

//Updating
bool 
ASCSprite::Update()
{
	return true;
}
//General
FLOAT32 
ASCSprite::GetWidth( void ) const
{
	return m_fWidth;
}
FLOAT32 
ASCSprite::GetHeight( void ) const
{
	return m_fHeight;
}
FLOAT32 
ASCSprite::PosX()
{
	if(false == m_bAgainstScreen)
	{
		return m_v2Pos.X();
	}
	FLOAT32 fReturn = m_v2Pos.X() * Ascension::Width();
	return fReturn;
}
FLOAT32 
ASCSprite::PosY()
{
	if(false == m_bAgainstScreen)
	{
		return m_v2Pos.Y();
	}
	FLOAT32 fReturn = m_v2Pos.Y() * Ascension::Height();
	return fReturn;
}

void 
ASCSprite::SetSize(FLOAT32 fWidth, FLOAT32 fHeight, bool bUpdateHotSpot)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_bPositionChanged = true;
	m_bRotationChanged = true;
	if(bUpdateHotSpot)
	{
		SetHotSpot(m_fWidth * 0.5f, m_fHeight * 0.5f);
	}
}
void 
ASCSprite::SetHotSpot(FLOAT32 fX, FLOAT32 fY)
{
	if(false == m_bAgainstScreen)
	{
		m_fHotSpotX = fX;
		m_fHotSpotY = fY;
		m_bPositionChanged = true;
		m_bRotationChanged = true;
	
		m_pSpriteVerts[0].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[0].m_fY = 0 - m_fHotSpotY;
		m_pSpriteVerts[3].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[3].m_fY = 0 - m_fHotSpotY;
						 
		m_pSpriteVerts[1].m_fX = 0 - m_fHotSpotX + m_fWidth;
		m_pSpriteVerts[1].m_fY = 0 - m_fHotSpotY;
						 
		m_pSpriteVerts[2].m_fX = 0 - m_fHotSpotX + m_fWidth;
		m_pSpriteVerts[2].m_fY = 0 - m_fHotSpotY + m_fHeight;
		m_pSpriteVerts[4].m_fX = 0 - m_fHotSpotX + m_fWidth;
		m_pSpriteVerts[4].m_fY = 0 - m_fHotSpotY + m_fHeight;
						 
		m_pSpriteVerts[5].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[5].m_fY = 0 - m_fHotSpotY + m_fHeight;
	}
	else
	{
#ifdef ASC_IOS
		m_fHotSpotX = fX / Ascension::Height();
		m_fHotSpotY = fY / Ascension::Width();
#else
		m_fHotSpotX = fX / Ascension::Width();
		m_fHotSpotY = fY / Ascension::Height();
#endif
		m_bPositionChanged = true;
		m_bRotationChanged = true;
	
		m_pSpriteVerts[0].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[0].m_fY = 0 - m_fHotSpotY;
		m_pSpriteVerts[3].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[3].m_fY = 0 - m_fHotSpotY;
        
#ifdef ASC_IOS
		m_pSpriteVerts[1].m_fX = 0 - m_fHotSpotX + (m_fWidth / Ascension::Height());
		m_pSpriteVerts[1].m_fY = 0 - m_fHotSpotY;
        
		m_pSpriteVerts[2].m_fX = 0 - m_fHotSpotX + (m_fWidth / Ascension::Height());
		m_pSpriteVerts[2].m_fY = 0 - m_fHotSpotY + (m_fHeight / Ascension::Width());
		m_pSpriteVerts[4].m_fX = 0 - m_fHotSpotX + (m_fWidth / Ascension::Height());
		m_pSpriteVerts[4].m_fY = 0 - m_fHotSpotY + (m_fHeight / Ascension::Width());
        
		m_pSpriteVerts[5].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[5].m_fY = 0 - m_fHotSpotY + (m_fHeight / Ascension::Width());
#else
		m_pSpriteVerts[1].m_fX = 0 - m_fHotSpotX + (m_fWidth / Ascension::Width());
		m_pSpriteVerts[1].m_fY = 0 - m_fHotSpotY;
						 
		m_pSpriteVerts[2].m_fX = 0 - m_fHotSpotX + (m_fWidth / Ascension::Width());
		m_pSpriteVerts[2].m_fY = 0 - m_fHotSpotY + (m_fHeight / Ascension::Height());
		m_pSpriteVerts[4].m_fX = 0 - m_fHotSpotX + (m_fWidth / Ascension::Width());
		m_pSpriteVerts[4].m_fY = 0 - m_fHotSpotY + (m_fHeight / Ascension::Height());
						 
		m_pSpriteVerts[5].m_fX = 0 - m_fHotSpotX;
		m_pSpriteVerts[5].m_fY = 0 - m_fHotSpotY + (m_fHeight / Ascension::Height());
#endif
	}
}
void 
ASCSprite::SetTexels(UINT32* pColour, UINT32 uSize)
{
	if ( m_pTexture )
	{
		UINT32 uPitch;
		UINT32* pTexels;
		const UINT32 uWidth = m_pTexture->GetWidth();
		const UINT32 uHeight = m_pTexture->GetHeight();
		if((uWidth * uHeight) == uSize)
		{
			if ( m_pTexture->LockTexels( pTexels, uPitch, false ) )
			{
				for ( UINT32 y=0; y<uHeight; y++ )
				{
					for ( UINT32 x=0; x<uWidth; x++ )
					{
						pTexels[x] = pColour[y*uWidth+x];
					}
					pTexels+=uPitch;
				}
				m_pTexture->UnlockTexels();
			}
		}
	}
}

void 
ASCSprite::SetPosition(FLOAT32 fX, FLOAT32 fY)
{
	if(false == m_bAgainstScreen)
	{
		if(m_v2Pos.X() != fX || m_v2Pos.Y() != fY)
		{
			m_bPositionChanged = true;
			m_v2Pos.Set(fX + m_fOffsetX, fY + m_fOffsetY);
			for(UINT32 i = 0; i < 6; ++i)
			{
				m_pSpriteVerts[i].m_fPosX = m_v2Pos.X();
				m_pSpriteVerts[i].m_fPosY = m_v2Pos.Y();
			}
		}
	}
	else
	{
		if(m_v2Pos.X() != fX || m_v2Pos.Y() != fY)
		{
			m_bPositionChanged = true;
#ifdef ASC_IOS
			m_v2Pos.Set((fX + m_fOffsetX) / Ascension::Height(), (fY + m_fOffsetY) / Ascension::Width());
#else
			m_v2Pos.Set((fX + m_fOffsetX) / Ascension::Width(), (fY + m_fOffsetY) / Ascension::Height());
#endif
			for(UINT32 i = 0; i < 6; ++i)
			{
				m_pSpriteVerts[i].m_fPosX = m_v2Pos.X();
				m_pSpriteVerts[i].m_fPosY = m_v2Pos.Y();
			}
		}
	}
}
void 
ASCSprite::IncPosition(FLOAT32 fX, FLOAT32 fY)
{
	if(false == m_bAgainstScreen)
	{
		if(0.0f != fX || 0.0f != fY)
		{
			m_bPositionChanged = true;
			ASCVector2 v2New(fX, fY);
			m_v2Pos += v2New;
			for(UINT32 i = 0; i < 6; ++i)
			{
				m_pSpriteVerts[i].m_fPosX = m_v2Pos.X();
				m_pSpriteVerts[i].m_fPosY = m_v2Pos.Y();
			}
		}
	}
	else
	{
		if(0.0f != fX || 0.0f != fY)
		{
			m_bPositionChanged = true;
#ifdef ASC_IOS
			ASCVector2 v2New(fX / Ascension::Height(), fY / Ascension::Width());
#else
			ASCVector2 v2New(fX / Ascension::Width(), fY / Ascension::Height());
#endif
			m_v2Pos += v2New;
			for(UINT32 i = 0; i < 6; ++i)
			{
				m_pSpriteVerts[i].m_fPosX = m_v2Pos.X();
				m_pSpriteVerts[i].m_fPosY = m_v2Pos.Y();
			}
		}
	}
}
void 
ASCSprite::SetPosition(ASCVector2& v2Pos)
{
	if(false == m_bAgainstScreen)
	{
		if(m_v2Pos.X() != v2Pos.X() || m_v2Pos.X() != v2Pos.Y())
		{
			m_bPositionChanged = true;
			m_v2Pos = v2Pos;
			m_v2Pos.X(m_v2Pos.X()  + m_fOffsetX);
			m_v2Pos.Y(m_v2Pos.Y()  + m_fOffsetY);
			for(UINT32 i = 0; i < 6; ++i)
			{
				m_pSpriteVerts[i].m_fPosX = m_v2Pos.X();
				m_pSpriteVerts[i].m_fPosY = m_v2Pos.Y();
			}
		}
	}
	else
	{
		if(m_v2Pos.X() != v2Pos.X() || m_v2Pos.X() != v2Pos.Y())
		{
			m_bPositionChanged = true;
#ifdef ASC_IOS
			v2Pos.X((v2Pos.X() + m_fOffsetX) / Ascension::Height());
			v2Pos.Y((v2Pos.Y() + m_fOffsetY) / Ascension::Width());
#else
			v2Pos.X((v2Pos.X() + m_fOffsetX) / Ascension::Width());
			v2Pos.Y((v2Pos.Y() + m_fOffsetY) / Ascension::Height());
#endif
			m_v2Pos = v2Pos;
			for(UINT32 i = 0; i < 6; ++i)
			{
				m_pSpriteVerts[i].m_fPosX = m_v2Pos.X();
				m_pSpriteVerts[i].m_fPosY = m_v2Pos.Y();
			}
		}
	}
}

void 
ASCSprite::SetRotation(FLOAT32 fRot)
{
	if(m_fRotation != fRot)
	{
		m_fRotation = fRot;
		m_bRotationChanged = true;
		for(UINT32 i = 0; i < 6; ++i)
		{
			m_pSpriteVerts[i].m_fRotation = m_fRotation;
		}
	}
}
void 
ASCSprite::SetAlpha(FLOAT32 fAlpha)
{
	m_uColour;
	CHAR8* pColour = reinterpret_cast<CHAR8*>(&m_uColour);

	pColour[3] = static_cast<CHAR8>(255.0f * fAlpha);

	SetColour(m_uColour);
}
void 
ASCSprite::SetColour(UINT32 uColour)
{
	m_uColour = uColour;
	for(UINT32 j = 0; j < 6; ++j)
	{
		m_pSpriteVerts[j].SetColour( uColour );
	}
}

void 
ASCSprite::SetActive(bool bVal)
{
	m_bActive = bVal;
	FLOAT32 fLevel = 0.0f;
	if(false == m_bActive)
	{
		fLevel = -1.0f;
	}
	for(UINT32 i = 0; i < 6; ++i)
	{
		m_pSpriteVerts[i].m_fZ = fLevel;
	}
}
void 
ASCSprite::SetDepth(FLOAT32 fVal)
{
	for(UINT32 i = 0; i < 6; ++i)
	{
		m_pSpriteVerts[i].m_fZ = fVal;
	}
}
void 
ASCSprite::SetUVs(SAtlasInfo& sUVInfo)
{
	m_pSpriteVerts[0].m_fTU = sUVInfo.m_fUL;
	m_pSpriteVerts[1].m_fTU = sUVInfo.m_fUR;
	m_pSpriteVerts[2].m_fTU = sUVInfo.m_fUR;
	m_pSpriteVerts[3].m_fTU = sUVInfo.m_fUL;
	m_pSpriteVerts[4].m_fTU = sUVInfo.m_fUR;
	m_pSpriteVerts[5].m_fTU = sUVInfo.m_fUL;
					 
	m_pSpriteVerts[0].m_fTV = sUVInfo.m_fVL;
	m_pSpriteVerts[1].m_fTV = sUVInfo.m_fVL;
	m_pSpriteVerts[2].m_fTV = sUVInfo.m_fVR;
	m_pSpriteVerts[3].m_fTV = sUVInfo.m_fVL;
	m_pSpriteVerts[4].m_fTV = sUVInfo.m_fVR;
	m_pSpriteVerts[5].m_fTV = sUVInfo.m_fVR;
	
	for(UINT32 i = 0; i < 6; ++i)
	{
		m_pSpriteVerts[i].TextureIndex(sUVInfo.m_uTextureID);
	}
}
void 
ASCSprite::SetAgainstScreen(bool bVal)
{
	m_bAgainstScreen = bVal;
	if(m_bAgainstScreen)
	{
		m_fNonAgainstScreenDepth = m_pSpriteVerts[0].m_fW;
		for(UINT32 i = 0; i < 6; ++i)
		{
			m_pSpriteVerts[i].m_fW = 1.0f;
		}
	}
	else
	{
		for(UINT32 i = 0; i < 6; ++i)
		{
			m_pSpriteVerts[i].m_fW = m_fNonAgainstScreenDepth;
		}
	}
	SetSize(m_fWidth, m_fHeight);
}
void 
ASCSprite::SetScale(FLOAT32 fXScale, FLOAT32 fYScale)
{
	for(UINT32 i = 0; i < 6; ++i)
	{
		m_pSpriteVerts[i].m_fScaleX = fXScale;
		m_pSpriteVerts[i].m_fScaleY = fYScale;
	}
}
void 
ASCSprite::IncScale(FLOAT32 fXScale, FLOAT32 fYScale)
{
	for(UINT32 i = 0; i < 6; ++i)
	{
		m_pSpriteVerts[i].m_fScaleX += fXScale;
		m_pSpriteVerts[i].m_fScaleY += fYScale;
	}
}
FLOAT32 
ASCSprite::GetScaleX()
{
	return m_pSpriteVerts[0].m_fScaleX;
}
FLOAT32 
ASCSprite::GetScaleY()
{
	return m_pSpriteVerts[0].m_fScaleY;
}
void 
ASCSprite::SetDepthLayer(UINT32 uDepth)
{
	m_uDepth = uDepth;
}
ASCTexture* 
ASCSprite::Texture()
{
	return m_pTexture;
}
void 
ASCSprite::SetOffset(FLOAT32 fOffsetX, FLOAT32 fOffsetY)
{
	m_fOffsetX = fOffsetX;
	m_fOffsetY = fOffsetY;
}
void 
ASCSprite::Release()
{
	m_bActive = false;
}
void 
ASCSprite::Create()
{
	m_bActive = true;
}
void 
ASCSprite::Render()
{
	if(m_bActive && m_pSpriteVertex)
	{
		for(UINT32 i = 0; i < 6; ++i)
		{
			m_pSpriteVertex->AddVertex( m_pSpriteVerts[i], m_uDepth );
		}
	}
}
void 
ASCSprite::SetTexture(ASCTexture* pTexture)
{
	m_pTexture = pTexture;
	m_pSpriteVertex->SetTexture(pTexture);
}
SVertex 
ASCSprite::GetVertex(UINT32 uVal)
{
	return m_pSpriteVerts[uVal];
}
void 
ASCSprite::SetGUISprite(bool bVal)
{
	m_bGUISprite = bVal;
	SetDepthLayer( ASC_MAX_SPRITE_DEPTH-1 );
}
bool 
ASCSprite::IsGUISprite()
{
	return m_bGUISprite;
}