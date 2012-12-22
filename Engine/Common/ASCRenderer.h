/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCRenderer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCRENDERER_H__Included_156427159
#define _ASCRENDERER_H__Included_156427159

#include "ASCGenInc.h"
#include "ASCVertexDeclaration.h"

namespace ASC
{

	enum EIndexType
	{
		IT_Invalid = -1,
		IT_16,
		IT_32,
		IT_Count,
	};
	enum EPrimitiveType
	{
		PT_LineList = 0,	
		PT_TriList,			
		PT_TriStrip,	
		PT_PointList,		
		PT_Count,
	};
	enum ERenderState
	{
		RS_eZTest = 0,	
		RS_bZWrite,		
		RS_bAlphaBlendEnable,
		RS_bAlphaTest,
		RS_eAlphaRef,
		RS_eAlphaFunc,
		RS_eAlphaSrcBlend,
		RS_eAlphaDestBlend,
		RS_bPointSpriteEnable,
		RS_bPointSpriteScaleEnable,
		RS_uPointSpriteSize,
		RS_uPointSpriteMinSize,
		RS_uPointSpriteScaleA,
		RS_uPointSpriteScaleB,
		RS_uPointSpriteScaleC,
		RS_eSrcBlend,
		RS_eDestBlend,
		RS_bLightingEnable,
		RS_bSpecularEnable,
		RS_eFillMode,
		RS_eCullMode,	
		RS_Count		
	};
	enum EAlphaBlendStates
	{
		AB_Zero = 0,		//<<! Blend factor is (0,0,0,0)
		AB_One,				//<<! Blend factor is (1,1,1,1)
		AB_SrcColor,		//<<! Blend factor is (Rs,Gs,Bs,As)
		AB_InvSrcColor,		//<<! Blend factor is (1-Rs, 1-Gs, 1-Bs, 1-As)
		AB_SrcAlpha,		//<<! Blend factor is (As, As, As, As)
		AB_InvSrcAlpha,		//<<! Blend factor is (1-As, 1-As, 1-As, 1-As)
		AB_DestAlpha,		//<<! Blend factor is (Ad, Ad, Ad, Ad)
		AB_InvDestAlpha,	//<<! Blend factor is (1-Ad, 1-Ad, 1-Ad, 1-Ad)
		AB_DestColor,		//<<! Blend factor is (Rd, Gd, Bd, Ad)
		AB_InvDestColor,	//<<! Blend factor is (1-Rd, 1-Gd, 1-Bd, 1-Ad)
		AB_SrcAlphaSat,		//<<! Blend factor is (f, f, f, 1)	where f = min(As, 1-Ad)	
		AB_BothSrcAlpha,	//<<! OBSELETE - Alternative is to set source and dest to D3DBLEND_SRCALPHA and D3DBLEND_INVSRCALPHA.
		AB_BothInvSrcAlpha, //<<! Blend factor is Source blend factor is (1 - As, 1 - As, 1 - As, 1 - As), and destination blend factor is (As, As, As, As)
		AB_BlendFactor,		//<<! Constant color blending factor used by the frame-buffer blender. This blend mode is supported only if D3DPBLENDCAPS_BLENDFACTOR is set in the SrcBlendCaps or DestBlendCaps members of D3DCAPS9.
		AB_InvBlendFactor,	//<<! Inverted constant color-blending factor used by the frame-buffer blender. Requirements are the same as above.
		AB_SrcColor2,		//<<! Blend factor is (PSOutColor[1]r, PSOutColor[1]g, PSOutColor[1]b, not used). 
		AB_InvSrcColor2,	//<<! Blend factor is (1 - PSOutColor[1]r, 1 - PSOutColor[1]g, 1 - PSOutColor[1]b, not used)). 
		AB_Count
	};

	struct SRendererInit
	{
		UINT32 m_uScreenWidth;
		UINT32 m_uScreenHeight;

		bool m_bWindowMode;

		ASCString m_strWindowName;
		ASCString m_strTextureAtlasDir;

		FLOAT32 m_fFOV;
		FLOAT32 m_fNear;
		FLOAT32 m_fFar;

		void* m_hInstance;
        void* m_pIOSGLView;

