#include "ASCGenInc.h"
#include "ASCAudioFile.h"
#ifdef ASC_IOS
#include <OpenAL/al.h>
#include <AudioToolbox/AudioFile.h>
#include <Foundation/NSString.h>
#include <vorbis/vorbisfile.h>

//#define ASC_THREADED_LOADING
#else
#include "Framework.h"
#include "Vorbis\vorbisfile.h"

#endif
#define BUFFER_SIZE   32768     // 32 KB buffers
#define BUFFER_RESIZE_SIZE 20000000


ASCAudioFile::ASCAudioFile(void)

: m_bOwned ( false )
{
}


ASCAudioFile::~ASCAudioFile(void)
{
}
#ifdef ASC_IOS
void LoadOggFile(ASCString strFile, UINT32 uiBuffer)
{
    // Load ogg file into OpenAL Buffer
    FILE* pFile = fopen( strFile.c_str(), "rb");
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
            for(SINT32 i = 0; i < bytes; ++i)
            {
                vecBuffer.push_back(Buffer[i]);
            }
            bytes = ov_read(&oggFile, Buffer, BUFFER_SIZE, endian, 2, 1, &bitStream);
        }
        
        ov_clear(&oggFile);
        
        alBufferData(uiBuffer, format, &vecBuffer[0], static_cast < ALsizei > (vecBuffer.size()), freq);
    }
}
#endif
bool 
ASCAudioFile::Load(ASCString strFile)
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


		// Attach Source to Buffer
		alSourcei( uiSource, AL_BUFFER, uiBuffer );

		SINT32 iSizeInBytes;
		SINT32 iChannels;
		SINT32 iBits;
		SINT32 iLengthInSamples;
		SINT32 iFrequency;

		//Calculate duration
		alGetBufferi(uiBuffer, AL_SIZE, &iSizeInBytes);
		alGetBufferi(uiBuffer, AL_CHANNELS, &iChannels);
		alGetBufferi(uiBuffer, AL_BITS, &iBits);

		iLengthInSamples = iSizeInBytes * 8 / (iChannels * iBits);

		alGetBufferi(uiBuffer, AL_FREQUENCY, &iFrequency);

		m_fDuration = (float)iLengthInSamples / (float)iFrequency;
	}
	return true;
}
bool 
ASCAudioFile::Play()
{
	// Play Source
    alSourcePlay( uiSource );
	return true;
}
bool 
ASCAudioFile::Pause()
{
	// Play Source
    alSourcePause( uiSource );
	return true;
}
void 
ASCAudioFile::Rewind()
{
	alSourceRewind( uiSource );
}
void 
ASCAudioFile::Volume(FLOAT32 fVal)
{
	m_fVolume = fVal;
	alSourcef(uiSource, AL_GAIN, m_fVolume);
}
FLOAT32 
ASCAudioFile::Volume()
{
	return m_fVolume;
}

bool 
ASCAudioFile::Owned()
{
	return m_bOwned;
}
FLOAT32 
ASCAudioFile::Duration()
{
    if(m_fDuration <= 0.0f)
    {
        SINT32 iSizeInBytes;
		SINT32 iChannels;
		SINT32 iBits;
		SINT32 iLengthInSamples;
		SINT32 iFrequency;
        
		//Calculate duration
		alGetBufferi(uiBuffer, AL_SIZE, &iSizeInBytes);
		alGetBufferi(uiBuffer, AL_CHANNELS, &iChannels);
		alGetBufferi(uiBuffer, AL_BITS, &iBits);
        
		iLengthInSamples = iSizeInBytes * 8 / (iChannels * iBits);
        
		alGetBufferi(uiBuffer, AL_FREQUENCY, &iFrequency);
        
		m_fDuration = (float)iLengthInSamples / (float)iFrequency;
    }
	return m_fDuration;
}
FLOAT32 
ASCAudioFile::CurrentTime()
{
	FLOAT32 fReturn;
	alGetSourcef(uiSource, AL_SEC_OFFSET, &fReturn);
	return fReturn;
}
		
bool 
ASCAudioFile::Take()
{
	if(false == m_bOwned)
	{
		m_bOwned = true;
		return true;
	}
	return false;
}
bool 
ASCAudioFile::Return()
{
	if(m_bOwned)
	{
		m_bOwned = false;
		return true;
	}
	return false;
}