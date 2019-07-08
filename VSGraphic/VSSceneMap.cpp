#include "VSSceneMap.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSSceneManager.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSceneMap,VSObject)
BEGIN_ADD_PROPERTY(VSSceneMap,VSObject)
REGISTER_PROPERTY(m_Name, Name, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pScene, Scene, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_NO_USE_GC)
REGISTER_PROPERTY(m_pActorArray, ActorArray, VSProperty::F_SAVE_LOAD_CLONE  | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSceneMap)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSSceneMapPtr VSSceneMap::Default = NULL;
bool VSSceneMap::ms_bIsEnableASYNLoader = true;
bool VSSceneMap::ms_bIsEnableGC = false;
VSSceneMap::VSSceneMap(const TCHAR * pName)
{
	m_Name = pName;
	m_pScene = VS_NEW VSScene();
	
}
VSSceneMap::~VSSceneMap()
{
	m_pScene = NULL;
}
VSSceneMap::VSSceneMap()
{
	
}
void VSSceneMap::OnDestroy()
{
	m_pScene = NULL;
}
bool VSSceneMap::InitialDefaultState()
{
	Default = NULL;// VS_NEW VSSceneMap();
	return true;
}
bool VSSceneMap::TerminalDefaultState()
{
	Default = NULL;
	return true;
}
void VSSceneMap::AddActor(VSActor * pActor)
{

	if (pActor)
	{
		for (unsigned int i = 0 ; i < m_pActorArray.GetNum() ; i++)
		{
			if (pActor == m_pActorArray[i])
			{
				pActor->m_pSceneMap = this;
				return ;
			}
		}
		if (pActor->m_pSceneMap)
		{
			pActor->m_pSceneMap->DeleteActor(pActor);
		}
		
		m_pActorArray.AddElement(pActor);

		m_pScene->AddObject(pActor->GetActorNode());

		pActor->AddToSceneMap(this);
	}
}
VSActor *VSSceneMap::GetActor(VSUsedName Name)
{
	for (unsigned int i = 0 ; i < m_pActorArray.GetNum() ; i++)
	{
		if (m_pActorArray[i]->m_ActorName == Name)
		{
			
			return m_pActorArray[i];
		}
	}	
	return NULL;
}

void VSSceneMap::DeleteActor(VSActor * pActor)
{

	for (unsigned int i = 0 ; i < m_pActorArray.GetNum() ; i++)
	{
		if (m_pActorArray[i] == pActor)
		{
			if (m_pActorArray[i]->GetOwner())
			{
				m_pActorArray[i]->GetOwner()->DeleteChildActor(m_pActorArray[i]);
			}
			else
			{
				m_pScene->DeleteObject(m_pActorArray[i]->GetActorNode());
			}
			m_pActorArray[i]->AddToSceneMap(NULL);
			m_pActorArray.Erase(i);
			return ;
		}
	}
}
bool VSSceneMap::PostLoad(void * pDate)
{
	VSObject::PostLoad(pDate);
	//VSSceneManager::ms_pSceneManager->AddScene(m_pScene);
	return  true;
}