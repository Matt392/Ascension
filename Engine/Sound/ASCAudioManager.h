/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Sound\ASCAudioManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCAUDIOMANAGER_H__Included_274469917
#define _ASCAUDIOMANAGER_H__Included_274469917

#ifdef ASC_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

namespace ASC
{
	class ASCAudioEffect;
	class ASCAudioFile;

	class ASCAudioManager
	{
	public:
		ASCAudioManager(void);
		~ASCAudioManager(void);
		bool Initialise();
		bool DeInitialise();
		bool PlayEffect(ASCString strFile);
		ASCAudioFile* GetFile(ASCString strFile);

		void MasterVolume(FLOAT32 fVal);

	protected:
		ASCVector<ASCAudioEffect*> m_VectorEffects;
		ASCVector<ASCAudioFile*> m_VectorFiles;
        
#ifdef ASC_IOS
        ALCdevice* m_pALDevice;
        ALCcontext* m_pContext;
#endif
        void* m_pALDevice;
        void* m_pContext;
	};

}

#endif //_ASCAUDIOMANAGER_H__Included_274469917

