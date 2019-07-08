#ifndef VSPLANE3_H
#define VSPLANE3_H
#include "VSVector3.h"
#include "VSMatrix3X3.h"

namespace VSEngine2
{
	class VSLine3;
	class VSPlane3;
	class VSSphere3;
	class VSCylinder3;
	class  VSMATH_API VSPlane3 
	{
	protected:
		VSVector3	m_N;       //平面法向量
		VSREAL		m_fD;       // 到原点的距离(ax+by+cz+d=0)
	public:
		VSPlane3();
		VSPlane3(const VSVector3 &N, const VSVector3 &P);
		VSPlane3(const VSVector3 &P0,  const VSVector3 &P1, const VSVector3 &P2);
		VSPlane3(const VSVector3 Point[3]);
		VSPlane3(const VSVector3 &N , VSREAL fD);
		void Transform(const VSPlane3 &Plane,const VSMatrix3X3W &Mat);
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE const VSVector3 & GetN()const;
		FORCEINLINE VSVector3 GetPoint()const;
		FORCEINLINE VSREAL GetfD()const;
		FORCEINLINE void  Set(const VSVector3 &N, const VSVector3 &P);
		FORCEINLINE void  Set(const VSVector3 &N , VSREAL fD);
		FORCEINLINE void  Set(const VSVector3 &P0,  const VSVector3 &P1, const VSVector3 &P2);
		FORCEINLINE void  Set(const VSVector3 Point[3]);
		FORCEINLINE VSPlane3 GetPlane()const;
		VSVector3 ReflectDir(const VSVector3 & Dir)const;
		/*************************************距离************************************************/
		//点到平面距离
		VSREAL Distance(const VSVector3 &Point,VSVector3 &PlanePoint)const;
		//平面和球的距离
		VSREAL Distance(const VSSphere3 &Sphere,VSVector3 & SpherePoint)const;
		//直线和平面距离
		VSREAL Distance(const VSLine3 &Line,VSVector3 &PlanePoint,VSVector3 &LinePoint)const;
		//射线和平面距离
		VSREAL Distance(const VSRay3 & Ray,VSVector3 &PlanePoint,VSVector3 &RayPoint)const;
		//线段和平面距离
		VSREAL Distance(const VSSegment3 & Segment,VSVector3 &PlanePoint,VSVector3 &SegmentPoint)const;

		//下面距离函数再没有考虑相交的情况下计算的，因为相交的情况下举例为0，先用RelationWith判断位置关系，再用下面来计算距离
		//平面和平面距离
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &Plane1Point,VSVector3 &Plane2Point)const;
		//平面和三角形距离
		VSREAL Distance(const VSTriangle3 &Triangle,VSVector3 &PlanePoint,VSVector3 &TrianglePoint)const;
		//矩形和平面距离
		VSREAL Distance(const VSRectangle3 &Rectangle,VSVector3 &PlanePoint,VSVector3 &RectanglePoint)const;
		//OBB和平面距离
		VSREAL Distance(const VSOBB3& OBB,VSVector3 &PlanePoint,VSVector3 &OBBPoint)const;
		//AABB和平面距离
		VSREAL Distance(const VSAABB3 &AABB,VSVector3 &PlanePoint,VSVector3 &AABBPoint)const;
		//平面和多边形距离
		VSREAL Distance(const VSPolygon3 &Polygon,VSVector3 &PlanePoint,int& IndexTriangle,
						VSVector3 &TrianglePoint)const;
		/********************************RelationWith******************************************/
		//点和平面的位置关系(VSFRONT VSBACK VSPLANAR)
		int RelationWith(const VSVector3 &Point)const;
		//测试直线与平面位置关系
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSLine3 &Line, bool bCull,VSREAL &fLineParameter)const;
		//测试射线与平面位置关系
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSRay3 &Ray, bool bCull,VSREAL &fRayParameter)const;
		//测试线段与平面位置关系
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSSegment3 &Segment, bool bCull,VSREAL &fSegmentParameter)const;
		//测试平面和OBB位置关系
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSOBB3 &OBB)const;
		//测试平面和AABB位置关系
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSAABB3 &AABB)const;
		//平面与球的位置关系
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSSphere3 &Sphere)const;
		//平面和三角形位置关系
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSTriangle3 &Triangle)const;
		int RelationWith(const VSTriangle3 &Triangle ,VSSegment3 & Segment)const;
		//测试做为参数平面和平面位置关系
		//VSNOINTERSECT VSINTERSECT 
		int RelationWith(const VSPlane3 &Plane)const;
		int RelationWith(const VSPlane3 &Plane,VSLine3 &Line)const;
		//平面和矩形位置关系
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSRectangle3 & Rectangle)const;		
		int RelationWith(const VSRectangle3 &Rectangle,VSSegment3 &Segment)const;

		//平面和多边形位置关系
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPolygon3 &Polygon)const;
		int RelationWith(const VSPolygon3 &Polygon,VSSegment3 & Segment)const;

		//平面和圆柱位置关系
		int RelationWith(const VSCylinder3 &Cylinder3)const;

	}; // class
	/*----------------------------------------------------------------*/
#include "VSPlane3.inl"

};
#endif