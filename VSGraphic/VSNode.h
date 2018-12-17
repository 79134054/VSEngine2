#ifndef VSNODE_H
#define VSNODE_H
#include "VSSpatial.h"
namespace VSEngine2
{
	class VSSort;
	class VSGeometry;
	class VSStream;
	class VSNodeComponent;
	class VSGRAPHIC_API VSNode : public VSSpatial
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSMeshComponent;
		VSNode();
		virtual ~VSNode() = 0;
		FORCEINLINE unsigned int GetNodeNum()const;
		virtual unsigned int AddChild(VSSpatial * pChild);
		virtual unsigned int DeleteChild(VSSpatial *pChild);
		virtual bool DeleteChild(unsigned int i);
		VSSpatial * GetChild(unsigned int i)const;
		virtual void		DeleteAllChild();
		FORCEINLINE VSArray<VSSpatialPtr> *GetChildList();

	protected:
		
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 

		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
	protected:
		VSArray<VSSpatialPtr>m_pChild;
		
		
	public:
		virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
		virtual void CreateLocalAABB();
	};
	#include "VSNode.inl"
	DECLARE_Ptr(VSNode);
	VSTYPE_MARCO(VSNode);
}
#endif