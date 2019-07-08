#ifndef VSSWITCHNODE_H
#define VSSWITCHNODE_H
#include "VSNode.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSSwitchNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSwitchNode();
		virtual ~VSSwitchNode();


		virtual VSSpatial * GetActiveNode()const;
	protected:
		unsigned int m_uiActiveNode;

	protected:
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 

		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
	};
	DECLARE_Ptr(VSSwitchNode);
	VSTYPE_MARCO(VSSwitchNode);

}
#endif