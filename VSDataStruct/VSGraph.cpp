#include "VSGraph.h"
using namespace VSEngine2;
void VSGraphNode::AddEdge(VSGraphEdge * pEdge)
{
	if (pEdge)
	{
		VSMAC_ASSERT(pEdge->GetFrom() && pEdge->GetTo());
		if (pEdge->GetFrom() && pEdge->GetTo())
		{
			m_Edge.AddElement(pEdge);
		}

	}

}
VSGraph::VSGraph(bool bIsDig)
{
	m_bIsDig = bIsDig;
}

VSGraph::~VSGraph()
{
	VSList<VSGraphNode *>::VSListIterator k = m_NodeList.Begin();
	for (;k.IsValid();++k)
	{
		if((*k) != NULL)
		{
			VSMAC_DELETE((*k));
		}
	}
	m_NodeList.Clear();
	VSList<VSGraphEdge *>::VSListIterator l = m_EdgeList.Begin();
	for (;l.IsValid();++l)
	{
		if((*l) != NULL)
		{
			VSMAC_DELETE((*l));
		}
	}
	m_EdgeList.Clear();
}

VSGraphNode * VSGraph::CreateNode()
{
	VSGraphNode * p = VS_NEW VSGraphNode();
	m_NodeList.AddElement(p);
	return p;
}

void VSGraph::DeleteNode(VSGraphNode * pNode)
{
	if (!pNode)
	{
		return ;
	}
	bool bFind = false;
	VSList<VSGraphNode *>::VSListIterator k = m_NodeList.Begin();
	for (;k.IsValid();++k)
	{
		if((*k) == pNode)
		{
			bFind = true;
		}
	}
	if (bFind)
	{
		VSList<VSGraphEdge *> &s = pNode->GetEdge();
		VSList<VSGraphEdge *>::VSListIterator m = s.Begin();
		for (;m.IsValid();++m)
		{
			VSGraphEdge * pEdge = (VSGraphEdge *)(*m);
			m_EdgeList.Erase(pEdge);
			VSMAC_DELETE(pEdge);
		}
		m_NodeList.Erase(pNode);
		VSMAC_DELETE(pNode);
	}
}

VSGraphEdge *VSGraph::CreateEdge(VSGraphNode * pFrom,VSGraphNode * pTo,VSREAL fCost)
{
	if (!pFrom || !pTo)
	{
		return NULL;
	}
	VSGraphEdge * p = VS_NEW VSGraphEdge();
	m_EdgeList.AddElement(p);
	p->SetFrom(pFrom);
	p->SetTo(pTo);
	p->SetCost(fCost);
	VSList<VSGraphNode *>::VSListIterator k = m_NodeList.Begin();
	bool FromFind =false;
	bool ToFind = false;
	for (;k.IsValid();++k)
	{
		if((*k) == pFrom)
		{
			pFrom->AddEdge(p);
			FromFind = true;
		}
		if((*k) == pTo)
		{
			if (!m_bIsDig)
			{

				pTo->AddEdge(p);

			}
			ToFind = true;
		}
	}
	if (!FromFind || !ToFind)
	{
		VSMAC_DELETE(p);
	}

	return p;
}

void VSGraph::DeleteEdge(VSGraphEdge * pEdge)
{
	if (!pEdge)
	{
		return ;
	}
	bool bFind = false;
	VSList<VSGraphEdge *>::VSListIterator k = m_EdgeList.Begin();
	for (;k.IsValid();++k)
	{
		if((*k) == pEdge)
		{
			bFind = true;
		}
	}	
	if (bFind)
	{
		VSGraphNode * pFrom = (VSGraphNode *)pEdge->GetFrom();
		VSList<VSGraphEdge *> &s = pFrom->GetEdge();
		s.Erase(pEdge);
		if (m_bIsDig)
		{
			VSGraphNode * pTo = (VSGraphNode *)pEdge->GetTo();
			VSList<VSGraphEdge *> &w = pTo->GetEdge();
			w.Erase(pEdge);
		}
		VSMAC_DELETE(pEdge);
	}
}

