#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9Renderer.h"
#include "ASCConversions.h"
#include "ASCDX9VertexDeclaration.h"
#include "ASCProfilingSystem.h"
#include "..\Ascension\Ascension.h"
#include "ASCDX9TextureManager.h"
#include "ASCDX9ShaderManager.h"
#include "ASCDX9VertexBuffer.h"
#include "ASCFont.h"
#include "ASCSprite.h"
#include "ASCSpriteManager.h"

#include <xnamath.h>


ASCDX9Renderer::ASCDX9Renderer(void)
: m_pD3D ( NULL )				
, m_pDevice ( NULL )
, m_hInst ( 0 )
, m_hWnd ( 0 )
, m_uDrawPrimitiveUPCount ( 0 )
, m_uDrawPrimitiveCount ( 0 )
, m_uDrawIndexedPrimitiveUPCount ( 0 )
, m_uDrawIndexedPrimitiveCount ( 0 )
, m_pDefaultVertDecl ( NULL )
, m_pTextureManager ( NULL )
, m_pShaderManager ( NULL )
{
}


ASCDX9Renderer::~ASCDX9Renderer(void)
{
	UnregisterClassW(  L"AscensionWindow", m_hInst );

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

	//Release the device:
	SafeRelease( m_pDevice );
	//Release DirectX
	SafeRelease( m_pD3D );

	SafeDelete( m_pDefaultVertDecl );
	SafeDelete( m_pTextureManager );
	SafeDelete( m_pShaderManager );
}

//Initialisation
bool 
ASCDX9Renderer::Initialise( SRendererInit& rParameters )
{
	WNDCLASSEX wc = 
	{ 
		sizeof(WNDCLASSEXA), 
		CS_CLASSDC,
		ASCDX9Renderer::MsgProc, 
		0L, 0L, 
		GetModuleHandle(NULL), 
		NULL, NULL, 
		NULL, NULL,
#ifdef _DEBUG
		"AscensionWindow",
#else
		L"AscensionWindow",
#endif
		NULL 
	};
	RegisterClassEx( &wc );

	LPCWSTR lstrWindowTitle = CharStrToLPCWSTR(rParameters.m_strWindowName.c_str());

	RECT rc = { 0, 0, rParameters.m_uScreenWidth, rParameters.m_uScreenHeight };

	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, false );

	m_hWnd = CreateWindowW(		L"AscensionWindow",
								lstrWindowTitle,
								WS_OVERLAPPEDWINDOW - (WS_MAXIMIZEBOX|WS_THICKFRAME), 
								CW_USEDEFAULT, CW_USEDEFAULT, 
								rc.right - rc.left, 
								rc.bottom - rc.top, 
								GetDesktopWindow(), NULL, 
								wc.hInstance, NULL );
	
	assert_msg(m_hWnd != NULL, "Guts, Failed to create window");
	
	UINT32 uScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
	UINT32 uScreenY = GetSystemMetrics(SM_CYFULLSCREEN);

	UINT32 uX = (uScreenX / 2) - (rParameters.m_uScreenWidth / 2);
	UINT32 uY = (uScreenY / 2) - (rParameters.m_uScreenHeight / 2);

	SetWindowPos(m_hWnd, NULL, uX, uY, 0, 0, SWP_NOSIZE);

	if ( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		assert_now("Guts, Failed to create D3D9");
		return false;
	}
	
	//Window parameters
	ZeroMemory( &m_d3dPP, sizeof( m_d3dPP ) );
	
	m_d3dPP.Windowed = rParameters.m_bWindowMode;
	m_d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dPP.EnableAutoDepthStencil = TRUE;
	m_d3dPP.AutoDepthStencilFormat = D3DFMT_D24X8;
	m_d3dPP.BackBufferWidth = rParameters.m_uScreenWidth;
	m_d3dPP.BackBufferHeight = rParameters.m_uScreenHeight;

	UINT32 AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL;
	for (UINT32 Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = m_pD3D->GetAdapterIdentifier(Adapter,0,&Identifier);
		if (strstr(Identifier.Description,"PerfHUD") != 0)
		{
			AdapterToUse=Adapter;
			DeviceType=D3DDEVTYPE_REF;
			break;
		}
	}
	
	if ( D3D_OK != m_pD3D->CreateDevice( AdapterToUse, DeviceType, 
										m_hWnd,
										 D3DCREATE_HARDWARE_VERTEXPROCESSING,
										&m_d3dPP, &m_pDevice ) ) 
	{
		assert_now("Guts, Failed to create D3D9 device");
		return false;
	}
	
	// Show the window
	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hWnd );

	m_hInst = wc.hInstance;
	
	VertElement sDeclElem[] = {	VertElement::Init(0,	ES_POSITION,	0, ET_FLOAT32,	4),
								VertElement::Init(16,	ES_COLOUR,		0, ET_UINT8,		4),
								VertElement::Init(20,	ES_TANGENT,		0, ET_UINT8,		4),
								VertElement::Init(24,	ES_TEXTURE,		0, ET_FLOAT32,	2),
								VertElement::Init(32,	ES_NORMAL,		0, ET_FLOAT32,	3),
								VertElement::Init(44,	ES_BINORMAL,	0, ET_FLOAT32,	3),};

	m_pDefaultVertDecl = new ASCDX9VertexDeclaration(sDeclElem, 6, m_pDevice);
	m_pDefaultVertDecl->Apply();

	D3DCAPS9 pCaps;
	m_pD3D->GetDeviceCaps(AdapterToUse, DeviceType, &pCaps);
	if (pCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	SetRenderState( RS_bAlphaBlendEnable, true );
	SetRenderState( RS_bAlphaBlendEnable, true );
	SetRenderState( RS_eAlphaSrcBlend, AB_SrcAlpha );
	SetRenderState( RS_eDestBlend, AB_InvSrcAlpha );
	
	m_pTextureManager = new ASCDX9TextureManager( m_pDevice );
	m_pShaderManager = new ASCDX9ShaderManager( m_pDevice );

	m_ProjectionMatrix = CreateProjectionMatrix( SC_FLOAT(rParameters.m_uScreenWidth), SC_FLOAT(rParameters.m_uScreenHeight), rParameters.m_fNear, rParameters.m_fFar);
	SetProjectionMatrix(m_ProjectionMatrix);

	m_ViewMatrix = CreateViewMatrix();
	SetViewMatrix(m_ViewMatrix);

	return true;
}
void 
ASCDX9Renderer::ReloadDevice()
{
	while(FAILED(m_pDevice->Reset( &m_d3dPP )))
	{
		while ( D3DERR_DEVICENOTRESET !=  m_pDevice->TestCooperativeLevel() )
		{
			//Busy waiting loop...
			ASCDebuggingManager::OutputWindowLine("ASC WARNING: Trying to reload the device");
			Sleep( 100 );
		}
	}
		
	m_pTextureManager->Reload();
	m_pShaderManager->Reload();
}

