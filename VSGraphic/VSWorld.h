#ifndef VSWORLD_H
#define VSWORLD_H
#include "VSObject.h"
#include "VSVector3.h"
#include "VSQuat.h"

#include "VSActor.h"
#include "VSSceneMap.h"
#include "VSCameraActor.h"
namespace VSEngine2
{
	class VSStream;
	class VSLightActor;
	class VSGRAPHIC_API VSWorld : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VSWorld();
		virtual ~VSWorld();

		DECLARE_INITIAL
		static VSWorld * ms_pWorld;

	protected:
		VSArray<VSSceneMapPtr>	m_SceneArray;
		VSArray<VSActor *>	m_ActorArray;
		VSArray<VSMessage> m_MessageArray;

	public:
		
		
		VSSceneMap* CreateScene(const TCHAR * pName);

		VSSceneMap * GetScene(const VSUsedName & Name);
		
		VSActor * CreateActor(const TCHAR *  ActorPath, const VSVector3 & Pos = VSVector3::ms_Zero, const VSMatrix3X3 & Rotate = VSMatrix3X3::ms_Identity, const VSVector3 &Scale = VSVector3::ms_One, VSSceneMap * pSceneMap = NULL);
		void DestroyActor(VSActor * pActor);

		template<typename T>
		VSActor *CreateActor(const VSVector3 & Pos = VSVector3::ms_Zero, const VSMatrix3X3 & Rotate = VSMatrix3X3::ms_Identity, const VSVector3 &Scale = VSVector3::ms_One, VSSceneMap * pSceneMap = NULL);

		bool LoadMap(const TCHAR * MapPath,bool IsAsyn);
		bool UnLoadMap(const TCHAR * MapPath);
		void AttachCaptureViewFamilyToCamera(VSCameraActor * pCameraActor,
			unsigned int uiCaptureViewFamilyType,const VSString & ViewFamilyName,
			unsigned int uiWidth,unsigned int uiHeight,
			VSArray<VSString> & SceneMapName,
			const TCHAR * RenderMethodRTTIName,
			VSPostEffectSetR *  pPostEffectSet = NULL);
		void AttachWindowViewFamilyToCamera(VSCameraActor * pCameraActor,
			unsigned int uiWindowViewFamilyType,const VSString & ViewFamilyName,
			VSArray<VSString> & SceneMapName,
			const TCHAR * RenderMethodRTTIName,
			int iWindowID = -1,
			VSPostEffectSetR *  pPostEffectSet = NULL);
		VSActor *GetActor(VSUsedName Name);
		void Update(double dAppTime);
		void AddMessage(const VSMessage & Message);
		void UpdateLightActorScene(VSLightActor * pActor);
		void ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z);
		
	protected:
		friend class VSResourceManager;
		void AddActor(VSActor * pActor);
		void DeleteActor(VSActor * pActor);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy,int Data = 0);
		void DestroyScene(const VSUsedName & Name);
		void AddSceneMap(VSSceneMap * pSceneMap);
	};
	DECLARE_Ptr(VSWorld);
	VSTYPE_MARCO(VSWorld);
	template<typename T>
	VSActor *VSWorld::CreateActor(const VSVector3 & Pos, const VSMatrix3X3 & Rotate, const VSVector3 &Scale, VSSceneMap * pSceneMap)
	{
		if (!T::ms_Type.IsDerived(VSActor::ms_Type))
			return NULL;
		VSActor * pActor = NULL;
		pActor = (VSActor*)VSObject::GetInstance<T>();
		pActor->CreateDefaultComponentNode();
		pActor->SetWorldPos(Pos);
		pActor->SetWorldRotate(Rotate);
		pActor->SetWorldScale(Scale);
		if (pSceneMap)
		{
			pSceneMap->AddActor(pActor);
		}
		else
		{
			m_SceneArray[0]->AddActor(pActor);
		}
		AddActor(pActor);
		return pActor;
	}
}
#endif