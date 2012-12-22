#include "ASCGenInc.h"
#include "DSVideo.h"

CDSVideo::CDSVideo(LPUNKNOWN pUnk,HRESULT *phr)
: m_pGraphBuilder ( NULL )
, m_pSourceFilter ( NULL )
, m_pMediaCntr ( NULL )
, m_pMediaPosition ( NULL )
, m_pTexture ( NULL )
, CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), TEXT("Texture Renderer"), pUnk, phr)
, m_pVideoBuffer ( NULL )
, m_pVideoInfo ( NULL )
, m_lVideoWidth ( 0 )
, m_lVideoHeight ( 0 )
, m_lVideoPitch ( 0 )
, m_bNewFrame ( false )
, m_pRendererInPin ( NULL )
, m_pSourceOutPin ( NULL )
{
	*phr = S_OK;
}


CDSVideo::~CDSVideo(void)
{
	SafeDelete( m_pVideoBuffer );
}

bool 
CDSVideo::Initialise(ASCString strFile)
{
	strFile;
	HRESULT hr = CoCreateInstance(/*id*/ CLSID_FilterGraph, /*LPUNKNOWN*/NULL, /*clsContext*/CLSCTX_INPROC_SERVER, /*IID*/IID_IGraphBuilder, (void**)&m_pGraphBuilder ); 
	if( FAILED( hr ) )
	{
		assert_now( "Guts, Failed to create the graph builder!" );
	}
	
	MultiByteToWideChar(0, 0, strFile.c_str(), -1, m_pwcName, 256);
	
	hr = m_pGraphBuilder->AddSourceFilter( m_pwcName, m_pwcName, &m_pSourceFilter ); 
	if( FAILED( hr ) )
	{
		assert_now( "Guts, FAILED TO ADD VIDEO TO THE GRAPH!" );
	}
	
	AttachRendererToSourceFilter(  );
	
	m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaCntr );
	m_pGraphBuilder->QueryInterface( IID_IMediaPosition, (void**)&m_pMediaPosition );
	return false;
}
ASCTexture* 
CDSVideo::GetTexture()
{
	return m_pTexture;
}
void 
CDSVideo::Update()
{
}
void 
CDSVideo::PlayVideo()
{
	m_pMediaCntr->Run( );
}
void 
CDSVideo::PauseVideo()
{
	m_pMediaCntr->Pause( );
}
void 
CDSVideo::StopVideo()
{
	m_pMediaCntr->Stop( );
}
bool 
CDSVideo::IsPlaying()
{
	return false;
}
// Renders the media into the byte buffer.
HRESULT CDSVideo::DoRenderSample(IMediaSample *pMediaSample)
{
	BYTE *buffer;
  
	pMediaSample->GetPointer(&buffer);


	
	if (!m_pVideoBuffer)
	{
		m_pVideoBuffer = (BYTE*)malloc(m_lVideoPitch * m_lVideoHeight);
	}


	if (!m_bNewFrame)
	{
		memcpy (m_pVideoBuffer,buffer, m_lVideoPitch * m_lVideoHeight);
		m_bNewFrame = true;
	}
		
  return S_OK;
}
HRESULT CDSVideo::CheckMediaType(const CMediaType *pmt)
{
    HRESULT   hr = E_FAIL;
    
    // Reject the connection if this is not a video type
    if( *pmt->FormatType() != FORMAT_VideoInfo ) {
        return E_INVALIDARG;
    }
    
    // Only accept RGB24
    m_pVideoInfo = (VIDEOINFO *)pmt->Format();
    if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
       IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_RGB24))
    {
        hr = S_OK;
    }
    
    return hr;
}
HRESULT CDSVideo::SetMediaType(const CMediaType *pmt)
{
	m_pVideoInfo = (VIDEOINFO *)pmt->Format();

	m_lVideoWidth  = m_pVideoInfo->bmiHeader.biWidth;
	m_lVideoHeight = abs(m_pVideoInfo->bmiHeader.biHeight);
	m_lVideoPitch  = (m_lVideoWidth * 3 + 3) & ~(3);
	return S_OK;
}
HRESULT FindDirectionalPin(IBaseFilter* baseFilter,
			 PIN_DIRECTION direction,
			 int pinNumber,
			 IPin** destPin) 
{
  HRESULT hr = E_INVALIDARG;

  if ( destPin )
  {
    *destPin = NULL;

    IEnumPins* enumPins;
    if (SUCCEEDED(hr = baseFilter->EnumPins(&enumPins))) 
    {
      ULONG numFound;
      IPin* pFound;

      // hr is S_FALSE when last we have returned the last one previously 
      while (S_OK == (hr = enumPins->Next(1, &pFound, &numFound))) 
      {
        PIN_DIRECTION pinDirection;
        if (S_OK == (hr = pFound->QueryDirection(&pinDirection)))
        {
          if (pinDirection == direction) {
            if (pinNumber == 0) {
              // Return the pin's interface
              *destPin = pFound;
              break;
            }
            pinNumber--;
          }
        }
        pFound->Release();
      }
    }

    // If nothing went wrong, but nothing found
    if ( SUCCEEDED(hr) && NULL == *destPin )
      hr = E_NOTIMPL;
	}

  return hr;
}
void CDSVideo::AttachRendererToSourceFilter(  )
{

	HRESULT hr = m_pGraphBuilder->AddFilter( this, m_pwcName );
	if( FAILED( hr ) )
	{
		assert_now( "FAILED TO ADD THE TEXURE RENDERER FILTER!" );
	}
	// Connect this texture renderer to the source filters out put pin.
	
	// Sources out put pin.
	 hr = FindDirectionalPin( m_pSourceFilter, 
						  PINDIR_OUTPUT,
						  0,
						  &m_pSourceOutPin );

	if( FAILED( hr ) )
	{
		assert_now( "FAILED TO FIND VIDEO's SOURCE FILTER OUTPUT PIN!" );
	}

	// Sources out put pin.
		hr = FindDirectionalPin( this, 
						  PINDIR_INPUT,
						  0,
						  &m_pRendererInPin );

	if( FAILED( hr ) )
	{
		assert_now( "FAILED TO FIND TEXURE RENDERERS FILTER INPUT PIN!" );
	}

	// Connect the pins together so that we receive data from the source filter.
	hr = m_pGraphBuilder->Connect( m_pSourceOutPin, m_pRendererInPin );

	if( FAILED( hr ) )
	{
		assert_now( "FAILED TO CONNECT THE TEXTURE RENDERER TO THE SOURCE FILTER!" );
	}
}