//Update
bool 
ASCDX9Renderer::BeginScene()
{
	m_vecDrawInfo.Clear();
	m_uDrawPrimitiveUPCount = 0;
	m_uDrawPrimitiveCount = 0;
	m_uDrawIndexedPrimitiveUPCount = 0;
	m_uDrawIndexedPrimitiveCount = 0;
	if(D3D_OK != m_pDevice->BeginScene())
	{
		assert_now("Guts, Failed to begin sceen");
		return false;
	}
	return true;
}
bool 
ASCDX9Renderer::EndScene()
{
	RenderFonts();
	Ascension::SpriteManager().PostFontRender();

	if(D3D_OK != m_pDevice->EndScene())
	{
		assert_now("Failed to end sceen");
		return false;
	}

	if(D3DERR_DEVICELOST == m_pDevice->Present( NULL, NULL, NULL, NULL ))
	{
		ReloadDevice();
	}

	return true;
}
void 
ASCDX9Renderer::ShowDrawInfo()
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
ASCDX9Renderer::AddDrawDebugInfo(ASCString& strLine)
{
	m_vecDrawInfo.push_back( strLine );
}

//Renderering
bool 
ASCDX9Renderer::DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, const void* pVertices, UINT32 uVertexStride )
{
	++m_uDrawPrimitiveUPCount;
	//Retrieve the D3D type of primitive to render:
	static const D3DPRIMITIVETYPE elutPrimType [] = 
	{
		D3DPT_LINELIST,
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_POINTLIST,
	};
	
	assert_msg(eType < PT_Count, "Guts, Primitive type not supported");

	//Render the primitive.
	if( D3D_OK != m_pDevice->DrawPrimitiveUP( elutPrimType[eType], uPrimitiveCount, pVertices, uVertexStride ) ) 
	{
		assert_now( "Guts, Failed to draw primitiveUP" );
		return false;
	}
	return true;
}

