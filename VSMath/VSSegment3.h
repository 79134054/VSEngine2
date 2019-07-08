#ifndef VSSEGMENT3_H
#define VSSEGMENT3_H
#include "VSRay3.h"
#include "VSMatrix3X3W.h"
namespace VSEngine2
{
	class VSMATH_API VSSegment3:public VSRay3
	{
	private:
		VSVector3	m_End;
		VSREAL		m_fLen;
	public:
		VSSegment3();
		~VSSegment3();
		VSSegment3(const VSVector3 &Orig,const VSVector3 &End);
		VSSegment3(const VSVector3 &Orig,const VSVector3 &Dir,VSREAL fLen);
		bool GetParameter(const VSVector3 &Point,VSREAL &fSegmentParameter )const;
		//用矩阵变换射线
		void Transform(const VSSegment3 & Segment,const VSMatrix3X3W &Mat); 
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE const VSVector3 & GetEnd()const;
		FORCEINLINE VSREAL GetLen()const;
		FORCEINLINE void Set(const VSVector3 &Orig,const VSVector3 &End);
		FORCEINLINE void Set(const VSVector3 &Orig,const VSVector3 &Dir,VSREAL fLen);
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fSegmentParameter)const;
	
		
		/********************************RelationWith******************************************/
		//测试线段与三角形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fSegmentParameter,
						VSREAL fTriangleParameter[3])const;
		//测试线段与平面位置关系
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fSegmentParameter)const;
		//测试线段与矩形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fSegmentParameter,
						VSREAL fRectangleParameter[2])const;



		//测试线段与OBB位置关系
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试线段与AABB位置关系
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试线段与球位置关系
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试线段与多边形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSPolygon3 &Polygon,VSREAL &fSegmentParameter,
						bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const;

		/*************************************距离************************************************/
		//点和线段距离
		VSREAL SquaredDistance(const VSVector3 &Point,VSREAL &fSegmentParameter)const;
		//直线和线段距离
		VSREAL SquaredDistance(const VSLine3 &Line,VSREAL &fSegmentParameter,VSREAL &fLineParameter)const;
		//射线和线段距离
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL &fSegmentParameter,VSREAL &fRayParameter)const;
		//线段和线段距离
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fSegment1Parameter,VSREAL &fSegment2Parameter)const;
		//线段和三角形距离
		VSREAL SquaredDistance(const VSTriangle3& Triangle,VSREAL &fSegmentParameter,VSREAL fTriangleParameter[3])const;
		//线段和矩形距离
		VSREAL SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fSegmentParameter,VSREAL fRectangleParameter[2])const;
		//线段和OBB距离
		VSREAL SquaredDistance(const VSOBB3 & OBB,VSREAL &fSegmentParameter,VSREAL fOBBParameter[3])const;

		//线段和球的距离
		VSREAL Distance(const VSSphere3 &Sphere,VSREAL &fSegmentParameter,VSVector3 & SpherePoint)const;

		//线段和平面距离
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 &SegmentPoint,VSVector3 &PlanePoint)const;

		//线段和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL &fSegmentParameter,VSREAL fAABBParameter[3])const;

		//线段和多边形距离
		VSREAL SquaredDistance(const VSPolygon3 &Polygon,VSREAL &fSegmentParameter,
								int& IndexTriangle,
								VSREAL fTriangleParameter[3])const;
	};

#include "VSSegment3.inl"
};
#endif