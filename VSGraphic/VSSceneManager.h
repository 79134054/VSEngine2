#ifndef VSSCENEMANAGER_H
#define VSSCENEMANAGER_H
#include "VSSceneRender.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSSceneManager : public VSMemObject
	{
	public:
		VSSceneManager();
		~VSSceneManager();
		
		void Update(double dAppTime);
		void Draw(double dAppTime);
		static VSSceneManager * ms_pSceneManager;
		FORCEINLINE void AddViewFamily(VSViewFamily* pViewFamily);
		FORCEINLINE VSViewFamily * GetViewFamily(unsigned int i)const;
		FORCEINLINE VSViewFamily * GetViewFamily(const VSString & ViewFamilyName)const;
		FORCEINLINE void ClearAllViewFamily();
		FORCEINLINE void DeleteViewFamily(unsigned int i);
		FORCEINLINE void DeleteViewFamily(VSViewFamily * pViewFamily);
		FORCEINLINE unsigned int GetViewFamilyNum()const;
		
		FORCEINLINE void AddScene(VSScene* pScene);
		FORCEINLINE VSScene * GetScene(unsigned int i)const;
		FORCEINLINE void ClearAllScene();
		FORCEINLINE void DeleteScene(unsigned int i);
		FORCEINLINE void DeleteScene(VSScene * pScene);
		FORCEINLINE unsigned int GetSceneNum()const;
	protected:

		VSArray<VSViewFamily*> m_pViewFamily;
		VSArray<VSScene *> m_pScene;
		
	
	};
	
	FORCEINLINE void VSSceneManager::AddScene(VSScene * pSecen)
	{
		if(pSecen)
		{
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				if (m_pScene[i] == pSecen)
				{
					return ;
				}
			}
			m_pScene.AddElement(pSecen);
		}
	}
	FORCEINLINE VSScene * VSSceneManager::GetScene(unsigned int i)const
	{
		if(i >= m_pScene.GetNum())
			return NULL;
		return m_pScene[i];
	}
	FORCEINLINE void VSSceneManager::ClearAllScene()
	{
		m_pScene.Clear();
	}
	FORCEINLINE void VSSceneManager::DeleteScene(unsigned int i)
	{	
		if(i >= m_pScene.GetNum())
			return ;
		m_pScene.Erase(i);

	}
	FORCEINLINE void VSSceneManager::DeleteScene(VSScene * pScene)
	{
		if (!pScene)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pScene.GetNum() ; i++)
		{
			if(pScene == m_pScene[i])
			{
				m_pScene.Erase(i);
			}
		}
	}

	FORCEINLINE unsigned int VSSceneManager::GetSceneNum()const
	{
		return m_pScene.GetNum();
	}
	FORCEINLINE void VSSceneManager::AddViewFamily(VSViewFamily * pViewFamily)
	{
		if(pViewFamily)
		{
			for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
			{
				if (m_pViewFamily[i] == pViewFamily)
				{
					return ;
				}
			}
			m_pViewFamily.AddElement(pViewFamily);
		}
	}
	FORCEINLINE VSViewFamily * VSSceneManager::GetViewFamily(unsigned int i)const
	{
		if(i >= m_pViewFamily.GetNum())
			return NULL;
		return m_pViewFamily[i];
	}
	FORCEINLINE void VSSceneManager::ClearAllViewFamily()
	{
		m_pViewFamily.Clear();
	}
	FORCEINLINE void VSSceneManager::DeleteViewFamily(unsigned int i)
	{	
		if(i >= m_pViewFamily.GetNum())
			return ;
		m_pViewFamily.Erase(i);

	}
	FORCEINLINE void VSSceneManager::DeleteViewFamily(VSViewFamily * pViewFamily)
	{
		if (!pViewFamily)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pViewFamily.GetNum() ; i++)
		{
			if(pViewFamily == m_pViewFamily[i])
			{
				m_pViewFamily.Erase(i);
			}
		}
	}

	FORCEINLINE unsigned int VSSceneManager::GetViewFamilyNum()const
	{
		return m_pViewFamily.GetNum();
	}

}
#endif