bool 
ASCDX9Renderer::DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount)
{
	++m_uDrawPrimitiveCount;
	//Retrieve the D3D type of primitive to render:
	static const D3DPRIMITIVETYPE elutPrimType [] = 
	{
		D3DPT_LINELIST,
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_POINTLIST,
	};
	
	assert_msg(eType < PT_Count, "Guts, Primitive type not supported");

	//Render the primitive.
	if( D3D_OK != m_pDevice->DrawPrimitive( elutPrimType[eType], 0, uPrimitiveCount) ) 
	{
		assert_now( "Guts, Failed to draw primitive" );
		return false;
	}
	return true;
}
bool 
ASCDX9Renderer::DrawIndexedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
									UINT32 uVertexCount, const void* pVertices, 
									UINT32 uVertexStride, const void* pIndices, 
									EIndexType eIndexType )
{
	++m_uDrawIndexedPrimitiveUPCount;
	//Retrieve the D3D type of primitive to render:
	static const D3DPRIMITIVETYPE elutPrimType [] = 
	{
		D3DPT_LINELIST,
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_POINTLIST,
	};

	static const D3DFORMAT d3dFMT[] = 
	{
		D3DFMT_INDEX16,
		D3DFMT_INDEX32,	
	};

	return D3D_OK == m_pDevice->DrawIndexedPrimitiveUP( elutPrimType[eType],0,uVertexCount, uPrimitiveCount, 
		pIndices, d3dFMT[eIndexType], pVertices, uVertexStride );
}

bool 
ASCDX9Renderer::DrawBufferedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
										UINT32 uVertexCount, UINT32 uOffset, 
										bool bIndexed )
{
	++m_uDrawIndexedPrimitiveCount;
	//Retrieve the D3D type of primitive to render:
	static const D3DPRIMITIVETYPE elutPrimType [] = 
	{
		D3DPT_LINELIST,
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_POINTLIST,
	};

	static const D3DFORMAT d3dFMT[] = 
	{
		D3DFMT_INDEX16,
		D3DFMT_INDEX32,	
	};
	
	if(bIndexed)
	{
		return D3D_OK == m_pDevice->DrawIndexedPrimitive(elutPrimType[eType],0,0,uVertexCount,uOffset,uPrimitiveCount);
	}
	else
	{
		return D3D_OK == m_pDevice->DrawPrimitive(elutPrimType[eType],uOffset,uPrimitiveCount);
	}
}
bool 
ASCDX9Renderer::ColourZClear( UINT32 colClear, FLOAT32 fZValue )
{
	if(D3D_OK == m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, colClear, 1.0f, 0 ))
	{
		if(D3D_OK == m_pDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, NULL, fZValue, NULL ))
		{
			return true;
		}
	}
	return false;
}
	
//Settings
bool 
ASCDX9Renderer::SetRenderState( ERenderState eState, UINT32 uValue )
{
	D3DRENDERSTATETYPE d3dRS = ERS_TO_D3DRS[eState];

	if ( D3DRS_ALPHAFUNC == d3dRS || D3DRS_ZFUNC == d3dRS )
	{
		return m_pDevice->SetRenderState( d3dRS, ECF_TO_D3DCF[ uValue ] ) == D3D_OK;
	}

	if ( D3DRS_SRCBLEND == d3dRS || D3DRS_DESTBLEND == d3dRS )
	{
		return m_pDevice->SetRenderState( d3dRS, EBL_TO_D3DRS[uValue] ) == D3D_OK;
	}

	return m_pDevice->SetRenderState( d3dRS, uValue ) == D3D_OK;
}

