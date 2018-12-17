#ifndef VSRAY3_H
#define VSRAY3_H
#include "VSLine3.h"
namespace VSEngine2
{
	//射线
	class  VSMATH_API VSRay3 :public VSLine3
	{
	public:

		VSRay3();
		VSRay3(const VSVector3 & Orig,const VSVector3 &Dir);
		~VSRay3();
		bool GetParameter(const VSVector3 &Point,VSREAL &fRayParameter )const;
		void Transform(const VSRay3 &Ray,const VSMatrix3X3W &Mat);
		/************************FORCEINLINE***************************************/
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fRayParameter)const;

		/********************************RelationWith******************************************/
		//测试射线与三角形是否位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fRayParameter,
						VSREAL fTriangleParameter[3])const;
		//测试射线与平面位置关系
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fRayParameter)const;
		//测试射线与矩形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fRayParameter,
						VSREAL fRectangleParameter[2])const;

		//测试射线与OBB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试直线与AABB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试射线与球位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试射线与多边形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSPolygon3 &Polygon,VSREAL &fRayParameter ,
						bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const;
		/*************************************距离************************************************/
		//点和射线距离
		VSREAL SquaredDistance(const VSVector3 &Point,VSREAL &fLineParameter)const;
		//直线和射线距离
		VSREAL SquaredDistance(const VSLine3 &Line,VSREAL &fRayParameter,VSREAL &fLineParameter)const;
		//射线和射线距离
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL &fRay1Parameter,VSREAL &fRay2Parameter)const;
		//射线和线段距离
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fRayParameter,VSREAL &fSegmentParameter)const;
		//射线和三角形距离
		VSREAL SquaredDistance(const VSTriangle3& Triangle,VSREAL &fRayParameter,VSREAL fTriangleParameter[3])const;
		//射线和矩形距离
		VSREAL SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fRayParameter,VSREAL fRectangleParameter[2])const;
		//射线和OBB距离
		VSREAL SquaredDistance(const VSOBB3 & OBB,VSREAL &fRayParameter,VSREAL fOBBParameter[3])const;
		//射线和球的距离
		VSREAL Distance(const VSSphere3 &Sphere,VSREAL &fRayParameter,VSVector3 & SpherePoint)const;
		//射线和平面距离
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 &RayPoint,VSVector3 &PlanePoint)const;
		//射线和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL &fRayParameter, VSREAL fAABBParameter[3])const;
		//射线和多边形距离
		VSREAL SquaredDistance(const VSPolygon3 &Polygon,VSREAL &fRayParameter,
								int& IndexTriangle,
								VSREAL fTriangleParameter[3])const;
	}; // class
	/*----------------------------------------------------------------*/
#include "VSRay3.inl"

}
#endif