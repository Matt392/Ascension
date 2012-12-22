/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCTexture.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCTEXTURE_H__Included_733625541
#define _ASCTEXTURE_H__Included_733625541

#include "ASCGenInc.h"

namespace ASC
{
	#define TEXTURE_ID_RENDER_TARGET "RENDERTARGET"
	#define TEXTURE_ID_DEPTH_TARGET "DEPTHTARGET"
	#define TEXTURE_NULL "NULL"
	#define TEXTURE_NONE "NONE"


	class ASCTexture
	{
	public:
		virtual ~ASCTexture()
		{
		}
		//Useage
		virtual bool Apply( UINT32 uTextureStage ) = 0;
		virtual bool LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly = true, const SIRect* pRegion = NULL ) = 0;
		virtual bool UnlockTexels( void ) = 0;
		virtual bool Target( UINT32 uIndex ) = 0;

		//Info
		virtual UINT32 GetWidth( void ) const = 0;
		virtual UINT32 GetHeight( void ) const = 0;
		virtual ASCString Name() = 0;
		virtual ASCString NameNoExt() = 0;
		virtual bool Loaded() = 0;
		virtual UINT32 MemSize() = 0;

		//Management
		virtual bool Save(ASCString strName) = 0;
		virtual void Reload() = 0;
		virtual void Load() = 0;
		virtual void UnLoad() = 0;

		virtual void AutoReload(bool bVal) = 0;
	};
}

#endif //_ASCTEXTURE_H__Included_733625541

