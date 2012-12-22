#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2Renderer.h"
#include "ASCConversions.h"
#include "ASCProfilingSystem.h"
#ifdef ASC_IOS
#include "Ascension.h"
#else
#include "..\Ascension\Ascension.h"
#endif
#include "ASCFont.h"
#include "ASCSprite.h"
#include "ASCSpriteManager.h"

#include "ASCGLES2VertexDeclaration.h"
#include "ASCGLES2TextureManager.h"
#include "ASCGLES2ShaderManager.h"
#include "ASCGLES2VertexBuffer.h"

#include <stdio.h>
#ifdef ASC_IOS
#import "ASCRenderView.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/EAGL.h>
#else
#include <TCHAR.h>
#endif

#include "OGLES2/Tools/PVRTVector.h"

#ifdef ASC_IOS
ASCGLES2Renderer::ASCGLES2Renderer(void)
: m_uDrawPrimitiveUPCount ( 0 )
, m_uDrawPrimitiveCount ( 0 )
, m_uDrawIndexedPrimitiveUPCount ( 0 )
, m_uDrawIndexedPrimitiveCount ( 0 )
, m_uVertexStride ( 0 )
, m_pDefaultVertDecl ( NULL )
, m_pTextureManager ( NULL )
, m_pShaderManager ( NULL )
, m_eglDisplay ( NULL )
, m_eglSurface ( NULL )
, m_eglContext ( NULL )
, m_pGeneralBuffer ( NULL )
, m_pRenderView ( NULL )
{
}
#else
ASCGLES2Renderer::ASCGLES2Renderer(void)
: m_hInst ( 0 )
, m_hWnd ( 0 )
, m_uDrawPrimitiveUPCount ( 0 )
, m_uDrawPrimitiveCount ( 0 )
, m_uDrawIndexedPrimitiveUPCount ( 0 )
, m_uDrawIndexedPrimitiveCount ( 0 )
, m_uVertexStride ( 0 )
, m_pDefaultVertDecl ( NULL )
, m_pTextureManager ( NULL )
, m_pShaderManager ( NULL )
, m_eglDisplay ( NULL )
, m_eglConfig ( NULL )
, m_eglSurface ( NULL )
, m_eglContext ( NULL )
, m_eglWindow ( NULL )
, m_pGeneralBuffer ( NULL )
{
}
#endif


