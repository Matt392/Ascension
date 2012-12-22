#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9ShaderManager.h"
#include "ASCDX9Shader.h"


ASCDX9ShaderManager::ASCDX9ShaderManager( IDirect3DDevice9* pDevice )
: m_pDevice ( pDevice )
, m_iShaderCount ( 0 )
{
}


ASCDX9ShaderManager::~ASCDX9ShaderManager(void)
{
	for(UINT32 i = 0; i < m_vecShaders.size(); ++i)
	{
		SafeDelete(m_vecShaders[i]);
	}
}

//Loading
ASCShader* 
ASCDX9ShaderManager::LoadShader(ASCString strFileName)
{
	for(UINT32 i = 0; i < m_vecShaders.size(); ++i)
	{
		if(m_vecShaders[i]->GetName() == strFileName)
		{
			return m_vecShaders[i];
		}
	}
	ASCShader* pShader = new ASCDX9Shader( m_pDevice, strFileName );

	m_vecShaders.push_back(pShader);
	return pShader;
}
void 
ASCDX9ShaderManager::Reload()
{
	for(UINT32 i = 0; i < m_vecShaders.size(); ++i)
	{
		m_vecShaders[i]->Reload();
	}
}

#endif