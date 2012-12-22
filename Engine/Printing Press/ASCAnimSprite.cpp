#include "ASCGenInc.h"
#include "ASCAnimSprite.h"
#include "ASCAnimTrack.h"
#include "ASCShader.h"
#include "../Ascension/Ascension.h"
#include "ASCINIFile.h"
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCSpriteVertex.h"
#include "ASCSpriteManager.h"


ASCAnimSprite::ASCAnimSprite(void)
: m_uCurrentPlayingTrack ( 0 )
, m_bIgnorePause ( false )
{
}


ASCAnimSprite::~ASCAnimSprite(void)
{
	for(UINT32 i = 0; i < m_AnimTracks.size(); ++i)
	{
		SafeDelete( m_AnimTracks[i] );
	}
}

//Creation
bool 
ASCAnimSprite::Init(ASCString strName, ASCShader* pShader, ASCString strTechnique, UINT32 uEventID)
{
	m_pShader = pShader;
	m_TechniqueHandle = m_pShader->GetNamedHandle(strTechnique.c_str());
	m_uEventID = uEventID;

	ASCINIFile INIFile;
	if(INIFile.Load(strName))
	{
		m_strFileID = strName;
		m_bActive = true;
		ASCString strAtlasName = INIFile.GetString( "General", "TextureAtlas");
		ASCTextureAtlas* pTextureAtlas = Ascension::Renderer().GetTextureManager()->LoadTextureAtlas(strAtlasName);
		m_pTexture = pTextureAtlas->GetTexture();

		m_strIdleAnim = INIFile.GetString( "General", "IdleAnim");
		
		m_fWidth = INIFile.GetFloat( "General", "Width");
		m_fHeight = INIFile.GetFloat( "General", "Height");

		UINT32 uAnimCount = INIFile.GetUInt( "General", "AnimCount");
		for(UINT32 i = 0; i < uAnimCount; ++i)
		{
			ASCString strAnim = "Anim";
			strAnim += i;
			ASCAnimTrack* pAnimTrack = new ASCAnimTrack();
			if(pAnimTrack->Init(INIFile, strAnim, uEventID, pTextureAtlas))
			{
				m_AnimTracks.push_back( pAnimTrack );
			}
			else
			{
				SafeDelete( pAnimTrack );
			}
		}

		m_uIdelID = GetAnimTrakID(m_strIdleAnim);
		

		m_fHotSpotX = m_fWidth * 0.5f;
		m_fHotSpotY = m_fHeight * 0.5f;

		
		m_strTechnique = strTechnique;
		m_pSpriteVertex = Ascension::SpriteManager().GetSpriteVertex(m_pTexture, m_strTechnique);

		SAtlasInfo sInfo;

		m_pSpriteVerts[0].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);
		m_pSpriteVerts[1].Init(356.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVL);
		m_pSpriteVerts[2].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVR);
		m_pSpriteVerts[3].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);
		m_pSpriteVerts[4].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVR);
		m_pSpriteVerts[5].Init(100.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVR);
		
		SetSize(INIFile.GetFloat( "General", "Width"), INIFile.GetFloat( "General", "Height"));
		
		SAtlasInfo* pUVData = NULL;
		m_AnimTracks[m_uCurrentPlayingTrack]->Update(1.0f, pUVData);
		
		PlayAnim( m_uIdelID );
		
		if(pUVData)
		{
			SetUVs(*pUVData);
		}

		return true;
	}
	return false;
}
bool 
ASCAnimSprite::FileReset(ASCString strFile)
{
	if(m_strFileID == strFile)
	{
		return true;
	}
	for(UINT32 i = 0; i < m_AnimTracks.size(); ++i)
	{
		SafeDelete( m_AnimTracks[i] );
	}
	m_AnimTracks.Clear();
	ASCINIFile INIFile;
	if(INIFile.Load(strFile))
	{
		m_strFileID = strFile;
		m_bActive = true;
		ASCString strAtlasName = INIFile.GetString( "General", "TextureAtlas");
		ASCTextureAtlas* pTextureAtlas = Ascension::Renderer().GetTextureManager()->LoadTextureAtlas(strAtlasName);
		m_pTexture = pTextureAtlas->GetTexture();

		m_strIdleAnim = INIFile.GetString( "General", "IdleAnim");
		
		m_fWidth = INIFile.GetFloat( "General", "Width");
		m_fHeight = INIFile.GetFloat( "General", "Height");

		UINT32 uAnimCount = INIFile.GetUInt( "General", "AnimCount");
		for(UINT32 i = 0; i < uAnimCount; ++i)
		{
			ASCString strAnim = "Anim";
			strAnim += i;
			ASCAnimTrack* pAnimTrack = new ASCAnimTrack();
			if(pAnimTrack->Init(INIFile, strAnim, m_uEventID, pTextureAtlas))
			{
				m_AnimTracks.push_back( pAnimTrack );
			}
			else
			{
				SafeDelete( pAnimTrack );
			}
		}

		m_uIdelID = GetAnimTrakID(m_strIdleAnim);
		

		m_fHotSpotX = m_fWidth * 0.5f;
		m_fHotSpotY = m_fHeight * 0.5f;


		m_pSpriteVertex = Ascension::SpriteManager().GetSpriteVertex(m_pTexture, m_strTechnique);

		SAtlasInfo sInfo;

		m_pSpriteVerts[0].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);
		m_pSpriteVerts[1].Init(356.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVL);
		m_pSpriteVerts[2].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVR);
		m_pSpriteVerts[3].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);
		m_pSpriteVerts[4].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVR);
		m_pSpriteVerts[5].Init(100.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVR);
		
		SetSize(INIFile.GetFloat( "General", "Width"), INIFile.GetFloat( "General", "Height"));
		
		SAtlasInfo* pUVData = NULL;
		m_AnimTracks[m_uCurrentPlayingTrack]->Update(1.0f, pUVData);
		
		PlayAnim( m_uIdelID );
		
		if(pUVData)
		{
			SetUVs(*pUVData);
		}

		return true;
	}
	return false;
}
//Update
bool 
ASCAnimSprite::Update()
{
	if(m_bActive)
	{
		ASCSprite::Update();
		SAtlasInfo* pUVData = NULL;
		if(m_AnimTracks[m_uCurrentPlayingTrack]->Update(Ascension::Delta(m_bIgnorePause), pUVData))
		{
			PlayAnim( m_uIdelID );
		}
		if(pUVData)
		{
			SetUVs(*pUVData);
		}
	}
	return true;
}