//General
void 
ASCDX9Renderer::ApplyDefaultVertDecl()
{
	m_pDefaultVertDecl->Apply();
}
bool 
ASCDX9Renderer::IsActiveWindow()
{
	assert_now("Guts, Not implemented");
	return false;
}
void* 
ASCDX9Renderer::GetDevice()
{
	return m_pDevice;
}
void* 
ASCDX9Renderer::GetWindowHandle()
{
	return m_hWnd;
}
ASCVertexDeclaration* 
ASCDX9Renderer::GetDefaultVertDecl()
{
	return m_pDefaultVertDecl;
}
ASCVertexDeclaration* 
ASCDX9Renderer::CreateVertDecl(VertElement veElements[], UINT32 uCount)
{
	return new ASCDX9VertexDeclaration(veElements, uCount, m_pDevice);
}
ASCTextureManager* 
ASCDX9Renderer::GetTextureManager()
{
	return m_pTextureManager;
}
ASCShaderManager*  
ASCDX9Renderer::GetShaderManager()
{
	return m_pShaderManager;
}
ASCMatrix4 
ASCDX9Renderer::GetViewMatrix()
{
	return m_ViewMatrix;
	//D3DXMATRIX mTransform;
	//ASCMatrix4 mReturn;
	//if( D3D_OK == m_pDevice->GetTransform( D3DTS_VIEW, &mTransform ))
	//{
	//	mReturn = *reinterpret_cast<ASCMatrix4*>( &mTransform );
	//}
	//return mReturn;
}
ASCMatrix4 
ASCDX9Renderer::GetProjectionMatrix()
{
	D3DXMATRIX mTransform;
	ASCMatrix4 mReturn;
	if( D3D_OK == m_pDevice->GetTransform( D3DTS_PROJECTION, &mTransform ))
	{
		mReturn = *reinterpret_cast<ASCMatrix4*>( &mTransform );
	}
	return mReturn;
}
void 
ASCDX9Renderer::SetViewMatrix(ASCMatrix4& mMat)
{
	m_ViewMatrix = mMat;
	//D3DXMATRIX mTransform = *reinterpret_cast<D3DXMATRIX*>( &m_ViewMatrix );
	//if( D3D_OK != m_pDevice->SetTransform( D3DTS_PROJECTION, &mTransform ))
	//{
	//	assert_now("Guts, Faild to set view matrix");
	//}
}
void 
ASCDX9Renderer::SetProjectionMatrix(ASCMatrix4& mMat)
{
	m_ProjectionMatrix = mMat;
	//D3DXMATRIX mTransform = *reinterpret_cast<D3DXMATRIX*>( &m_ProjectionMatrix );
	//if( D3D_OK != m_pDevice->SetTransform( D3DTS_PROJECTION, &mTransform ))
	//{
	//	assert_now("Guts, Faild to set projection matrix");
	//}
}

