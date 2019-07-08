#ifndef VSMESHCOMPONENT_H
#define VSMESHCOMPONENT_H
#include "VSNodeComponent.h"
namespace VSEngine2
{

	DECLARE_Ptr(VSModelMeshNode);
	class VSGRAPHIC_API VSMeshComponent : public VSNodeComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSMeshComponent();
		virtual ~VSMeshComponent() = 0;
		virtual bool IsNeedDraw()
		{
			return true;
		}
	protected:
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 

		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime);
		virtual void SetPostLoadNodeParam();
	public:
		virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
		virtual void CreateLocalAABB();
		void SetIsDrawBoundVolume(bool bIsDrawBoundVolume);

		void SetMorphTreeNodePara(const VSUsedName & ShowName, void * pPara);

		void CastShadow(bool bCastShadow);


		VSMaterialInstance * GetUseMaterialInstance(unsigned int SubMeshID)const;

		VSMaterialInstance * GetMaterialInstance(unsigned int SubMeshID,unsigned int UseID)const;

		bool SetUseMaterialInstance(unsigned int SubMeshID, unsigned int UseID);

		unsigned int GetSubMeshNum();
		virtual void DelayUpdate();
	protected:
		VSModelMeshNodePtr m_pNode;
		//Cache Last ModelMeshNode m_pNode，other render thread will crash when it is delete.
		VSModelMeshNodePtr m_pSaveNode;
		VSArray<unsigned int> m_UseID;
		bool m_bCastShadow;
		void ResetUseID();
	};
	DECLARE_Ptr(VSMeshComponent);
	VSTYPE_MARCO(VSMeshComponent);
}
#endif