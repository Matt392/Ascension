#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9Texture.h"



ASCDX9Texture::ASCDX9Texture( IDirect3DDevice9* pDevice, ASCString& strFileName, bool bLoad )
: m_pTexture ( NULL )
, m_pSurface ( NULL )
, m_pDevice ( pDevice )
, m_uiWidth ( 0 )
, m_uiHeight ( 0 )
, m_strTextureName ( strFileName )
, m_bDepthTexture ( false )
, m_bLoaded ( false )
, m_bAutoReload ( true )
{
	if(m_strTextureName != "NULL" && bLoad)
	{
		Load();
	}
}
ASCDX9Texture::ASCDX9Texture( IDirect3DDevice9* pDevice, LPDIRECT3DTEXTURE9 pTexture, ASCString& strFileName )
: m_pTexture ( pTexture )
, m_pSurface ( NULL )
, m_pDevice ( pDevice )
, m_uiWidth ( 0 )
, m_uiHeight ( 0 )
, m_strTextureName ( strFileName )
, m_bDepthTexture ( false )
, m_bLoaded ( false )
, m_bAutoReload ( true )
{
	if ( m_pTexture )
	{
		D3DSURFACE_DESC Desc;

		m_pTexture->GetLevelDesc( 0, &Desc );
		m_pTexture->GetSurfaceLevel( 0, &m_pSurface ); 

		//Set with and height from surface desc
		m_uiWidth = Desc.Width;
		m_uiHeight = Desc.Height;
	}
}
ASCDX9Texture::ASCDX9Texture( IDirect3DDevice9* pDevice, IDirect3DSurface9* pSurface, bool bDepthTexture )
: m_pTexture ( NULL )
, m_pSurface ( pSurface )
, m_pDevice ( pDevice )
, m_uiWidth ( 0 )
, m_uiHeight ( 0 )
, m_strTextureName ( TEXTURE_NONE )
, m_bDepthTexture ( bDepthTexture )
, m_bLoaded ( false )
, m_bAutoReload ( true )
{
	if(bDepthTexture)
	{
		m_strTextureName = TEXTURE_ID_DEPTH_TARGET;
	}
	if ( m_pSurface )
	{
		D3DSURFACE_DESC Desc;
		m_pSurface->GetDesc( &Desc );
		
		//Set with and height from surface desc
		m_uiWidth = Desc.Width;
		m_uiHeight = Desc.Height;
	}
}


ASCDX9Texture::~ASCDX9Texture(void)
{
	UnLoad();

	m_pDevice = NULL;
}
		
//Useage
bool 
ASCDX9Texture::Apply( UINT32 uTextureStage )
{
	return m_pDevice->SetTexture( uTextureStage, m_pTexture ) == D3D_OK;
}
bool 
ASCDX9Texture::LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly, const SIRect* pRegion )
{
	RECT Region;
	ZeroMemory( &Region, sizeof( Region ) );

	if ( pRegion )
	{
		Region.left = pRegion->m_iX;
		Region.top = pRegion->m_iY;
		Region.right = pRegion->m_iX + pRegion->m_iWidth;
		Region.bottom = pRegion->m_iX + pRegion->m_iHeight;
	}
	else
	{
		// Can't use zero rect
		Region.left = 0;
		Region.top = 0;
		Region.right = 1;
		Region.bottom = 1;
	}

	D3DLOCKED_RECT sLockedRect;
	ZeroMemory( &sLockedRect, sizeof( sLockedRect ) );

	DWORD dwFlags = 0;

	if (bReadOnly)
	{
		dwFlags = D3DLOCK_READONLY;
	}

	if ( FAILED( m_pTexture->LockRect( 0, &sLockedRect, &Region, dwFlags ) ) )
	{
		return false;
	}
	
	uPitch = sLockedRect.Pitch/4;
	pTexels = (UINT32*)sLockedRect.pBits;

	return true;
}
bool 
ASCDX9Texture::UnlockTexels( void )
{
	return m_pTexture->UnlockRect( 0 ) == D3D_OK;
}
bool 
ASCDX9Texture::Target( UINT32 uIndex )
{
	if ( m_pSurface )
	{
		if ( m_bDepthTexture )
		{
			return m_pDevice->SetDepthStencilSurface( m_pSurface ) == D3D_OK;
		}
		else
		{
			return m_pDevice->SetRenderTarget( uIndex, m_pSurface ) == D3D_OK;
		}
	}
	return false;
}

//Info
UINT32 
ASCDX9Texture::GetWidth( void ) const
{
	return m_uiWidth;
}
UINT32 
ASCDX9Texture::GetHeight( void ) const
{
	return m_uiHeight;
}
ASCString 
ASCDX9Texture::Name()
{
	return m_strTextureName;
}
ASCString 
ASCDX9Texture::NameNoExt()
{
	ASCString strReturn = m_strTextureName;
	ASCString strTemp;
	while(strReturn.find('/') >= 0)
	{
		ASCString strTemp;
		strReturn.substr(strReturn.find('/')+1, strTemp);
		strReturn = strTemp;
	}
	if(strReturn.find('.') >= 0)
	{
		strReturn.split(strReturn.find('.'), strReturn, strTemp);
	}
	return strReturn;
}

