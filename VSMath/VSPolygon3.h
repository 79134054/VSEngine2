#ifndef VSPOLYGON3_H
#define VSPOLYGON3_H
#include"VSPlane3.h"
#include "VSMemManager.h"
namespace VSEngine2
{
	class VSMATH_API VSPolygon3 : public VSPlane3 , public VSMemObject
	{
	private:

		unsigned int	m_PointNum;		//点个数
		unsigned int	m_IndexNum;		//索引个数
		bool			m_Flag;			// 标志位
		VSVector3		*m_pPoint;		// 点列表
		VSUSHORT_INDEX	*m_pIndex;		// 索引列表
	public:
		VSPolygon3();
		~VSPolygon3();
	
		VSPolygon3(const VSVector3 * pPoint,unsigned int PointNum);
		//拷贝
		bool CopyFrom(const VSPolygon3 & Polygon);
		//反转面
		void SwapFaces();
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Set(const VSVector3 * pPoint,unsigned int PointNum);
		FORCEINLINE unsigned int GetPointNum()const;
		FORCEINLINE unsigned int GetIndexNum()const;
		FORCEINLINE const VSVector3 * const GetpPoint()const;
		FORCEINLINE const VSUSHORT_INDEX * const GetpIndex()const;
		FORCEINLINE unsigned int GetFlag()const;
		/*************************************距离************************************************/
		//点和多边形距离
		VSREAL SquaredDistance(const VSVector3 & Point,int& IndexTriangle,VSREAL fTriangleParameter[3])const;
		//多边形和多边形距离
		VSREAL SquaredDistance(const VSTriangle3 &Triangle,
								int& IndexTriangle,
								VSREAL fTriangle1Parameter[3],
								VSREAL fTriangle2Parameter[3])const;
		//多边形和矩形距离
		VSREAL SquaredDistance(const VSRectangle3 &Rectangle,
								int& IndexTriangle,
								VSREAL fTriangleParameter[3],
								VSREAL fRectangleParameter[2])const;

		//直线和多边形距离
		VSREAL SquaredDistance(const VSLine3 &Line,int& IndexTriangle,
								VSREAL fTriangleParameter[3],VSREAL &fLineParameter)const;
		//射线和多边形距离
		VSREAL SquaredDistance(const VSRay3 & Ray,int& IndexTriangle,
								VSREAL fTriangleParameter[3],VSREAL &fRayParameter)const;
		//线段和多边形距离
		VSREAL SquaredDistance(const VSSegment3& Segment,int& IndexTriangle,
								VSREAL fTriangleParameter[3],VSREAL &fSegmentParameter)const;
		//多边形和OBB距离
		VSREAL SquaredDistance(const VSOBB3 &OBB,int& IndexTriangle,
								VSREAL TriangleParameter[3], VSREAL OBBParameter[3])const;
		//多边形和球的距离
		VSREAL Distance(const VSSphere3 &Sphere,int& IndexTriangle,
						VSREAL fTriangleParameter[3],VSVector3 & SpherePoint)const;

		//平面和多边形距离
		VSREAL Distance(const VSPlane3 &Plane,int& IndexTriangle,
						VSVector3 &TrianglePoint,VSVector3 &PlanePoint)const;
		//多边形和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB ,int& IndexTriangle,
								VSREAL TriangleParameter[3]
								, VSREAL AABBParameter[3])const;

		//多边形和多边形距离
		VSREAL SquaredDistance(const VSPolygon3 & Polygon ,int& Index1Triangle,
								VSREAL Triangle1Parameter[3],int& Index2Triangle,
								VSREAL Triangle2Parameter[3])const;
	/********************************RelationWith******************************************/
		//测试直线与多边形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSLine3 &Line, bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3],
						VSREAL &fLineParameter)const;
		//测试射线与多边形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRay3 &Ray, bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3],
						VSREAL &fRayParameter)const;
		//测试线段与多边形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSSegment3 &Segment, bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3],
						VSREAL &fSegmentParameter)const;
		//平面和多边形位置关系
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPlane3 & Plane)const;
		int RelationWith(const VSPlane3 & Plane,VSSegment3 & Segment)const;
		//多边形和三角形位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSTriangle3 & Triangle)const;
		int RelationWith(const VSTriangle3 & Triangle,VSSegment3 & Segment)const;
		//多边形和矩形位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSRectangle3 & Rectangle)const;
		int RelationWith(const VSRectangle3 & Rectangle,VSSegment3 & Segment)const;
		//多边形和AABB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSAABB3 &AABB)const;

		//多边形和OBB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSOBB3 &OBB)const;

		//多边形和圆位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSSphere3 &Sphere)const;
		/*************************************** Clip *********************************/
		//平面减切多边形
		void	Clip(const VSPlane3 &Plane, 
							VSPolygon3 &pFront,
							VSPolygon3 &pBack)const;
		//用AABB剪切多边形用来适合AABB
		void	Clip(const VSAABB3 &ABBB);
	};
	#include "VSPolygon3.inl"
}
#endif