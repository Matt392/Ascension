/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2Renderer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2RENDERER_H__Included_113010662
#define _ASCGLES2RENDERER_H__Included_113010662

#ifdef ASC_RENDERER_GLES2

#include "ASCRenderer.h"

#ifdef ASC_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/EAGL.h>


#import "ASCRenderView.h"

typedef void* EGLDisplay;
typedef void* EGLSurface;
typedef void* EGLContext;

typedef SINT32 EGLint;

#else
#include <windows.h>
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#endif


namespace ASC
{
	class ASCVertexDeclaration;
	class ASCTextureManager;
	class ASCShaderManager;

	class ASCGLES2Renderer : public ASCRenderer
	{
	public:
		ASCGLES2Renderer(void);
		~ASCGLES2Renderer(void);

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
#ifdef ASC_WINDOWS
		static LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
#endif
		
		ASCMatrix4 CreateProjectionMatrix( FLOAT32 fFOV, FLOAT32 fAspect, FLOAT32 fNear, FLOAT32 fFar );
		ASCMatrix4 CreateViewMatrix();

		virtual ASCVertexBuffer* CreateVertexBuffer(UINT32 uSize, UINT32 uStride);
		

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
		
#ifdef ASC_WINDOWS
		bool TestEGLError(HWND hWnd, char* pszLocation);
#endif
#ifdef ASC_IOS
        virtual void InitForIOS( void* pRenderView );
#endif

	protected:
		
#ifdef ASC_IOS
		EGLDisplay m_eglDisplay;
		EGLSurface m_eglSurface;
		EGLContext m_eglContext;
        ASCRenderView* m_pRenderView;
#else
		EGLDisplay m_eglDisplay;
		EGLConfig m_eglConfig;
		EGLSurface m_eglSurface;
		EGLContext m_eglContext;
		EGLNativeWindowType m_eglWindow;

		HINSTANCE m_hInst;					
		HWND m_hWnd;
		HDC m_hDC;
#endif

		ASCVertexDeclaration* m_pDefaultVertDecl;
		ASCTextureManager* m_pTextureManager;
		ASCShaderManager* m_pShaderManager;
		
		ASCMatrix4 m_ProjectionMatrix;
		ASCMatrix4 m_ViewMatrix;

		ASCVertexBuffer* m_pGeneralBuffer;
		
		UINT32 m_uDrawPrimitiveUPCount;
		UINT32 m_uDrawPrimitiveCount;
		UINT32 m_uDrawIndexedPrimitiveUPCount;
		UINT32 m_uDrawIndexedPrimitiveCount;

		UINT32 m_uVertexStride;

		ASCVector<ASCVertexBuffer*> m_vecVertexBuffers;
		ASCVector<ASCFont*> m_vecFonts;
		ASCVector<ASCVector<SFontData>*> m_vecTextToRender;

		ASCVector<ASCString> m_vecDrawInfo;
	};

}

#endif

#endif //_ASCGLES2RENDERER_H__Included_113010662