LRESULT WINAPI 
ASCDX9Renderer::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
ASCMatrix4 
ASCDX9Renderer::CreateProjectionMatrix( FLOAT32 fFOV, FLOAT32 fAspect, FLOAT32 fNear, FLOAT32 fFar )
{
	//Assume a LH perspective matrix

	ASCMatrix4 matReturn;
	XMMATRIX matResult;

	//matResult = XMMatrixPerspectiveFovLH(  DegsToRad(fFOV), fAspect, fNear, fFar );
	matResult = XMMatrixOrthographicLH(  fFOV, fAspect, fNear, fFar );

	matReturn = *reinterpret_cast<ASCMatrix4*>(&matResult);

	return matReturn;
}
ASCMatrix4 
ASCDX9Renderer::CreateViewMatrix()
{
	ASCMatrix4 matReturn;

	XMMATRIX mView = XMMatrixIdentity();

	mView._11 = SC_FLOAT(Ascension::Height()) / SC_FLOAT(Ascension::Width());
	mView._22 *= -1.0f;
	mView._43 = 0.0f;

	matReturn = *reinterpret_cast<ASCMatrix4*>(&mView);

	return matReturn;
}
ASCVertexBuffer* 
ASCDX9Renderer::CreateVertexBuffer(UINT32 uSize, UINT32 uStride)
{
	ASCDX9VertexBuffer* pVertexBuffer = new ASCDX9VertexBuffer();
	pVertexBuffer->Initialise(uSize, uStride );
	m_vecVertexBuffers.push_back( pVertexBuffer );


	return pVertexBuffer;
}
UINT32 
ASCDX9Renderer::VertexMemUsed()
{
	UINT32 uSize = 0;

	for(UINT32 i = 0; i < m_vecVertexBuffers.size(); ++i)
	{
		uSize += m_vecVertexBuffers[i]->MemSize();
	}

	return uSize;
}
void 
ASCDX9Renderer::AddTextLineToFont(ASCFont* pFont, SFontData& sFontData)
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
ASCDX9Renderer::LoadFont(ASCString strFile)
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
ASCDX9Renderer::RenderFonts()
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
ASCDX9Renderer::ClearFontBuffer()
{
	for(UINT32 i = 0; i < m_vecTextToRender.size(); ++i)
	{
		m_vecTextToRender[i]->Clear();
	}
	//m_vecDrawInfo.Clear();
}
ASCVector2 
ASCDX9Renderer::GetWorldPosition(FLOAT32 fX, FLOAT32 fY)
{
#define CustomPickingThing
	ASCVector2 vReturn;
	vReturn.Set( fX, fY );

#ifdef CustomPickingThing

	vReturn.Set( fX - ((SC_FLOAT(Ascension::Width())+7.5f) * 0.5f), fY - (SC_FLOAT(Ascension::Height()) * 0.5f) );
	//vReturn.Set((vReturn.X() / 38.0f) - 0.079f, (vReturn.Y() / 38.5f) - 0.143f);
	vReturn.Set((vReturn.X() / 38.6f) - 0.0f, (vReturn.Y() / 38.8f) - 0.0f);
#else
	XMMATRIX matView = *reinterpret_cast<XMMATRIX*>(&m_ViewMatrix);
	XMMATRIX matProj = *reinterpret_cast<XMMATRIX*>(&m_ProjectionMatrix);
	XMVECTOR vecSource;
	XMVECTOR vecTransformed;
	matProj = XMMatrixInverse(&vecSource, matProj);
	matView = XMMatrixInverse(&vecSource, matView);
	XMMATRIX matTransform = matView;//XMMatrixMultiply( matView, matProj );
	
	vecSource.m128_f32[0] = fX;
	vecSource.m128_f32[1] = fY;
	vecSource.m128_f32[2] = 0.0f;
	vecSource.m128_f32[3] = 0.0f;
	vecTransformed = XMVector2Transform(vecSource, matTransform);

	vReturn.Set( vecTransformed.m128_f32[0], vecTransformed.m128_f32[1] );
#endif

	return vReturn;
}
void 
ASCDX9Renderer::CalibratePicking()
{
//	ASCSprite* pSprite = Ascension::SpriteManager().CreateSpriteFile("Sprites/CalibrationSprite.ascspr");
//	pSprite->SetPosition(0.0f, 0.0f);
//	pSprite->Update();
//	SVertex sVert;
//	ASCVector2 v2TopLeft;
//	XMMATRIX matView = *reinterpret_cast<XMMATRIX*>(&m_ViewMatrix);
//	XMMATRIX matProj = *reinterpret_cast<XMMATRIX*>(&m_ProjectionMatrix);
//	XMMATRIX matTransform = XMMatrixMultiply( matView, matProj );
//	XMVECTOR vecSource;
//	XMVECTOR vecTransformed;
//	FLOAT32 fX;
//	FLOAT32 fY;
//
//	sVert = pSprite->GetVertex(0);
//	vecSource.m128_f32[0] = sVert.m_fX / 10.0f;
//	vecSource.m128_f32[1] = sVert.m_fY / 10.0f;
//	vecSource.m128_f32[2] = 0.0f;
//	vecSource.m128_f32[3] = 0.0f;
//	vecTransformed = XMVector4Transform(vecSource, matTransform);
//	vecTransformed.m128_f32[0] *= 0.1f;
//	vecTransformed.m128_f32[1] *= 0.1f;
//	
//	fX = vecTransformed.m128_f32[1] * SC_FLOAT(Ascension::Width());
//	fY = vecTransformed.m128_f32[0] * SC_FLOAT(Ascension::Height());
//	v2TopLeft.Set( fX - (SC_FLOAT(Ascension::Width()) * 0.5f), fY - (SC_FLOAT(Ascension::Height()) * 0.5f) );
//
//
//	sVert = pSprite->GetVertex(2);
//	vecSource.m128_f32[0] = sVert.m_fX;// / 10.0f;
//	vecSource.m128_f32[1] = sVert.m_fY;// / 10.0f;
//	vecSource.m128_f32[2] = 0.0f;
//	vecSource.m128_f32[3] = 0.0f;
//	vecTransformed = XMVector4Transform(vecSource, matTransform);
//	vecTransformed.m128_f32[0] *= 0.1f;
//	vecTransformed.m128_f32[1] *= 0.1f;
//	
//	fX = vecTransformed.m128_f32[1] * SC_FLOAT(Ascension::Width());
//	fY = vecTransformed.m128_f32[0] * SC_FLOAT(Ascension::Height());
//	v2TopLeft.Set( fX - (SC_FLOAT(Ascension::Width()) * 0.5f), fY - (SC_FLOAT(Ascension::Height()) * 0.5f) );
//	
//	//m_pSpriteVerts[0].Init(100.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVL);//UL
//	//m_pSpriteVerts[1].Init(356.0f,	100.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVL);//UR
//	//m_pSpriteVerts[2].Init(356.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUR, sInfo.m_fVR);//LR
//	//m_pSpriteVerts[5].Init(100.0f,	356.0f,	0.0f, 0xFFFFFFFF, sInfo.m_fUL, sInfo.m_fVR);//LL
}

UINT32 
ASCDX9Renderer::CurrentVertexStride()
{
	return 0;
}
void 
ASCDX9Renderer::SetVertexStride(UINT32 uVal)
{
	assert_now("Gut, Not implemented");
	uVal;
}

#endif