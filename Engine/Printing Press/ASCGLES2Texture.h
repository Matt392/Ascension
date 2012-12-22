/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2Texture.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2TEXTURE_H__Included_1953645990
#define _ASCGLES2TEXTURE_H__Included_1953645990

#ifdef ASC_RENDERER_GLES2

#include "ASCTexture.h"

namespace ASC
{
	class ASCGLES2Texture : public ASCTexture
	{
	public:
		~ASCGLES2Texture(void);
		
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

		//Management
		bool Save(ASCString strName);
		void Reload();
		void Load();
		void UnLoad();

		void AutoReload(bool bVal);
        
	protected:
		UINT32 m_uTextureID;
		UINT32 m_uiWidth;
		UINT32 m_uiHeight;

		UINT32 m_uAssignedSampler;

		ASCString m_strTextureName;
		
		bool m_bDepthTexture;	
		bool m_bLoaded;

		bool m_bAutoReload;

		bool m_bReadOnlyTexels;

		UINT32* m_pTempData;

		
	private:
		friend class ASCGLES2TextureManager;
		ASCGLES2Texture( ASCString& strFileName, bool bLoad );
		ASCGLES2Texture( UINT32 uWidth, UINT32 uHeight, ASCString& strFileName );
		ASCGLES2Texture( bool bDepthTexture );
	};

}

#endif

#endif //_ASCGLES2TEXTURE_H__Included_1953645990

