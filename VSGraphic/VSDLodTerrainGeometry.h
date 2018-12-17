#ifndef VSDLODTERRAINGEOMETRY_H
#define VSDLODTERRAINGEOMETRY_H
#include "VSGeometry.h"
namespace VSEngine2
{

	class VSDLodTerrainNode;
	class VSGRAPHIC_API VSDLodTerrainGeometry : public VSGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDLodTerrainGeometry();
		virtual ~VSDLodTerrainGeometry();
		bool CreateMesh(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
			unsigned int uiLevel,unsigned int uiTotleLevel);
	protected:
		VSDLodTerrainNode * GetParentTerrainNode()const;
	};
	DECLARE_Ptr(VSDLodTerrainGeometry);
	VSTYPE_MARCO(VSDLodTerrainGeometry);
};
#endif