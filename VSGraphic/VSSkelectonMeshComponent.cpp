#include "VSSkelectonMeshComponent.h"
#include "VSSkelectonMeshNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSkelectonMeshComponent, VSMeshComponent)
BEGIN_ADD_PROPERTY(VSSkelectonMeshComponent, VSMeshComponent)
REGISTER_PROPERTY(m_pSkelectonMeshResource, SkelectonMeshResource, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkelectonMeshComponent)
IMPLEMENT_INITIAL_END
VSSkelectonMeshComponent::VSSkelectonMeshComponent()
{
	m_bIsStatic = false;
	m_pSkelectonMeshResource = VSResourceManager::ms_DefaultSkelectonMeshNodeResource;
}
VSSkelectonMeshComponent::~VSSkelectonMeshComponent()
{

}
bool VSSkelectonMeshComponent::BeforeSave(void * pDate)
{
	if (m_pSkelectonMeshResource == VSResourceManager::ms_DefaultSkelectonMeshNodeResource)
	{
		m_pSkelectonMeshResource = NULL;
	}
	return true;
}
void VSSkelectonMeshComponent::SetSkelectonMeshResource(VSSkelectonMeshNodeR * pSkelectonMeshResource)
{
	if (m_pSkelectonMeshResource == pSkelectonMeshResource)
	{
		return;
	}
	m_pSkelectonMeshResource = pSkelectonMeshResource;
	m_UseID.Clear();
	PostCreate();
}
void VSSkelectonMeshComponent::PostCreate()
{
	if (!m_pSkelectonMeshResource)
	{
		return;
	}
	LoadedEvent(NULL);
	if (!m_pSkelectonMeshResource->IsLoaded())
	{
		m_pSkelectonMeshResource->AddLoadEventObject(this);
	}
}
void VSSkelectonMeshComponent::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	if (m_pSkelectonMeshResource)
	{
		if (m_pNode)
		{
			m_pNode->SetParent(NULL);
			m_pSaveNode = m_pNode;
			VSResourceManager::GetDelayUpdateObjectOneFrame().AddMethod<VSMeshComponent, &VSMeshComponent::DelayUpdate>(this);
		}	
		m_pNode = (VSModelMeshNode *)VSObject::CloneCreateObject(m_pSkelectonMeshResource->GetResource());
		m_pNode->SetParent(this);
		if (m_pSkelectonMeshResource->IsLoaded())
		{
			ResetUseID();
		}
		SetPostLoadNodeParam();
	}
}
void VSSkelectonMeshComponent::SetIsDrawSkelecton(bool bIsDrawSkelecton)
{
	VSMeshNode * pMeshNode = m_pNode;
	if (pMeshNode)
	{
		((VSSkelectonMeshNode *)pMeshNode)->SetIsDrawSkelecton(bIsDrawSkelecton);
	}
	
}
bool VSSkelectonMeshComponent::PlayAnim(const VSString & AnimName, VSREAL fRatio, unsigned int uiRepeatType)
{
	VSMeshNode * pMeshNode = m_pNode;
	if (pMeshNode)
	{
		((VSSkelectonMeshNode *)pMeshNode)->PlayAnim(AnimName,fRatio,uiRepeatType);
	}
	return true;
}
VSSocketNode * VSSkelectonMeshComponent::GetSocketNode(const VSUsedName & SocketName)
{
	VSSkelectonMeshNode * pSkelectonNode = (VSSkelectonMeshNode *)m_pNode.GetObject();
	if (pSkelectonNode)
	{
		return pSkelectonNode->GetSocket(SocketName);
	}
	return NULL;
}
void VSSkelectonMeshComponent::SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara)
{

	VSSkelectonMeshNode * pSkelectonNode = (VSSkelectonMeshNode *)m_pNode.GetObject();
	if (pSkelectonNode)
	{
		return pSkelectonNode->SetAnimTreeNodePara(ShowName, pPara);
	}
}
bool VSSkelectonMeshComponent::PostLoad(void * pDate)
{
	if (!VSMeshComponent::PostLoad(pDate))
	{
		return false;
	}
	PostCreate();
	return true;
}
