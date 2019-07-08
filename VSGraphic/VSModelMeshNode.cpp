#include "VSModelMeshNode.h"
#include "VSSwitchNode.h"
#include "VSGraphicInclude.h"
#include "VSModelSwitchNode.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSModelMeshNode,VSMeshNode)
BEGIN_ADD_PROPERTY(VSModelMeshNode,VSMeshNode)
REGISTER_PROPERTY(m_uiLodType,LodType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMorphTree, MorphTree, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSModelMeshNode)
IMPLEMENT_INITIAL_END
VSModelMeshNode::VSModelMeshNode()
{
	m_uiLodType = 0;
	m_pMorphTree = NULL;
	m_pMorphTreeInstance = NULL;
}
VSModelMeshNode::~VSModelMeshNode()
{
	m_pMorphTree = NULL;
	m_pMorphTreeInstance = NULL;
}
VSGeometryNode * VSModelMeshNode::GetGeometryNode(unsigned int uiLodLevel)
{
	if (m_pChild.GetNum() > 1)
	{
		return NULL;
	}
	else
	{
		VSModelSwitchNode* Temp = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
		if (!Temp)
		{
			VSGeometryNode * pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[0]);
			return pGeometryNode;
		}
		else
		{
			return	Temp->GetGeometryNode(uiLodLevel);
		}
	}
}
VSSwitchNode * VSModelMeshNode::GetDlodNode()const
{
	if (m_pChild.GetNum())
	{
		return DynamicCast<VSSwitchNode>(m_pChild[0]);
	}
	return NULL;
}
void VSModelMeshNode::SetMorphTree(VSMorphTreeR * pMorphTree)
{
	if (pMorphTree)
	{
		m_bIsStatic = false;
		m_pMorphTree = pMorphTree;
		m_pMorphTree->AddLoadEventObject(this);
	}
}
void VSModelMeshNode::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{

	if (m_pMorphTree == pResourceProxy)
	{
		m_pMorphTreeInstance = (VSMorphTree *)VSObject::CloneCreateObject(m_pMorphTree->GetResource());
		m_pMorphTreeInstance->SetObject(this);
	}
}
bool VSModelMeshNode::PostLoad(void * pDate)
{
	VSMeshNode::PostLoad(pDate);
	if (m_pMorphTree)
	{
		m_pMorphTree->AddLoadEventObject(this);
	}

	return true;
}
bool VSModelMeshNode::PostClone(VSObject * pObjectSrc)
{
	VSMeshNode::PostClone(pObjectSrc);
	if (m_pMorphTree)
	{
		m_pMorphTree->AddLoadEventObject(this);
	}
	return true;
}
void VSModelMeshNode::UpdateController(double dAppTime)
{
	VSMeshNode::UpdateController(dAppTime);

	if (m_pMorphTreeInstance && m_bEnable)
	{
		m_pMorphTreeInstance->Update(dAppTime);
	}
}
void VSModelMeshNode::SetMorphTreeNodePara(const VSUsedName & ShowName, void * pPara)
{
	if (m_pMorphTreeInstance)
	{
		m_pMorphTreeInstance->SetNodePara(ShowName, pPara);
	}
}