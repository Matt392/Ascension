#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9Texture.h"
#include "ASCDX9TextureManager.h"
#include "ASCTextureAtlas.h"

ASCDX9TextureManager::ASCDX9TextureManager( IDirect3DDevice9* pDevice )
: m_pDevice ( pDevice )
, m_pNullTexture ( NULL )
, m_pDefaultRT ( NULL )
{
	IDirect3DSurface9* pBackbuffer;

	m_pDevice->GetRenderTarget( 0, &pBackbuffer );
	
	//Set render target and renderer, and return.
	m_pDefaultRT =  new ASCDX9Texture( m_pDevice, pBackbuffer, false );
}


ASCDX9TextureManager::~ASCDX9TextureManager(void)
{
	for(UINT32 i = 0; i < m_TextureVector.size(); ++i)
	{
		SafeDelete(m_TextureVector[i]);
	}
	for(UINT32 i = 0; i < m_TextureAtlasVector.size(); ++i)
	{
		SafeDelete(m_TextureAtlasVector[i]);
	}
	
	SafeDelete( m_pDefaultRT );
	m_pDevice = NULL;
}

//Loading
void 
ASCDX9TextureManager::Reload()
{
	for(UINT32 i = 0; i < m_TextureVector.size(); ++i)
	{
		m_TextureVector[i]->Reload();
	}
}
		
//Creation
ASCTexture* 
ASCDX9TextureManager::CreateTexture( ASCString& strFileName, bool bLoad )
{
	for(UINT32 i = 0; i < m_TextureVector.size(); ++i)
	{
		if(m_TextureVector[i]->Name() == strFileName)
		{
			return m_TextureVector[i];
		}
	}
	//Create the new texture.
	ASCDX9Texture* pResource = new ASCDX9Texture( m_pDevice, strFileName, bLoad );

	m_TextureVector.push_back( pResource );
	if(false == pResource->m_bLoaded && true == bLoad)
	{
		return NULL;
	}
	return pResource;
}
ASCTexture* 
ASCDX9TextureManager::CreateRenderTarget( ESurfaceFormat eFormat, UINT32 uWidth, UINT32 uHeight, bool bWriteOnly )
{
	IDirect3DTexture9* pTexture;

	static D3DFORMAT FmtLookup[] =
	{	
		D3DFMT_A8R8G8B8,		//SF_Colour_A8R8G8B8
		D3DFMT_D24X8,			//SF_Depth_u24
	};	

	const D3DFORMAT Fmt = FmtLookup[eFormat];

	if ( SF_Depth_u24 == eFormat && bWriteOnly )
	{
		IDirect3DSurface9* pSurface;
		if ( SUCCEEDED( m_pDevice->CreateDepthStencilSurface( uWidth, uHeight, Fmt, D3DMULTISAMPLE_NONE, 
															  0, TRUE, &pSurface, NULL ) ) )
		{
			return new ASCDX9Texture( m_pDevice, pSurface, true );
		}

		return NULL;
	}

	if (SUCCEEDED( m_pDevice->CreateTexture( uWidth, uHeight, 1, D3DUSAGE_RENDERTARGET, Fmt,
											 D3DPOOL_DEFAULT, &pTexture, NULL ) ) )
	{
		ASCString strID = TEXTURE_ID_RENDER_TARGET;
		return new ASCDX9Texture( m_pDevice, pTexture, strID );
	}

	return NULL;
}
ASCTexture* 
ASCDX9TextureManager::CreateTexture( UINT32 uWidth, UINT32 uHeight, ASCString& strName)
{
	IDirect3DTexture9* pTexture;
	
	//if (D3D_OK == m_pDevice->CreateTexture( uWidth, uHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture, NULL ) ) 
	if(D3D_OK == D3DXCreateTexture(m_pDevice, uWidth, uHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture))
	{
		if(strName.length() <= 0)
		{
			static UINT32 suCTTCount = 0;
			strName = "CreatedTexture";
			++suCTTCount;
			strName += suCTTCount;
		}
		ASCTexture* pResource = new ASCDX9Texture( m_pDevice, pTexture, strName );
		m_TextureVector.push_back( pResource );
		return pResource;
	}
	return NULL;
}

//Gets
ASCTexture* 
ASCDX9TextureManager::GetNULLTexture()
{
	if ( NULL == m_pNullTexture )
	{
		//Create the null texture.
		ASCString strID = TEXTURE_NULL;
		m_pNullTexture = new ASCDX9Texture( m_pDevice, strID, true ); 	
	}
	return m_pNullTexture;
}
ASCTexture* 
ASCDX9TextureManager::GetDefaultRT( void )
{
	return m_pDefaultRT;
}
ASCTexture* 
ASCDX9TextureManager::GetDefaultDT( void )
{
	return NULL;
}

//Texture Atlases
ASCTextureAtlas* 
ASCDX9TextureManager::LoadTextureAtlas(ASCString& strAtlasName)
{
	for(UINT32 i = 0; i < m_TextureAtlasVector.size(); ++i)
	{
		if(m_TextureAtlasVector[i]->GetName() == strAtlasName)
		{
			return m_TextureAtlasVector[i];
		}
	}
	ASCTextureAtlas* pTextureAtlas = new ASCTextureAtlas();
	if(pTextureAtlas->Load(strAtlasName))
	{
		m_TextureAtlasVector.push_back(pTextureAtlas);
		return pTextureAtlas;
	}
	else
	{
		SafeDelete( pTextureAtlas );
	}
	return NULL;
}
bool 
ASCDX9TextureManager::IsInTextureAtlas(ASCString& strID, ASCTextureAtlas* &pAtlas)
{
	for(UINT32 i = 0; i < m_TextureAtlasVector.size(); ++i)
	{
		if(m_TextureAtlasVector[i]->IsContained(strID))
		{
			pAtlas = m_TextureAtlasVector[i];
			return true;
		} 
	}
	pAtlas = NULL;
	return false;
}
UINT32 
ASCDX9TextureManager::MemoryUsed()
{
	UINT32 uMemoryUsed = 0;
	for(UINT32 i = 0; i < m_TextureVector.size(); ++i)
	{
		uMemoryUsed += m_TextureVector[i]->MemSize();
	}
	return uMemoryUsed;
}
#endif