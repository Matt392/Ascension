#include "ASCGenInc.h"
#include "ASCSound.h"
#ifdef ASC_IOS
#include <OpenAL/al.h>
#include <AudioToolbox/AudioFile.h>
#include <Foundation/NSString.h>
#else
#include "Framework.h"
#endif

ASCSound::ASCSound(void)
: uiBuffer ( 0 )
, uiSource ( 0 )  
, iState ( 0 )
, m_strFileLoaded ( "" )
{
}


ASCSound::~ASCSound(void)
{
}

bool 
ASCSound::Init()
{
	// Generate an AL Buffer
	alGenBuffers( 1, &uiBuffer );

	// Generate a Source to playback the Buffer
    alGenSources( 1, &uiSource );


	return true;
}
bool 
ASCSound::Active()
{
	alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
	return iState == AL_PLAYING;
}