void VSGraph::DeleteEdge(VSGraphNode * pFrom,VSGraphNode * pTo)
{
	VSList<VSGraphNode *>::VSListIterator k = m_NodeList.Begin();
	bool FromFind =false;
	bool ToFind = false;
	for (;k.IsValid();++k)
	{
		if((*k) == pFrom)
		{
			FromFind = true;
		}
		if((*k) == pTo)
		{
			ToFind = true;

		}
	}
	if (!FromFind || !ToFind)
	{
		return;
	}
	VSList<VSGraphEdge *> &l = pFrom->GetEdge();
	VSList<VSGraphEdge *>::VSListIterator m = l.Begin();
	VSGraphEdge * pEdge = NULL;
	for (;m.IsValid(); ++m)
	{
		if ((*m)->GetTo() == pTo)
		{
			pEdge = (VSGraphEdge *)(*m);
		}
	}
	VSMAC_ASSERT(pEdge);

	pFrom = (VSGraphNode *)pEdge->GetFrom();
	VSList<VSGraphEdge *> &s = pFrom->GetEdge();
	s.Erase(pEdge);
	if (m_bIsDig)
	{
		VSGraphNode * pTo = (VSGraphNode *)pEdge->GetTo();
		VSList<VSGraphEdge *> &w = pTo->GetEdge();
		w.Erase(pEdge);
	}
	VSMAC_DELETE(pEdge);
}

unsigned int VSGraph::GetNodeNum()const
{
	return m_NodeList.GetNum();
}

unsigned int VSGraph::GetEdgeNum()const
{
	return m_EdgeList.GetNum();
}
VSGraph_Search::VSGraph_Search(VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget)
{
	m_pGraph = pGraph;
	m_pTarget = pTarget;
	m_pSource = pSource;
}

VSGraph_Search::~VSGraph_Search()
{

}
VSGraph_SearchCommon::VSGraph_SearchCommon(VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget)
:VSGraph_Search(pGraph,pSource,pTarget)
{
	VSList<VSGraphNode*>& List = pGraph->GetAllNode();
	VSList<VSGraphNode*>::VSListIterator k = List.Begin();
	for(;k.IsValid() ; ++k)
	{
		m_NodePathShortestEdge.AddElement((*k),NULL);
		m_NodeCurShortEdge.AddElement((*k),NULL);
	}
}
VSGraph_SearchCommon::~VSGraph_SearchCommon()
{

}
VSArray<VSGraphNode *> VSGraph_SearchCommon::GetPathToTarget()const
{
	VSArray<VSGraphNode *> Path;
	if (!m_pTarget)
	{
		return Path;
	}
	VSGraphNode * pIndexNode = m_pTarget;
	Path.AddElement(pIndexNode);

	while(pIndexNode != m_pSource)
	{
		unsigned int uiIndex = m_NodePathShortestEdge.Find(pIndexNode);

		if (m_NodePathShortestEdge[uiIndex].Value != NULL)
		{
			pIndexNode = m_NodePathShortestEdge[uiIndex].Value->GetFrom();
			Path.AddElement(pIndexNode);
		}
		else
		{
			break;
		}


	}
	return Path;
}
unsigned int VSGraph_SearchCommon::RemoveEdge(VSGraphNode * pGraphNode)
{
	unsigned int uiIndex1 = m_NodePathShortestEdge.Find(pGraphNode);
	//unsigned int uiIndex2 = m_NodeCurShortEdge.Find(pGraphNode);

	m_NodePathShortestEdge[uiIndex1].Value = m_NodeCurShortEdge[uiIndex1].Value;
	return uiIndex1;
}
void VSGraph_SearchCommon::ReSort(VSGraphNode * pGraphNode,VSREAL fCost)
{
	for (unsigned int i = 0 ; i < m_Priority.GetNum() ; i++)
	{
		if(m_Priority[i].m_pGraphNode == pGraphNode)
		{
			m_Priority[i].m_fCost = fCost;
			m_Priority.ReSort(i);
			return;
		}
	}
}
VSGraph_SearchDijkstra::VSGraph_SearchDijkstra(VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget)
:VSGraph_SearchCommon(pGraph,pSource,pTarget)
{
	VSList<VSGraphNode*>& List = pGraph->GetAllNode();
	VSList<VSGraphNode*>::VSListIterator k = List.Begin();
	for(;k.IsValid() ; ++k)
	{
		m_CostToThisNode.AddElement((*k),0.0f);
	}
	Search();
}
VSGraph_SearchDijkstra::~VSGraph_SearchDijkstra()
{

}