//Management
bool 
ASCDX9Texture::Save(ASCString strName)
{
	ASCString strFileNoExt;
	strName.prestrc('.', strFileNoExt);
	strName = strFileNoExt;
	strName += ASCString(".png");
	if(D3D_OK != D3DXSaveTextureToFileA(strName.c_str(), D3DXIFF_PNG, m_pTexture, NULL))
	{
		assert_nowex("Guts could not save png version of ", strName);
	}
	//strName = strFileNoExt;
	//strName += ASCString(".dds");
	//if(D3D_OK != D3DXSaveTextureToFileA(strName.c_str(), D3DXIFF_DDS, m_pTexture, NULL))
	//{
	//	assert_nowex("Guts could not save dds version of ", strName);
	//}
	return true;
}
void 
ASCDX9Texture::Reload()
{
	if(m_bAutoReload && false == m_bLoaded)
	{
		if(m_strTextureName == TEXTURE_ID_RENDER_TARGET)
		{
			D3DSURFACE_DESC SurfaceDesc;
			m_pSurface->GetDesc(&SurfaceDesc);

		
			const D3DFORMAT Fmt = SurfaceDesc.Format;
			UINT32 uWidth = SurfaceDesc.Width;
			UINT32 uHeight = SurfaceDesc.Height;

			UnLoad();

			if (SUCCEEDED( m_pDevice->CreateTexture( uWidth, uHeight, 1, D3DUSAGE_RENDERTARGET, Fmt,
													 D3DPOOL_DEFAULT, &m_pTexture, NULL ) ) )
			{
				if ( m_pTexture )
				{
					D3DSURFACE_DESC Desc;

					m_pTexture->GetLevelDesc( 0, &Desc );
					m_pTexture->GetSurfaceLevel( 0, &m_pSurface ); 

					//Set with and height from surface desc
					m_uiWidth = Desc.Width;
					m_uiHeight = Desc.Height;
				}
			}
		}
		else if(m_strTextureName == TEXTURE_ID_DEPTH_TARGET)
		{
			D3DSURFACE_DESC SurfaceDesc;
			m_pSurface->GetDesc(&SurfaceDesc);

		
			const D3DFORMAT Fmt = SurfaceDesc.Format;
			UINT32 uWidth = SurfaceDesc.Width;
			UINT32 uHeight = SurfaceDesc.Height;

			UnLoad();

			if ( SUCCEEDED( m_pDevice->CreateDepthStencilSurface( uWidth, uHeight, Fmt, D3DMULTISAMPLE_NONE, 
																  0, TRUE, &m_pSurface, NULL ) ) )
			{
				if ( m_pSurface )
				{
					D3DSURFACE_DESC Desc;
					m_pSurface->GetDesc( &Desc );
				
					//Set with and height from surface desc
					m_uiWidth = Desc.Width;
					m_uiHeight = Desc.Height;
				}
			}
		}
		else if(m_strTextureName == TEXTURE_NULL)
		{
		}
		else if(m_strTextureName == TEXTURE_NONE)
		{
			assert_now("Guts, No texture ??????");
		}
		else
		{
			UnLoad();
			Load();
		}
	}
}
void 
ASCDX9Texture::Load()
{
	//Load the texture from file.
	if(false == m_bLoaded)
	{
		if(FAILED(D3DXCreateTextureFromFileExA(m_pDevice, m_strTextureName.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture)))
		{
			if ( FAILED( D3DXCreateTextureFromFileA(  m_pDevice, m_strTextureName.c_str(), &m_pTexture ) ) )
			{
				m_bLoaded = false;
			}
		}
		if ( m_pTexture )
		{
			D3DSURFACE_DESC Desc;

			m_pTexture->GetLevelDesc( 0, &Desc );
			m_pTexture->GetSurfaceLevel( 0, &m_pSurface ); 

			//Set with and height from surface desc
			m_uiWidth = Desc.Width;
			m_uiHeight = Desc.Height;

			m_bLoaded = true;
		}
	}
}
bool 
ASCDX9Texture::Loaded()
{
	return m_bLoaded;
}
void 
ASCDX9Texture::UnLoad()
{
	if ( m_pSurface )
	{
		m_pSurface->Release();
		m_pSurface = NULL;
	}

	if ( m_pTexture )
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	m_bLoaded = false;
}
void 
ASCDX9Texture::AutoReload(bool bVal)
{
	m_bAutoReload = bVal;
}
UINT32 
ASCDX9Texture::MemSize()
{
	if(m_bLoaded)
	{
		return (m_uiHeight * m_uiWidth * 4);
	}
	return 0;
}
#endif 