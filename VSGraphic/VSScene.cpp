#include "VSScene.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSUpdateThread.h"
#include "VSStream.h"
#include "VSSceneManager.h"
#include "VSMeshComponent.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSScene,VSObject)
BEGIN_ADD_PROPERTY(VSScene,VSObject)
REGISTER_PROPERTY(m_pDynamic, Dynamic, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pStaticRoot, StaticRoot, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_ObjectNodes, ObjectNodes, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_bIsBuild, IsBuild, VSProperty::F_SAVE_LOAD_COPY)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSScene)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(ComputeVisibleSet, ViewFamilyUpdate)
#define MAX_NUM 200

VSScene::VSScene()
{

	m_pAllLight.Clear();
	m_pAllCamera.Clear();
	m_pStaticRoot = NULL;
	VSSceneManager::ms_pSceneManager->AddScene(this);
	m_bIsBuild = false;
}
VSScene::~VSScene()
{

	VSSceneManager::ms_pSceneManager->DeleteScene(this);
	m_pAllLight.Clear();
	m_pAllCamera.Clear();

	m_pDynamic.Clear();
	m_pStaticRoot = NULL;
	m_ObjectNodes.Clear();
	
}
void VSScene::Update(double dAppTime)
{
	if (m_bIsBuild == true)
	{
		struct UpdateNode
		{
			VSNode * pSpatial;
			double dAppTime;
		};
	
		if (m_pStaticRoot)
		{
			UpdateNode StaticUpdateNode;
			StaticUpdateNode.pSpatial = m_pStaticRoot;
			StaticUpdateNode.dAppTime = dAppTime;
			ENQUEUE_UNIQUE_UPDATE_COMMAND_ONEPARAMETER(VSUpdateStaticNode,
				UpdateNode, StaticUpdateNode, StaticUpdateNode,
				{
				StaticUpdateNode.pSpatial->UpdateAll(StaticUpdateNode.dAppTime);
			})
				StaticUpdateNode.pSpatial->UpdateAll(StaticUpdateNode.dAppTime);
			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}

		for (unsigned int i = 0; i < m_pDynamic.GetNum(); i++)
		{
			UpdateNode DynamicUpdateNode;
			DynamicUpdateNode.pSpatial = m_pDynamic[i];
			DynamicUpdateNode.dAppTime = dAppTime;
			ENQUEUE_UNIQUE_UPDATE_COMMAND_ONEPARAMETER(VSUpdateDynamicNode,
				UpdateNode, DynamicUpdateNode, DynamicUpdateNode,
				{
				DynamicUpdateNode.pSpatial->UpdateAll(DynamicUpdateNode.dAppTime);
			})
				DynamicUpdateNode.pSpatial->UpdateAll(DynamicUpdateNode.dAppTime);
			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_ObjectNodes.GetNum(); i++)
		{
			m_ObjectNodes[i]->UpdateAll(dAppTime);
		}
	}

}
void VSScene::CollectUpdateInfo()
{
	if(m_pAllCamera.GetNum() > 0)
		m_pAllCamera.Clear();
	if(m_pAllLight.GetNum() > 0)
		m_pAllLight.Clear();
	if (m_bIsBuild == false)
	{

		for (unsigned int i = 0; i < m_ObjectNodes.GetNum(); i++)
		{
			if (m_ObjectNodes[i])
			{
				if (m_ObjectNodes[i]->m_pAllLight.GetNum() > 0)
					m_pAllLight.AddElement(m_ObjectNodes[i]->m_pAllLight, 0, m_ObjectNodes[i]->m_pAllLight.GetNum() - 1);
			}
		}
		for (unsigned int i = 0; i < m_ObjectNodes.GetNum(); i++)
		{
			if (m_ObjectNodes[i])
			{
				if (m_ObjectNodes[i]->m_pAllCamera.GetNum() > 0)
					m_pAllCamera.AddElement(m_ObjectNodes[i]->m_pAllCamera, 0, m_ObjectNodes[i]->m_pAllCamera.GetNum() - 1);
			}
		}
	}
	else
	{
		if (m_pStaticRoot)
		{
			if (m_pStaticRoot->m_pAllCamera.GetNum() > 0)
				m_pAllCamera.AddElement(m_pStaticRoot->m_pAllCamera, 0, m_pStaticRoot->m_pAllCamera.GetNum() - 1);
		}


		for (unsigned int i = 0; i < m_pDynamic.GetNum(); i++)
		{
			if (m_pDynamic[i])
			{
				if (m_pDynamic[i]->m_pAllCamera.GetNum() > 0)
					m_pAllCamera.AddElement(m_pDynamic[i]->m_pAllCamera, 0, m_pDynamic[i]->m_pAllCamera.GetNum() - 1);
			}

		}

		if (m_pStaticRoot)
		{
			if (m_pStaticRoot->m_pAllLight.GetNum() > 0)
				m_pAllLight.AddElement(m_pStaticRoot->m_pAllLight, 0, m_pStaticRoot->m_pAllLight.GetNum() - 1);
		}



		for (unsigned int i = 0; i < m_pDynamic.GetNum(); i++)
		{
			if (m_pDynamic[i])
			{
				if (m_pDynamic[i]->m_pAllLight.GetNum() > 0)
					m_pAllLight.AddElement(m_pDynamic[i]->m_pAllLight, 0, m_pDynamic[i]->m_pAllLight.GetNum() - 1);
			}


		}
	}
}
void VSScene::AddObject(VSNode * pObject)
{
	if (pObject)
	{
		m_ObjectNodes.AddElement(pObject);
	}
	if (m_bIsBuild)
	{
		m_pDynamic.AddElement(pObject);
	}
}
void VSScene::DeleteObject(VSNode *pObject)
{
	
	if (pObject)
	{
		
		for (unsigned int i = 0; i < m_ObjectNodes.GetNum(); i++)
		{
			if (pObject == m_ObjectNodes[i])
			{
				m_ObjectNodes.Erase(i);
				if (m_bIsBuild)
				{
					for (unsigned int j = 0; j < m_pDynamic.GetNum(); j++)
					{
						if (pObject == m_pDynamic[i])
						{
							m_pDynamic.Erase(j);
							return;
						}
					}
					VSNode * pSpatial = DynamicCast<VSNode>(pObject->GetParent());
					if (pSpatial)
					{
						pSpatial->DeleteChild(pObject);
					}
					return;
				}
			}
		}
		
	}
}

