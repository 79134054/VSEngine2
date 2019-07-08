#ifndef VSLINE3_H
#define VSLINE3_H
#include "VSMath.h"
#include "VSPlane3.h"
#include "VSOBB3.h"
#include "VSSphere3.h"
#include "VSTriangle3.h"
#include "VSRectangle3.h"
/*
	直线类


*/
namespace VSEngine2
{
	class VSRay3;
	class VSSegment3;
	class VSMATH_API VSLine3
	{
	protected:
		VSVector3	m_Orig;  // 源点
		VSVector3	m_Dir;   // 方向
	public:
		VSLine3();
		VSLine3(const VSVector3 & Orig,const VSVector3 &Dir);
		~VSLine3();
		void Transform(const VSLine3 &Line,const VSMatrix3X3W &Mat);
		bool GetParameter(const VSVector3 &Point,VSREAL &fLineParameter )const;
		/************************FORCEINLINE***************************************/
		FORCEINLINE void Set(const VSVector3 & Orig,const VSVector3 &Dir);
		FORCEINLINE const VSVector3 & GetOrig()const;
		FORCEINLINE const VSVector3 & GetDir()const;

		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fLineParameter)const;	
		
		
		/********************************RelationWith******************************************/
		//测试直线与三角形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fLineParameter,
						VSREAL fTriangleParameter[3])const;
		//测试直线与平面位置关系
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fLineParameter)const;
		//测试直线与矩形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fLineParameter,
						VSREAL fRectangleParameter[2])const;
		//测试直线与球位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试直线与OBB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		

		//测试直线与AABB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;

		//测试直线与多边形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSPolygon3 &Polygon,VSREAL &fLineParameter, 
						bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const;
		/*************************************距离************************************************/

		//点到直线距离
		VSREAL SquaredDistance(const VSVector3 &Point,VSREAL &fLineParameter)const;
		//直线和直线的距离
		VSREAL SquaredDistance(const VSLine3 &Line,VSREAL &fLine1Parameter,VSREAL &fLine2Parameter)const;
		//直线和射线距离
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL &fLineParameter,VSREAL &fRayParameter)const;
		//直线和线段距离
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fLineParameter,VSREAL &fSegmentParameter)const;
		//直线和三角形距离
		VSREAL SquaredDistance(const VSTriangle3& Triangle,VSREAL &fLineParameter,VSREAL fTriangleParameter[3])const;
		//直线和矩形距离
		VSREAL SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fLineParameter,VSREAL fRectangleParameter[2])const;
		//直线和OBB距离
		VSREAL SquaredDistance(const VSOBB3 & OBB,VSREAL &fLineParameter,VSREAL fOBBParameter[3])const;
		//直线和球的距离
		VSREAL Distance(const VSSphere3 &Sphere,VSREAL &fLineParameter,VSVector3 & SpherePoint)const;
		//直线和平面距离
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &LinePoint,VSVector3 &PlanePoint)const;
		//直线和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL &fLineParameter, VSREAL fAABBParameter[3])const;

		//直线和多边形距离
		VSREAL SquaredDistance(const VSPolygon3 & Polygon,VSREAL &fLineParameter,
								int& IndexTriangle,
								VSREAL fTriangleParameter[3])const;
	};

#include "VSLine3.inl"
};
#endif