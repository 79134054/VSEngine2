#include "VSCameraActor.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSWorld.h"
#include "VSViewFamily.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSViewFamliyInfo,VSObject)
BEGIN_ADD_PROPERTY(VSViewFamliyInfo,VSObject)
REGISTER_PROPERTY(m_SceneMapName,SceneMapName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiWidth,Width,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiHeight,Height,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCaptureViewFamilyType,CaptureViewFamilyType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ViewFamilyName,ViewFamilyName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffect, PostEffect, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RenderMethodRTTIName, RenderMethodRTTIName, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSViewFamliyInfo)
IMPLEMENT_INITIAL_END
VSViewFamliyInfo::~VSViewFamliyInfo()
{

}
VSViewFamliyInfo::VSViewFamliyInfo()
{
	
}
void VSViewFamliyInfo::GetScene()
{

	m_pViewFamily->ClearAllScene();
	if (m_SceneMapName.GetNum() > 0)
	{
		for (unsigned int i = 0 ; i < m_SceneMapName.GetNum() ; i++)
		{
			VSSceneMap * pSceneMap = VSWorld::ms_pWorld->GetScene(m_SceneMapName[i]);
			if (pSceneMap)
			{
				m_pViewFamily->AddScene(pSceneMap->m_pScene);
			}			
		}
	}
	else
	{
		VSSceneMap * pSceneMap = VSWorld::ms_pWorld->GetScene(VSUsedName::ms_cMainScene);
		m_pViewFamily->AddScene(pSceneMap->m_pScene);
		
	}
	
}
IMPLEMENT_RTTI(VSCameraActor,VSActor)
BEGIN_ADD_PROPERTY(VSCameraActor,VSActor)
REGISTER_PROPERTY(m_ViewFamliyInfoArray,ViewFamliyInfoArray,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_NO_USE_GC)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCameraActor)
IMPLEMENT_INITIAL_END
VSCameraActor::VSCameraActor()
{
	
}
VSCameraActor::~VSCameraActor()
{

}
void VSCameraActor::Update(double dAppTime)
{
	VSActor::Update(dAppTime);
	for (unsigned int i = 0 ; i < m_ViewFamliyInfoArray.GetNum() ; i++)
	{
		m_ViewFamliyInfoArray[i]->GetScene();
		
	}
}
void VSCameraActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponet<VSCamera>();
}
void VSCameraActor::AddCustomCullPlane(const VSPlane3& P)
{
	GetTypeNode()->AddCustomCullPlane(P);
}
void VSCameraActor::ClearCustomCullPlane()
{
	GetTypeNode()->ClearCustomCullPlane();
}