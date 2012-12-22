#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2Texture.h"
#include "ASCMemoryManagement.h"

#include "STB_Image/stb_image.h"

#include "Ascension.h"

#ifdef ASC_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/EAGL.h>
#else
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#endif


ASCGLES2Texture::ASCGLES2Texture( ASCString& strFileName, bool bLoad)
: m_uiWidth ( 0 )
, m_uiHeight ( 0 )
, m_uTextureID ( 0 )
, m_uAssignedSampler ( 0 )
, m_strTextureName ( strFileName )
, m_bDepthTexture ( false )
, m_bLoaded ( false )
, m_bAutoReload ( true )
, m_bReadOnlyTexels ( false )
, m_pTempData ( NULL )
{
	ASCString strTemp;
	m_strTextureName.poststrc('.', strTemp);
	m_strTextureName = strTemp;
	m_strTextureName += ASCString(".png");
    
#ifdef ASC_IOS
    m_strTextureName = ASCGeneralFunctions::ConvertFilePathForIOS(m_strTextureName);
#endif
    static ASCString s_strNULL = "NULL";
	if(m_strTextureName != s_strNULL && bLoad)
	{
		Load();
	}
}
ASCGLES2Texture::ASCGLES2Texture( UINT32 uWidth, UINT32 uHeight, ASCString& strFileName )
: m_uiWidth ( uWidth )
, m_uiHeight ( uHeight )
, m_uTextureID ( 0 )
, m_uAssignedSampler ( 0 )
, m_strTextureName ( strFileName )
, m_bDepthTexture ( false )
, m_bLoaded ( false )
, m_bAutoReload ( true )
, m_bReadOnlyTexels ( false )
, m_pTempData ( NULL )
{
	glGenTextures(1, &m_uTextureID);
	
	glBindTexture( GL_TEXTURE_2D, m_uTextureID );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	glBindTexture( GL_TEXTURE_2D, 0 );

	m_bLoaded = true;
}
ASCGLES2Texture::ASCGLES2Texture( bool bDepthTexture )
: m_uiWidth ( 0 )
, m_uiHeight ( 0 )
, m_uTextureID ( 0 )
, m_uAssignedSampler ( 0 )
, m_strTextureName ( "" )
, m_bDepthTexture ( false )
, m_bLoaded ( false )
, m_bAutoReload ( true )
, m_bReadOnlyTexels ( false )
, m_pTempData ( NULL )
{
	bDepthTexture;
	assert_now("Where is this comming from");
}


ASCGLES2Texture::~ASCGLES2Texture(void)
{
	SafeDelete( m_pTempData );
	glDeleteTextures(1, &m_uTextureID);
	m_bLoaded = false;
}
		
//Useage
bool 
ASCGLES2Texture::Apply( UINT32 uTextureStage )
{
	static const UINT32 sk_uTargets[] = 
	{
		GL_TEXTURE0,	GL_TEXTURE1,	GL_TEXTURE2,	GL_TEXTURE3,	GL_TEXTURE4,	GL_TEXTURE5,	GL_TEXTURE6,	GL_TEXTURE7,
		GL_TEXTURE8,	GL_TEXTURE9,	GL_TEXTURE10,	GL_TEXTURE11,	GL_TEXTURE12,	GL_TEXTURE13,	GL_TEXTURE14,	GL_TEXTURE15,
		GL_TEXTURE16,	GL_TEXTURE17,	GL_TEXTURE18,	GL_TEXTURE19,	GL_TEXTURE20,	GL_TEXTURE21,	GL_TEXTURE22,	GL_TEXTURE23,
		GL_TEXTURE24,	GL_TEXTURE25,	GL_TEXTURE26,	GL_TEXTURE27,	GL_TEXTURE28,	GL_TEXTURE29,	GL_TEXTURE30,	GL_TEXTURE31,
	};
	m_uAssignedSampler = uTextureStage;
	glActiveTexture(sk_uTargets[m_uAssignedSampler]);
	glBindTexture(GL_TEXTURE_2D, m_uTextureID);
	return true;
}
bool 
ASCGLES2Texture::LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly, const SIRect* pRegion )
{
	if(NULL != pRegion)
	{
		if(NULL == m_pTempData)
		{
			m_bReadOnlyTexels = bReadOnly;
			uPitch = m_uiWidth * sizeof(UINT32);
			m_pTempData = new UINT32[m_uiWidth * m_uiHeight];
			pTexels = m_pTempData;
			return true;
		}
		assert_now("Guts, Texture is already locked");
		return false;
	}
	assert_now("Guts, Specific region not supported in GLES2 renderer");

	return false;
}
bool 
ASCGLES2Texture::UnlockTexels( void )
{
	if(m_pTempData)
	{
		if(false == m_bReadOnlyTexels)
		{
			glBindTexture( GL_TEXTURE_2D, m_uTextureID );

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_uiWidth, m_uiHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pTempData);
	
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			glBindTexture( GL_TEXTURE_2D, 0 );
		}

		SafeDelete( m_pTempData );
		m_pTempData = NULL;
		return true;
	}
	assert_now("Guts, Texture has nt been locked");
	return false;
}
bool 
ASCGLES2Texture::Target( UINT32 uIndex )
{
	uIndex;
	assert_now("Guts, No idead how to do this in GLES2");
	return false;
}

