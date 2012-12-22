#include "ASCGenInc.h"
#include "DSVideoManager.h"
#include "DSVideo.h"


CDSVideoManager::CDSVideoManager(void)
{
}


CDSVideoManager::~CDSVideoManager(void)
{
	for(UINT32 i = 0; i < m_Videos.size(); ++i)
	{
		SafeDelete( m_Videos[i] );
	}
}
ASCVideo* 
CDSVideoManager::LoadVideo(ASCString strFile)
{
	HRESULT hr2 = S_OK;

	ASCVideo* pVideo = new CDSVideo(NULL, &hr2);
	if(pVideo->Initialise(strFile))
	{
		m_Videos.push_back( pVideo );
		return pVideo;
	}

	return NULL;
}
