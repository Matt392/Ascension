#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2VertexDeclaration.h"
#include "../Ascension/Ascension.h"
#include "ASCRenderer.h"


ASCGLES2VertexDeclaration::ASCGLES2VertexDeclaration(SVertElement VertElements[], UINT32 iElementCount)
: m_iElementCount ( iElementCount )
, m_iStride ( 0 )
{	
	static const UINT32 s_kiStride[] = {4, 8, 1, 2, 4, 1, 2, 4, 0};

	for(UINT32 i = 0; i < iElementCount; ++i)
	{
		if( iElementCount > 10 || 0 == iElementCount )
		{
			assert_now( "Guts, Invalid Count" );
		}

		m_iStride += s_kiStride[VertElements[i].m_eType] * VertElements[i].m_uCount;
	}
}


ASCGLES2VertexDeclaration::~ASCGLES2VertexDeclaration(void)
{
}

bool 
ASCGLES2VertexDeclaration::Apply()
{
	Ascension::Renderer().SetVertexStride(m_iStride);
	return true;
}
UINT32 
ASCGLES2VertexDeclaration::GetStride()
{
	return m_iStride;
}


#endif