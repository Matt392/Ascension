#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2VertexBuffer.h"
#include "ASCMemoryManagement.h"

#ifdef ASC_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/EAGL.h>
#else
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#endif

ASCGLES2VertexBuffer::ASCGLES2VertexBuffer(void)
: m_uSize ( 0 )
, m_uStride ( 0 )
, m_uBuffer ( 0 )
, m_pTempBuffer ( NULL )
{
}


ASCGLES2VertexBuffer::~ASCGLES2VertexBuffer(void)
{
	//Delete the buffer
	glDeleteBuffers(m_uSize, &m_uBuffer);
	if(m_pTempBuffer)
	{
		ASCMemoryManagement::ReleaseMemory(m_pTempBuffer);
	}
}

bool 
ASCGLES2VertexBuffer::Initialise(UINT32 uSize, UINT32 uStride)
{
	//Set the size and stride
	m_uSize = uSize;
	m_uStride = uStride;
	
	//Create the new buffer
	glGenBuffers(1, &m_uBuffer);
	Resize(uSize);
	return false;
}

bool 
ASCGLES2VertexBuffer::Resize(UINT32 uSize)
{
	if(m_pTempBuffer)
	{
		ASCMemoryManagement::ReleaseMemory(m_pTempBuffer);
	}

	m_uSize = uSize;
	m_pTempBuffer = ASCMemoryManagement::GetMemory(m_uStride * m_uSize);

	return true;
}

void* 
ASCGLES2VertexBuffer::Lock()
{
	if(NULL == m_pTempBuffer)
	{
		assert_now("Guts, Temp buffer is NULL");
	}
	return m_pTempBuffer;
}
void 
ASCGLES2VertexBuffer::UnLock()
{
	if(m_pTempBuffer)
	{
		//Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_uBuffer);

		//Set the buffer's data
		glBufferData(GL_ARRAY_BUFFER, m_uStride * m_uSize, m_pTempBuffer, GL_STATIC_DRAW);

		//Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return;
	}
	assert_now("Guts, Buffer was not lock and no data exists to fill buffer with");
}

UINT32 
ASCGLES2VertexBuffer::Size()
{
	return m_uSize;
}

void 
ASCGLES2VertexBuffer::SetSource()
{
	//Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_uBuffer);
}

void 
ASCGLES2VertexBuffer::Unbind()
{
	//Unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif