#include "VSWorld.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSResourceManager.h"
#include "VSStaticActor.h"
#include "VSSkelectonActor.h"
#include "VSGraphicInclude.h"
#include "VSSceneManager.h"
#include "VSLightActor.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSWorld,VSObject)
BEGIN_ADD_PROPERTY(VSWorld,VSObject)
REGISTER_PROPERTY(m_SceneArray, SceneArray, VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSWorld)
IMPLEMENT_INITIAL_END
VSWorld * VSWorld::ms_pWorld = NULL;
DECLEAR_TIME_PROFILENODE(VSWorldUpdate, Update)
VSWorld::VSWorld()
{
	VSMAC_ASSERT(ms_pWorld == NULL);
	ms_pWorld = this;
	VSSceneMap * pSceneMap = VS_NEW VSSceneMap("Main");
	m_SceneArray.AddElement(pSceneMap);
	VSResourceManager::AddRootObject(this);
	IncreRef();//default reference = 1 ,避免智能指针指向后释放。
}
VSWorld::~VSWorld()
{
	VSResourceManager::DeleteAllMapResource();
	for (unsigned int i = 0; i < m_SceneArray.GetNum();i++)
	{
		if (m_SceneArray[i])
		{
			m_SceneArray[i]->OnDestroy();
		}
	}
	m_SceneArray.Clear();
	m_ActorArray.Clear();
	VSResourceManager::GCObject();
	VSResourceManager::RunAllGCTask();
}
void VSWorld::DestroyScene(const VSUsedName & Name)
{
	//first scene  must be not delete
	for (unsigned int i = 1; i < m_SceneArray.GetNum(); i++)
	{
		if (m_SceneArray[i]->m_Name == Name)
		{
			for (unsigned int j = 0; j < m_ActorArray.GetNum();)
			{
				if (m_ActorArray[j]->m_pSceneMap == m_SceneArray[i])
				{
					DestroyActor(m_ActorArray[j]);
				}
				else
				{
					j++;
				}
			}
			m_SceneArray[i]->OnDestroy();
			m_SceneArray[i]->SetFlag(OF_PendingKill);
			m_SceneArray.Erase(i);
			return;
		}
	}

}

VSSceneMap* VSWorld::CreateScene(const TCHAR * pName)
{
	for (unsigned int i = 0 ; i < m_SceneArray.GetNum() ;i++)
	{
		if(m_SceneArray[i]->m_Name == pName)
			return NULL;

	}
	VSSceneMap * pSceneMap = VS_NEW VSSceneMap(pName);
	VSResourceManager::AddGCObject(pSceneMap);
	m_SceneArray.AddElement(pSceneMap);
	return pSceneMap;
}
VSSceneMap * VSWorld::GetScene(const VSUsedName & Name)
{
	for (unsigned int i = 0 ; i < m_SceneArray.GetNum() ;i++)
	{
		if(m_SceneArray[i]->m_Name == Name)
			return m_SceneArray[i];

	}
	return NULL;
}
void VSWorld::ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z)
{

	for (unsigned int i = 0; i < m_ActorArray.GetNum(); i++)
	{
		if (!m_ActorArray[i]->IsHasFlag(VSObject::OF_PendingKill))
		{
			m_ActorArray[i]->ProcessInput(uiInputType,uiEvent,uiKey,x,y,z);
		}
	}
}
void VSWorld::Update(double dAppTime)
{
	ADD_TIME_PROFILE(VSWorldUpdate)
	static double LastTime = dAppTime;
	double DetTime = dAppTime - LastTime;
	LastTime = dAppTime;

	for (unsigned int i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (!m_ActorArray[i]->IsHasFlag(VSObject::OF_PendingKill))
		{
			m_ActorArray[i]->Update(dAppTime);
		}
	}
	unsigned int i = 0 ;
	while(i < m_MessageArray.GetNum())
	{
		if (m_MessageArray[i].DispatchTime <= 0.0)
		{
			VSActor *pActor = GetActor(m_MessageArray[i].Receiver);
			if (pActor)
			{
				pActor->HandleMessage(m_MessageArray[i]);
			}
			m_MessageArray.Erase(i);
		}
		else
		{
			m_MessageArray[i].DispatchTime -= DetTime;
			i++;
		}
	}

}
VSActor *VSWorld::GetActor(VSUsedName Name)
{
	for (unsigned int i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (m_ActorArray[i]->m_ActorName == Name)
		{

			return m_ActorArray[i];
		}
	}	
	return NULL;
}

