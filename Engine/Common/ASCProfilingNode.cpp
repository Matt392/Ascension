//#include "ASCGenInc.h"
//#include "ASCClock.h"
//#include "../../Engine/Ascension/Ascension.h"
//#include "ASCProfilingNode.h"
//
//
//
//ASCProfilingNode::ASCProfilingNode(void)
//: m_pTimer ( NULL )
//, m_fFrameTime ( 0.0f )
//, m_uCallCount ( 0 )
//, m_bDisplayInOutputWindow ( false )
//, m_bEnded ( true )
//{
//}
//
//
//ASCProfilingNode::~ASCProfilingNode(void)
//{
//	SafeDelete( m_pTimer );
//}
//
//bool 
//ASCProfilingNode::operator == (ASCString strOther)
//{
//	return m_strID == strOther;
//}
//bool 
//ASCProfilingNode::Initialise(ASCString strID, ASCProfilingNode* pParent, bool bOutputWindow)
//{
//	m_bDisplayInOutputWindow = bOutputWindow;
//	m_strID = strID;
//	m_pTimer = new ASCClock();
//	if(pParent)
//	{
//		pParent->AddChild(this);
//	}
//	return true;
//}
//void 
//ASCProfilingNode::AddChild(ASCProfilingNode* pChild)
//{
//	m_vecChildren.push_back(pChild);
//}
//void 
//ASCProfilingNode::Start()
//{
//	m_bEnded = false;
//	m_bDisplay = true;
//	m_pTimer->Update();
//	++m_uCallCount;
//}
//void 
//ASCProfilingNode::End()
//{
//	if(false == m_bEnded)
//	{
//		m_bEnded = true;
//		m_pTimer->Update();
//		m_fFrameTime += m_pTimer->GetDeltaTick();
//	}
//}
//void 
//ASCProfilingNode::Display(FLOAT32 fParentTime, UINT32 uDepth)
//{
//	if(m_bDisplay)
//	{
//		ASCString strInfo = "";
//		FLOAT32 fPercentage = ( ( m_fFrameTime / fParentTime ) * 100.0f );
//		if(fParentTime < m_fFrameTime)
//		{
//			 fPercentage = 100.0f;
//		}
//		for(UINT32 i = 0; i < uDepth; ++i)
//		{
//			strInfo += ASCString("   ");
//		}
//		strInfo += m_strID;
//		strInfo += ASCString(" - Percent: ");
//		strInfo += fPercentage;
//		strInfo += ASCString(" - Time: ");
//		strInfo.AddFloatHP(m_fFrameTime);
//		strInfo += ASCString(" - Call Count: ");
//		strInfo += m_uCallCount;
//		Ascension::Debugging().AddScreenLine(strInfo);
//		if(m_bDisplayInOutputWindow)
//		{
//			Ascension::Debugging().OutputWindowLine(strInfo.c_str());
//		}
//		++uDepth;
//		for(UINT32 i = 0; i < m_vecChildren.size(); ++i)
//		{
//			if(m_vecChildren[i])
//			{
//				m_vecChildren[i]->Display(fParentTime, uDepth);
//			}
//		}
//		m_uCallCount = 0;
//		m_fFrameTime = 0.0f;
//		m_bDisplay = false;
//	}
//}