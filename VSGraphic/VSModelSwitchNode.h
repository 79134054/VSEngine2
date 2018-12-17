#ifndef VSMODELSWITCHNODE_H
#define VSMODELSWITCHNODE_H
#include "VSSwitchNode.h"
#include "VSGeometryNode.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSModelSwitchNode : public VSSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSModelSwitchNode();
		virtual ~VSModelSwitchNode();
	public:
		virtual VSGeometryNode * GetGeometryNode(unsigned int uiLodLevel);
		virtual void UpDateView(VSCuller & Culler, double dAppTime);
	};
	DECLARE_Ptr(VSModelSwitchNode);
	VSTYPE_MARCO(VSModelSwitchNode);

}
#endif