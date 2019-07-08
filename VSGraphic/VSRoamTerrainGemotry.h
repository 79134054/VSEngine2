#ifndef VSROAMTERRAINGEOMETRY_H
#define VSROAMTERRAINGEOMETRY_H
#include "VSCLodTerrainGeometry.h"
namespace VSEngine2
{

	/*
			    A
				 /\
				/  \
			  C/____\ B
	顺时针排列，可见

	 |Z
	 |
	A|____B
	 |   /|
	 |  / |
	 | /  |
	C|/___|_______X

	*/
	class VSStream;
	class VSGRAPHIC_API VSRoamTerrainGeometry : public VSCLodTerrainGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSRoamTerrainGeometry();
		virtual ~VSRoamTerrainGeometry();


	public:
		virtual unsigned int GetTerrainGeometryType() {return TGT_ROAM;}
		class VSTriTreeNode
		{
		public:
			VSTriTreeNode *pBaseNeighbor;
			VSTriTreeNode *pLeftNeighbor;
			VSTriTreeNode *pRightNeighbor;
			VSTriTreeNode *pLeftChild;
			VSTriTreeNode *pRightChild;

			VSRoamTerrainGeometry * pOwner;
			VSTriTreeNode()
			{
				pOwner = NULL;
				pBaseNeighbor = NULL;
				pLeftNeighbor = NULL;
				pRightNeighbor = NULL;
				pLeftChild = NULL;
				pRightChild = NULL;
			}
			FORCEINLINE void Clear()
			{
				//pOwner = NULL;
				pBaseNeighbor = NULL;
				pLeftNeighbor = NULL;
				pRightNeighbor = NULL;
				pLeftChild = NULL;
				pRightChild = NULL;
			}
			
		};
		virtual void ComputeVariance(unsigned int uiCurLevel = 0);
		virtual void LinkNeighbor();	
		virtual void ClearInfo();
		virtual unsigned int UpdateGeometry();
	protected:
		
		virtual void Tessellate(const VSVector3 & CameraPos);
		VSREAL RecursiveComputeVariance(VSArray<VSREAL>& Variance,unsigned int uiIndex,
			unsigned int A,unsigned int B,unsigned int C);	
		void RecursiveTessellate(VSTriTreeNode * pTri,const VSVector3 & CameraPos,VSArray<VSREAL>& Variance,
			unsigned int uiIndex,unsigned int A,unsigned int B,unsigned int C);
		void Split(VSTriTreeNode * pTri);
		VSTriTreeNode * GetBuffer();
		
		void RecursiveBuildRenderTriange(VSTriTreeNode * pTri,
			unsigned int A,unsigned int B,unsigned int C,VSUSHORT_INDEX * &pIndexData,unsigned int &uiCurRenderTriNum);
		virtual void UpdateOther(double dAppTime);

		void TessellateEx(const VSVector3 & CameraPos, unsigned int uiLevel);
		void ClearInfoEx();
		void UpdateOtherEx(unsigned int uiLevel);
	protected:
		VSArray<VSREAL> m_Variance[2];
		VSTriTreeNode	m_TriTreeNode[2];
		VSArray<VSTriTreeNode>	m_TriBuffer;
		unsigned int m_uiCurBufferNum;
	};
	DECLARE_Ptr(VSRoamTerrainGeometry);
	VSTYPE_MARCO(VSRoamTerrainGeometry);
}
#endif