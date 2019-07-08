#ifndef VSTRIANGLE3_H
#define VSTRIANGLE3_H
#include "VSVector3.h"
#include "VSPlane3.h"
/***************************************************************************************

				    fTriangleParameter[1]
		   m_V[0]	_________________________m_V[1]
				   \                       /
				    \                     /
				     \                   /
				      \                 /  
				       \               /
					    \             /
fTriangleParameter[2]  \           /  fTriangleParameter[0]
					      \         /
					       \       /
						     \    /
						      \  /
						       \/m_V[2]
***************************************************************************************/
namespace VSEngine2
{
	class VSOBB3;
	class VSMATH_API VSTriangle3:public VSPlane3
	{
	private:	
		VSVector3 m_V[3];
	public:
		
		VSTriangle3();
		~VSTriangle3();
		VSTriangle3(const VSVector3 & P0,const VSVector3 &P1,const VSVector3 &P2);
		VSTriangle3(const VSVector3 Point[3]);
		bool GetParameter(const VSVector3 &Point,VSREAL fTriangleParameter[3])const;
		void Transform(const VSTriangle3 & Triangle,const VSMatrix3X3W &Mat); 
		/*************************************FORCEINLINE************************************************/

		FORCEINLINE void Set(const VSVector3 & P1,const VSVector3 &P2,const VSVector3 &P3);
		FORCEINLINE void Set(const VSVector3 Point[3]);
		FORCEINLINE void GetPoint(VSVector3  Point[3])const;
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fTriangleParameter[3])const;
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fTriangleParameter0,
										VSREAL fTriangleParameter1,VSREAL fTriangleParameter2)const;
		/*************************************距离************************************************/
		//点和三角形距离
		VSREAL SquaredDistance(const VSVector3 & Point,VSREAL fTriangleParameter[3])const;
		//三角形和三角形距离
		VSREAL SquaredDistance(const VSTriangle3 &Triangle,
			VSREAL fTriangle1Parameter[3],
			VSREAL fTriangle2Parameter[3])const;
		//三角形和矩形距离
		VSREAL SquaredDistance(const VSRectangle3 &Rectangle,
							VSREAL fTriangleParameter[3],
							VSREAL fRectangleParameter[2])const;
		
		//直线和三角形距离
		VSREAL SquaredDistance(const VSLine3 &Line,VSREAL fTriangleParameter[3],VSREAL &fLineParameter)const;
		//射线和三角形距离
		VSREAL SquaredDistance(const VSRay3 & Ray,VSREAL fTriangleParameter[3],VSREAL &fRayParameter)const;
		//线段和三角形距离
		VSREAL SquaredDistance(const VSSegment3& Segment,VSREAL fTriangleParameter[3],VSREAL &fSegmentParameter)const;
		//三角形和OBB距离
		VSREAL SquaredDistance(const VSOBB3 &OBB,VSREAL TriangleParameter[3], VSREAL OBBParameter[3])const;
		//三角形和球的距离
		VSREAL Distance(const VSSphere3 &Sphere,VSREAL fTriangleParameter[3],VSVector3 & SpherePoint)const;

		//平面和三角形距离
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &TrianglePoint,VSVector3 &PlanePoint)const;
		//三角形和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB ,VSREAL TriangleParameter[3]
								, VSREAL AABBParameter[3])const;
		//三角形和多边形距离
		VSREAL SquaredDistance(const VSPolygon3& Polygon,
								VSREAL fTriangle1Parameter[3],
								int& IndexTriangle,
								VSREAL fTriangle2Parameter[3])const;
		/********************************RelationWith******************************************/
		//测试直线与三角形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSLine3 &Line, bool bCull,VSREAL fTriangleParameter[3],
						VSREAL &fLineParameter)const;
		//测试射线与三角形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRay3 &Ray, bool bCull,VSREAL fTriangleParameter[3],
						VSREAL &fRayParameter)const;
		//测试线段与三角形位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSSegment3 &Segment, bool bCull,VSREAL fTriangleParameter[3],
						VSREAL &fSegmentParameter)const;
		//平面和三角形位置关系
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPlane3 & Plane)const;
		int RelationWith(const VSPlane3 & Plane,VSSegment3 & Segment)const;
		//三角形和三角形位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSTriangle3 & Triangle)const;
		int RelationWith(const VSTriangle3 & Triangle,VSSegment3 & Segment)const;
		//三角形和矩形位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSRectangle3 & Rectangle)const;
		int RelationWith(const VSRectangle3 & Rectangle,VSSegment3 & Segment)const;
		//三角形和AABB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSAABB3 &AABB)const;

		//三角形和OBB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSOBB3 &OBB)const;

		//三角形和圆位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSSphere3 &Sphere)const;
	};

#include "VSTriangle3.inl"

};
#endif