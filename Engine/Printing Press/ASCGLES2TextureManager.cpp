#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2TextureManager.h"
#include "ASCGLES2Texture.h"
#include "ASCTextureAtlas.h"


ASCGLES2TextureManager::ASCGLES2TextureManager(void)
: m_pNullTexture ( NULL )
, m_pDefaultRT ( NULL )
{
}


ASCGLES2TextureManager::~ASCGLES2TextureManager(void)
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
}

//Loading
void 
ASCGLES2TextureManager::Reload()
{
	assert_now("Guts, Should not be needed in GLES");
}
		
//Creation
ASCTexture* 
ASCGLES2TextureManager::CreateTexture( ASCString& strFileName, bool bLoad  )
{
    //ASCDebuggingManager::OutputWindowLine("Loading Texture: ", ASCDebugString(strFileName.c_str()));
	for(UINT32 i = 0; i < m_TextureVector.size(); ++i)
	{
		if(m_TextureVector[i]->Name() == strFileName)
		{
			return m_TextureVector[i];
		}
	}
	//Create the new texture.
	ASCGLES2Texture* pResource = new ASCGLES2Texture( strFileName, bLoad );

	m_TextureVector.push_back( pResource );
	//if(false == pResource->m_bLoaded && true == bLoad)
	//{ ,kml
	//	return NULL;
	//}
	return m_TextureVector[m_TextureVector.size()-1];
}
ASCTexture* 
ASCGLES2TextureManager::CreateRenderTarget( ESurfaceFormat eFormat, UINT32 uWidth, UINT32 uHeight, bool bWriteOnly )
{
	assert_now("Guts, Is this needed in GLES?");
	eFormat;
	uWidth;
	uHeight;
	bWriteOnly;
	return NULL;
}
ASCTexture* 
ASCGLES2TextureManager::CreateTexture( UINT32 uWidth, UINT32 uHeight, ASCString& strName)
{
	ASCTexture* pResource = new ASCGLES2Texture( uWidth, uHeight, strName );
	m_TextureVector.push_back( pResource );
	return pResource;
}

//Gets
ASCTexture* 
ASCGLES2TextureManager::GetNULLTexture()
{
	if ( NULL == m_pNullTexture )
	{
		//Create the null texture.
		ASCString strID = TEXTURE_NULL;
		m_pNullTexture = new ASCGLES2Texture( strID, TRUE ); 	
	}
	return m_pNullTexture;
}
ASCTexture* 
ASCGLES2TextureManager::GetDefaultRT( void )
{
	return m_pDefaultRT;
}
ASCTexture* 
ASCGLES2TextureManager::GetDefaultDT( void )
{
	assert_now("Guts, Not implemented");
	return NULL;
}
		
//Texture Atlases
ASCTextureAtlas* 
ASCGLES2TextureManager::LoadTextureAtlas(ASCString& strAtlasName)
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
ASCGLES2TextureManager::IsInTextureAtlas(ASCString& strID, ASCTextureAtlas* &pAtlas)
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

#endif