ASCGLES2Renderer::~ASCGLES2Renderer(void)
{
#ifdef ASC_WINDOWS
	UnregisterClass( L"AscensionWindow", m_hInst );
#endif

	for(UINT32 i = 0; i < m_vecVertexBuffers.size(); ++i)
	{
		SafeDelete( m_vecVertexBuffers[i] );
	}

	for(UINT32 i = 0; i < m_vecFonts.size(); ++i)
	{
		SafeDelete( m_vecFonts[i] ); 
	}
	for(UINT32 i = 0; i < m_vecTextToRender.size(); ++i)
	{
		SafeDelete( m_vecTextToRender[i] );
	}

	

    
#ifdef ASC_WINDOWS
	
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(m_eglDisplay);
    
    
	if (m_hDC) 
	{`
		ReleaseDC(m_hWnd, m_hDC);
	}

	if (m_hWnd) 
	{
		DestroyWindow(m_hWnd);
	}
#endif
    
#ifdef ASC_IOS
    [m_pRenderView release];
#endif

	SafeDelete( m_pDefaultVertDecl );
	SafeDelete( m_pTextureManager );
	SafeDelete( m_pShaderManager );
}
//Initialisation
void 
ASCGLES2Renderer::InitForIOS( void* pRenderView )
{
    ASCRenderView* pView = reinterpret_cast<ASCRenderView*>(pRenderView);
    [pView retain];
	m_pRenderView = pView; 
	m_eglContext = [m_pRenderView getGLContext];
}
bool 
ASCGLES2Renderer::Initialise( SRendererInit& rParameters )
{
#ifdef ASC_IOS
    InitForIOS(rParameters.m_pIOSGLView);
#else
	WNDCLASSEX wc = 
	{ 
		sizeof(WNDCLASSEXA), 
		CS_CLASSDC,
		ASCGLES2Renderer::MsgProc, 
		0L, 0L, 
		GetModuleHandle(NULL), 
		NULL, NULL, 
		NULL, NULL,
		L"AscensionWindow",
		NULL 
	};
	ATOM registerClass = RegisterClassEx( &wc );
	
	if (!registerClass)
	{
		assert_now("Guts, Failed to register the window class");
	}


	RECT rc = { 0, 0, rParameters.m_uScreenWidth, rParameters.m_uScreenHeight };

	LPCWSTR lstrWindowTitle = CharStrToLPCWSTR(rParameters.m_strWindowName.c_str());
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, false );

	m_hWnd = CreateWindow(		L"AscensionWindow",
								lstrWindowTitle,
								WS_OVERLAPPEDWINDOW - (WS_MAXIMIZEBOX|WS_THICKFRAME), 
								CW_USEDEFAULT, CW_USEDEFAULT, 
								rc.right - rc.left, 
								rc.bottom - rc.top, 
								GetDesktopWindow(), NULL, 
								wc.hInstance, NULL );

	m_eglWindow = m_hWnd;
	assert_msg(m_hWnd != NULL, "Guts, Failed to create window");

	m_hDC = GetDC(m_hWnd);
	
	if (!m_hDC)
	{
		assert_now("Guts, Failed to create the device context");
	}
	m_eglDisplay = eglGetDisplay(m_hDC);

	if(m_eglDisplay == EGL_NO_DISPLAY)
	{
		m_eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
	}
	
	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(m_eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		assert_now("Guts, eglInitialize() failed");
	}
	
	eglBindAPI(EGL_OPENGL_ES_API);
	if (!TestEGLError(m_hWnd, "eglBindAPI"))
	{
		assert_now("Guts, eglBindAPI() failed");
	}
	
	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,
		EGL_DEPTH_SIZE,			EGL_DONT_CARE,
		EGL_NONE
	};

	int iConfigs;
	if (!eglChooseConfig(m_eglDisplay, pi32ConfigAttribs, &m_eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		assert_now("Guts, eglChooseConfig() failed");
	}
	
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_eglWindow, NULL);

    if(m_eglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, NULL, NULL);
	}
	
	if (!TestEGLError(m_hWnd, "eglCreateWindowSurface"))
	{
		assert_now("Guts, Something failed");
	}
	
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, ai32ContextAttribs);
	if (!TestEGLError(m_hWnd, "eglCreateContext"))
	{
		assert_now("Guts, eglCreateContext() failed");
	}
	
	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
	if (!TestEGLError(m_hWnd, "eglMakeCurrent"))
	{
		assert_now("Guts, eglMakeCurrent() failed");
	}
	
    
	// Show the window
	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hWnd );

	m_hInst = wc.hInstance;
#endif
	
	VertElement sDeclElem[] = {	VertElement::Init(0,	ES_POSITION,	0, ET_FLOAT32,	4),
									VertElement::Init(16,	ES_COLOUR,		0, ET_FLOAT32,		4),
									VertElement::Init(32,	ES_COLOUR,		0, ET_FLOAT32,		4),
									VertElement::Init(48,	ES_TEXTURE,		0, ET_FLOAT32,	2),
									VertElement::Init(56,	ES_NORMAL,		0, ET_FLOAT32,	3),
									VertElement::Init(68,	ES_BINORMAL,	0, ET_FLOAT32,	2),};

	m_pDefaultVertDecl = new ASCGLES2VertexDeclaration(sDeclElem, 6);
	m_pDefaultVertDecl->Apply();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    

	//SetRenderState( RS_bAlphaBlendEnable, true );
	//SetRenderState( RS_bAlphaBlendEnable, true );
	//SetRenderState( RS_eAlphaSrcBlend, AB_SrcAlpha );
	//SetRenderState( RS_eDestBlend, AB_InvSrcAlpha );
	
	m_pTextureManager = new ASCGLES2TextureManager( );
	m_pShaderManager = new ASCGLES2ShaderManager( );

	m_ProjectionMatrix = CreateProjectionMatrix( SC_FLOAT(rParameters.m_uScreenWidth), SC_FLOAT(rParameters.m_uScreenHeight), rParameters.m_fNear, rParameters.m_fFar);
	SetProjectionMatrix(m_ProjectionMatrix);

	m_ViewMatrix = CreateViewMatrix();
	SetViewMatrix(m_ViewMatrix);
	
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	m_pGeneralBuffer = CreateVertexBuffer(10*6, m_pDefaultVertDecl->GetStride());

	return true;
}
void 
ASCGLES2Renderer::ReloadDevice()
{
	assert_now("Guts, Not implemented");
}

//Update
bool 
ASCGLES2Renderer::BeginScene()
{
	m_vecDrawInfo.Clear();
	//glClear(GL_COLOR_BUFFER_BIT);
#ifdef ASC_IOS
    [m_pRenderView clearScreen];
#else
	if (!TestEGLError(m_hWnd, "glClear"))
	{
		assert_now( "Guts, glClear Failed" );
		return false;
	}
#endif
	return true;
}
bool 
ASCGLES2Renderer::EndScene()
{
    RenderFonts();
	Ascension::SpriteManager().PostFontRender();
#ifdef ASC_IOS
	[m_pRenderView present];
#else
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
	if (!TestEGLError(m_hWnd, "eglSwapBuffers"))
	{
		assert_now( "Guts, eglSwapBuffers Failed" );
		return false;
	}
#endif
	return true;
}
void 
ASCGLES2Renderer::ShowDrawInfo()
{
	Ascension::Debugging().AddScreenLine("### Draw Info ###");
	Ascension::Debugging().AddScreenLine("");
	Ascension::Debugging().AddScreenLine("Draw Primitive UP Count: ", m_uDrawPrimitiveUPCount);
	Ascension::Debugging().AddScreenLine("Draw Primitive  Count: ", m_uDrawPrimitiveCount);
	Ascension::Debugging().AddScreenLine("Draw Indexed Primitive UP Count: ", m_uDrawIndexedPrimitiveUPCount);
	Ascension::Debugging().AddScreenLine("Draw Indexed Primitive Count: ", m_uDrawIndexedPrimitiveCount);
	Ascension::Debugging().AddScreenLine("");
	for(UINT32 i = 0; i < m_vecDrawInfo.size(); ++i)
	{
		Ascension::Debugging().AddScreenLine(m_vecDrawInfo[i]);
	}
}
void 
ASCGLES2Renderer::AddDrawDebugInfo(ASCString& strLine)
{
	m_vecDrawInfo.push_back( strLine );
}

//Renderering
bool 
ASCGLES2Renderer::DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, const void* pVertices, UINT32 uVertexStride )
{
	uVertexStride;
	switch(eType)
	{
	case PT_TriList:
		{
			m_pGeneralBuffer->Resize((uPrimitiveCount * 3) + 10);
			
			const UINT32* pVerticiesSource = reinterpret_cast<const UINT32*>(pVertices);
			UINT32* pVerticiesTarget = reinterpret_cast<UINT32*>(m_pGeneralBuffer->Lock());
			for(UINT32 i = 0; i < uPrimitiveCount * 3; ++i)
			{
				pVerticiesTarget[i] = pVerticiesSource[i];
			}
			m_pGeneralBuffer->UnLock();
			m_pGeneralBuffer->SetSource();
			glDrawArrays(GL_TRIANGLES, 0, uPrimitiveCount * 3);
			m_pGeneralBuffer->Unbind();
			return true;
		}
	default:
		{
			assert_now("Guts, invalid primitive type");
			return false;
		}
	}
}

bool 
ASCGLES2Renderer::DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount)
{
	switch(eType)
	{
	case PT_TriList:
		{
			glDrawArrays(GL_TRIANGLES, 0, uPrimitiveCount * 3);
			return true;
		}
	default:
		{
			assert_now("Guts, invalid primitive type");
			return false;
		}
	}
}
bool 
ASCGLES2Renderer::DrawIndexedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
									UINT32 uVertexCount, const void* pVertices, 
									UINT32 uVertexStride, const void* pIndices, 
									EIndexType eIndexType )
{
	eType;
	uPrimitiveCount;
	uVertexCount; 
	pVertices; 
	uVertexStride; 
	pIndices; 
	eIndexType;
	assert_now("Guts, Not implemented");
	return false;
}

bool 
ASCGLES2Renderer::DrawBufferedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
										UINT32 uVertexCount, UINT32 uOffset, 
										bool bIndexed )
{
	eType;
	uPrimitiveCount;
	uVertexCount; 
	uOffset;
	bIndexed;
	assert_now("Guts, Not implemented");
	return false;
}
bool 
ASCGLES2Renderer::ColourZClear( UINT32 colClear, FLOAT32 fZValue )
{
	UINT32 uA = colClear >> 24;
	UINT32 uR = colClear >> 16 & 0xff;
	UINT32 uG = colClear >> 8 & 0xff;
	UINT32 uB = colClear & 0xff;
	glClearColor(SC_FLOAT(uR), SC_FLOAT(uG), SC_FLOAT(uB), SC_FLOAT(uA));
	glClearDepthf( fZValue );
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
#ifdef ASC_IOS
#else
	if (!TestEGLError(m_hWnd, "glClear"))
	{
		assert_now( "Guts, glClear Failed" );
		return false;
	}
#endif
	return true;
}
	
//Settings
bool 
ASCGLES2Renderer::SetRenderState( ERenderState eState, UINT32 uValue )
{
	eState;
	uValue;
	assert_now("Guts, Not implemented");
	return false;
}

//General
void 
ASCGLES2Renderer::ApplyDefaultVertDecl()
{
	m_pDefaultVertDecl->Apply();
}
bool 
ASCGLES2Renderer::IsActiveWindow()
{
	assert_now("Guts, Not implemented");
	return false;
}
void* 
ASCGLES2Renderer::GetDevice()
{
	assert_now("Guts, Not implemented");
	return NULL;
}
void* 
ASCGLES2Renderer::GetWindowHandle()
{
#ifdef ASC_IOS
    assert_now("Guts, need this somewhere");
    return NULL;
#else
	return m_hWnd;
#endif
}
ASCVertexDeclaration* 
ASCGLES2Renderer::GetDefaultVertDecl()
{
	return m_pDefaultVertDecl;
}
ASCVertexDeclaration* 
ASCGLES2Renderer::CreateVertDecl(VertElement veElements[], UINT32 uCount)
{
	return new ASCGLES2VertexDeclaration(veElements, uCount);
}
ASCTextureManager* 
ASCGLES2Renderer::GetTextureManager()
{
	return m_pTextureManager;
}
ASCShaderManager*  
ASCGLES2Renderer::GetShaderManager()
{
	return m_pShaderManager;
}
ASCMatrix4 
ASCGLES2Renderer::GetViewMatrix()
{
	return m_ViewMatrix;
}
ASCMatrix4 
ASCGLES2Renderer::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}
void 
ASCGLES2Renderer::SetViewMatrix(ASCMatrix4& mMat)
{
	m_ViewMatrix = mMat;
}
void 
ASCGLES2Renderer::SetProjectionMatrix(ASCMatrix4& mMat)
{
	m_ProjectionMatrix = mMat;
}

#ifdef ASC_WINDOWS
LRESULT WINAPI 
ASCGLES2Renderer::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
		{
            PostQuitMessage( 0 );
            return 0;
		}
		break;
		case WM_KILLFOCUS:
		{
		}
		break;
		case WM_SETFOCUS:
		{
		}
		break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}
#endif
ASCMatrix4 
ASCGLES2Renderer::CreateProjectionMatrix( FLOAT32 fFOV, FLOAT32 fAspect, FLOAT32 fNear, FLOAT32 fFar )
{
	PVRTMat4 mTemp = PVRTMat4::Ortho(0, 0, fFOV, fAspect, fNear, fFar, PVRTMat4::D3D);
	
	ASCMatrix4 matReturn;
	matReturn = *reinterpret_cast<ASCMatrix4*>(&mTemp);
	
	return matReturn;
}
ASCMatrix4 
ASCGLES2Renderer::CreateViewMatrix()
{
	ASCMatrix4 matReturn;
	PVRTMat4 mTemp = PVRTMat4::Identity();

	mTemp.f[0] = SC_FLOAT(Ascension::Height()) / SC_FLOAT(Ascension::Width());
	//mTemp.f[5] *= -1.0f;
	//mTemp.f[14] = 0.0f;

	matReturn = *reinterpret_cast<ASCMatrix4*>(&mTemp);

	return matReturn;
}
ASCVertexBuffer* 
ASCGLES2Renderer::CreateVertexBuffer(UINT32 uSize, UINT32 uStride)
{
	ASCGLES2VertexBuffer* pVertexBuffer = new ASCGLES2VertexBuffer();
	pVertexBuffer->Initialise(uSize, uStride );
	m_vecVertexBuffers.push_back( pVertexBuffer );

	return pVertexBuffer;
}
void 
ASCGLES2Renderer::AddTextLineToFont(ASCFont* pFont, SFontData& sFontData)
{
	for(UINT32 i = 0; i < m_vecFonts.size(); ++i)
	{
		if(m_vecFonts[i] == pFont)
		{
			m_vecTextToRender[i]->push_back(sFontData);
		}
	}
}
ASCFont* 
ASCGLES2Renderer::LoadFont(ASCString strFile)
{
	for(UINT32 i = 0; i < m_vecFonts.size(); ++i)
	{
		if(m_vecFonts[i]->ID() == strFile)
		{
			return m_vecFonts[i];
		}
	}
	ASCFont* pFont = new ASCFont(strFile);

	m_vecFonts.push_back( pFont );

	m_vecTextToRender.push_back( new ASCVector<SFontData>() );
	return pFont;
}
void 
ASCGLES2Renderer::RenderFonts()
{
	for(UINT32 i = 0; i < m_vecFonts.size(); ++i)
	{
		for(UINT32 j = 0; j < m_vecTextToRender[i]->size(); ++j)
		{
			SFontData& sData = m_vecTextToRender[i]->at(j);

			m_vecFonts[i]->Render(sData.fX, sData.fY, sData.strMessage, sData.iColour, sData.bCentered, sData.bRight, sData.iColour2);
		}
		m_vecTextToRender[i]->Clear();
	}
}
void 
ASCGLES2Renderer::ClearFontBuffer()
{
	for(UINT32 i = 0; i < m_vecTextToRender.size(); ++i)
	{
		m_vecTextToRender[i]->Clear();
	}
}
ASCVector2 
ASCGLES2Renderer::GetWorldPosition(FLOAT32 fX, FLOAT32 fY)
{
	fX;
	fY;
	ASCVector2 vReturn;
	
	assert_now("Guts, Not implemented");
	return vReturn;
}
void 
ASCGLES2Renderer::CalibratePicking()
{
	//assert_now("Guts, Not implemented");
}
#ifdef ASC_WINDOWS
bool 
ASCGLES2Renderer::TestEGLError(HWND hWnd, char* pszLocation)
{
	pszLocation;
	hWnd;
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		return false;
	}

	return true;
}
#endif

UINT32 
ASCGLES2Renderer::CurrentVertexStride()
{
	return m_uVertexStride;
}

void 
ASCGLES2Renderer::SetVertexStride(UINT32 uVal)
{
	m_uVertexStride = uVal;
}

#endif