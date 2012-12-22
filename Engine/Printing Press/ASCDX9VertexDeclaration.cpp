#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9VertexDeclaration.h"


ASCDX9VertexDeclaration::ASCDX9VertexDeclaration(SVertElement VertElements[], UINT32 iElementCount, void* pDevice)
: m_pDevice ( reinterpret_cast<IDirect3DDevice9*>(pDevice))
, m_pDecl ( NULL )
, m_pVertElem ( NULL )
, m_iElementCount ( iElementCount )
, m_iStride ( 0 )
{
	static const BYTE UsageLookup[] = 
	{
		0xFF,						//ES_NONE = 0,			//!< This data has no meaning!
		D3DDECLUSAGE_POSITION,		//ES_POSITION,			//!< This is a position
		D3DDECLUSAGE_POSITIONT,		//ES_POSITIONT,			//!< This is a transformed position
		D3DDECLUSAGE_NORMAL,		//ES_NORMAL,			//!< This is a normal.
		D3DDECLUSAGE_COLOR,			//ES_COLOUR,			//!< This is a colour.
		D3DDECLUSAGE_TEXCOORD,		//ES_TEXTURE,			//!< This is a texture.
		D3DDECLUSAGE_TANGENT,		// ES_TANGENT,			//!< Tangental normal, for basis vectors.
		D3DDECLUSAGE_BINORMAL,		//ES_BINORMAL,			//!< Binormal normal, for basis vectors.
		D3DDECLUSAGE_BLENDINDICES,		//ES_BLENDINDICES,			//!< Binormal normal, for basis vectors.
	};
	static const D3DDECLTYPE TypeLookup[][4] = 
	{
		{ D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3 , D3DDECLTYPE_FLOAT4 },	//ET_FLOAT32 = 0,	//!< A 32Bit floating point value
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED },		//ET_FLOAT64,		//!< A 64Bit float
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED },		//ET_SINT8,			//!< signed 8 bit integer
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_SHORT2N, D3DDECLTYPE_UNUSED, D3DDECLTYPE_SHORT4N },	//ET_SINT16,		//!< signed 16 bit integer
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED },		//ET_SINT32,		//!< signed 32 bit integer
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED,  D3DDECLTYPE_D3DCOLOR },	//ET_UINT8,			//!< unsigned 8 bit integer
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_USHORT2N, D3DDECLTYPE_UNUSED, D3DDECLTYPE_USHORT4N },	//ET_UINT16,		//!< unsigned 16 bit integer
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED },		//ET_UINT32,		//!< unsigned 32 bit integer
		{ D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED },		//ET_NONE,			//!< This means there is nothing there
	};
	
	D3DVERTEXELEMENT9 VertElem [m_kiMaxElements+1];
	const static D3DVERTEXELEMENT9 declEnd = D3DDECL_END();
	VertElem[m_iElementCount] = declEnd;
	static const UINT32 s_kiStride[] = {4, 8, 1, 2, 4, 1, 2, 4, 0};

	for(UINT32 i = 0; i < iElementCount; ++i)
	{
		const SVertElement& Source = VertElements[i];
		D3DVERTEXELEMENT9& Dest = VertElem[i];

		Dest.Stream = (WORD)Source.m_uStream;
		Dest.Offset = (WORD)Source.m_uOffset;
		Dest.Method = D3DDECLMETHOD_DEFAULT;
		Dest.UsageIndex = (BYTE)Source.m_uIndex;
		Dest.Usage = UsageLookup[Source.m_eSemantic];
		if( 0xFF == Dest.Usage )
		{
			assert_now("Guts, Invalid Usage");
		}

		if( iElementCount > 10 || 0 == iElementCount )
		{
			assert_now( "Guts, Invalid Count" );
		}
		Dest.Type = (BYTE)TypeLookup[Source.m_eType][Source.m_uCount-1];
		if( D3DDECLTYPE_UNUSED == Dest.Type )
		{
			assert_now( "Guts, Invalid Type" );
		}
		m_iStride += s_kiStride[VertElements[i].m_eType] * VertElements[i].m_uCount;
	}
	if(m_pDevice->CreateVertexDeclaration( VertElem, &m_pDecl ) == D3D_OK)
	{
		m_pVertElem = new D3DVERTEXELEMENT9[m_iElementCount];
		for(UINT32 i = 0; i < m_iElementCount; ++i)
		{
			m_pVertElem[i] = VertElem[i];
		}
	}
	else
	{
		assert_now("Guts, Unable to create vertex declaration");
	}
}


ASCDX9VertexDeclaration::~ASCDX9VertexDeclaration(void)
{
	SafeRelease( m_pDecl );
	SafeDelete( m_pVertElem );
}
bool 
ASCDX9VertexDeclaration::Apply()
{
	return m_pDevice->SetVertexDeclaration( m_pDecl ) == D3D_OK;
}
UINT32 
ASCDX9VertexDeclaration::GetStride()
{
	return m_iStride;
}

#endif