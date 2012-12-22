/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCDX9Renderer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDX9RENDERER_H__Included_251702026
#define _ASCDX9RENDERER_H__Included_251702026

#ifdef ASC_RENDERER_DX9

#include "ASCRenderer.h"
#include <d3dx9.h>

namespace ASC
{
	static const D3DRENDERSTATETYPE 
	ERS_TO_D3DRS[RS_Count] =	{	D3DRS_ZFUNC,
									D3DRS_ZWRITEENABLE,	
									D3DRS_ALPHABLENDENABLE,
									D3DRS_ALPHATESTENABLE,
									D3DRS_ALPHAREF,
									D3DRS_ALPHAFUNC,
									D3DRS_SRCBLEND,	
									D3DRS_DESTBLEND,
									D3DRS_POINTSPRITEENABLE,
									D3DRS_POINTSCALEENABLE,
									D3DRS_POINTSIZE,
									D3DRS_POINTSIZE_MIN,
									D3DRS_POINTSCALE_A,
									D3DRS_POINTSCALE_B,
									D3DRS_POINTSCALE_C,
									D3DRS_SRCBLEND,
									D3DRS_DESTBLEND,
									D3DRS_LIGHTING,
									D3DRS_SPECULARENABLE,
									D3DRS_FILLMODE, 
									D3DRS_CULLMODE,			
								};	
	const static D3DCMPFUNC
	ECF_TO_D3DCF[] =	{			D3DCMP_NEVER,
									D3DCMP_LESS,
									D3DCMP_LESSEQUAL,
									D3DCMP_EQUAL,
									D3DCMP_GREATEREQUAL,
									D3DCMP_GREATER,
									D3DCMP_ALWAYS,
						};
	const static D3DBLEND 
	EBL_TO_D3DRS[] =	{			D3DBLEND_ZERO,
									D3DBLEND_ONE,
									D3DBLEND_SRCCOLOR,
									D3DBLEND_INVSRCCOLOR,
									D3DBLEND_SRCALPHA,
									D3DBLEND_INVSRCALPHA,
									D3DBLEND_DESTALPHA,
									D3DBLEND_INVDESTALPHA,
									D3DBLEND_DESTCOLOR,
									D3DBLEND_INVDESTCOLOR,
									D3DBLEND_SRCALPHASAT,
									D3DBLEND_BOTHSRCALPHA,
									D3DBLEND_BOTHINVSRCALPHA,
									D3DBLEND_BLENDFACTOR,
									D3DBLEND_INVBLENDFACTOR,
									D3DBLEND_SRCCOLOR2,
									D3DBLEND_INVSRCCOLOR2
						};

	class ASCVertexDeclaration;
	class ASCTextureManager;
	class ASCShaderManager;

	class ASCDX9Renderer : public ASCRenderer
	{
	public:
		ASCDX9Renderer(void);
		~ASCDX9Renderer(void);

		//Initialisation
		bool Initialise( SRendererInit& rParameters );
		void ReloadDevice();

		//Update
		bool BeginScene();
		bool EndScene();


		//Renderering
		bool DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, const void* pVertices, UINT32 uVertexStride );

		bool DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount);

		bool DrawIndexedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
											UINT32 uVertexCount, const void* pVertices, 
											UINT32 uVertexStride, const void* pIndices, 
											EIndexType eIndexType );

		bool DrawBufferedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
											 UINT32 uVertexCount, UINT32 uOffset, 
											 bool bIndexed = true );
		bool ColourZClear( UINT32 colClear, FLOAT32 fZValue );
	
		//Settings
		bool SetRenderState( ERenderState eState, UINT32 uValue );

		//General
		void ApplyDefaultVertDecl();
		bool IsActiveWindow();
		void* GetDevice();
		void* GetWindowHandle();
		ASCVertexDeclaration* GetDefaultVertDecl();
		ASCVertexDeclaration* CreateVertDecl(VertElement veElements[], UINT32 uCount);

		ASCTextureManager* GetTextureManager();
		ASCShaderManager* GetShaderManager();
		
		ASCMatrix4 GetViewMatrix();
		ASCMatrix4 GetProjectionMatrix();
		void SetViewMatrix(ASCMatrix4& mMat);
		void SetProjectionMatrix(ASCMatrix4& mMat);

		//Other
		static LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		
		ASCMatrix4 CreateProjectionMatrix( FLOAT32 fFOV, FLOAT32 fAspect, FLOAT32 fNear, FLOAT32 fFar );
		ASCMatrix4 CreateViewMatrix();

		virtual ASCVertexBuffer* CreateVertexBuffer(UINT32 uSize, UINT32 uStride);
		virtual UINT32 VertexMemUsed();

		virtual void AddTextLineToFont(ASCFont* pFont, SFontData& sFontData);
		virtual ASCFont* LoadFont(ASCString strFile);
		virtual void ClearFontBuffer();

		void RenderFonts();

		virtual ASCVector2 GetWorldPosition(FLOAT32 fX, FLOAT32 fY);

		virtual void CalibratePicking();

		virtual void ShowDrawInfo();
		virtual void AddDrawDebugInfo(ASCString& strLine);

		virtual UINT32 CurrentVertexStride();
		virtual void SetVertexStride(UINT32 uVal);

	protected:
		IDirect3D9* m_pD3D;						
		IDirect3DDevice9* m_pDevice;					
		HINSTANCE m_hInst;					
		HWND m_hWnd;
		D3DPRESENT_PARAMETERS m_d3dPP;

		ASCVertexDeclaration* m_pDefaultVertDecl;
		ASCTextureManager* m_pTextureManager;
		ASCShaderManager* m_pShaderManager;
		
		ASCMatrix4 m_ProjectionMatrix;
		ASCMatrix4 m_ViewMatrix;
		
		UINT32 m_uDrawPrimitiveUPCount;
		UINT32 m_uDrawPrimitiveCount;
		UINT32 m_uDrawIndexedPrimitiveUPCount;
		UINT32 m_uDrawIndexedPrimitiveCount;

		ASCVector<ASCVertexBuffer*> m_vecVertexBuffers;
		ASCVector<ASCFont*> m_vecFonts;
		ASCVector<ASCVector<SFontData>*> m_vecTextToRender;

		ASCVector<ASCString> m_vecDrawInfo;
	};

}

#endif

#endif //_ASCDX9RENDERER_H__Included_251702026
