#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2ShaderManager.h"
#include "ASCGLES2Shader.h"


ASCGLES2ShaderManager::ASCGLES2ShaderManager(void)
{
}


ASCGLES2ShaderManager::~ASCGLES2ShaderManager(void)
{
	for(UINT32 i = 0; i < m_vecShaders.size(); ++i)
	{
		SafeDelete(m_vecShaders[i]);
	}
}
//Loading
ASCShader* 
ASCGLES2ShaderManager::LoadShader(ASCString strFileName)
{
	for(UINT32 i = 0; i < m_vecShaders.size(); ++i)
	{
		if(m_vecShaders[i]->GetName() == strFileName)
		{
			return m_vecShaders[i];
		}
	}
	ASCShader* pShader = new ASCGLES2Shader( strFileName );

	m_vecShaders.push_back(pShader);
	return pShader;
}
void 
ASCGLES2ShaderManager::Reload()
{
	for(UINT32 i = 0; i < m_vecShaders.size(); ++i)
	{
		m_vecShaders[i]->Reload();
	}
}

#endif