		SRendererInit()
		{
			m_uScreenWidth = 1024;
			m_uScreenHeight = 768;
		
			m_fFOV = 70.0f;
			m_fNear = 0.0f;
			m_fFar = 100.0f;

			m_bWindowMode = true;

			m_strWindowName = "Ascension Window";
			m_strTextureAtlasDir = "";

            m_pIOSGLView = NULL;
			m_hInstance = NULL;
		}
	};

	struct SFontData
	{
		 FLOAT32 fX;
		 FLOAT32 fY;
		 ASCString strMessage;
		 UINT32 iColour;
		 UINT32 iColour2;
		 bool bCentered;
		 bool bRight;
		 SFontData()
		 {
			 bRight = false;
		 }
		 void Alpha(FLOAT32 fAlpha, bool bColourOne = true)
		 {
			CHAR8* pColour = NULL;
			if(bColourOne)
			{
				pColour = reinterpret_cast<CHAR8*>(&iColour);
			}
			else
			{
				pColour = reinterpret_cast<CHAR8*>(&iColour2);
			}

			pColour[3] = static_cast<CHAR8>(255.0f * fAlpha);
		 }
	};

	class ASCVertexDeclaration;
	class ASCTextureManager;
	class ASCShaderManager;
	class ASCVertexBuffer;
	class ASCFont;
	class ASCVideoManager;
    

	class ASCRenderer
	{
	public:
		virtual ~ASCRenderer()
		{
		}
		//Initialisation
		virtual bool Initialise( SRendererInit& rParameters ) = 0;
		
		virtual void ReloadDevice() = 0;

		//Update
		virtual bool BeginScene() = 0;
		virtual bool EndScene() = 0;


		//Renderering
		virtual bool DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, const void* pVertices, UINT32 uVertexStride ) = 0;
		virtual bool DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount) = 0;
		virtual bool DrawIndexedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
											UINT32 uVertexCount, const void* pVertices, 
											UINT32 uVertexStride, const void* pIndices, 
											EIndexType eIndexType ) = 0;

		virtual bool DrawBufferedPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, 
											 UINT32 uVertexCount, UINT32 uOffset = 0, 
											 bool bIndexed = true ) = 0;
		virtual bool ColourZClear( UINT32 colClear, FLOAT32 fZValue ) = 0;
	
		//Settings
		virtual bool SetRenderState( ERenderState eState, UINT32 uValue ) = 0;

		//General
		virtual void ApplyDefaultVertDecl() = 0;
		virtual bool IsActiveWindow() = 0;
		virtual void* GetDevice() = 0;
		virtual void* GetWindowHandle() = 0;
		virtual ASCVertexDeclaration* GetDefaultVertDecl() = 0;
		virtual ASCVertexDeclaration* CreateVertDecl(VertElement veElements[], UINT32 uCount) = 0;

		virtual ASCTextureManager* GetTextureManager() = 0;
		virtual ASCShaderManager* GetShaderManager() = 0;
		
		virtual ASCMatrix4 GetViewMatrix() = 0;
		virtual ASCMatrix4 GetProjectionMatrix() = 0;
		
		virtual void SetViewMatrix(ASCMatrix4& mMat) = 0;
		virtual void SetProjectionMatrix(ASCMatrix4& mMat) = 0;

		virtual ASCVertexBuffer* CreateVertexBuffer(UINT32 uSize, UINT32 uStride) = 0;
		virtual UINT32 VertexMemUsed() = 0;

		virtual void AddTextLineToFont(ASCFont* pFont, SFontData& sFontData) = 0;
		virtual ASCFont* LoadFont(ASCString strFile) = 0;
		virtual void ClearFontBuffer() = 0;

		virtual ASCVector2 GetWorldPosition(FLOAT32 fX, FLOAT32 fY) = 0;

		virtual void CalibratePicking() = 0;

		virtual void ShowDrawInfo() = 0;
		virtual void AddDrawDebugInfo(ASCString& strLine) = 0; 
		
		virtual UINT32 CurrentVertexStride() = 0;
		virtual void SetVertexStride(UINT32 uVal) = 0;
        
        
#ifdef ASC_IOS
        virtual void InitForIOS( void* pRenderView ) = 0;
#endif
	};
}

#endif //_ASCRENDERER_H__Included_156427159

