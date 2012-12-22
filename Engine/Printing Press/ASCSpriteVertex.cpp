#include "ASCGenInc.h"
#include "ASCShader.h"
#include "ASCSpriteVertex.h"
#include "ASCRenderer.h"
#include "ASCVertexDeclaration.h"
#include "ASCTexture.h"
#include "ASCTextureAtlas.h"
#include "ASCCamera.h"
#include "ASCVector2.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif
#include "ASCVertexBuffer.h"


ASCSpriteVertex::ASCSpriteVertex(void)
: m_TechniqueHandle ( NULL )
, m_CameraPosHandle ( NULL )
, m_pTexture ( NULL )
, m_pTextureAtlas ( NULL )
, m_pShader ( NULL )
, m_pVertexBuffer ( NULL )
, m_strTechnique ( "" )
, m_uAvaliableVetexCount ( 0 )
, m_bHardwareVerticies ( true )
//, m_bHardwareVerticies ( false )
{
}


ASCSpriteVertex::~ASCSpriteVertex(void)
{
}

//Initialise
bool 
ASCSpriteVertex::Init(ASCString& strTechnique, ASCTexture* pTexture, ASCShader* pShader)
{
	static UINT32 suCount = 1;
	m_strID = "Sprite Vericies ";
	m_strID += suCount;
	m_strTechnique = strTechnique;
	m_pTexture = pTexture;
	m_pShader = pShader;
	++suCount;

	m_TechniqueHandle = m_pShader->GetNamedHandle(m_strTechnique.c_str());
	m_CameraPosHandle = m_pShader->GetNamedHandle("g_mCameraPos");

	m_pVertexBuffer = Ascension::Renderer().CreateVertexBuffer(m_vecVertex.TotalSize(), Ascension::Renderer().GetDefaultVertDecl()->GetStride());

	return m_TechniqueHandle && m_pTexture && m_pShader;
}
bool 
ASCSpriteVertex::Init(ASCString& strTechnique, ASCTextureAtlas* pTextureAtlas, ASCShader* pShader)
{
	static UINT32 suCount = 1;
	m_strID = "Sprite Vericies ";
	m_strID += suCount;
	m_strTechnique = strTechnique;
	m_pTextureAtlas = pTextureAtlas;
	m_pShader = pShader;
	++suCount;

	m_TechniqueHandle = m_pShader->GetNamedHandle(m_strTechnique.c_str());
	m_CameraPosHandle = m_pShader->GetNamedHandle("g_mCameraPos");

	m_pVertexBuffer = Ascension::Renderer().CreateVertexBuffer(m_vecVertex.TotalSize(), Ascension::Renderer().GetDefaultVertDecl()->GetStride());

	return m_TechniqueHandle && m_pTextureAtlas && m_pShader;
}

//General
bool 
ASCSpriteVertex::IsCompatable(ASCTexture* pTexture, ASCString& strTechnique)
{
	return (pTexture == m_pTexture) && (strTechnique == m_strTechnique);
}
bool 
ASCSpriteVertex::IsCompatable(ASCTextureAtlas* pTextureAtlas, ASCString& strTechnique)
{
	return (pTextureAtlas == m_pTextureAtlas) && (strTechnique == m_strTechnique);
}
ASCString 
ASCSpriteVertex::TextureID()
{
	if(m_pTexture)
	{
		return m_pTexture->NameNoExt();
	}
	return "";
}
ASCString 
ASCSpriteVertex::TechniqeID()
{
	return m_strTechnique;
}
//Rendering
void 
ASCSpriteVertex::Render()
{
	OrderSpritesForDepth();

	m_pShader->SetTechnique(m_TechniqueHandle);

	ASCMatrix4 mProj = Ascension::Renderer().GetProjectionMatrix();

#ifdef ASC_RENDERER_DX9
	static ASC_Handle pProj = m_pShader->GetNamedHandle("g_mProjection");
	static ASC_Handle pView = m_pShader->GetNamedHandle("g_mView");
#else 
#ifdef ASC_RENDERER_GLES2
	static ASC_Handle pProj = m_pShader->GetNamedHandle("m_mProjection");
	static ASC_Handle pView = m_pShader->GetNamedHandle("m_mView");
#endif
#endif

	m_pShader->SetTechnique(m_TechniqueHandle);
	m_pShader->SetFVectors(pProj, &mProj, 4);
	ASCMatrix4 mView = Ascension::Renderer().GetViewMatrix();
	m_pShader->SetFVectors(pView, &mView, 4);

	Ascension::Renderer().GetDefaultVertDecl()->Apply();
	if(m_pTexture)
	{
		m_pTexture->Apply( 0 );
	}
	else if (m_pTextureAtlas)
	{
		m_pTextureAtlas->ApplyTextures();
	}

	if(m_vecVertex.size() > 0)
	{
		if(m_bHardwareVerticies)
		{
			Prepare();
			m_pVertexBuffer->SetSource();
		}
		m_pShader->Begin();

		if(m_bHardwareVerticies)
		{
			Ascension::Renderer().DrawPrimitives( PT_TriList, m_uCount/3);
			ASCString strInfo = m_strID;
			strInfo += ASCString(" Hardware Verticies Drawn: ");
			strInfo.AddSmallInt( m_vecVertex.size() );
			Ascension::Renderer().AddDrawDebugInfo(strInfo);
		}
		else
		{
			Ascension::Renderer().DrawPrimitives( PT_TriList, m_vecVertex.size()/3, m_vecVertex.raw(), sizeof(SVertex) );
			ASCString strInfo = m_strID;
			strInfo += ASCString(" Software Verticies Drawn: ");
			strInfo.AddSmallInt( m_vecVertex.size() );
			Ascension::Renderer().AddDrawDebugInfo(strInfo);
		}

		m_pShader->End();
		if(m_bHardwareVerticies)
		{
			//Prepare();
			m_pVertexBuffer->Unbind();
		}
	}
	m_vecVertex.Clear();
}
void 
ASCSpriteVertex::OrderSpritesForDepth()
{
	for(UINT32 i = 0; i < ASC_MAX_SPRITE_DEPTH; ++i)
	{
		for(UINT32 j = 0; j < m_vecVertexPreDepthSort[i].size(); ++j)
		{
			if(m_vecVertex.push_back(m_vecVertexPreDepthSort[i][j]))
			{
				m_pVertexBuffer->Resize( m_vecVertex.TotalSize() );
			}
		}
		m_vecVertexPreDepthSort[i].Clear();
	}
}
void 
ASCSpriteVertex::Prepare()
{
	SVertex* pData = reinterpret_cast<SVertex*>(m_pVertexBuffer->Lock());

	m_uCount = 0;
	for(UINT32 i = 0; i < m_vecVertex.size(); ++i)
	{
		pData[m_uCount] = m_vecVertex[i];
		++m_uCount;
	}

	m_pVertexBuffer->UnLock();
}
void 
ASCSpriteVertex::AddVertex(SVertex sVertex, UINT32 uDepth)
{
	if(uDepth < ASC_MAX_SPRITE_DEPTH)
	{
		m_vecVertexPreDepthSort[uDepth].push_back(sVertex);
	}
	else
	{
		assert_now("Guts, Unsupported sprite depth");
	}
}
void 
ASCSpriteVertex::SetTexture(ASCTexture* pTexture)
{
	m_pTexture = pTexture;
}