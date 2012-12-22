/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9TextureManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9TEXTUREMANAGER_H__Included_701782468
#define _ASCDX9TEXTUREMANAGER_H__Included_701782468

#ifdef ASC_RENDERER_DX9

#include "ASCTextureManager.h"
#include <d3dx9.h>

namespace ASC
{
	class ASCTexture;
	class ASCTextureAtlas;

	class ASCDX9TextureManager : public ASCTextureManager
	{
	public:
		ASCDX9TextureManager( IDirect3DDevice9* pDevice );
		~ASCDX9TextureManager(void);

		//Loading
		void Reload();
		
		//Creation
		ASCTexture* CreateTexture( ASCString& strFileName, bool bLoad );
		ASCTexture* CreateRenderTarget( ESurfaceFormat eFormat, UINT32 uWidth, UINT32 uHeight, bool bWriteOnly );
		ASCTexture* CreateTexture( UINT32 uWidth, UINT32 uHeight, ASCString& strName);

		//Gets
		ASCTexture* GetNULLTexture();
		ASCTexture* GetDefaultRT( void );
		ASCTexture* GetDefaultDT( void );
		
		//Texture Atlases
		ASCTextureAtlas* LoadTextureAtlas(ASCString& strAtlasName);
		bool IsInTextureAtlas(ASCString& strID, ASCTextureAtlas* &pAtlas);

		//Info
		UINT32 MemoryUsed();

	protected:
		IDirect3DDevice9* m_pDevice;
		ASCTexture* m_pNullTexture;
		ASCTexture* m_pDefaultRT;

		ASCVector<ASCTexture*> m_TextureVector;
		ASCVector<ASCTextureAtlas*> m_TextureAtlasVector;
	};

}

#endif

#endif //_ASCDX9TEXTUREMANAGER_H__Included_701782468

