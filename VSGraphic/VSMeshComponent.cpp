#include "VSMeshComponent.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMeshComponent, VSNodeComponent)
BEGIN_ADD_PROPERTY(VSMeshComponent, VSNodeComponent)
REGISTER_PROPERTY(m_pNode, pNode, VSProperty::F_CLONE | VSProperty::F_NO_USE_GC)
REGISTER_PROPERTY(m_UseID, UseID, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bCastShadow, bCastShadow, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMeshComponent)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSMeshComponent::VSMeshComponent()
{
	m_pNode = NULL;
	m_bCastShadow = true;
}
VSMeshComponent::~VSMeshComponent()
{

}
void VSMeshComponent::CreateLocalAABB()
{
	if (!m_pNode)
	{
		return;
	}

	m_pNode->CreateLocalAABB();

}
void VSMeshComponent::UpdateWorldBound(double dAppTime)
{
	if (!m_pNode)
	{
		return;
	}

	m_WorldBV = m_pNode->GetWorldAABB();
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;;
	}
	
}
void VSMeshComponent::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}

	UpdateTransform(dAppTime);


	if (m_pNode)
		m_pNode->UpdateAll(dAppTime);

	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
			m_pChild[i]->UpdateNodeAll(dAppTime);
	}

	UpdateLightState(dAppTime);

	UpdateCameraState(dAppTime);

	if (m_bIsChanged)
	{
		UpdateWorldBound(dAppTime);
	}
	m_bIsChanged = false;

}
void VSMeshComponent::ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime)
{

	UpDateView(Culler, dAppTime);

	if (m_pNode)
	{
		//set use material before cull geometry
		VSGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum() && i < m_UseID.GetNum(); i++)
		{
			pGeometryNode->GetGeometry(i)->SetUseMaterialInstance(m_UseID[i]);
		}

		m_pNode->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		
	}
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{

			m_pChild[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		}

	}
}

void VSMeshComponent::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	if (m_pNode)
	{
		m_pNode->SetIsVisibleUpdate(bIsVisibleUpdate);

	}

	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
void VSMeshComponent::SetIsDrawBoundVolume(bool bIsDrawBoundVolume)
{
	if (m_pNode)
	{
		m_pNode->SetIsDrawBoundVolume(bIsDrawBoundVolume);
	}
}
void VSMeshComponent::SetMorphTreeNodePara(const VSUsedName & ShowName, void * pPara)
{
	VSModelMeshNode * pModelMeshNode = DynamicCast<VSModelMeshNode>(m_pNode);
	if (pModelMeshNode)
	{
		return pModelMeshNode->SetMorphTreeNodePara(ShowName, pPara);
	}
}
void VSMeshComponent::CastShadow(bool bCastShadow)
{
	if (m_pNode)
	{
		m_bCastShadow = bCastShadow;
		m_pNode->m_bCastShadow = bCastShadow;
	}
}
void VSMeshComponent::SetPostLoadNodeParam()
{
	CastShadow(m_bCastShadow);
}
void VSMeshComponent::ResetUseID()
{
	//get lod 0
	if (m_UseID.GetNum() == 0)
	{
		VSGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);
		m_UseID.SetBufferNum(pGeometryNode->GetNormalGeometryNum());
		for (unsigned int i = 0; i < m_UseID.GetNum(); i++)
		{
			m_UseID[i] = 0;
		}
	}
}
VSMaterialInstance * VSMeshComponent::GetUseMaterialInstance(unsigned int SubMeshID)const
{
	if (SubMeshID >= m_UseID.GetNum())
	{
		return NULL;
	}
	VSGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);
	
	if (SubMeshID >= pGeometryNode->GetNormalGeometryNum())
	{
		return NULL;
	}

	return pGeometryNode->GetGeometry(SubMeshID)->GetUseMaterialInstance();
}

VSMaterialInstance * VSMeshComponent::GetMaterialInstance(unsigned int SubMeshID, unsigned int UseID)const
{

	if (SubMeshID >= m_UseID.GetNum())
	{
		return NULL;
	}
	VSGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);

	if (SubMeshID >= pGeometryNode->GetNormalGeometryNum())
	{
		return NULL;
	}

	return pGeometryNode->GetGeometry(SubMeshID)->GetMaterialInstance(UseID);
}

bool VSMeshComponent::SetUseMaterialInstance(unsigned int SubMeshID, unsigned int UseID)
{
	if (SubMeshID >= m_UseID.GetNum())
	{
		return false;
	}
	m_UseID[SubMeshID] = UseID;
	return true;
}
unsigned int VSMeshComponent::GetSubMeshNum()
{
	VSGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);

	return pGeometryNode->GetNormalGeometryNum();
}
void VSMeshComponent::DelayUpdate()
{
	m_pSaveNode = NULL;
}