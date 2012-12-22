#include "ASCGenInc.h"
#include "ASCAudioManager.h"
#include "ASCAudioEffect.h"
#include "ASCAudioFile.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "Framework.h"
#include "..\Ascension\Ascension.h"
#endif


ASCAudioManager::ASCAudioManager(void)
{
}


ASCAudioManager::~ASCAudioManager(void)
{
	DeInitialise();
}
bool 
ASCAudioManager::Initialise()
{
#ifdef ASC_IOS
    
	m_pALDevice = alcOpenDevice( NULL );
	if( m_pALDevice )
	{
		// Sucessfully created the devive, now create the AL context.
		m_pContext = alcCreateContext( m_pALDevice, NULL );
		alcMakeContextCurrent(m_pContext);
        
        FLOAT32 pListenerPos[] = { static_cast< FLOAT32 >(Ascension::Width()) * 0.5f , 
            static_cast< FLOAT32 >(Ascension::Height() ) * 0.5f,
            0.0f };
        
        alListenerfv( AL_POSITION, pListenerPos );
	}
    else
    {
        return false;
    }
    
#else
	//Initialize Framework
	
	m_pALDevice = alcOpenDevice( NULL );
	if( m_pALDevice )
	{
		// Sucessfully created the devive, now create the AL context.
		m_pContext = alcCreateContext( reinterpret_cast<ALCdevice*>(m_pALDevice), NULL );
		alcMakeContextCurrent(reinterpret_cast<ALCcontext*>(m_pContext));
        
        FLOAT32 pListenerPos[] = { static_cast< FLOAT32 >(Ascension::Width()) * 0.5f , 
            static_cast< FLOAT32 >(Ascension::Height() ) * 0.5f,
            0.0f };
        
        alListenerfv( AL_POSITION, pListenerPos );
	}
    else
    {
        return false;
    }
#endif
	return true;
}
bool 
ASCAudioManager::PlayEffect(ASCString strFile)
{
	for(UINT32 i = 0; i < m_VectorEffects.size(); ++i)
	{
		if(false == m_VectorEffects[i]->Active())
		{
			return m_VectorEffects[i]->Play(strFile);
		}
	}
	ASCAudioEffect* pEffect = new ASCAudioEffect();
	if(pEffect->Init())
	{
		m_VectorEffects.push_back(pEffect);
		return pEffect->Play(strFile);
	}
	SafeDelete( pEffect );
	return false;
}
ASCAudioFile* 
ASCAudioManager::GetFile(ASCString strFile)
{
	for(UINT32 i = 0; i < m_VectorFiles.size(); ++i)
	{
		if(m_VectorFiles[i]->Owned() == false)
		{
			if(m_VectorFiles[i]->Load(strFile))
			{
				m_VectorFiles[i]->Take();
				return m_VectorFiles[i];
			}
		}
	}
	ASCAudioFile* pFile = new ASCAudioFile();
	if(pFile->Init())
	{
		pFile->Load(strFile);
		pFile->Take();
		m_VectorFiles.push_back(pFile);
		return pFile;
	}
	return NULL;
}
bool 
ASCAudioManager::DeInitialise()
{
	for(UINT32 i = 0; i < m_VectorEffects.size(); ++i)
	{
		SafeDelete( m_VectorEffects[i] );
	}

	for(UINT32 i = 0; i < m_VectorFiles.size(); ++i)
	{
		SafeDelete( m_VectorFiles[i] );
	}

#ifdef ASC_IOS
    //Get active context
	m_pContext = alcGetCurrentContext();
	
	//Get device for active context
	m_pALDevice=alcGetContextsDevice(m_pContext);
    
	//Disable context
	alcMakeContextCurrent(NULL);
    
	//Release context(s)
	alcDestroyContext(m_pContext);
    
	//Close device
	alcCloseDevice(m_pALDevice);
#else
    //Get active context
	m_pContext = alcGetCurrentContext();
	
	//Get device for active context
	m_pALDevice=alcGetContextsDevice(reinterpret_cast<ALCcontext*>(m_pContext));
    
	//Disable context
	alcMakeContextCurrent(NULL);
    
	//Release context(s)
	alcDestroyContext(reinterpret_cast<ALCcontext*>(m_pContext));
    
	//Close device
	alcCloseDevice(reinterpret_cast<ALCdevice*>(m_pALDevice));
#endif
	return true;
}
void 
ASCAudioManager::MasterVolume(FLOAT32 fVal)
{
	alListenerf(AL_GAIN, fVal);
}
