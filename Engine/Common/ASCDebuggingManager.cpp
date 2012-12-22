#include "ASCGenInc.h"
#include "ASCDebuggingManager.h"
#ifdef ASC_IOS
#else
#include <Windows.h>
#endif
#include "ASCOutputWindow.h"

static ASCOutputWindow* sm_pOutputWindow = NULL;

ASCDebuggingManager::ASCDebuggingManager(bool bUse)
: m_bActive ( bUse )
, m_pOutputWindow ( NULL )
{
}


ASCDebuggingManager::~ASCDebuggingManager(void)
{
	SafeDelete( m_pOutputWindow );
	sm_pOutputWindow = NULL;
}
void 
ASCDebuggingManager::Initialise(UINT32 uBufferSize)
{
	m_pOutputWindow = new ASCOutputWindow();
	m_pOutputWindow->Initialise(uBufferSize);
	sm_pOutputWindow = m_pOutputWindow;
}
ASCOutputWindow* 
ASCDebuggingManager::OutputWindow()
{
	return m_pOutputWindow;
}
void 
ASCDebuggingManager::AddScreenLine(ASCString strLine)
{
	if(m_bActive && m_uLineCount < m_uMaxLinesToDraw)
	{
		++m_uLineCount;
		strLine += '\n';
		m_strLines += strLine;
		if(m_uLineCount == m_uMaxLinesToDraw)
		{
			m_strLines += ASCString(".........");
		}
	}
}
void 
ASCDebuggingManager::AddScreenLine(ASCString strPre, FLOAT32 fVal, ASCString strPost)
{
	if(m_bActive && m_uLineCount < m_uMaxLinesToDraw)
	{
		++m_uLineCount;
		ASCString strNew;
		strNew = strPre;
		strNew += fVal;
		strNew += strPost;
		strNew += '\n';
		m_strLines += strNew;
		if(m_uLineCount == m_uMaxLinesToDraw)
		{
			m_strLines += ASCString(".........");
		}
	} 
}
void 
ASCDebuggingManager::AddScreenLine(ASCString strPre, UINT32 uVal, ASCString strPost)
{
	if(m_bActive && m_uLineCount < m_uMaxLinesToDraw)
	{
		++m_uLineCount;
		ASCString strNew;
		strNew = strPre;
		strNew += uVal;
		strNew += strPost;
		strNew += '\n';
		m_strLines += strNew;
		if(m_uLineCount == m_uMaxLinesToDraw)
		{
			m_strLines += ASCString(".........");
		}
	}
}
void 
ASCDebuggingManager::AddScreenLine(ASCString strPre, SINT32 iVal, ASCString strPost)
{
	if(m_bActive && m_uLineCount < m_uMaxLinesToDraw)
	{
		++m_uLineCount;
		ASCString strNew;
		strNew = strPre;
		strNew += iVal;
		strNew += strPost;
		strNew += '\n';
		m_strLines += strNew;
		if(m_uLineCount == m_uMaxLinesToDraw)
		{
			m_strLines += ASCString(".........");
		}
	}
}
void 
ASCDebuggingManager::AddScreenLine(ASCString strPre, void* pVal, ASCString strPost)
{
	if(m_bActive && m_uLineCount < m_uMaxLinesToDraw)
	{
		++m_uLineCount;
		ASCString strNew;
		strNew = strPre;
		strNew += pVal;
		strNew += strPost;
		strNew += '\n';
		m_strLines += strNew;
		if(m_uLineCount == m_uMaxLinesToDraw)
		{
			m_strLines += ASCString(".........");
		}
	}
}
void 
ASCDebuggingManager::AddScreenLine(ASCString strPre, ASCString strVal, ASCString strPost)
{
	if(m_bActive && m_uLineCount < m_uMaxLinesToDraw)
	{
		++m_uLineCount;
		ASCString strNew;
		strNew = strPre;
		strNew += strVal;
		strNew += strPost;
		strNew += '\n';
		m_strLines += strNew;
		if(m_uLineCount == m_uMaxLinesToDraw)
		{
			m_strLines += ASCString(".........");
		}
	}
}
void 
ASCDebuggingManager::MaxLinesToDraw(UINT32 uVal)
{
	m_uMaxLinesToDraw = uVal;
}
ASCString* 
ASCDebuggingManager::GetLineBuffer()
{
	return &m_strLines;
}
void 
ASCDebuggingManager::ClearLineBuffer()
{
	m_uLineCount = 0;
	m_strLines = "";
}