void VSWorld::DeleteActor(VSActor * pActor)
{

	for (unsigned int i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (m_ActorArray[i] == pActor)
		{
			m_ActorArray.Erase(i);
			return ;
		}
	}
}
void VSWorld::AddActor(VSActor * pActor)
{
	if (pActor)
	{
		m_ActorArray.AddElement(pActor);
	}
	
}
bool VSWorld::LoadMap(const TCHAR * MapPath, bool IsAsyn)
{
	if (!MapPath)
	{
		return false;
	}
	VSResourceProxyBase * pResouce = VSResourceManager::LoadASYNMap(MapPath, IsAsyn);
	if (!pResouce)
	{
		return false;
	}
	else
	{
		pResouce->AddLoadEventObject(this);
		
	}
	return true;
}
bool VSWorld::UnLoadMap(const TCHAR * MapPath)
{
	if (!MapPath)
	{
		return false;
	}
	VSResourceManager::DeleteMapResource(MapPath);

	return true;
}
VSActor * VSWorld::CreateActor(const TCHAR *  ActorPath, const VSVector3 & Pos, const VSMatrix3X3 & Rotate, const VSVector3 &Scale, VSSceneMap * pSceneMap)
{
	if (!ActorPath)
	{
		return NULL;
	}
	VSActor * pActor = NULL;
	VSResourceProxyBase * pResouce = VSResourceManager::LoadResource(ActorPath,false);
	if (pResouce)
	{
		unsigned int ResourceType = pResouce->GetResourceType();
		if (ResourceType == VSResource::RT_STATIC_MODEL)
		{
			pActor = CreateActor<VSStaticActor>(Pos, Rotate, Scale,pSceneMap);
			((VSStaticMeshComponent *)pActor->GetActorNode())->SetStaticMeshResouce((VSStaticMeshNodeR *)pResouce);
		}
		else if (ResourceType == VSResource::RT_SKELECTON_MODEL)
		{
			pActor = CreateActor<VSSkelectonActor>(Pos, Rotate, Scale, pSceneMap);
			((VSSkelectonMeshComponent *)pActor->GetActorNode())->SetSkelectonMeshResource((VSSkelectonMeshNodeR *)pResouce);
		}
		else if (ResourceType == VSResource::RT_ACTOR)
		{
			if (!pResouce->IsLoaded())
			{
				unsigned int i = 0;
				for (; i < m_SceneArray.GetNum(); i++)
				{
					if (m_SceneArray[i]->m_Name == pSceneMap->m_Name)
						break;
				}
				pResouce->AddLoadEventObject(this,i);
				pActor = NULL;
			}
			else
			{
				pActor = (VSActor *)VSObject::CloneCreateObject(((VSActorR *)pResouce)->GetResource());
				if (pSceneMap)
				{
					pSceneMap->AddActor(pActor);
				}
				else
				{
					m_SceneArray[0]->AddActor(pActor);
				}
			}
		}
	}
	return pActor;
}
void VSWorld::AddSceneMap(VSSceneMap * pSceneMap)
{
	VSResourceManager::AddGCObject(pSceneMap);
	for (unsigned int i = 0; i < pSceneMap->m_pActorArray.GetNum(); i++)
	{
		AddActor(pSceneMap->m_pActorArray[i]);
	}
	m_SceneArray.AddElement(pSceneMap);
}
void VSWorld::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	if (pResourceProxy->GetResourceType() == VSResource::RT_ACTOR)
	{
		VSActor * pActor = (VSActor *)VSObject::CloneCreateObject(((VSActorR *)pResourceProxy)->GetResource());
		m_SceneArray[Data]->AddActor(pActor);
	}
	else if (pResourceProxy->GetResourceType() == VSResource::RT_MAP)
	{
		VSSceneMap * pSceneMap = ((VSResourceProxy<VSSceneMap> *)pResourceProxy)->GetResource();
		AddSceneMap(pSceneMap);
	}

}
void VSWorld::DestroyActor(VSActor * pActor)
{
	pActor->OnDestory();
	pActor->SetFlag(OF_PendingKill);
	DeleteActor(pActor);

}

