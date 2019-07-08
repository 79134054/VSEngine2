#include "VSProfiler.h"
#include "VSTimer.h"
#include "VSRenderer.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
VSProfilerNode::VSProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
{
	m_Name = Name;
	if (ParentName)
	{
		m_ParentName = ParentName;
	}
	
	VSProfiler::AddProfileNode(this);
	m_pParentNode = NULL;
}
void VSProfilerNode::AddChild(VSProfilerNode * pChild)
{
	VSMAC_ASSERT(pChild && !pChild->m_pParentNode);
	pChild->m_pParentNode = this;
	m_pChildNode.AddElement(pChild);
}
void VSProfilerNode::Draw(unsigned int uiLayer)
{
	OnDraw(uiLayer);
	VSProfiler::ms_uiCurCount++;
	for (unsigned int i = 0; i < m_pChildNode.GetNum();i++)
	{
		m_pChildNode[i]->Draw(uiLayer + 1);
	}
}
IMPLEMENT_INITIAL_ONLY_BEGIN(VSProfiler);
ADD_INITIAL_FUNCTION(InitialDefaultState)
IMPLEMENT_INITIAL_ONLY_END
unsigned int VSProfiler::ms_uiCurCount = 0;
VSArray<VSProfilerNode *> VSProfiler::m_pRoot;
TCHAR *  VSProfiler::ms_LayerFlag[] = { _T(""),
										_T("  "),
										_T("    "),
										_T("      "),
										_T("        "),
										_T("          "),
										_T("            "),
										_T("              "),
										_T("                "),
										_T("                  ") };
bool VSProfiler::InitialDefaultState()
{
	for (unsigned int i = 0; i < GetProfilerNode().GetNum(); i++)
	{
		if (GetProfilerNode()[i]->m_ParentName == VSString::ms_StringNULL)
		{
			m_pRoot.AddElement(GetProfilerNode()[i]);
		}
		else
		{
			bool bFound = false;
			for (unsigned int j = 0; j < GetProfilerNode().GetNum(); j++)
			{
				if (GetProfilerNode()[i]->m_ParentName == GetProfilerNode()[j]->m_Name)
				{
					GetProfilerNode()[j]->AddChild(GetProfilerNode()[i]);
					bFound = true;
					break;
				}
			}
			if (bFound == false)
			{
				m_pRoot.AddElement(GetProfilerNode()[i]);
			}
			
		}
	}
	return 1;
}
void VSProfiler::Draw()
{
#ifdef PROFILER
	for (unsigned int i = 0; i < m_pRoot.GetNum(); i++)
	{
		m_pRoot[i]->Draw( 0);
	}
	ClearAll();
#endif	
}
void VSProfiler::ClearAll()
{
	for (unsigned int i = 0; i < GetProfilerNode().GetNum(); i++)
	{
		GetProfilerNode()[i]->Clear();
	}
	ms_uiCurCount = 0;
}

void VSProfiler::AddProfileNode(VSProfilerNode * pProfilerNode)
{
	GetProfilerNode().AddElement(pProfilerNode);
}
void VSProfiler::GetProfilerNameString(const VSString & Name, unsigned int uiLayer, VSString & OutString)
{
	OutString = VSProfiler::ms_LayerFlag[uiLayer] + Name;
}
VSTimeProfilerNode::VSProfilerNodeTImer::VSProfilerNodeTImer(VSTimeProfilerNode * pProfilerNode)
{
	m_fBeginTime = (VSTimer::ms_pTimer != NULL ? (VSREAL)VSTimer::ms_pTimer->GetGamePlayTime() : 0.0f);
	m_pOwner = pProfilerNode;
}
VSTimeProfilerNode::VSProfilerNodeTImer::~VSProfilerNodeTImer()
{
	m_pOwner->m_fProfilerTime += (VSTimer::ms_pTimer != NULL ? (VSREAL)VSTimer::ms_pTimer->GetGamePlayTime() : 0.0f) - m_fBeginTime;
}
VSTimeProfilerNode::VSTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:VSProfilerNode(Name, ParentName)
{
	m_fProfilerTime = 0.0f;
}
void VSTimeProfilerNode::OnDraw(unsigned int uiLayer)
{
	VSString Out;
	VSProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%fms");
	VSRenderer::ms_pRenderer->DrawText(0, VSProfiler::ms_uiCurCount * 20, VSDWCOLORABGR(255, 0, 255, 0), Out.GetBuffer(), m_fProfilerTime);
}


VSCountProfilerNode::VSProfilerNodeCounter::VSProfilerNodeCounter(VSCountProfilerNode * pProfilerNode, int uiCount)
{
	pProfilerNode->m_uiCounter += uiCount;
}

VSCountProfilerNode::VSCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:VSProfilerNode(Name, ParentName)
{
	m_uiCounter = 0;
}
void VSCountProfilerNode::OnDraw(unsigned int uiLayer)
{
	VSString Out;
	VSProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%d");
	VSRenderer::ms_pRenderer->DrawText(0, VSProfiler::ms_uiCurCount * 20, VSDWCOLORABGR(255, 255, 255, 255), Out.GetBuffer(), m_uiCounter);
}
VSNoClearCountProfilerNode::VSNoClearCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:VSCountProfilerNode(Name, ParentName)
{
	m_uiCounter = 0;
}
VSOnlyTimeProfilerNode::VSProfilerNodeOnlyTImer::VSProfilerNodeOnlyTImer(VSOnlyTimeProfilerNode * pProfilerNode, float fProfilerTime)
{
	pProfilerNode->m_fProfilerTime = fProfilerTime;
}

VSOnlyTimeProfilerNode::VSOnlyTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:VSProfilerNode(Name, ParentName)
{
	m_fProfilerTime = 0.0f;
}
void VSOnlyTimeProfilerNode::OnDraw(unsigned int uiLayer)
{
	VSString Out;
	VSProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%f");
	VSRenderer::ms_pRenderer->DrawText(0, VSProfiler::ms_uiCurCount * 20, VSDWCOLORABGR(255, 255, 255, 0), Out.GetBuffer(), m_fProfilerTime);
}