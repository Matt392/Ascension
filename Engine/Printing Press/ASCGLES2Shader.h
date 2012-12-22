/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCGLES2Shader.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGLES2SHADER_H__Included_533375336
#define _ASCGLES2SHADER_H__Included_533375336

#ifdef ASC_RENDERER_GLES2

#include "ASCShader.h"

namespace ASC
{
	enum ASC_GLES_UNIFORM_TYPES
	{
		GLESUT_MAT4,
		GLESUT_FLOAT,
		GLESUT_COUNT,
	};
	struct ASC_Techinque
	{
		UINT32 m_uPixelShader;
		UINT32 m_uVertexShader;
		UINT32 m_uTechniqueNum;

		UINT32 m_vecAttributeCount;
		ASCVector<ASCString>* m_vecAttributeNames;
		UINT32* m_vecAttributeSize;
		UINT32* m_vecAttributeIDs;
		
		UINT32 m_vecTextureCount;
		ASCVector<ASCString>* m_vecTextureNames;
		UINT32* m_vecTextureIDs;

		UINT32 m_uUniformCount;
		ASCVector<ASCString>* m_vecUniformNames;
		ASC_GLES_UNIFORM_TYPES* m_vecUniformTypes;
		UINT32* m_vecUniformIDs;
		UINT32* m_vecUniformLocation;

		ASCString m_strTechniqueName;

		UINT32 m_uID;
        
        bool m_bUseCamera;

		ASC_Techinque()
		: m_vecAttributeSize ( NULL )
		, m_vecAttributeIDs ( NULL )
		, m_vecUniformTypes ( NULL )
		, m_vecTextureIDs ( NULL )
		, m_vecUniformIDs ( NULL )
		, m_vecUniformLocation ( NULL )
		, m_vecAttributeNames ( NULL )
		, m_vecTextureNames ( NULL )
		, m_vecUniformNames ( NULL )
        , m_bUseCamera ( false )
		{
			m_vecAttributeNames = new ASCVector<ASCString>();
			m_vecTextureNames = new ASCVector<ASCString>();
			m_vecUniformNames = new ASCVector<ASCString>();
		}
		~ASC_Techinque()
		{
			SafeDelete( m_vecAttributeNames );
			SafeDelete( m_vecTextureNames );
			SafeDelete( m_vecUniformNames );
		}
		void operator=(const ASC_Techinque& str)
		{
			for(UINT32 i = 0; i < str.m_vecAttributeNames->size(); ++i)
			{
				m_vecAttributeNames->push_back(str.m_vecAttributeNames->at(i));
			}
			for(UINT32 i = 0; i < str.m_vecTextureNames->size(); ++i)
			{
				m_vecTextureNames->push_back(str.m_vecTextureNames->at(i));
			}
			for(UINT32 i = 0; i < str.m_vecUniformNames->size(); ++i)
			{
				m_vecUniformNames->push_back(str.m_vecUniformNames->at(i));
			}
		}
	};

	class ASCGLES2Shader : public ASCShader
	{
	public:
		ASCGLES2Shader(ASCString& strFileName);
		~ASCGLES2Shader(void);

		void Initialise();
		void LoadTechnique(UINT32 uVal);

		//Rendering
		bool Begin ( UINT32& uPassCount );
		bool Begin ( void );
		bool BeginPass ( UINT32 uPassNum );
		bool EndPass ( void );
		bool End ( void );

		//Get handles
		ASC_Handle GetSemanticHandle( ASCString strSemantic );
		ASC_Handle GetNamedHandle( ASCString strName );
		bool SetFVectors( ASC_Handle hTarget, const void* pSrc, UINT32 uQuadCount );
		bool SetTechnique( ASC_Handle hTarget );

		//Loading
		void Reload();

		//Get
		ASCString GetName();

		ASC_GLES_UNIFORM_TYPES ConvertStringToUniformType(ASCString& str);

	protected:
		ASCString m_strFileName;
		ASCString m_strFileNameNoExt;
		ASCVector<ASC_Techinque> m_vecTechinques;
		UINT32 m_uActiveTechinque;

	};

}

#endif

#endif //_ASCGLES2SHADER_H__Included_533375336

