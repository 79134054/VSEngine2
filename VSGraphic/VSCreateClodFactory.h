#ifndef VSCREATECLODFACTORY_H
#define VSCREATECLODFACTORY_H
#include "VSCollapseRecord.h"
#include "VSVector3.h"
#include "VSArray.h"
#include "VSTriangleSet.h"
#include "VSLineSet.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSCreateClodLineFactory
	{
	protected:
		class Vertex3DAttr
		{
		public:
			Vertex3DAttr()
			{

			}
			~Vertex3DAttr()
			{

			}
			bool m_bIsDelete;
			VSVector3 m_Position;
			VSREAL m_fWeight;
			unsigned int m_uiIndexInVertexArray;
			VSArray<unsigned int> m_InEdgeIndexArray;
			bool FORCEINLINE operator == (const Vertex3DAttr &Vertex3D)const
			{
				return (m_uiIndexInVertexArray == Vertex3D.m_uiIndexInVertexArray);
			}
		};
		class Edge3DAttr
		{
		public:
			Edge3DAttr()
			{

			}
			~Edge3DAttr()
			{

			}
			bool m_bIsDelete;
			unsigned int m_ContainVertexIndex[2];
			

		};
		static VSArray<Vertex3DAttr> ms_V3Attr;
		static VSArray<Edge3DAttr> ms_E3Attr;
		static VSCollapseRecordLine * ms_pCRL;

		static VSArray<unsigned int> ms_RecordLine;
		static unsigned int ms_uiLeftLineNum;
		static VSLineSet * ms_pLineSet;
		static void InitalDate();
		static VSREAL GetCollapseWeight(const Vertex3DAttr & Vertex3D);
		static bool GetCollapseV(unsigned int &uiThrowV3DIndex);
		static bool Collapse(unsigned int uiThrowV3DIndex);
	public:
		static VSCollapseRecordLine * CreateClodLine( VSLineSet * pLineSet);
	};
	class VSGRAPHIC_API VSCreateClodTriFactory
	{
	protected:
		class Vertex3DAttr
		{
		public:
			Vertex3DAttr()
			{

			}
			~Vertex3DAttr()
			{

			}
			bool m_bIsDelete;
			VSVector3 m_Position;
			VSArray<unsigned int> m_InEdgeIndexArray;
			VSArray<unsigned int> m_InTriangleIndexArray;
		};

		class Edge3DAttr
		{
		public:
			Edge3DAttr()
			{
				
			}
			~Edge3DAttr()
			{

			}
			bool m_bIsDelete;
			unsigned int m_ContainVertexIndex[2];
			VSArray<unsigned int> m_InTriangleIndexArray;
			VSREAL m_fWeight;
			bool FORCEINLINE operator == (const Edge3DAttr &Edge3D)const
			{
				return ((m_ContainVertexIndex[0] == Edge3D.m_ContainVertexIndex[0] &&
					m_ContainVertexIndex[1] == Edge3D.m_ContainVertexIndex[1]) || 
					(m_ContainVertexIndex[1] == Edge3D.m_ContainVertexIndex[0] &&
					m_ContainVertexIndex[0] == Edge3D.m_ContainVertexIndex[1]));
			}

		};
		class Triangle3DAttr
		{
		public:
			Triangle3DAttr()
			{

			}
			~Triangle3DAttr()
			{
			
			}
			bool m_bIsDelete;
			unsigned int m_ContainVertexIndex[3];
			unsigned int m_ContainEdgeIndex[3];
		};

		static VSArray<Vertex3DAttr> ms_V3Attr;
		static VSArray<Edge3DAttr> ms_E3Attr;
		static VSArray<Triangle3DAttr> ms_T3Attr;

		static VSCollapseRecordTri * ms_pCRT;

		static VSArray<unsigned int> ms_RecordTriangle;

		static unsigned int ms_uiLeftTrangleNum;
		static VSREAL GetCollapseWeight(const Edge3DAttr & Edge3D);
		static void InitalDate();

		static VSTriangleSet * ms_pTriangleSet;

		static bool GetCollapseEV(unsigned int &uiE3DIndex,unsigned int &uiKeepV3DIndex,unsigned int &uiThrowV3DIndex);

		static bool Collapse(unsigned int uiE3DIndex,unsigned int uiKeepV3DIndex,unsigned int uiThrowV3DIndex);

	public:
		static VSCollapseRecordTri * CreateClodTriangle( VSTriangleSet * pTriangleSet);


	};
}
#endif