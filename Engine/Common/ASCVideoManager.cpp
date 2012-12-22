#include "ASCGenInc.h"
#include "ASCVideoManager.h"
#include "ASCVideo.h"

#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif
#include "ASCSpriteManager.h"
#include "ASCSprite.h"


ASCVideoManager::ASCVideoManager(void)
{
}


ASCVideoManager::~ASCVideoManager(void)
{
	for(UINT32 i = 0; i < m_Videos.size(); ++i)
	{
		SafeDelete( m_Videos[i] );
	}
}
ASCVideo* 
ASCVideoManager::LoadVideo(ASCString strVideoFile, ASCString strName, ASCSprite*& pVideoSprite, ASCString strTechnique, bool bScreenSpace)
{
	ASCVideo* pVideo = NULL;
	pVideo = new ASCVideo();
    Ascension::Debugging().OutputWindowLine("Loading Video: ", ASCDebugString( strVideoFile.c_str() ));
	if(pVideo->Initialise(strVideoFile, strName))
	{
		m_Videos.push_back(pVideo);
		pVideoSprite = CreateVideoSprite(pVideo, strTechnique, bScreenSpace);
		pVideo->LinkSprite(pVideoSprite);

		return pVideo;
	}
	assert_now("Guts, Video did not load");
	SafeDelete( pVideo );
	return NULL;
}
ASCSprite* 
ASCVideoManager::CreateVideoSprite(ASCVideo* pVideo, ASCString strTechnique, bool bScreenSpace)
{
	ASCSprite* pSprite = NULL;
	if(bScreenSpace)
	{
		pSprite = Ascension::SpriteManager().CreateSprite(pVideo->CurrentFrame(), strTechnique, true);
		pSprite->SetAgainstScreen(true);
        
#ifdef ASC_WINDOWS
		pSprite->SetPosition( Ascension::Width() / 2.0f, Ascension::Height() / 2.0f );
		pSprite->SetSize(SC_FLOAT(pVideo->Width()), SC_FLOAT(pVideo->Height()));
#endif
        
#ifdef ASC_IOS
        /*
        Ascension::Debugging().OutputWindowLine("Scale X: ", Ascension::ScaleX() );
        Ascension::Debugging().OutputWindowLine("Scale Y: ", Ascension::ScaleY() );
        
        Ascension::Debugging().OutputWindowLine("Pos    X: ", (Ascension::Width() / 2.0f) );
        Ascension::Debugging().OutputWindowLine("Pos    Y: ", (Ascension::Height() / 2.0f) );
        Ascension::Debugging().OutputWindowLine("Pos PS X: ", (Ascension::Width() / 2.0f) * Ascension::ScaleX() );
        Ascension::Debugging().OutputWindowLine("Pos PS Y: ", (Ascension::Height() / 2.0f) * Ascension::ScaleY() );
        
        Ascension::Debugging().OutputWindowLine("Size    X: ", SC_FLOAT(pVideo->Width()) );
        Ascension::Debugging().OutputWindowLine("Size    Y: ", SC_FLOAT(pVideo->Height()) );
        Ascension::Debugging().OutputWindowLine("Size PS X: ", SC_FLOAT(pVideo->Width()) * Ascension::ScaleX() );
        Ascension::Debugging().OutputWindowLine("Size PS Y: ", SC_FLOAT(pVideo->Height()) * Ascension::ScaleY() );
        */
        
        
		pSprite->SetPosition( (Ascension::Width() / 2.0f) * Ascension::ScaleX(), (Ascension::Height() / 2.0f) * Ascension::ScaleY() );
		pSprite->SetSize(SC_FLOAT(pVideo->Width()) * Ascension::ScaleX(), SC_FLOAT(pVideo->Height()) * Ascension::ScaleY() );
#endif
        
	}
	else
	{
        Ascension::Debugging().OutputWindowLine("How the hell did I get here");
		pSprite = Ascension::SpriteManager().CreateSprite(pVideo->CurrentFrame(), strTechnique, true);
	}

	return pSprite;
}
void 
ASCVideoManager::Update()
{
	for(UINT32 i = 0; i < m_Videos.size(); ++i)
	{
		m_Videos[i]->Update();
	}
}
