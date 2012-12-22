#include "ASCGenInc.h"
#include "ASCTextureAtlas.h"
#include "ASCINIFile.h"
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif


ASCTextureAtlas::ASCTextureAtlas(void)
{
	for(UINT32 i = 0; i < ASC_MAX_TEXTURES_IN_ATLAS; ++i)
	{
		m_pTexture[i] = NULL;
	}
}


ASCTextureAtlas::~ASCTextureAtlas(void)
{
}

//Loading
bool 
ASCTextureAtlas::Load(ASCString strFileName)
{
	m_strFileName = strFileName;
	ASCINIFile INIFile;
	if(INIFile.Load(strFileName))
	{
		//Load textures
		UINT32 uTextureFileCount = INIFile.GetUInt("General", "TextureCount");
		ASCString strTextureFile;
		ASCString strID;
		for(UINT32 i = 0; i < uTextureFileCount; ++i)
		{
			strID = "Texture_";
			strID.AddSmallInt(i); 
			m_strFileName.poststrc('/', strTextureFile, 1);
			strTextureFile += INIFile.GetString( "General", strID);
			m_pTexture[i] = Ascension::Renderer().GetTextureManager()->CreateTexture(strTextureFile, true);
		}
		
		//Get textrue count
		UINT32 uTextureCount = INIFile.GetUInt( "General", "Count");
		uTextureCount;

		ASCString strTextName;
		for(UINT32 i = 0; i < uTextureCount; ++i)
		{
			strTextName = "Texture_";
			strTextName += i;
			LoadTexture(strTextName, INIFile);
		}

		return true;
	}
	return false;
}
bool 
ASCTextureAtlas::LoadTexture(ASCString& strTexture, ASCINIFile& rINIFile)
{
	//Add the texture to the list
	ASCString strName = rINIFile.GetString( strTexture, "Name");
	m_strContainedTextures.push_back(strName);
	
	//Calculate the textures variables
	SAtlasInfo sInfo;
	sInfo.m_uTextureID = rINIFile.GetUInt( strTexture, "Texture" );
	
	FLOAT32 fWidth = SC_FLOAT(m_pTexture[sInfo.m_uTextureID]->GetWidth());
	FLOAT32 fHeight = SC_FLOAT(m_pTexture[sInfo.m_uTextureID]->GetHeight());

	sInfo.m_fUL = rINIFile.GetFloat( strTexture, "TL") / fWidth;
	sInfo.m_fUR = rINIFile.GetFloat( strTexture, "TR") / fWidth;
	sInfo.m_fVL = rINIFile.GetFloat( strTexture, "BL") / fHeight;
	sInfo.m_fVR = rINIFile.GetFloat( strTexture, "BR") / fHeight;
	
	sInfo.m_fWidth = fWidth * (sInfo.m_fUR - sInfo.m_fUL);
	sInfo.m_fHeight = fHeight * (sInfo.m_fVR - sInfo.m_fVL);
	
	m_uTextureUVs.push_back(sInfo);

	return true;
}

//Use
SAtlasInfo 
ASCTextureAtlas::GetAtlasUVs(ASCString strID)
{
	for(UINT32 i = 0; i < m_strContainedTextures.size(); ++i)
	{
		if(m_strContainedTextures[i] == strID)
		{
			return m_uTextureUVs[i];
		}
	}
	SAtlasInfo sNone;
	return sNone;
}
ASCTexture* 
ASCTextureAtlas::GetTexture()
{
	assert_now("Guts, not sure what is using this");
	return m_pTexture[0];
}
ASCTexture* 
ASCTextureAtlas::GetSubTexture(ASCString strName)
{
	for(UINT32 i = 0; i < m_strContainedTextures.size(); ++i)
	{
		if(m_strContainedTextures[i] == strName)
		{
			SAtlasInfo sRect = GetAtlasUVs(strName);
			ASCString strNewName = m_pTexture[m_uTextureUVs[i].m_uTextureID]->Name();
			strNewName += '/';
			strNewName += strName;
			ASCTexture* pNewTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(SC_UINT(sRect.m_fWidth), SC_UINT(sRect.m_fHeight), strNewName);
			if(pNewTexture)
			{
				const UINT32 uSrcWidth = m_pTexture[m_uTextureUVs[i].m_uTextureID]->GetWidth();
				const UINT32 uSrcHeight = m_pTexture[m_uTextureUVs[i].m_uTextureID]->GetHeight();
				
				const UINT32 uWidth = pNewTexture->GetWidth();
				const UINT32 uHeight = pNewTexture->GetHeight();
						
				UINT32* pTexels = NULL;
				UINT32 uPitch = 0;

				UINT32* pSrcTexels = NULL;
				UINT32 uSrcPitch = 0;
				
				UINT32 uOffsetX = SC_UINT(sRect.m_fUL * SC_FLOAT(uSrcWidth));
				UINT32 uOffsetY = SC_UINT(sRect.m_fVL * SC_FLOAT(uSrcHeight));

				if((SC_UINT(sRect.m_fUR * SC_FLOAT(uSrcWidth)) > uSrcWidth) || (SC_UINT(sRect.m_fVR * SC_FLOAT(uSrcHeight)) > uSrcHeight))
				{
					assert_now("Bad texture size");
					return NULL;
				}

				UINT32 uWidthPitch = uWidth;
				if(m_pTexture[m_uTextureUVs[i].m_uTextureID]->LockTexels(pSrcTexels, uSrcPitch, false))
				{
					if(pNewTexture->LockTexels(pTexels, uPitch, false))
					{
						if(uPitch != uWidth)
						{
							uWidthPitch = uPitch;
						}
						for ( UINT32 y = 0; y < uHeight; y++ )
						{
							for ( UINT32 x = 0; x < uWidth; x++ )
							{
								pTexels[(uWidthPitch*y)+x] = pSrcTexels[(uSrcWidth * (y+uOffsetY))+x+uOffsetX];
							}
						}
						pNewTexture->UnlockTexels();
					}
					m_pTexture[m_uTextureUVs[i].m_uTextureID]->UnlockTexels();
				}
				return pNewTexture;
			}
		}
	}
	return NULL;
}
bool 
ASCTextureAtlas::IsContained(ASCString strID)
{
	for(UINT32 i = 0; i < m_strContainedTextures.size(); ++i)
	{
		if(m_strContainedTextures[i] == strID)
		{
			return true;
		}
	}
	return false;
}
ASCVector<ASCString>* 
ASCTextureAtlas::GetContainedTextureStrs()
{
	return &m_strContainedTextures;
}
ASCString 
ASCTextureAtlas::GetName()
{
	return m_strFileName;
}
void 
ASCTextureAtlas::ApplyTextures()
{
	for(UINT32 i = 0; i < ASC_MAX_TEXTURES_IN_ATLAS; ++i)
	{
		if(NULL != m_pTexture[i])
		{
			m_pTexture[i]->Apply(i);
		}
	}
}