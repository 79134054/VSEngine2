#ifndef VSSCENEMAP_H
#define VSSCENEMAP_H
#include "VSScene.h"
#include "VSActor.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSSceneMap : public VSObject, public VSResource
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSceneMap(const TCHAR * pName);
		VSSceneMap();
		virtual ~VSSceneMap();
		void AddActor(VSActor * pActor);
		VSActor *GetActor(VSUsedName Name);
		void DeleteActor(VSActor * pActor);
		VSUsedName m_Name;
		friend class VSViewFamliyInfo;
		friend class VSWorld;
		friend class VSReflectCubLightActor;
		virtual bool PostLoad(void * pDate = NULL);
		VSScene *GetScene()
		{
			return m_pScene;
		}
		virtual void OnDestroy();
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		static VSPointer<VSSceneMap> Default;
	public:
		static const VSSceneMap *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual unsigned int GetResourceType()const
		{
			return RT_MAP;
		}
	protected:

		VSScenePtr m_pScene;
		VSArray<VSActorPtr> m_pActorArray;

	};
	DECLARE_Ptr(VSSceneMap);
	VSTYPE_MARCO(VSSceneMap);
	DECLARE_Proxy(VSSceneMap);
}
#endif