void VSGraph_SearchDijkstra::Search()
{
	SortElement SE;
	SE.m_fCost = 0.0f;
	SE.m_pGraphNode = m_pSource;
	m_Priority.AddElement(SE);
	while(m_Priority.GetNum())
	{
		VSGraphNode * pNextNode = m_Priority[m_Priority.GetNum() - 1].m_pGraphNode;
		m_Priority.Erase(m_Priority.GetNum() - 1);
		unsigned int uiIndex = RemoveEdge(pNextNode);
		if (pNextNode == m_pTarget)
		{
			return ;
		}
		VSList<VSGraphEdge*>& List  = pNextNode->GetEdge();
		VSList<VSGraphEdge*>::VSListIterator k = List.Begin();
		//unsigned int uiIndex = m_CostToThisNode.Find(pNextNode);
		VSREAL fCurCost = m_CostToThisNode[uiIndex].Value; 
		for(;k.IsValid() ; ++k)
		{
			VSREAL fNewCost = fCurCost + (*k)->GetCost();
			unsigned int uiIndex1 = m_NodeCurShortEdge.Find((*k)->GetTo());
			if (m_NodeCurShortEdge[uiIndex1].Value == NULL)
			{
				//unsigned int uiIndex2 = m_CostToThisNode.Find((*k)->GetTo());
				m_CostToThisNode[uiIndex1].Value = fNewCost;
				SortElement SE;
				SE.m_fCost = fNewCost;
				SE.m_pGraphNode = (*k)->GetTo();
				m_Priority.AddElement(SE);
				m_NodeCurShortEdge[uiIndex1].Value = (*k);
			}
			else 
			{
				//unsigned int uiIndex2 = m_NodePathShortestEdge.Find((*k)->GetTo());
				if (m_NodePathShortestEdge[uiIndex1].Value == NULL)
				{
					//unsigned int uiIndex3 = m_CostToThisNode.Find((*k)->GetTo());
					if(m_CostToThisNode[uiIndex1].Value > fNewCost)
					{
						m_CostToThisNode[uiIndex1].Value  = fNewCost;
						ReSort((*k)->GetTo(),fNewCost);
						m_NodeCurShortEdge[uiIndex1].Value = (*k);
					}
				}
			}
		}

	}
}
VSREAL VSGraph_SearchDijkstra::GetCostToTarget()const
{
	if (!m_pTarget)
	{
		return 0.0f;
	}
	unsigned int uiIndex = m_CostToThisNode.Find(m_pTarget);
	return m_CostToThisNode[uiIndex].Value;
}
VSREAL VSGraph_SearchDijkstra::GetCostToNode(VSGraphNode * pGraphNode)const
{
	if (!pGraphNode)
	{
		return 0.0f;
	}
	unsigned int uiIndex = m_CostToThisNode.Find(pGraphNode);
	return m_CostToThisNode[uiIndex].Value;
}

void main()
{
	//VSGraph *K = CreateFrom2DMatrix<VSGraph>(NULL,1,1);
}