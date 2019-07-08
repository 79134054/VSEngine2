#ifndef VSRECTANGLE3_H
#define VSRECTANGLE3_H
#include "VSVector3.h"
#include "VSPlane3.h"
#include "VSMatrix3X3W.h"
/********************************************************************************************


__________________
|        |        |  
|    A[0]|        |      
|		  |        |      
|        |_A[1]___|
|                 |
|                 |
|_________________|


左手定则 法向量 为 cross (A[0],A[1]),规定可见 ，方向朝向屏幕外， 
********************************************************************************************/
namespace VSEngine2
{
class VSTriangle3;
class VSMATH_API VSRectangle3:public VSPlane3
{
private:
	VSVector3 m_A[2];
	VSREAL m_fA[2];
	VSVector3 m_Center;	
public:	
	VSRectangle3();
	~VSRectangle3();
	VSRectangle3(const VSVector3 & Center,const VSVector3 &A0,const VSVector3 & A1,VSREAL fA0, VSREAL fA1);
	VSRectangle3(const VSVector3 & Center,const VSVector3 A[2],const VSREAL fA[2]);
	bool GetParameter(const VSVector3 &Point,VSREAL fRectangleParameter[2])const;
	void Transform(const VSRectangle3 &Rectangle, 
				const VSMatrix3X3W &Mat);
	/*************************************FORCEINLINE************************************************/
	FORCEINLINE void Set(const VSVector3 & Center,const VSVector3 &A0,const VSVector3 & A1,VSREAL fA0, VSREAL fA1);
	FORCEINLINE void Set(const VSVector3 & Center,const VSVector3 A[2],const VSREAL fA[2]);
	FORCEINLINE void GetA(VSVector3 A[2])const;
	FORCEINLINE void GetfA(VSREAL fA[2])const;
	FORCEINLINE const VSVector3 & GetCenter()const;
	FORCEINLINE void GetPoint(VSVector3 Point[4])const;
	FORCEINLINE VSVector3 GetParameterPoint(VSREAL fRectangleParameter[2])const;
	FORCEINLINE VSVector3 GetParameterPoint(VSREAL fRectangleParameter0,VSREAL fRectangleParameter1)const;
	/*************************************距离************************************************/
	//点和矩形距离
	VSREAL SquaredDistance(const VSVector3 &Point,VSREAL fRectangleParameter[2])const;

	//矩形和矩形距离
	VSREAL SquaredDistance(const VSRectangle3 &Rectangle,
							VSREAL fRectangle1Parameter[2],VSREAL fRectangle2Parameter[2])const;

	//三角形和矩形距离
	VSREAL SquaredDistance(const VSTriangle3 &Triangle,VSREAL fRectangleParameter[2],VSREAL fTriangleParameter[3])const;

	//直线和矩形距离
	VSREAL SquaredDistance(const VSLine3& Line,VSREAL fRectangleParameter[2],VSREAL &fLineParameter)const;
	//射线和矩形距离
	VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL fRectangleParameter[2],VSREAL &fRayParameter)const;
	//线段和矩形距离
	VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL fRectangleParameter[2],VSREAL &fSegmentParameter)const;
	//矩形和OBB距离
	VSREAL SquaredDistance(const VSOBB3 &OBB,VSREAL RectangleParameter[2], VSREAL OBBParameter[3])const;
	//矩形和球的距离
	VSREAL Distance(const VSSphere3 &Sphere,VSREAL fRectangleParameter[2],VSVector3 & SpherePoint)const;
	//矩形和平面距离
	VSREAL Distance(const VSPlane3 &Plane,VSVector3 &RectanglePoint,VSVector3 &PlanePoint)const;
	//矩形和AABB距离
	VSREAL SquaredDistance(const VSAABB3 &AABB ,VSREAL RectangleParameter[2]
							, VSREAL AABBParameter[3])const;

	//多边形和矩形距离
	VSREAL SquaredDistance(const VSPolygon3 &Polygon,
								VSREAL fRectangleParameter[2],
								int& IndexTriangle,
								VSREAL fTriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//测试直线与矩形位置关系
	//VSNOINTERSECT VSNTERSECT
	int RelationWith(const VSLine3 &Line,bool bCull,VSREAL fRectangleParameter[2],VSREAL &fLineParameter)const;
	//测试射线与矩形位置关系
	//VSNOINTERSECT VSNTERSECT
	int RelationWith(const VSRay3 &Ray,bool bCull,VSREAL fRectangleParameter[2],VSREAL &fRayParameter)const;
	//测试线段与矩形位置关系
	//VSNOINTERSECT VSNTERSECT
	int RelationWith(const VSSegment3 &Segment,bool bCull,VSREAL fRectangleParameter[2],VSREAL &fSegmentParameter)const;
	//平面和矩形位置关系
	//VSON VSFRONT VSBACK VSINTERSECT
	int RelationWith(const VSPlane3 &Plane)const;
	int RelationWith(const VSPlane3 &Plane,VSSegment3 &Segment)const;
	//三角形和矩形位置关系
	//VSNOINTERSECT VSINTERSECT
	int RelationWith(const VSTriangle3 &Triangle)const;
	int RelationWith(const VSTriangle3 &Triangle,VSSegment3 & Segment)const;
	//矩形和矩形位置关系
	//VSNOINTERSECT VSINTERSECT
	int RelationWith(const VSRectangle3 &Rectangle)const;
	int RelationWith(const VSRectangle3 &Rectangle,VSSegment3 & Segment)const;
	//矩形和AABB位置关系
	//VSNOINTERSECT VSINTERSECT VSIN
	int RelationWith(const VSAABB3 &AABB)const;

	//矩形和OBB位置关系
	//VSNOINTERSECT VSINTERSECT VSIN
	int RelationWith(const VSOBB3 &OBB)const;

	//矩形和圆位置关系
	//VSNOINTERSECT VSINTERSECT VSIN
	int RelationWith(const VSSphere3 & Sphere)const;
};

#include "VSRectangle3.inl"
};
#endif