//#include "ASCGenInc.h"
//#include "ASCProfilingNode.h"
//#include "../../Engine/Ascension/Ascension.h"
//#include "ASCProfilingSystem.h"
//
//
//ASCProfilingSystem::ASCProfilingSystem(void)
//: m_pGrandParent ( NULL )
//{
//}
//
//ASCProfilingSystem::~ASCProfilingSystem(void)
//{
//	for(UINT32 i = 0; i < m_vecProfilingNodes.size(); ++i)
//	{
//		SafeDelete(m_vecProfilingNodes[i]);
//	}
//}
//bool 
//ASCProfilingSystem::Initialise()
//{
//	return true;
//}
//void 
//ASCProfilingSystem::DisplayInfo()
//{
//	if(m_pGrandParent)
//	{
//		m_pGrandParent->End();
//		m_pGrandParent->Display(Ascension::Delta(), 0);
//	}
//}
//void 
//ASCProfilingSystem::StartNode(ASCString strID, ASCString strParentID, bool bOutputWindow)
//{
//	for(UINT32 i = 0; i < m_vecProfilingNodes.size(); ++i)
//	{
//		if(*m_vecProfilingNodes[i] == strID)
//		{
//			m_vecProfilingNodes[i]->Start();
//			return;
//		}
//	}
//	ASCProfilingNode* pNode = new ASCProfilingNode();
//	if(strParentID.Valid())
//	{
//		for(UINT32 i = 0; i < m_vecProfilingNodes.size(); ++i)
//		{
//			if(*m_vecProfilingNodes[i] == strParentID)
//			{
//				pNode->Initialise(strID, m_vecProfilingNodes[i], bOutputWindow);
//				break;
//			}
//		}
//	}
//	else
//	{
//		if(NULL == m_pGrandParent)
//		{
//			m_pGrandParent = pNode;
//			pNode->Initialise(strID, NULL, bOutputWindow);
//		}
//		else
//		{
//			pNode->Initialise(strID, m_pGrandParent, bOutputWindow);
//		}
//	}
//	m_vecProfilingNodes.push_back( pNode );
//	pNode->Start();
//}
//void 
//ASCProfilingSystem::EndNode(ASCString strID)
//{
//	for(UINT32 i = 0; i < m_vecProfilingNodes.size(); ++i)
//	{
//		if(*m_vecProfilingNodes[i] == strID)
//		{
//			m_vecProfilingNodes[i]->End();
//		}
//	}
//}