#include "ASCGenInc.h"
#include "ASCSpriteManager.h"
#include "ASCRenderer.h"
#include "ASCShaderManager.h"
#include "ASCSprite.h"
#include "ASCSpriteVertex.h"
#include "ASCAnimSprite.h"
#include "ASCProfilingSystem.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#include "ASCTextureAtlas.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif


ASCSpriteManager::ASCSpriteManager(void)
: m_pRenderer ( NULL )
, m_pSpriteShader ( NULL )
, m_uEmptySlots ( 0 )
, m_OrderMode ( ESROM_FORWARDS )
{
}


ASCSpriteManager::~ASCSpriteManager(void)
{
	for(UINT32 i = 0; i < m_SpriteList.size(); ++i)
	{
		SafeDelete(m_SpriteList[i]);
	}
	m_SpriteVertexesRenderOrder.Clear();
	for(UINT32 i = 0; i < m_SpriteVertexes.size(); ++i)
	{
		SafeDelete( m_SpriteVertexes[i] );
	}
}
		
//Initialise
bool 
ASCSpriteManager::Initialise()
{
	m_pRenderer = &Ascension::Renderer();
	ASCShaderManager* pShaderManager = m_pRenderer->GetShaderManager();
	if(pShaderManager)
	{
		m_pSpriteShader = pShaderManager->LoadShader("Shaders/SpriteShaders"); 
		return true;
	}
	assert_now("Guts, No Shader Manager");
	return false;
}

//Create
ASCSprite* 
ASCSpriteManager::CreateSprite(ASCString strSpriteName, ASCString strTechnique)
{
	ASCSprite* pSprite = new ASCSprite();
	if(pSprite->Init(strSpriteName, m_pSpriteShader, strTechnique))
	{
		AddSprite(pSprite);
		return pSprite;
	}
	SafeDelete( pSprite );
	return NULL;
}
ASCSprite* 
ASCSpriteManager::CreateSprite(ASCTexture* pTexture, ASCString strTechnique, bool bVideoSprite)
{
	ASCSprite* pSprite = new ASCSprite();
	pSprite->Init(pTexture, m_pSpriteShader, strTechnique, bVideoSprite);
	AddSprite(pSprite);
	return pSprite;
}
ASCSprite*
ASCSpriteManager::CreateSprite(FLOAT32 fWidth, FLOAT32 fHeight, UINT32 uTextureWidth, UINT32 uTextureHeight, ASCString strName, ASCString strTechnique)
{
	ASCTexture* pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(uTextureWidth, uTextureHeight, strName);
	UINT32* pTexels = NULL;
	UINT32 uPitch;
	if(pTexture->LockTexels(pTexels, uPitch, false))
	{
		CHAR8* pColour = reinterpret_cast<CHAR8*>( pTexels );
		UINT32 uFull = 255;
		for ( UINT32 y=0; y<uTextureHeight; ++y )
		{
			for ( UINT32 x=0; x<uTextureWidth; ++x )
			{
				pColour[0] = SC_CHAR(uFull);
				pColour[1] = SC_CHAR(uFull);
				pColour[2] = SC_CHAR(uFull);
				pColour[3] = 0;
				pColour += 4;
			}
		}
		pTexture->UnlockTexels();
	}
	ASCSprite* pSprite = new ASCSprite();
	pSprite->Init(pTexture, m_pSpriteShader, strTechnique, false);
	pSprite->SetSize( fWidth, fHeight );
	AddSprite(pSprite);
	return pSprite;
}
ASCSprite* 
ASCSpriteManager::CreateSpriteFile(ASCString strSpriteName, ASCString strTechnique)
{
	ASCSprite* pSprite = new ASCSprite();
	if(pSprite->InitFile(strSpriteName, m_pSpriteShader, strTechnique))
	{
		AddSprite(pSprite);
		return pSprite;
	}
	SafeDelete( pSprite );
	return NULL;
}
ASCAnimSprite* 
ASCSpriteManager::CreateAnimSprite(ASCString strSpriteName, ASCString strTechnique)
{
	static UINT32 uEventID = 0;
	ASCAnimSprite* pSprite = new ASCAnimSprite();
	if(pSprite->Init(strSpriteName, m_pSpriteShader, strTechnique, uEventID))
	{
		++uEventID;
		AddSprite(pSprite);
		return pSprite;
	}
	SafeDelete( pSprite );
	return NULL;
}
void 
ASCSpriteManager::AddSprite(ASCSprite* pSprite)
{
	if(m_uEmptySlots > 0)
	{
		for(UINT32 i = 0; i < m_SpriteList.size(); ++i)
		{
			if(NULL == m_SpriteList[i])
			{
				m_SpriteList[i] = pSprite;
				--m_uEmptySlots;
				return;
			}
		}
		m_uEmptySlots = 0;
		m_SpriteList.push_back(pSprite);
	}
	else
	{
		m_SpriteList.push_back(pSprite);
	}
}
void 
ASCSpriteManager::RemoveSprite(ASCSprite* pSprite)
{
	for(UINT32 i = 0; i < m_SpriteList.size(); ++i)
	{
		if(pSprite == m_SpriteList[i])
		{
			m_SpriteList[i] = NULL;
			++m_uEmptySlots;
			SafeDelete( pSprite );
			break;
		}
	}
}

