/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCAnimSprite.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCANIMSPRITE_H__Included_1697949533
#define _ASCANIMSPRITE_H__Included_1697949533

#include "ASCSprite.h"

namespace ASC
{
	class ASCAnimTrack;

	class ASCAnimSprite : public ASCSprite
	{
	public:
		ASCAnimSprite(void);
		~ASCAnimSprite(void);

		//Creation
		bool Init(ASCString strName, ASCShader* pShader, ASCString strTechnique, UINT32 uEventID);

		//Update
		virtual bool Update();

		//Use
		SINT32 GetAnimTrakID(ASCString strID);
		bool PlayAnim(UINT32 uID, bool bLooped = false, bool bReset = true, FLOAT32 fTimeScale = 1.0f, FLOAT32 fABSLength = 0.0f);
		bool PlayAnim(ASCString strID, bool bLooped = false, bool bReset = true, FLOAT32 fTimeScale = 1.0f, FLOAT32 fABSLength = 0.0f);
		UINT32 GetAnimCount();
		void GetAnimName(UINT32 uID, ASCString& strReturn);

		virtual bool FileReset(ASCString strFile);

		virtual void Render();

		UINT32 AnimEventID();

		void IgnorePause(bool bIgnorePause);

	protected:
		ASCVector<ASCAnimTrack*> m_AnimTracks;

		UINT32 m_uEventID;
		UINT32 m_uCurrentPlayingTrack;
		UINT32 m_uIdelID;

		ASCString m_strIdleAnim;

		ASCString m_strFileID;

		bool m_bIgnorePause;
	};

}

#endif //_ASCANIMSPRITE_H__Included_1697949533

