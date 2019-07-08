#ifndef VSOBB3_H
#define VSOBB3_H
#include "VSVector3.h"
#include "VSTriangle3.h"
#include "VSPlane3.h"
#include "VSMatrix3X3W.h"
/*****************************************************************************************


                               p1  _______________________________ P0
                                  /|                            /|
                                /  |                          /  |
							  /    |m_A[1]  |        m_A[2] /    |
						  p5/______|________|______/______/ p4   |
							|      |        |    /        |      |
							|  p2  |_ _  _ _|  / _ _ _ _ _|_ _ __| p3
							|     /         |/____________|      /
							|	 /	             m_A[0]    |    /      
							|  /                          |  /
						p6	|/____________________________|/ p7



Plane0 right Plane1 left Plane2 front Plane3 back Plane4 up Plane5 down
****************************************************************************************/
namespace VSEngine2
{
class VSMatrix3X3;
class VSLine3;
class VSRay3;
class VSRectangle3;
class VSSegment3;
	class VSMATH_API VSOBB3
	{
	private:
		VSVector3	m_A[3];		// 3个轴
		VSVector3	m_Center;	// 中心点
		VSREAL		m_fA[3];	// 3个半轴的长度
	public:	
		
		
		VSOBB3();
		~VSOBB3();
		//构造AABB
		VSOBB3(const VSVector3 A[3],VSREAL fA[3],const VSVector3 & Center);
		VSOBB3(const VSVector3 &A0,const VSVector3 &A1,const VSVector3 &A2,
				VSREAL fA0,VSREAL fA1,VSREAL fA2,
				const VSVector3 & Center);
		//通过点集合构造AABB
		void CreateOBB(const VSVector3 * const pPointArray,unsigned int uiPointNum);
		//合并AABB
		VSOBB3 MergOBB(const VSOBB3 &OBB)const;
		//取得6个平面
		void GetPlane(VSPlane3 pPlanes[6])const;
		//取得8个点
		void GetPoint(VSVector3 Point[8])const;
		//取得6个矩形
		void GetRectangle(VSRectangle3 Rectangle[6])const;
		//获得变换到OBB下变换矩阵，物体必须和OBB在同一个坐标系下
		void GetTransform(VSMatrix3X3W &m)const;
		//得到点参数
		bool GetParameter(const VSVector3 &Point,VSREAL fOBBParameter[3])const;
		//用给定的OBB和变换矩阵来构造OBB
		void Transform(const VSOBB3 &OBB, 
			const VSMatrix3X3W &Mat);
		VSAABB3 GetAABB()const;
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Set(const VSVector3 A[3],VSREAL fA[3],const VSVector3 & Center);
		FORCEINLINE void Set(const VSVector3 &A0,const VSVector3 &A1,const VSVector3 &A2,
						VSREAL fA0,VSREAL fA1,VSREAL fA2,
						const VSVector3 & Center);
		
		FORCEINLINE void GetA(VSVector3 A[3])const;
		FORCEINLINE void GetfA(VSREAL fA[3])const;
		FORCEINLINE const VSVector3 & GetCenter()const;
		//根据参数返回点
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fOBBParameter[3])const;
		FORCEINLINE VSVector3 GetParameterPoint(VSREAL fOBBParameter0,VSREAL fOBBParameter1,VSREAL fOBBParameter2)const;
		 
		
		/*************************************距离************************************************/
		//点和OBB距离
		VSREAL SquaredDistance(const VSVector3 & Point,VSREAL fOBBParameter[3])const;
		//直线和OBB距离
		VSREAL SquaredDistance(const VSLine3 & Line, VSREAL fOBBParameter[3],VSREAL &fLineParameter)const;
		//射线和OBB距离
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL fOBBParameter[3],VSREAL &fRayParameter)const;
		//线段和OBB距离
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL fOBBParameter[3],VSREAL &fSegmentParameter)const;

		//三角形和OBB距离
		VSREAL SquaredDistance(const VSTriangle3 & Triangle , VSREAL OBBParameter[3],
								VSREAL TriangleParameter[3])const;

		//矩形和OBB距离
		VSREAL SquaredDistance(const VSRectangle3& Rectangle , VSREAL OBBParameter[3],
								VSREAL RectangleParameter[2])const;
		//OBB和OBB距离
		VSREAL SquaredDistance(const VSOBB3 &OBB , VSREAL OBB1Parameter[3],
								VSREAL OBB2Parameter[3])const;

		//OBB和球的距离
		VSREAL Distance(const VSSphere3 & Sphere,VSREAL fOBBParameter[3],VSVector3 & SpherePoint)const;

		//OBB和平面距离
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &OBBPoint,VSVector3 &PlanePoint)const;

		//多边形和OBB距离
		VSREAL SquaredDistance(const VSPolygon3 &Polygon, 
							VSREAL OBBParameter[3],int& IndexTriangle,
							VSREAL TriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//点和OBB位置关系
		//VSIN VSOUT VSON
		int RelationWith(const VSVector3 &Point)const;
		//测试直线与OBB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSLine3 &Line, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试射线与OBB位置关系
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRay3 &Ray, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试线段与OBB位置关系
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSSegment3 &Segment, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//测试平面和OBB位置关系
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPlane3 &Plane)const;
		//测试三角形与OBB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSTriangle3 & Triangle)const;    

		//矩形和OBB位置关系
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSRectangle3 &Rectangle)const;

		//OBB和OBB位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSOBB3 &OBB)const;

		//OBB和圆位置关系
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSSphere3 &Sphere)const;
	private:
		/*void OBBProj(const VSOBB3 &OBB, const VSVector3 &vcV, 
			VSREAL *pfMin, VSREAL *pfMax)const;
		void TriProj(const VSVector3 &v0, const VSVector3 &v1, 
			const VSVector3 &v2, const VSVector3 &vcV, 
			VSREAL *pfMin, VSREAL *pfMax)const;

		void Face (int i0, int i1, int i2, VSVector3 Pnt,
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

#include "VSOBB3.inl"
};
#endif