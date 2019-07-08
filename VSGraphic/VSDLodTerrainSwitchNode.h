#ifndef VSDLODTERRAINSWITCHNODE_H
#define VSDLODTERRAINSWITCHNODE_H
#include "VSSwitchNode.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSDLodTerrainSwitchNode : public VSSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDLodTerrainSwitchNode(unsigned int uiIndexXInTerrain,unsigned int uiIndexZInTerrain);
		virtual ~VSDLodTerrainSwitchNode();

	public:

		virtual void UpDateView(VSCuller & Culler,double dAppTime);
		
	
	protected:
		VSDLodTerrainSwitchNode();
		unsigned int m_uiIndexXInTerrain;
		unsigned int m_uiIndexZInTerrain;
	};
	DECLARE_Ptr(VSDLodTerrainSwitchNode);
	VSTYPE_MARCO(VSDLodTerrainSwitchNode);
}
#endif