#ifndef VSAABB3_H
#define VSAABB3_H
#include "VSVector3.h"
#include "VSPlane3.h"
#include "VSRectangle3.h"
/*
AABB轴对称边界盒，3个中心轴A1,A2,A3永远平行于当前所在坐标轴，中心轴为单位坐标轴，fA1,fA2,fA3为半轴长度，所有内部的点都可以表示成
a * A1 + b * A2 + c * A3 a,b,c为参数，并且|a|<=fA1,|b|<=fA2,|c|<=fA3
*/
namespace VSEngine2
{
	class VSMATH_API VSAABB3
	{
	private:
		static const VSVector3	m_A[3];			// 3个轴				
		VSVector3	m_Center;					// 中心点
		VSREAL		m_fA[3];					// 3个半轴的长度
		VSVector3	m_Max;						//最大点
		VSVector3	m_Min;						//最小点
	public:
		
		VSAABB3();
		~VSAABB3();

		//通过最大点和最小点构造AABB
		VSAABB3(const VSVector3 & Max , const VSVector3 & Min);
		//通过中心点和3个轴的半长度构造AABB
		VSAABB3(const VSVector3 & Center,VSREAL fA0,VSREAL fA1,VSREAL fA2);
		VSAABB3(const VSVector3 & Center,VSREAL fA[3]);
		//通过点集合构造AABB
		void CreateAABB(const VSVector3 * const pPointArray,unsigned int uiPointNum);
		/*********************************** FORCEINLINE *************************************/
		//设置相应的参数
		FORCEINLINE void Set(const VSVector3 & Max , const VSVector3 & Min);
		FORCEINLINE void Set(const VSVector3 & Center,VSREAL fA0,VSREAL fA1,VSREAL fA2);
		FORCEINLINE void Set(const VSVector3 & Center,VSREAL fA[3]);
		//获取相应的参数
		FORCEINLINE void GetfA(VSREAL fA[3])const;
		FORCEINLINE const VSVector3 & GetCenter()const;
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fAABBParameter[3])const;
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fAABBParameter0,VSREAL fAABBParameter1,VSREAL fAABBParameter2)const;
		FORCEINLINE const VSVector3 & GetMaxPoint()const;
		FORCEINLINE const VSVector3 & GetMinPoint()const;
		//得到AABB6个平面
		void GetPlane(VSPlane3 pPlanes[6])const;
		//得到AABB8个点
		void GetPoint(VSVector3 Point[8])const;
		//得到AABB6个矩形
		void GetRectangle(VSRectangle3 Rectangle[6])const;
		//给定AABB内一点返回AABB的参数
		bool GetParameter(const VSVector3 &Point,VSREAL fAABBParameter[3])const;
		//用矩阵变换AABB
		void Transform(const VSAABB3 &AABB, 
						const VSMatrix3X3W &m,bool bHasProject = false);
		//合并2个AABB
		VSAABB3 MergAABB(const VSAABB3 &AABB)const;
		void GetQuadAABB(VSAABB3 AABB[4])const;
		void GetOctAABB(VSAABB3 AABB[8])const;
		VSAABB3 GetMin(const VSAABB3 & AABB) const;
		/*************************************距离************************************************/
		//点和AABB距离
		VSREAL SquaredDistance(const VSVector3 & Point,VSREAL fAABBParameter[3])const;
		//直线和AABB距离
		VSREAL SquaredDistance(const VSLine3 & Line, VSREAL fAABBParameter[3],VSREAL &fLineParameter)const;
		//射线和AABB距离
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL fAABBParameter[3],VSREAL &fRayParameter)const;
		//线段和AABB距离
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL fAABBParameter[3],VSREAL &fSegmentParameter)const;

		//三角形和AABB距离
		VSREAL SquaredDistance(const VSTriangle3 & Triangle , VSREAL AABBParameter[3],
			VSREAL TriangleParameter[3])const;

		//矩形和AABB距离
		VSREAL SquaredDistance(const VSRectangle3& Rectangle , VSREAL AABBParameter[3],
			VSREAL RectangleParameter[2])const;
		//AABB和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB , VSREAL AABB1Parameter[3],
								VSREAL AABB2Parameter[3])const;
		//AABB和球的距离
		VSREAL Distance(const VSSphere3 & Sphere,VSREAL fAABBParameter[3],VSVector3 & SpherePoint)const;

		//AABB和平面距离
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &AABBPoint,VSVector3 &PlanePoint)const;

		//多边形和AABB距离
		VSREAL SquaredDistance(const VSPolygon3 &Polygon,VSREAL AABBParameter[3],int& IndexTriangle,
								VSREAL TriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//点和AABB位置关系
		//VSIN VSOUT VSON
		int RelationWith(const VSVector3 &Point)const;
		//测试直线与AABB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSLine3 &Line, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试射线与AABB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRay3 &Ray, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试线段与AABB位置关系
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSSegment3 &Segment, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试平面和AABB位置关系
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPlane3 &Plane)const;
		//三角形和AABB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSTriangle3 &Triangle)const;
		//矩形和AABB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSRectangle3 &Rectangle)const;

		//AABB和AABB位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSAABB3 &AABB)const;

		//AABB和圆位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSSphere3 &Sphere)const;

		bool GetIntersect(VSAABB3& AABB,VSAABB3 & OutAABB)const;
	private:
		/*void Face (int i0, int i1, int i2, VSVector3 Pnt,
			VSVector3 Dir, VSVector3 PmE,
			VSREAL& rfSqrDistance,VSREAL &fLParam)const;
		void CaseNoZeros (VSVector3& Pnt,
			const VSVector3& Dir, VSREAL& rfSqrDistance,VSREAL &fLParam)const;
		void Case0 (int i0, int i1, int i2, VSVector3& Pnt,
			const VSVector3& Dir, VSREAL& rfSqrDistance,VSREAL &fLParam)const;

		void Case00 (int i0, int i1, int i2,
			VSVector3& Pnt, const VSVector3& Dir,  VSREAL& rfSqrDistance,VSREAL &fLParam)const;
		void Case000 (VSVector3& Pnt, VSREAL& rfSqrDistance)const;*/
	};

#include "VSAABB3.inl"
};
#endif