//Info
UINT32 
ASCGLES2Texture::GetWidth( void ) const
{
	return m_uiWidth;
}
UINT32 
ASCGLES2Texture::GetHeight( void ) const
{
	return m_uiHeight;
}
ASCString 
ASCGLES2Texture::Name()
{
	return m_strTextureName;
}
bool 
ASCGLES2Texture::Loaded()
{
    return m_bLoaded;
}
ASCString 
ASCGLES2Texture::NameNoExt()
{
	ASCString strReturn = m_strTextureName;
	ASCString strTemp;
	while(strReturn.findlast('/') >= 0)
	{
        //Ascension::Debugging().OutputWindowLine(strReturn.c_str());
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
ASCGLES2Texture::Save(ASCString strName)
{
	assert_now("Guts, Saving to file not supported in GLES2 renderer");
	strName;
	return false;
}
void 
ASCGLES2Texture::Reload()
{
	assert_now("Guts, Not implemented");
}
void 
ASCGLES2Texture::Load()
{
	#ifdef __OBJC__
		stbi_convert_iphone_png_to_rgb(1);
	#endif

	//Open the texture file
	FILE* pFile = NULL; 
#ifdef ASC_IOS
	pFile = fopen(m_strTextureName.c_str(), "rb");
#else
	fopen_s(&pFile, m_strTextureName.c_str(), "rb");
#endif
	if(NULL == pFile)
	{
		assert_nowex("Guts, failed to load texture file: ", m_strTextureName);
		m_bLoaded = false;
		return;
	}
	
	UINT32 uForceChannels = 0;
	SINT32 iChannels;
	SINT32 iWidth;
	SINT32 iHeight;

	//Load in the texture data
	UINT8* pImg = stbi_load_from_file(pFile, &iWidth, &iHeight, &iChannels, uForceChannels);

	//Close the texture file
	fclose( pFile );

	if(NULL == pImg)
	{
		assert_nowex("Guts, failed to load texture because: ", stbi_failure_reason());
		m_bLoaded = false;
		return;
	}

	if( ( uForceChannels >= 1 ) && ( uForceChannels <= 4 ) )
	{
		iChannels = SC_SINT(uForceChannels);
	}
	
	m_uiWidth = iWidth;
	m_uiHeight = iHeight;

	//Generate the texture
	glGenTextures(1, &m_uTextureID);

	//Bind the texture
	glBindTexture( GL_TEXTURE_2D, m_uTextureID );

	UINT32 uTextureFormat = ( iChannels == 3 ) ? GL_RGB : GL_RGBA;

	if(uTextureFormat != GL_RGBA)
	{
		//assert_now("Guts, Texture is not RGBA, dont realy want this");
	}

	glTexImage2D( GL_TEXTURE_2D, 0, uTextureFormat, m_uiWidth, m_uiHeight, 0, uTextureFormat, GL_UNSIGNED_BYTE, pImg);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	glBindTexture( GL_TEXTURE_2D, 0 );

	ASCMemoryManagement::ReleaseMemory(pImg, false);
	m_bLoaded = true;
}
void 
ASCGLES2Texture::UnLoad()
{
	if(m_bLoaded)
	{
		glDeleteTextures(1, &m_uTextureID);
		m_bLoaded = false;
	}
}

void 
ASCGLES2Texture::AutoReload(bool bVal)
{
	m_bAutoReload = bVal;
}

#endif