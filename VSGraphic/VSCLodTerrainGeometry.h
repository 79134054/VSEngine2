#ifndef VSCLODTERRAINGEOMETRY_H
#define VSCLODTERRAINGEOMETRY_H
#include "VSGeometry.h"
/*
	顶点排列顺序
j	4 9 14 19 24
	3 8 13 18 23
	2 7 12 17 22
	1 6 11 16 21
	0 5 10 15 20 i
*/
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSCLodTerrainGeometry : public VSGeometry
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum	// TerrainGeometry Type
		{
			TGT_ROAM,
			TGT_QUAD,
			TGT_MAX
		};
		enum	// Neightbor Type
		{
			NT_LEFT,
			NT_RIGHT,
			NT_TOP,
			NT_BOTTOM,
			NT_MAX
		};
		VSCLodTerrainGeometry();
		virtual ~VSCLodTerrainGeometry() = 0;
	public:

		virtual bool CreateMeshDate(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
									unsigned int uiTesselationLevel);
		virtual unsigned int GetTerrainGeometryType() = 0;
		virtual bool AddNeighbor(VSCLodTerrainGeometry * pTerrainGemetry,unsigned int uiNeighbor);
		virtual void LinkNeighbor() = 0;
		//每个节点自己不允许变换，以免相邻的邻居和当前节点LOD出现问题
		virtual void SetLocalScale(const VSVector3& Scale){};
		virtual void SetLocalTranslate(const VSVector3& Translate){};
		virtual void SetLocalRotate(const VSMatrix3X3 & Rotate){};
		virtual void SetLocalTransform(const VSTransform & LocalTransform){};
		virtual void SetLocalMat(const VSMatrix3X3W VSMat){};

		virtual void ComputeVariance(unsigned int uiCurLevel = 0) = 0;
		virtual void ClearInfo() = 0;
		
		virtual void UpDateView(VSCuller & Culler,double dAppTime);
	protected:
		VSCLodTerrainGeometry * m_pNeighbor[NT_MAX];
		unsigned int m_uiIndexXInTerrain;
		unsigned int m_uiIndexZInTerrain;
		virtual void Tessellate(const VSVector3 & CameraPos) = 0;
	};
	DECLARE_Ptr(VSCLodTerrainGeometry);
	VSTYPE_MARCO(VSCLodTerrainGeometry);
}
#endif