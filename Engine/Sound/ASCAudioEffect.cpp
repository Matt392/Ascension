#include "ASCGenInc.h"
#include "ASCAudioEffect.h"
#ifdef ASC_IOS
#include <OpenAL/al.h>
#include <AudioToolbox/AudioFile.h>
#include <Foundation/NSString.h>
#include <vorbis/vorbisfile.h>

#else
#include "Framework.h"
#include "Vorbis\vorbisfile.h"

#endif
#define BUFFER_SIZE   32768     // 32 KB buffers
#define BUFFER_RESIZE_SIZE 20000000

ASCAudioEffect::ASCAudioEffect(void)
{
}


ASCAudioEffect::~ASCAudioEffect(void)
{
	alSourceStop(uiSource);
    alDeleteSources(1, &uiSource);
	alDeleteBuffers(1, &uiBuffer);
}

bool 
ASCAudioEffect::Play(ASCString strFile)
{
	if(m_strFileLoaded != strFile)
	{
		//recreate buffer
		alDeleteBuffers(1, &uiBuffer);
		alGenBuffers( 1, &uiBuffer );
        
		m_strFileLoaded = strFile;
      

#ifdef ASC_IOS
        ASCString strTempFile = ASCGeneralFunctions::ConvertFilePathForIOS(m_strFileLoaded);
        AudioFileID fileID;
        NSString* audioPath = [NSString stringWithUTF8String:strTempFile.c_str()];
        NSURL* audioURL = [NSURL fileURLWithPath:audioPath];
        
		ASCString strExtension;
		ASCString strTemp;
		m_strFileLoaded.splitc('.', strTemp, strExtension);
		if(strExtension == ".wav")
		{
			//Give hint of type (kAudioFileWAVEType?)
            OSStatus result = AudioFileOpenURL ((CFURLRef)audioURL, kAudioFileReadPermission, 0, &fileID);
            
            //ASSERT( !result );
            
            //Find out how big the audio data is.
            UInt64 dataSize = 0;
            UInt32 propSize = sizeof(UInt64);
            result = AudioFileGetProperty( fileID, kAudioFilePropertyAudioDataByteCount, &propSize, &dataSize);
            
            //Copy data from file into an OpenAL buffer.
            void* audioData = malloc( dataSize );
            result = noErr;
            result = AudioFileReadBytes( fileID, false, 0, (UInt32*)(&dataSize), audioData );
            AudioFileClose( fileID );
            
            //ASSERT( !result );
            
            // Create the OpenAL sound.
            // @TODO: Read format and frequency from file!!
            alBufferData( uiBuffer, AL_FORMAT_STEREO16, audioData, dataSize, 44100 );
            
            //Free our buffer.
            if ( audioData )
            {
                free( audioData );
                audioData = NULL;
            }
		}
		else if( strExtension == ".ogg")
		{
#ifdef ASC_THREADED_LOADING
            [NSThread detachNewThreadSelector:@selector(LoadOggFile) toTarget:self withObject:nil];
#else
			// Load ogg file into OpenAL Buffer
			FILE* pFile = fopen( strTempFile.c_str(), "rb");
			if(pFile)
			{
				vorbis_info *pInfo;
				OggVorbis_File oggFile;
				ALenum format;
				ALsizei freq;
				long bytes;
				int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
				int bitStream;
				CHAR8 Buffer[BUFFER_SIZE];
				ASCVector<CHAR8> vecBuffer;
				vecBuffer.SetResizeSize(BUFFER_RESIZE_SIZE);
                
				ov_open(pFile, &oggFile, NULL, 0);
                
				pInfo = ov_info(&oggFile, -1);
                
				if (pInfo->channels == 1)
				{
					format = AL_FORMAT_MONO16;
				}
				else
				{
					format = AL_FORMAT_STEREO16;
				}
				freq = pInfo->rate;
                
				bytes = ov_read(&oggFile, Buffer, BUFFER_SIZE, endian, 2, 1, &bitStream);
				while(bytes > 0)
				{
					vecBuffer.push_back_bulk(&Buffer[0], bytes);
					//for(SINT32 i = 0; i < bytes; ++i)
					//{
					//	vecBuffer.push_back(Buffer[i]);
					//}
					bytes = ov_read(&oggFile, Buffer, BUFFER_SIZE, endian, 2, 1, &bitStream);
				}
                
				ov_clear(&oggFile);
                
				alBufferData(uiBuffer, format, &vecBuffer[0], static_cast < ALsizei > (vecBuffer.size()), freq);
#endif
			}
			else
			{
				assert_now("Guts, Failed to load file");
			}
		}
		else
		{
			assert_now("Guts, Unsupported audio format");
		}
#else
		ASCString strExtension;
		ASCString strTemp;
		m_strFileLoaded.poststrc('.', strTemp);
		m_strFileLoaded.split(strTemp.length(), strTemp, strExtension);
		if(strExtension == ".wav")
		{
			// Load Wave file into OpenAL Buffer
			if (!ALFWLoadWaveToBuffer(m_strFileLoaded.c_str(), uiBuffer))
			{
				m_strFileLoaded = "";
				return false;
			}
		}
		else if( strExtension == ".ogg")
		{
			// Load ogg file into OpenAL Buffer
			FILE* pFile = NULL;
			fopen_s(&pFile, m_strFileLoaded.c_str(), "rb");
			if(pFile)
			{
				vorbis_info *pInfo;
				OggVorbis_File oggFile;
				ALenum format;
				ALsizei freq;
				long bytes;
				int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
				int bitStream;
				CHAR Buffer[BUFFER_SIZE];
				ASCVector<CHAR8> vecBuffer;
				vecBuffer.SetResizeSize(BUFFER_RESIZE_SIZE);

				ov_open(pFile, &oggFile, NULL, 0);

				pInfo = ov_info(&oggFile, -1);

				if (pInfo->channels == 1)
				{
					format = AL_FORMAT_MONO16;
				}
				else
				{
					format = AL_FORMAT_STEREO16;
				}
				freq = pInfo->rate;

				bytes = ov_read(&oggFile, Buffer, BUFFER_SIZE, endian, 2, 1, &bitStream);

				while(bytes > 0)
				{
					vecBuffer.push_back_bulk(&Buffer[0], bytes);
					//for(SINT32 i = 0; i < bytes; ++i)
					//{
					//	vecBuffer.push_back(Buffer[i]);
					//}
					bytes = ov_read(&oggFile, Buffer, BUFFER_SIZE, endian, 2, 1, &bitStream);
				}

				ov_clear(&oggFile);

				alBufferData(uiBuffer, format, &vecBuffer[0], static_cast < ALsizei > (vecBuffer.size()), freq);
			}
			else
			{
				assert_now("Guts, Failed to load file");
			}
		}
		else
		{
			assert_now("Guts, Unsupported audio format");
		}
#endif
	}
	// Play Source
    alSourcePlay( uiSource );
	alSourcef(uiSource, AL_GAIN, 1.0f);
	return true;
}