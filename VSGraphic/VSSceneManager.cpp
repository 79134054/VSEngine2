#include "VSSceneManager.h"
#include "VSGraphicInclude.h"
#include "VSUpdateThread.h"
#include "VSTimer.h"
#include "VSProfiler.h"
using namespace VSEngine2;
VSSceneManager * VSSceneManager::ms_pSceneManager = NULL;
DECLEAR_TIME_PROFILENODE(VSSceneManagerUpdate,Update)
DECLEAR_TIME_PROFILENODE(VSSceneManagerDraw,Update)
DECLEAR_TIME_PROFILENODE(ViewFamilyUpdate,VSSceneManagerDraw);
DECLEAR_TIME_PROFILENODE(ViewFamilyDraw, VSSceneManagerDraw);
VSSceneManager::VSSceneManager()
{
	
	ms_pSceneManager = this;
}
VSSceneManager::~VSSceneManager()
{
	
}

void VSSceneManager::Update(double dAppTime)
{
	ADD_TIME_PROFILE(VSSceneManagerUpdate)
	for (unsigned int i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		m_pScene[i]->Update(dAppTime);
	}
	if (VSResourceManager::ms_bUpdateThread)
	{
		VSUpdateThreadSys::ms_pUpdateThreadSys->Begin();
/*		double Time = VSTimer::ms_pTimer->GetGamePlayTime();*/
		VSUpdateThreadSys::ms_pUpdateThreadSys->ExChange();
// 		Time = VSTimer::ms_pTimer->GetGamePlayTime() - Time;
// 		VSOutPutDebugString("Time %f\n",Time);
	}
	
	for (unsigned int i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		m_pScene[i]->CollectUpdateInfo();
	}
}

void VSSceneManager::Draw(double dAppTime)
{
	ADD_TIME_PROFILE(VSSceneManagerDraw)
	for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
	{
		if (m_pViewFamily[i] && m_pViewFamily[i]->m_bEnable)
		{
			{
				ADD_TIME_PROFILE(ViewFamilyUpdate)
				m_pViewFamily[i]->Update(dAppTime);
			}

			{
				ADD_TIME_PROFILE(ViewFamilyDraw)
				m_pViewFamily[i]->Draw(dAppTime);
			}
			
		}
		
	}
	for (unsigned int i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		for (unsigned int j = 0 ; j < m_pScene[i]->GetAllLightNum() ;j++)
		{
			VSLocalLight * pLight = DynamicCast<VSLocalLight>(m_pScene[i]->GetAllLight(j));
			if (pLight)
			{
				pLight->DrawLightMaterial(dAppTime);
				pLight->DisableNoDepandenceShadowMap(dAppTime);
			}
			
		}

	}
}
VSViewFamily * VSSceneManager::GetViewFamily(const VSString & ViewFamilyName)const
{
	for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
	{
		if (m_pViewFamily[i]->m_ViewFamilyName == ViewFamilyName)
		{
			return m_pViewFamily[i];
		}
	}
	return NULL;
}