//Use
SINT32 
ASCAnimSprite::GetAnimTrakID(ASCString strID)
{
	for(UINT32 i = 0; i < m_AnimTracks.size(); ++i)
	{
		if(m_AnimTracks[i]->GetID() == strID)
		{
			return i;
		}
	}
	return -1;
}
bool 
ASCAnimSprite::PlayAnim(UINT32 uID, bool bLooped, bool bReset, FLOAT32 fTimeScale, FLOAT32 fABSLength)
{
	if(uID <= m_AnimTracks.size())
	{
		SAtlasInfo* pUVData = NULL;
		if(m_AnimTracks[uID]->Play(bLooped, bReset, fTimeScale, fABSLength, pUVData))
		{
			m_uCurrentPlayingTrack = uID;
			if(pUVData)
			{
				SetUVs(*pUVData);
			}
			return true;
		}
		assert_now("Failed to play track");
		return false;
	}
	assert_now("Anim track does not exist");
	return false;
}
bool 
ASCAnimSprite::PlayAnim(ASCString strID, bool bLooped, bool bReset, FLOAT32 fTimeScale, FLOAT32 fABSLength)
{
	return PlayAnim(GetAnimTrakID(strID), bLooped, bReset, fTimeScale, fABSLength);
}
UINT32 
ASCAnimSprite::GetAnimCount()
{
	return m_AnimTracks.size();
}
void 
ASCAnimSprite::GetAnimName(UINT32 uID, ASCString& strReturn)
{
	if(uID < m_AnimTracks.size())
	{
		strReturn = m_AnimTracks[uID]->GetID();
	}
}
void 
ASCAnimSprite::Render()
{
	if(m_bActive && m_pSpriteVertex)
	{
		for(UINT32 i = 0; i < 6; ++i)
		{
			m_pSpriteVertex->AddVertex( m_pSpriteVerts[i], m_uDepth );
		}
	}
}
UINT32 
ASCAnimSprite::AnimEventID()
{
	return m_uEventID;
}
void 
ASCAnimSprite::IgnorePause(bool bIgnorePause)
{
	m_bIgnorePause = bIgnorePause;
}