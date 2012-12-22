/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCTextureManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCTEXTUREMANAGER_H__Included_1350046839
#define _ASCTEXTUREMANAGER_H__Included_1350046839

#include "ASCGenInc.h"

namespace ASC
{
	enum ESurfaceFormat
	{
		SF_Colour_A8R8G8B8 = 0,	
		SF_Depth_u24,			
		SF_Count				
	};

	class ASCTexture;
	class ASCTextureAtlas;

	class ASCTextureManager
	{
	public:
		virtual ~ASCTextureManager()
		{
		}
		//Loading
		virtual void Reload() = 0;
		
		//Creation
		virtual ASCTexture* CreateTexture( ASCString& strFileName, bool bLoad ) = 0;
		virtual ASCTexture* CreateRenderTarget( ESurfaceFormat eFormat, UINT32 uWidth, UINT32 uHeight, bool bWriteOnly ) = 0;
		virtual ASCTexture* CreateTexture( UINT32 uWidth, UINT32 uHeight, ASCString& strName) = 0;

		//Gets
		virtual ASCTexture* GetNULLTexture() = 0;
		virtual ASCTexture* GetDefaultRT( void ) = 0;
		virtual ASCTexture* GetDefaultDT( void ) = 0;

		//virtual ASCTextureAtlas* ForceLoadAtlas(ASCString strName) = 0;
		virtual ASCTextureAtlas* LoadTextureAtlas(ASCString& strAtlasName) = 0;
		virtual bool IsInTextureAtlas(ASCString& strID, ASCTextureAtlas* &pAtlas) = 0;

		//Info
		virtual UINT32 MemoryUsed() = 0;
	};
}

#endif //_ASCTEXTUREMANAGER_H__Included_1350046839