void VSWorld::AddMessage(const VSMessage & Message)
{
	m_MessageArray.AddElement(Message);
}
void VSWorld::AttachCaptureViewFamilyToCamera(VSCameraActor * pCameraActor,
									 unsigned int uiCaptureViewFamilyType,const VSString & ViewFamilyName,
									 unsigned int uiWidth,unsigned int uiHeight,
									 VSArray<VSString> & SceneMapName,
									 const TCHAR * RenderMethodRTTIName,
									 VSPostEffectSetR*  pPostEffectSet)
{
	VSCamera * pCamera = (VSCamera *)pCameraActor->GetActorNode();
	if (pCamera)
	{
		VSViewFamliyInfo * pInfo = VS_NEW VSViewFamliyInfo();
		
		VSViewFamily * pViewFamily = NULL;
		if (uiCaptureViewFamilyType == VSCaptureViewFamily::VT_CAPTURE_2D_CAPTURE_NORMAL)
		{
			pViewFamily = VS_NEW VS2DCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName);

		}
		else if (uiCaptureViewFamilyType == VSCaptureViewFamily::VT_CAPTURE_CUB_CAPTURE_NORMAL)
		{
			pViewFamily = VS_NEW VSCubCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName);
		}

		pInfo->m_SceneMapName = SceneMapName;
		pInfo->m_uiCaptureViewFamilyType = uiCaptureViewFamilyType;
		pInfo->m_uiHeight = uiHeight;
		pInfo->m_uiWidth = uiWidth;
		pInfo->m_pPostEffect = pPostEffectSet;
		pInfo->m_ViewFamilyName = ViewFamilyName;
		pInfo->m_pViewFamily = pViewFamily;
		pCameraActor->m_ViewFamliyInfoArray.AddElement(pInfo);
		pInfo->GetScene();
	}
}
void VSWorld::AttachWindowViewFamilyToCamera(VSCameraActor * pCameraActor,
									unsigned int uiWindowViewFamilyType,const VSString & ViewFamilyName,
									VSArray<VSString> & SceneMapName,
									const TCHAR * RenderMethodRTTIName,
									int iWindowID,				
									VSPostEffectSetR* pPostEffectSet)
{
	VSCamera * pCamera = (VSCamera *)pCameraActor->GetActorNode();
	if (pCamera)
	{
		VSViewFamliyInfo * pInfo = VS_NEW VSViewFamliyInfo();

		VSViewFamily * pViewFamily = NULL;
		if (uiWindowViewFamilyType == VSWindowViewFamily::VT_WINDOW_NORMAL)
		{
			pViewFamily= VS_NEW VSWindowViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName,iWindowID);
			
			
		}
		pInfo->m_SceneMapName = SceneMapName;
		pInfo->m_uiCaptureViewFamilyType = uiWindowViewFamilyType;
		pInfo->m_pPostEffect = pPostEffectSet;
		pInfo->m_ViewFamilyName = ViewFamilyName;
		pInfo->m_pViewFamily = pViewFamily;
		pCameraActor->m_ViewFamliyInfoArray.AddElement(pInfo);
		pInfo->GetScene();
	}
}
void VSWorld::UpdateLightActorScene(VSLightActor * pActor)
{
	VSLight * pLightNode = pActor->GetTypeNode();
	VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(pLightNode);
	if (pLocalLight)
	{
		pLocalLight->ClearAllShadowScene();
		for (unsigned int i = 0 ; i < m_SceneArray.GetNum() ; i++)
		{
			pLocalLight->AddShadowScene(m_SceneArray[i]->m_pScene);
		}
	}
	
}