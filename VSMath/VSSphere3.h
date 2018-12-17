#ifndef VSSPHERE3_H
#define VSSPHERE3_H
#include "VSVector3.h"
namespace VSEngine2
{
	class VSMATH_API VSSphere3
	{
	public:
		VSVector3 m_Center;		//中心
		VSREAL m_fRadius;		//半径
		VSSphere3();
		~VSSphere3();
		VSSphere3(const VSVector3 & Center, VSREAL fRadius);
		//根绝点来建立包围球
		void CreateSphere(const VSVector3 *pPointArray,unsigned int uiPointNum);
		//结合包围球
		VSSphere3 MergSpheres(const VSSphere3 &Sphere)const;
		//变换球体
		void Transform(const VSSphere3 & Sphere,const VSMatrix3X3W &Mat); 
		VSAABB3 GetAABB()const;
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void Set(const VSVector3 & Center, VSREAL fRadius);
		
		/*************************************距离************************************************/
		//点和球的距离
		VSREAL Distance(const VSVector3 & Point,VSVector3 & SpherePoint)const;
		//线和球的距离
		VSREAL Distance(const VSLine3 & Line,VSVector3 & SpherePoint,VSREAL &fLineParameter)const;
		//射线和球的距离
		VSREAL Distance(const VSRay3 & Ray,VSVector3 & SpherePoint,VSREAL &fRayParameter)const;
		//线段和球的距离
		VSREAL Distance(const VSSegment3 & Segment,VSVector3 & SpherePoint,VSREAL &fSegmentParameter)const;
		//OBB和球的距离
		VSREAL Distance(const VSOBB3 &OBB,VSVector3 & SpherePoint,VSREAL fOBBParameter[3])const;
		//平面和球的距离
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 & SpherePoint)const;
		//矩形和球的距离
		VSREAL Distance(const VSRectangle3 & Rectangle,VSVector3 & SpherePoint,VSREAL fRectangleParameter[2])const;
		//三角形和球的距离
		VSREAL Distance(const VSTriangle3 Triangle,VSVector3 & SpherePoint,VSREAL fTriangleParameter[3])const;
		//AABB和球的距离
		VSREAL Distance(const VSAABB3 &AABB,VSVector3 & SpherePoint,VSREAL fAABBParameter[3])const;

		//多边形和球的距离
		VSREAL Distance(const VSPolygon3 &Polygon,VSVector3 & SpherePoint,int& IndexTriangle,
						VSREAL fTriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//点和球位置关系
		//VSIN VSOUT VSON	
		int RelationWith(const VSVector3 &Point)const;
		//直线与球位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSLine3 &Line, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//射线与球位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRay3 &Ray, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//线段与球位置关系
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSSegment3 &Segment, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//平面与球的位置关系
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPlane3 &Plane)const;
		//三角形和圆位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSTriangle3 Triangle)const;
		//矩形和圆位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSRectangle3 &Rectangle)const;

		//OBB和圆位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSOBB3 &OBB)const;
		//园和圆位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSSphere3 &Sphere)const;
	};

#include "VSSphere3.inl"
};
#endif