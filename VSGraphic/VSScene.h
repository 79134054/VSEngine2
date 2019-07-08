#ifndef VSSCENE_H
#define VSSCENE_H
#include "VSNode.h"
namespace VSEngine2
{
	class VSRenderer;
	class VSStream;
	class VSQuadNode : public VSNode
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSQuadNode();
		~VSQuadNode();
		virtual void ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime);
		bool RecursiveBuild(const VSArray<VSSpatial *> &pObjectArray);
		virtual unsigned int AddChild(VSSpatial * pChild);
		virtual unsigned int DeleteChild(VSSpatial *pChild);
		virtual bool DeleteChild(unsigned int i);
		virtual void		DeleteAllChild();
		VSNodeComponent *GetNeedDrawNode(unsigned int uiIndex)const;
		unsigned int GetNeedDrawNodeNum()const;
	protected:
		void DeleteNeedDrawNode(VSNodeComponent *pNeedDrawNode);
		void AddNeedDrawNode(VSNodeComponent *pNeedDrawNode);
		VSArray<VSNodeComponent *> m_pNeedDrawNode;

	};
	DECLARE_Ptr(VSQuadNode);
	VSTYPE_MARCO(VSQuadNode);
	
	class VSGRAPHIC_API VSScene:public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSSceneManager;
		void Update(double dAppTime);
		void CollectUpdateInfo();
		VSScene();
		~VSScene();
		

		FORCEINLINE VSLight * GetAllLight(unsigned int i)const;
		FORCEINLINE unsigned int GetAllLightNum()const;


		virtual void AddObject(VSNode * pObject);
		virtual void DeleteObject(VSNode *pObject);
		virtual void DeleteAllObject();	

		virtual void ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		bool Build();
		FORCEINLINE bool IsBuild()
		{
			return m_bIsBuild;
		}
	protected:
		VSArray<VSLight *> m_pAllLight;
		VSArray<VSCamera *> m_pAllCamera;

		VSArray<VSNode *> m_pDynamic;
		VSQuadNodePtr m_pStaticRoot;
		VSArray<VSNode *> m_ObjectNodes;
		bool m_bIsBuild;
	};
	DECLARE_Ptr(VSScene);
	VSTYPE_MARCO(VSScene);
}
#endif