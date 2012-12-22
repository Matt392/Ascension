#ifdef ASC_RENDERER_DX9

#include "ASCGenInc.h"
#include "ASCDX9VertexBuffer.h"
#include "../Ascension/Ascension.h"
#include "ASCRenderer.h"


ASCDX9VertexBuffer::ASCDX9VertexBuffer(void)
: m_pVertexBuffer ( NULL )
, m_uSize ( 0 )
, m_uStride ( 0 )
{
}


ASCDX9VertexBuffer::~ASCDX9VertexBuffer(void)
{
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
}
bool 
ASCDX9VertexBuffer::Initialise(UINT32 uSize, UINT32 uStride)
{
	DWORD d3dUsage = 0;
	d3dUsage |= D3DUSAGE_DYNAMIC;
	IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>(Ascension::Renderer().GetDevice());
	if(pDevice->CreateVertexBuffer(uSize*uStride, d3dUsage, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL) == D3D_OK)
	{
		m_uStride = uStride;
		m_uSize = uSize;
		return true;
	}
	return false;
}
bool 
ASCDX9VertexBuffer::Resize(UINT32 uSize)
{
	m_pVertexBuffer->Release();
	m_pVertexBuffer = NULL;
	
	IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>(Ascension::Renderer().GetDevice());
	if(pDevice->CreateVertexBuffer(uSize*m_uStride, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL) == D3D_OK)
	{
		m_uSize = uSize;
		return true;
	}
	return false;
}
void* 
ASCDX9VertexBuffer::Lock()
{
	void* pData = NULL;
	if(m_pVertexBuffer->Lock(0, m_uSize, &pData, 0) == D3D_OK)
	{
		return pData;
	}
	return NULL;
}
void 
ASCDX9VertexBuffer::UnLock()
{
	if(m_pVertexBuffer->Unlock() == D3D_OK)
	{
	}
	else
	{
		assert_now("Guts, Failed to unlock");
	}
}
UINT32 
ASCDX9VertexBuffer::Size()
{
	return m_uSize;
}
void 
ASCDX9VertexBuffer::SetSource()
{
	IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>(Ascension::Renderer().GetDevice());
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, m_uStride);
}
void 
ASCDX9VertexBuffer::Unbind()
{

}
UINT32 
ASCDX9VertexBuffer::MemSize()
{
	return m_uStride * m_uSize;
}

#endif