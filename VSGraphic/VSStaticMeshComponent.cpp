#include "VSStaticMeshComponent.h"
#include "VSStaticMeshNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSStaticMeshComponent, VSMeshComponent)
BEGIN_ADD_PROPERTY(VSStaticMeshComponent, VSMeshComponent)
REGISTER_PROPERTY(m_pStaticMeshResource, StaticMeshResource, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStaticMeshComponent)
IMPLEMENT_INITIAL_END
VSStaticMeshComponent::VSStaticMeshComponent()
{
	m_pStaticMeshResource = VSResourceManager::ms_DefaultStaticMeshNodeResource;
}
VSStaticMeshComponent::~VSStaticMeshComponent()
{

}
bool VSStaticMeshComponent::BeforeSave(void * pDate)
{
	if (m_pStaticMeshResource == VSResourceManager::ms_DefaultStaticMeshNodeResource)
	{
		m_pStaticMeshResource = NULL;
	}
	return true;
}
void VSStaticMeshComponent::SetStaticMeshResouce(VSStaticMeshNodeR * pStaticMeshResource)
{
	if (m_pStaticMeshResource == pStaticMeshResource)
	{
		return;
	}
	m_pStaticMeshResource = pStaticMeshResource;
	m_UseID.Clear();
	PostCreate();
}
void VSStaticMeshComponent::PostCreate()
{
	if (!m_pStaticMeshResource)
	{
		return;
	}
	LoadedEvent(NULL);

	if (!m_pStaticMeshResource->IsLoaded())
	{
		m_pStaticMeshResource->AddLoadEventObject(this);
	}

}
void VSStaticMeshComponent::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	if (m_pStaticMeshResource)
	{
		if (m_pNode)
		{
			m_pNode->SetParent(NULL);

			m_pSaveNode = m_pNode;
			VSResourceManager::GetDelayUpdateObjectOneFrame().AddMethod<VSMeshComponent, &VSMeshComponent::DelayUpdate>(this);
		}
		m_pNode = (VSModelMeshNode *)VSObject::CloneCreateObject(m_pStaticMeshResource->GetResource());
		m_pNode->SetParent(this);
		if (m_pStaticMeshResource->IsLoaded())
		{
			ResetUseID();
		}
		SetPostLoadNodeParam();
		m_bIsStatic = !m_pNode->IsDynamic();
	}
}
bool VSStaticMeshComponent::PostLoad(void * pDate)
{
	if (!VSMeshComponent::PostLoad(pDate))
	{
		return false;
	}
	PostCreate();
	return true;
}