void VSScene::DeleteAllObject()
{
	m_pDynamic.Clear();
	m_pStaticRoot = NULL;
	m_ObjectNodes.Clear();
}

void VSScene::ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	ADD_TIME_PROFILE(ComputeVisibleSet);
	if (m_bIsBuild == false)
	{
		for (unsigned int i = 0; i < m_ObjectNodes.GetNum(); i++)
		{
			if (m_ObjectNodes[i])
			{

				m_ObjectNodes[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);
			}

		}
	}
	else
	{
		if (m_pStaticRoot)
		{

			m_pStaticRoot->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		}
		for (unsigned int i = 0; i < m_pDynamic.GetNum(); i++)
		{
			if (m_pDynamic[i])
			{

				m_pDynamic[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);
			}

		}
	}
	
}
//add loaded Scene
bool VSScene::Build()
{	
	m_pDynamic.Clear();
	m_pStaticRoot = NULL;
	VSArray<VSSpatial *> pStatic;
	for (unsigned int i = 0; i < m_ObjectNodes.GetNum();i++)
	{
		m_ObjectNodes[i]->UpdateAll(0);
		VSMeshComponent * pMesh = DynamicCast<VSMeshComponent>(m_ObjectNodes[i]);
		if (pMesh && !pMesh->IsDynamic())
		{
			pStatic.AddElement(m_ObjectNodes[i]);
		}
		else
		{
			m_pDynamic.AddElement(m_ObjectNodes[i]);
		}
	}
	m_pStaticRoot = VS_NEW VSQuadNode();
	if (!m_pStaticRoot->RecursiveBuild(pStatic))
	{
		m_pStaticRoot = NULL;
	}
	else
	{
		m_pStaticRoot->UpdateAll(0.0f);
	}
	
	CollectUpdateInfo();
	for (unsigned int i = 0; i < m_pAllLight.GetNum(); i++)
	{
		m_pAllLight[i]->BuildSceneInfo(this);
	}
	m_bIsBuild = true;
	return 1;
}
IMPLEMENT_RTTI(VSQuadNode,VSNode)
BEGIN_ADD_PROPERTY(VSQuadNode,VSNode)
REGISTER_PROPERTY(m_pNeedDrawNode, NeedDrawNode, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSQuadNode)
IMPLEMENT_INITIAL_END
VSQuadNode::VSQuadNode()
{

}
VSQuadNode::~VSQuadNode()
{

}
unsigned int VSQuadNode::AddChild(VSSpatial * pChild)
{
	unsigned int id = VSNode::AddChild(pChild);
	VSNodeComponent * pNode = DynamicCast<VSNodeComponent>(pChild);
	if (pNode &&  pNode->IsNeedDraw())
	{
		AddNeedDrawNode(pNode);
	}
	return id;
}
unsigned int VSQuadNode::DeleteChild(VSSpatial *pChild)
{
	unsigned int id = VSNode::DeleteChild(pChild);

	VSNodeComponent * pNode = DynamicCast<VSNodeComponent>(pChild);
	if (pNode &&  pNode->IsNeedDraw())
	{
		DeleteNeedDrawNode(pNode);
	}
	return id;
}
bool VSQuadNode::DeleteChild(unsigned int i)
{
	if (i >= m_pChild.GetNum())
		return 0;
	VSSpatial * Temp = m_pChild[i];
	VSNodeComponent * pNode = DynamicCast<VSNodeComponent>(Temp);
	if (pNode && pNode->IsNeedDraw())
	{
		DeleteNeedDrawNode(pNode);
	}
	return VSNode::DeleteChild(i);
}
void VSQuadNode::DeleteAllChild()
{
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		VSNodeComponent * pNode = DynamicCast<VSNodeComponent>(m_pChild[i]);
		if (pNode&& pNode->IsNeedDraw())
		{
			DeleteNeedDrawNode(pNode);
		}
	}
	VSNode::DeleteAllChild();
}
VSNodeComponent *VSQuadNode::GetNeedDrawNode(unsigned int uiIndex)const
{
	if (uiIndex < m_pNeedDrawNode.GetNum())
	{
		return m_pNeedDrawNode[uiIndex];
	}
	return NULL;
}
unsigned int VSQuadNode::GetNeedDrawNodeNum()const
{
	return m_pNeedDrawNode.GetNum();
}

