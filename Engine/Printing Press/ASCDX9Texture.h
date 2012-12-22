/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9Texture.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9TEXTURE_H__Included_1807791816
#define _ASCDX9TEXTURE_H__Included_1807791816

#ifdef ASC_RENDERER_DX9

#include "ASCTexture.h"
#include <d3dx9.h>

namespace ASC
{
	class ASCDX9Texture : public ASCTexture
	{
	public:
		~ASCDX9Texture(void);
		
		//Useage
		bool Apply( UINT32 uTextureStage );
		bool LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly = true, const SIRect* pRegion = NULL );
		bool UnlockTexels( void );
		bool Target( UINT32 uIndex );

		//Info
		UINT32 GetWidth( void ) const;
		UINT32 GetHeight( void ) const;
		ASCString Name();
		ASCString NameNoExt();
		bool Loaded();
		UINT32 MemSize();

		//Management
		bool Save(ASCString strName);
		void Reload();
		void Load();
		void UnLoad();

		void AutoReload(bool bVal);

	protected:
		LPDIRECT3DTEXTURE9 m_pTexture;
		IDirect3DSurface9* m_pSurface;		
		IDirect3DDevice9* m_pDevice;

		UINT32 m_uiWidth;
		UINT32 m_uiHeight;

		ASCString m_strTextureName;
		
		bool m_bDepthTexture;	
		bool m_bLoaded;

		bool m_bAutoReload;

		
	private:
		friend class ASCDX9TextureManager;
		ASCDX9Texture( IDirect3DDevice9* pDevice, ASCString& strFileName, bool bLoad );
		ASCDX9Texture( IDirect3DDevice9* pDevice, LPDIRECT3DTEXTURE9 pTexture, ASCString& strFileName );
		ASCDX9Texture( IDirect3DDevice9* pDevice, IDirect3DSurface9* pSurface, bool bDepthTexture );
	};

}

#endif

#endif //_ASCDX9TEXTURE_H__Included_1807791816

