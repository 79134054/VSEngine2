#ifndef VSMESHNODE_H
#define VSMESHNODE_H
#include "VSNode.h"
#include "VSResource.h"
namespace VSEngine2
{
	class VSGeometryNode;
	class VSSwitchNode;
	class VSStream;
	class VSGRAPHIC_API VSMeshNode : public VSNode,public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSMeshNode();
		virtual ~VSMeshNode();
		virtual unsigned int GetResourceType()const
		{
			return RT_MAX;
		}
		FORCEINLINE void SetIsDrawBoundVolume(bool bIsDrawBoundVolume)
		{
			m_bIsDrawBoundVolume = bIsDrawBoundVolume;
		}
		FORCEINLINE unsigned int GetRenderGroup()const
		{
			return m_uiRenderGroup;
		}
		FORCEINLINE void SetRenderGroup(unsigned int uiRenderGroup)
		{
			m_uiRenderGroup = uiRenderGroup;
		}

		virtual void UpDateView(VSCuller & Culler,double dAppTime);

		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		bool m_bReceiveShadow;
		bool m_bCastShadow;
		bool m_bLighted;
	protected:
		bool m_bIsDrawBoundVolume;
		unsigned int m_uiRenderGroup;
	};
	DECLARE_Ptr(VSMeshNode);
	VSTYPE_MARCO(VSMeshNode);
}
#endif