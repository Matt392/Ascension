/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCTextureAtlas.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCTEXTUREATLAS_H__Included_1298178325
#define _ASCTEXTUREATLAS_H__Included_1298178325


namespace ASC
{
#define ASC_MAX_TEXTURES_IN_ATLAS 8
	struct SAtlasInfo
	{
		FLOAT32 m_fUL;
		FLOAT32 m_fUR;
		FLOAT32 m_fVL;
		FLOAT32 m_fVR;
		FLOAT32 m_fHeight;
		FLOAT32 m_fWidth;
		UINT32 m_uTextureID;
		SAtlasInfo()
		{
			m_uTextureID = 0;
			m_fUL = 0.0f;
			m_fUR = 1.0f;
			m_fVL = 0.0f;
			m_fVR = 1.0f;
			m_fHeight = 0.0f;
			m_fWidth = 0.0f;
		}
	};

	class ASCTexture;
	class ASCINIFile;

	class ASCTextureAtlas
	{
	public:
		ASCTextureAtlas(void);
		~ASCTextureAtlas(void);

		//Loading
		bool Load(ASCString strFileName);
		bool LoadTexture(ASCString& strTexture, ASCINIFile& rINIFile);

		//Use
		SAtlasInfo GetAtlasUVs(ASCString strID);
		ASCTexture* GetTexture();
		ASCTexture* GetSubTexture(ASCString strName);
		bool IsContained(ASCString strID);
		ASCVector<ASCString>* GetContainedTextureStrs();

		ASCString GetName();

		void ApplyTextures();

	protected:
		ASCTexture* m_pTexture[ASC_MAX_TEXTURES_IN_ATLAS];
		ASCVector<ASCString> m_strContainedTextures;
		ASCVector<SAtlasInfo> m_uTextureUVs;

		ASCString m_strFileName;
	};

}

#endif //_ASCTEXTUREATLAS_H__Included_1298178325