//General
ASCSpriteVertex* 
ASCSpriteManager::GetSpriteVertex(ASCTexture* pTexture, ASCString& strTechnique, bool bVideoSprite)
{
	if( bVideoSprite == false )
	{
		for(UINT32 i = 0; i < m_SpriteVertexes.size(); ++i)
		{
			if(m_SpriteVertexes[i]->IsCompatable(pTexture, strTechnique))
			{
				return m_SpriteVertexes[i];
			}
		}
	}
	ASCSpriteVertex* pVertex = new ASCSpriteVertex();
	pVertex->Init(strTechnique, pTexture, m_pSpriteShader);
	m_SpriteVertexes.push_back(pVertex);
	m_SpriteVertexesRenderOrder.push_back(pVertex);
	return pVertex;
}
ASCSpriteVertex*  
ASCSpriteManager::GetSpriteVertex(ASCTextureAtlas* pTextureAtlas, ASCString& strTechnique, bool bVideoSprite)
{
	if( bVideoSprite == false )
	{
		for(UINT32 i = 0; i < m_SpriteVertexes.size(); ++i)
		{
			if(m_SpriteVertexes[i]->IsCompatable(pTextureAtlas, strTechnique))
			{
				return m_SpriteVertexes[i];
			}
		}
	}
	ASCSpriteVertex* pVertex = new ASCSpriteVertex();
	pVertex->Init(strTechnique, pTextureAtlas, m_pSpriteShader);
	m_SpriteVertexes.push_back(pVertex);
	m_SpriteVertexesRenderOrder.push_back(pVertex);
	return pVertex;
}
ASCVector<ASCSpriteVertex*>& 
ASCSpriteManager::SpriteVerticies()
{
	return m_SpriteVertexes;
}
ASCVector<ASCSpriteVertex*>& 
ASCSpriteManager::SpriteVerticiesRenderOrderPre()
{
	return m_SpriteVertexesRenderOrderPre;
}
ASCVector<ASCSpriteVertex*>& 
ASCSpriteManager::SpriteVerticiesRenderOrder()
{
	return m_SpriteVertexesRenderOrder;
}
ASCVector<ASCSpriteVertex*>& 
ASCSpriteManager::SpriteVerticiesRenderOrderPost()
{
	return m_SpriteVertexesRenderOrderPost;
}
ASCVector<ASCSpriteVertex*>& 
ASCSpriteManager::SpriteVerticiesPostFontRenderOrder()
{
	return m_SpriteVertexesPostFontRenderOrder;
}
void 
ASCSpriteManager::Render()
{
	static ASCVector<ASCSprite*> m_GUISprites;
	m_GUISprites.Clear();

	for(UINT32 i = 0; i < m_SpriteList.size(); ++i)
	{
		if(m_SpriteList[i])
		{
			m_SpriteList[i]->Render();
		}
	}
	
	switch(m_OrderMode)
	{
	case ESROM_FORWARDS:
		{
			for(UINT32 i = 0; i < m_SpriteVertexesRenderOrder.size(); ++i)
			{
				m_SpriteVertexesRenderOrder[i]->Render();
			}
			break;
		}
	case ESROM_BACKWARDS:
		{
			for(SINT32 i = m_SpriteVertexesRenderOrder.size()-1; i >= 0; --i)
			{
				m_SpriteVertexesRenderOrder[i]->Render();
			}
			break;
		}
	case ESROM_PRE_POST_FORWARD:
		{
			for(UINT32 i = 0; i < m_SpriteVertexesRenderOrderPre.size(); ++i)
			{
				m_SpriteVertexesRenderOrderPre[i]->Render();
			}
			for(UINT32 i = 0; i < m_SpriteVertexesRenderOrder.size(); ++i)
			{
				m_SpriteVertexesRenderOrder[i]->Render();
			}
			for(UINT32 i = 0; i < m_SpriteVertexesRenderOrderPost.size(); ++i)
			{
				m_SpriteVertexesRenderOrderPost[i]->Render();
			}
			break;
		}
	case ESROM_PRE_POST_BACKWARD:
		{
			for(SINT32 i = m_SpriteVertexesRenderOrderPre.size()-1; i >= 0; --i)
			{
				m_SpriteVertexesRenderOrderPre[i]->Render();
			}
			for(SINT32 i = m_SpriteVertexesRenderOrder.size()-1; i >= 0; --i)
			{
				m_SpriteVertexesRenderOrder[i]->Render();
			}
			for(SINT32 i = m_SpriteVertexesRenderOrderPost.size()-1; i >= 0; --i)
			{
				m_SpriteVertexesRenderOrderPost[i]->Render();
			}
			break;
		}
	default:
		{
			assert_now("Unsupported sprite order mode");
			break;
		}
	}
}
void 
ASCSpriteManager::PostFontRender()
{
	switch(m_OrderMode)
	{
	case ESROM_PRE_POST_FORWARD:
	case ESROM_FORWARDS:
		{
			for(UINT32 i = 0; i < m_SpriteVertexesPostFontRenderOrder.size(); ++i)
			{
				m_SpriteVertexesPostFontRenderOrder[i]->Render();
			}
			break;
		}
	case ESROM_PRE_POST_BACKWARD:
	case ESROM_BACKWARDS:
		{
			for(SINT32 i = m_SpriteVertexesPostFontRenderOrder.size()-1; i >= 0; --i)
			{
				m_SpriteVertexesPostFontRenderOrder[i]->Render();
			}
			break;
		}
	default:
		{
			assert_now("Unsupported sprite order mode");
			break;
		}
	}
}
void 
ASCSpriteManager::Update()
{
	for(UINT32 i = 0; i < m_SpriteList.size(); ++i)
	{
		if(m_SpriteList[i])
		{
			m_SpriteList[i]->Update();
		}
	}
}
ASCShader* 
ASCSpriteManager::GetSpriteShader()
{
	return m_pSpriteShader;
}
void 
ASCSpriteManager::SetRenderOrderMode(ESpriteRenderOrderMode eMode)
{
	m_OrderMode = eMode;
}