void VSQuadNode::ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime)
{
	
	if (bNoCull && m_pNeedDrawNode.GetNum() > 0)
	{
		for (unsigned int i = 0; i < m_pNeedDrawNode.GetNum(); i++)
		{
			if (m_pNeedDrawNode[i])
			{
				m_pNeedDrawNode[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);
			}
		}
	}
	else
	{
		VSNode::ComputeNodeVisibleSet(Culler, bNoCull, dAppTime);
	}


}
void VSQuadNode::AddNeedDrawNode(VSNodeComponent *pNeedDrawNode)
{


	VSQuadNode * pNode = DynamicCast<VSQuadNode>(m_pParent);

	if (pNode)
	{
		pNode->AddNeedDrawNode(pNeedDrawNode);
	}
	unsigned int i = m_pNeedDrawNode.FindElement(pNeedDrawNode);
	if (i >= m_pNeedDrawNode.GetNum())
	{
		m_pNeedDrawNode.AddElement(pNeedDrawNode);
	}


}
void VSQuadNode::DeleteNeedDrawNode(VSNodeComponent *pNeedDrawNode)
{

	VSQuadNode * pNode = DynamicCast<VSQuadNode>(m_pParent);
	if (pNode)
	{
		pNode->DeleteNeedDrawNode(pNeedDrawNode);
	}
	unsigned int i = m_pNeedDrawNode.FindElement(pNeedDrawNode);
	if (i < m_pNeedDrawNode.GetNum())
	{
		m_pNeedDrawNode.Erase(i);
	}

}
bool VSQuadNode::RecursiveBuild(const VSArray<VSSpatial *> &pObjectArray)
{
	if (!pObjectArray.GetNum())
	{
		return 0;
	}
	//递归结束掉条件
	if (pObjectArray.GetNum() < MAX_NUM)
	{
		for (unsigned int i = 0 ; i < pObjectArray.GetNum() ; i++)
		{
			AddChild(pObjectArray[i]);
		}
		return 1;
	}
	//算出当前所有节点的最大Bounds
	bool bFound = false;
	VSAABB3 Total;
	for (unsigned int i = 0 ; i < pObjectArray.GetNum() ; i++)
	{
		if (pObjectArray[i])
		{
			const VSAABB3 & WorldAABB = pObjectArray[i]->GetWorldAABB();

			if (!bFound)
			{
				Total = WorldAABB;
				bFound = true;
			}
			else
			{
				Total = Total.MergAABB(WorldAABB);
			}

		}
	}
	//划分四个区域，然后给节点归类
	VSAABB3 ChildAABB[4];
	Total.GetQuadAABB(ChildAABB);
	VSArray<VSSpatial *> ChildStatic[4];
	for (unsigned int i = 0 ; i < pObjectArray.GetNum() ; i++)
	{
		if (!pObjectArray[i])
		{
			continue;
		}
		for (unsigned int j = 0 ; j < 4 ; j++)
		{
			if (ChildAABB[j].RelationWith(pObjectArray[i]->GetWorldTranslate()) != VSOUT)
			{
				ChildStatic[j].AddElement(pObjectArray[i]);
				break;
			}
		}
	}
	//递归继续划分
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		if (ChildStatic[i].GetNum())
		{
			VSQuadNode * pQuadNode = VS_NEW VSQuadNode();
			AddChild(pQuadNode);
			if(!pQuadNode->RecursiveBuild(ChildStatic[i]))
				return 0;
		}
	}
	return 1;
}
FORCEINLINE VSLight * VSScene::GetAllLight(unsigned int i)const
{
	if (i >= m_pAllLight.GetNum())
	{
		return NULL;
	}
	return m_pAllLight[i];
}
FORCEINLINE unsigned int VSScene::GetAllLightNum()const
{
	return m_pAllLight.GetNum();
}
