/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2TextureManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2TEXTUREMANAGER_H__Included_25981266
#define _ASCGLES2TEXTUREMANAGER_H__Included_25981266

#ifdef ASC_RENDERER_GLES2

#include "ASCTextureManager.h"

namespace ASC
{
	class ASCTexture;
	class ASCTextureAtlas;

	class ASCGLES2TextureManager : public ASCTextureManager
	{
	public:
		ASCGLES2TextureManager(void);
		~ASCGLES2TextureManager(void);

		//Loading
		void Reload();
		
		//Creation
		ASCTexture* CreateTexture( ASCString& strFileName, bool bLoad  );
		ASCTexture* CreateRenderTarget( ESurfaceFormat eFormat, UINT32 uWidth, UINT32 uHeight, bool bWriteOnly );
		ASCTexture* CreateTexture( UINT32 uWidth, UINT32 uHeight, ASCString& strName);

		//Gets
		ASCTexture* GetNULLTexture();
		ASCTexture* GetDefaultRT( void );
		ASCTexture* GetDefaultDT( void );
		
		//Texture Atlases
		ASCTextureAtlas* LoadTextureAtlas(ASCString& strAtlasName);
		bool IsInTextureAtlas(ASCString& strID, ASCTextureAtlas* &pAtlas);

	protected:
		ASCTexture* m_pNullTexture;
		ASCTexture* m_pDefaultRT;

		ASCVector<ASCTexture*> m_TextureVector;
		ASCVector<ASCTextureAtlas*> m_TextureAtlasVector;
	};

}

#endif

#endif //_ASCGLES2TEXTUREMANAGER_H__Included_25981266

