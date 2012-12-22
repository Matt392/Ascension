#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9Shader.h"


ASCDX9Shader::ASCDX9Shader( IDirect3DDevice9* pDevice, ASCString& strFileName )
: m_pDevice ( pDevice )
, m_pEffect ( NULL )
{
	m_strFileName = strFileName;
	m_strFileName += ASCString(".fx");
	CHAR8* strFile = m_strFileName.c_str();
	if(D3D_OK != D3DXCreateEffectFromFileA( m_pDevice, strFile, NULL, NULL, 0, NULL, &m_pEffect, NULL ))
	{
		assert_now("Guts, Failed to load shader");
	}
	strFile = NULL;
}


ASCDX9Shader::~ASCDX9Shader(void)
{
	SafeRelease( m_pEffect );
}

//Rendering
bool 
ASCDX9Shader::Begin ( UINT32& uPassCount )
{
	bool bReturn = D3D_OK == m_pEffect->Begin( &uPassCount, 0 );
	assert_msg( bReturn, "Guts, Failed to begin effect" );
	return bReturn;
}
bool 
ASCDX9Shader::Begin ( void )
{
	UINT32 uPass = 0;
	bool bReturn = D3D_OK == m_pEffect->Begin( &uPass, 0 );
	assert_msg( bReturn, "Guts, Failed to begin effect" );

	if(uPass != 1)
	{
		assert_now( "Guts, Too manny passes" );
		return false ;
	}
	
	return D3D_OK == m_pEffect->BeginPass( 0 ) && bReturn;
}
bool 
ASCDX9Shader::BeginPass ( UINT32 uPassNum )
{
	bool bReturn = D3D_OK == m_pEffect->BeginPass( uPassNum );
	assert_msg( bReturn, "Guts, Failed to begin effect" );
	return bReturn;
}
bool 
ASCDX9Shader::EndPass ( void )
{
	bool bReturn = D3D_OK == m_pEffect->EndPass();
	assert_msg( bReturn, "Guts, Failed to end current pass" );
	return bReturn;
}
bool 
ASCDX9Shader::End ( void )
{
	bool bReturn = D3D_OK == m_pEffect->End() && EndPass();
	assert_msg( bReturn, "Guts, Failed to end current pass" );
	return bReturn;
}

//Get handles
ASC_Handle 
ASCDX9Shader::GetSemanticHandle( ASCString strSemantic )
{
	return m_pEffect->GetParameterBySemantic( NULL, strSemantic.c_str() );
}
ASC_Handle 
ASCDX9Shader::GetNamedHandle( ASCString strName )
{
	ASC_Handle hRes = m_pEffect->GetParameterByName( NULL, strName.c_str() );
	if ( NULL == hRes )
	{
		// see if it a technique not a param.
		hRes = m_pEffect->GetTechniqueByName( strName.c_str() );
	}
	assert_msg( NULL != hRes, "Guts, Unable to find a shader object" );
	return hRes;
}
bool 
ASCDX9Shader::SetFVectors( ASC_Handle hTarget, const void* pSrc, UINT32 uQuadCount )
{
	return D3D_OK == m_pEffect->SetRawValue( (D3DXHANDLE)hTarget, pSrc, 0, uQuadCount * 16 );
}
bool 
ASCDX9Shader::SetTechnique( ASC_Handle hTarget )
{
	return D3D_OK == m_pEffect->SetTechnique( (D3DXHANDLE)hTarget );
}

//Loading
void 
ASCDX9Shader::Reload()
{
	SafeRelease( m_pEffect );
	D3DXCreateEffectFromFileA( m_pDevice, m_strFileName.c_str(), NULL, NULL, 0, NULL, &m_pEffect, NULL );
}

//Get
ASCString 
ASCDX9Shader::GetName()
{
	return m_strFileName;
}

#endif