void 
ASCDebuggingManager::OutputWindowLine(ASCDebugString strLine)
{
	if(sm_pOutputWindow)
	{
		sm_pOutputWindow->AddLine(strLine.c_str());
	}
	strLine += ASCDebugString("\n");
#ifdef ASC_IOS
    NSString* DebugLine = [[NSString alloc] initWithUTF8String: strLine.c_str()];
    NSLog(DebugLine);
    [DebugLine release];
#else
	OutputDebugStringA(strLine.c_str());
#endif
}
void 
ASCDebuggingManager::OutputWindowLine(ASCDebugString strPre, FLOAT32 fVal, ASCDebugString strPost)
{
	ASCDebugString strNew;
	strNew = strPre;
	strNew += fVal;
	strNew += strPost;
#ifdef ASC_IOS
    NSString* DebugLine = [[NSString alloc] initWithUTF8String: strNew.c_str()];
    NSLog(DebugLine);
    [DebugLine release];
#else
	OutputDebugStringA(strNew.c_str());
#endif
}
void 
ASCDebuggingManager::OutputWindowLine(ASCDebugString strPre, ASCDebugString strVal, ASCDebugString strPost)
{
	ASCDebugString strNew;
	strNew = strPre;
	strNew += strVal;
	strNew += strPost;
	if(sm_pOutputWindow)
	{
		sm_pOutputWindow->AddLine(strNew.c_str());
	}
#ifdef ASC_IOS
    NSString* DebugLine = [[NSString alloc] initWithUTF8String: strNew.c_str()];
    NSLog(DebugLine);
    [DebugLine release];
#else
	OutputDebugStringA(strNew.c_str());
#endif
}
void 
ASCDebuggingManager::OutputWindowLine(ASCDebugString strPre, UINT32 uVal, ASCDebugString strPost)
{
	ASCDebugString strNew;
	strNew = strPre;
	strNew += uVal;
	strNew += strPost;
#ifdef ASC_IOS
    NSString* DebugLine = [[NSString alloc] initWithUTF8String: strNew.c_str()];
    NSLog(DebugLine);
    [DebugLine release];
#else
	OutputDebugStringA(strNew.c_str());
#endif
}
void 
ASCDebuggingManager::OutputWindowLine(ASCDebugString strPre, SINT32 iVal, ASCDebugString strPost)
{
	ASCDebugString strNew;
	strNew = strPre;
	strNew += iVal;
	strNew += strPost;
	if(sm_pOutputWindow)
	{
		sm_pOutputWindow->AddLine(strNew.c_str());
	}
#ifdef ASC_IOS
    NSString* DebugLine = [[NSString alloc] initWithUTF8String: strNew.c_str()];
    NSLog(DebugLine);
    [DebugLine release];
#else
	OutputDebugStringA(strNew.c_str());
#endif
}
void 
ASCDebuggingManager::OutputWindowLine(ASCDebugString strPre, void* pVal, ASCDebugString strPost)
{
	ASCDebugString strNew;
	strNew = strPre;
	strNew += pVal;
	strNew += strPost;
	if(sm_pOutputWindow)
	{
		sm_pOutputWindow->AddLine(strNew.c_str());
	}
#ifdef ASC_IOS
    NSString* DebugLine = [[NSString alloc] initWithUTF8String: strNew.c_str()];
    NSLog(DebugLine);
    [DebugLine release];
#else
	OutputDebugStringA(strNew.c_str());
#endif
}