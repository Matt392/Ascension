#include "ASCGenInc.h"
#include "ASCVideo.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif
#include "ASCINIFile.h"
#include "ASCMemoryManagement.h"
#include "ASCFolder.h"
#include "ASCRenderer.h"
#include "ASCTexture.h"
#include "ASCTextureManager.h"
#include "ASCAudioManager.h"
#include "ASCAudioFile.h"
#include "ASCSprite.h"

ASCVideo::ASCVideo(void)
: m_pFrames ( NULL )
, m_uPreBuffer ( 0 )
, m_uPostBuffer ( 0 )
, m_fFrameRate ( 0.0f )
, m_fTimeElapsed ( 0.0f )
, m_bPlaying ( false )
, m_uCurrentFrame ( 0 )
, m_uFrameCount ( 0 )
, m_uWidth ( 0 )
, m_uHeight ( 0 )
, m_pSound ( NULL )
, m_bHasSound ( NULL )
{
}


ASCVideo::~ASCVideo(void)
{
	SafeDelete( m_pFrames );
	m_LinkedSprites.Clear();
}

bool 
ASCVideo::Initialise(ASCString strFile, ASCString strName)
{
	ASCString strInfo = strFile;
	strInfo += ASCString("/");
    strInfo += strName;
	strInfo += ASCString("_Info.ini");
	
	ASCINIFile INIFile;
	if(INIFile.Load( strInfo ))
	{
	
		m_uPreBuffer = INIFile.GetUInt( "VideoInfo", "PreBuffer");
		m_uPostBuffer = INIFile.GetUInt( "VideoInfo", "PostBuffer");

	
		m_uWidth = INIFile.GetUInt( "VideoInfo", "Width");
		m_uHeight = INIFile.GetUInt( "VideoInfo", "Height");
	
		m_uFrameCount = INIFile.GetUInt( "VideoInfo", "FrameCount");

		m_fFrameRate = 1.0f / INIFile.GetFloat( "VideoInfo", "FrameRate");

		m_pFrames = reinterpret_cast<ASCTexture**>(ASCMemoryManagement::GetMemory(sizeof(ASCTexture*) *  m_uFrameCount));
        
		ASCString strFolder = strFile;
    
		if(strFolder[strFolder.length()-1] == '\r')
		{
			ASCString strTemp;
			strFolder.prestrc('\r', strTemp);
			strFolder = strTemp;
		}
    
		strFolder += ASCString( "/Frames" );

		ASCFolder Folder;
		Folder.Load( strFolder );
	
		ASCVector<ASCString>* pFrameFiles = Folder.Files();

		ASCTextureManager* pTextureManager = Ascension::Renderer().GetTextureManager();

		ASCTexture* pTexture = NULL;
		ASCString strTemp;
    
		for(UINT32 i = 0; i < pFrameFiles->size() && i < m_uFrameCount; ++i)
		{
			strTemp = pFrameFiles->at(i);
			if(strTemp.length() > 0)
			{
				//Ascension::Debugging().OutputWindowLine("Video frame: ", ASCDebugString( strTemp.c_str()));
        
				pTexture = pTextureManager->CreateTexture(strTemp, false);
				if(pTexture)
				{
					m_pFrames[i] = pTexture;
					//m_pFrames[i]->UnLoad();
					m_pFrames[i]->AutoReload( false );
				}
				else 
				{
					Ascension::Debugging().OutputWindowLine("### WARNING FAILED TO LOAD ###: ", ASCDebugString( strTemp.c_str()));
				}
			}
		}
		m_pFrames[0]->Load();
		m_pFrames[0]->UnLoad();

		for(UINT32 i = 0; i < m_uPostBuffer; ++i)
		{
			m_pFrames[i]->Load();
			m_pFrames[i]->AutoReload( true );
		}

		m_bHasSound = INIFile.GetBool( "VideoInfo", "Sound");

		if(m_bHasSound)
		{
			ASCString strSound = strFile;
			strSound += ASCString("/");
			strSound += strName;
			strSound += ASCString("_Sound.ogg");
			m_pSound = Ascension::Audio().GetFile( strSound );
		}

		return true;
	}

	assert_now("Guts, Failed to load video info file");

	return false;
}
void 
ASCVideo::Play()
{
	m_bPlaying = true;
	if(false == m_pFrames[m_uCurrentFrame]->Loaded())
	{
		m_pFrames[m_uCurrentFrame]->Load();
		m_pFrames[m_uCurrentFrame]->AutoReload( true );
	}
	if(m_pSound)
	{
		m_pSound->Play();
	}
}
void 
ASCVideo::Pause()
{
	m_bPlaying = false;
	if(m_pSound)
	{
		m_pSound->Pause();
	}
}
void 
ASCVideo::Stop()
{
	m_bPlaying = false;
	m_pFrames[m_uCurrentFrame]->UnLoad();
	m_pFrames[m_uCurrentFrame]->AutoReload( false );
	if(m_pSound)
	{
		m_pSound->Pause();
	}
}
void 
ASCVideo::Rewind()
{
	if(m_pSound)
	{
		m_pSound->Rewind();
	}
	m_fTimeElapsed = 0.0f;
	if(m_pFrames[m_uCurrentFrame]->Loaded())
	{
		m_pFrames[m_uCurrentFrame]->UnLoad();
		m_pFrames[m_uCurrentFrame]->AutoReload( false );
		m_uCurrentFrame = 0;
		m_pFrames[m_uCurrentFrame]->Load();
		m_pFrames[m_uCurrentFrame]->AutoReload( true );
	}
	else
	{
		m_uCurrentFrame = 0;
	}
}
void 
ASCVideo::FastFoward()
{
	m_fTimeElapsed = m_fFrameRate * m_uFrameCount;
	if(m_pFrames[m_uCurrentFrame]->Loaded())
	{
		m_pFrames[m_uCurrentFrame]->UnLoad();
		m_pFrames[m_uCurrentFrame]->AutoReload( false );
		m_uCurrentFrame = m_uFrameCount-1;
		m_pFrames[m_uCurrentFrame]->Load();
		m_pFrames[m_uCurrentFrame]->AutoReload( true );
	}
	else
	{
		m_uCurrentFrame = m_uFrameCount-1;
	}
	
	for(UINT32 i = 0; i < m_LinkedSprites.size(); ++i)
	{
		if(m_LinkedSprites[i])
		{
			m_LinkedSprites[i]->SetTexture(m_pFrames[m_uCurrentFrame]);
		}
	}
}
void 
ASCVideo::NextFrame()
{
	UINT32 uPreviousFrame = m_uCurrentFrame;

	m_fTimeElapsed = 0.0f;
			
	++m_uCurrentFrame;
	if(m_uCurrentFrame >= m_uFrameCount)
	{
		m_bPlaying = false;
		m_uCurrentFrame = 0;
		if(m_pSound)
		{
			m_pSound->Rewind();
		}
	}

	if(uPreviousFrame != m_uCurrentFrame)
	{
        ASCTexture* pFrame = m_pFrames[uPreviousFrame];
        if(pFrame)
        {
            pFrame->UnLoad();
            pFrame->AutoReload( false );
        }

        pFrame = m_pFrames[m_uCurrentFrame];
		if(pFrame)
        {
            pFrame->Load();
            pFrame->AutoReload( true );
        }
	}
		
	for(UINT32 i = 0; i < m_LinkedSprites.size(); ++i)
	{
		if(m_LinkedSprites[i])
		{
			m_LinkedSprites[i]->SetTexture(m_pFrames[m_uCurrentFrame]);
		}
	}
}
void 
ASCVideo::Update()
{
	if(m_bPlaying)
	{
		m_fTimeElapsed += Ascension::Delta();
		UINT32 uPreviousFrame = m_uCurrentFrame;
		while(m_fTimeElapsed >= m_fFrameRate)
		{
			m_fTimeElapsed -= m_fFrameRate;
			
			++m_uCurrentFrame;
			if(m_uCurrentFrame >= m_uFrameCount)
			{
				m_bPlaying = false;
				m_uCurrentFrame = 0;
				if(m_pSound)
				{
					m_pSound->Rewind();
				}
				break;
			}
		}
		if(uPreviousFrame != m_uCurrentFrame)
		{
            ASCTexture* pFrame = m_pFrames[uPreviousFrame];
            if(pFrame)
            {
                pFrame->UnLoad();
                pFrame->AutoReload( false );
            }

            pFrame = m_pFrames[m_uCurrentFrame];
			if(pFrame)
            {
                pFrame->Load();
                pFrame->AutoReload( true );
            }
		}
		
		for(UINT32 i = 0; i < m_LinkedSprites.size(); ++i)
		{
			if(m_LinkedSprites[i])
			{
				m_LinkedSprites[i]->SetTexture(m_pFrames[m_uCurrentFrame]);
			}
		}
	}
}
ASCTexture* 
ASCVideo::CurrentFrame()
{
	return m_pFrames[m_uCurrentFrame];
}
bool 
ASCVideo::Playing()
{
	return m_bPlaying;
}
UINT32 
ASCVideo::Width()
{
	return m_uWidth;
}
UINT32 
ASCVideo::Height()
{
	return m_uHeight;
}
void 
ASCVideo::LinkSprite(ASCSprite* pSprite)
{
	for(UINT32 i = 0; i < m_LinkedSprites.size(); ++i)
	{
		if(m_LinkedSprites[i] == pSprite)
		{
			return;
		}
	}
	m_LinkedSprites.push_back(pSprite);
	pSprite->SetActive( true );
}
void 
ASCVideo::UnLinkSprite(ASCSprite* pSprite)
{
	for(UINT32 i = 0; i < m_LinkedSprites.size(); ++i)
	{
		if(m_LinkedSprites[i] == pSprite)
		{
			pSprite->SetActive( false );
			m_LinkedSprites[i] = NULL;
		}
	}
}