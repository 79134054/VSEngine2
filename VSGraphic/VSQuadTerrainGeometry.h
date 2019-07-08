#ifndef VSQUADTERRAINGEOMETRY_H
#define VSQUADTERRAINGEOMETRY_H
#include "VSCLodTerrainGeometry.h"
namespace VSEngine2
{
	/*
		|Z j
		|
	   B|____C
		|   /|
		|  / |
		| /  |
	   A|/___|D______X i
	*/
	class VSStream;
	class VSGRAPHIC_API VSQuadTerrainGeometry : public VSCLodTerrainGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSQuadTerrainGeometry();
		virtual ~VSQuadTerrainGeometry();

	public:
		virtual unsigned int GetTerrainGeometryType() {return TGT_QUAD;}
		virtual void ComputeVariance(unsigned int uiCurLevel = 0);
		virtual void LinkNeighbor();	
		virtual void ClearInfo();
		virtual unsigned int UpdateGeometry();
	protected:
		enum
		{
			AT_ALL		= 0x0000,
			AT_NLEFT	= 0x0001,
			AT_NRIGHT	= 0x0002,
			AT_NTOP		= 0x0004,
			AT_NBOTTOM	= 0x0008
			
		};

		virtual void Tessellate(const VSVector3 & CameraPos);
		bool RecursiveComputeVariance(unsigned int uiLevel);	
		void RecursiveTessellate(const VSVector3 & CameraPos,unsigned int uiLevel,unsigned int A,unsigned int B,
			unsigned int C,unsigned int D);	
		static FORCEINLINE void AddTriangle(VSUSHORT_INDEX * &pIndexData,unsigned int A,unsigned int B,
							unsigned int C,unsigned int D,unsigned int uiATFlag,unsigned int& uiCurRenderTriNum);
		virtual void UpdateOther(double dAppTime);
	protected:

		unsigned int m_uiCurLevel;
		VSArray<VSREAL> m_Variance;
	};
	DECLARE_Ptr(VSQuadTerrainGeometry);
	VSTYPE_MARCO(VSQuadTerrainGeometry);


}
#endif