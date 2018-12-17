#ifndef VSLINE3_H
#define VSLINE3_H
#include "VSMath.h"
#include "VSPlane3.h"
#include "VSOBB3.h"
#include "VSSphere3.h"
#include "VSTriangle3.h"
#include "VSRectangle3.h"
/*
	ֱ����


*/
namespace VSEngine2
{
	class VSRay3;
	class VSSegment3;
	class VSMATH_API VSLine3
	{
	protected:
		VSVector3	m_Orig;  // Դ��
		VSVector3	m_Dir;   // ����
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
		//����ֱ����������λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fLineParameter,
						VSREAL fTriangleParameter[3])const;
		//����ֱ����ƽ��λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fLineParameter)const;
		//����ֱ�������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fLineParameter,
						VSREAL fRectangleParameter[2])const;
		//����ֱ������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//����ֱ����OBBλ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		

		//����ֱ����AABBλ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;

		//����ֱ��������λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSPolygon3 &Polygon,VSREAL &fLineParameter, 
						bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const;
		/*************************************����************************************************/

		//�㵽ֱ�߾���
		VSREAL SquaredDistance(const VSVector3 &Point,VSREAL &fLineParameter)const;
		//ֱ�ߺ�ֱ�ߵľ���
		VSREAL SquaredDistance(const VSLine3 &Line,VSREAL &fLine1Parameter,VSREAL &fLine2Parameter)const;
		//ֱ�ߺ����߾���
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL &fLineParameter,VSREAL &fRayParameter)const;
		//ֱ�ߺ��߶ξ���
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fLineParameter,VSREAL &fSegmentParameter)const;
		//ֱ�ߺ������ξ���
		VSREAL SquaredDistance(const VSTriangle3& Triangle,VSREAL &fLineParameter,VSREAL fTriangleParameter[3])const;
		//ֱ�ߺ;��ξ���
		VSREAL SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fLineParameter,VSREAL fRectangleParameter[2])const;
		//ֱ�ߺ�OBB����
		VSREAL SquaredDistance(const VSOBB3 & OBB,VSREAL &fLineParameter,VSREAL fOBBParameter[3])const;
		//ֱ�ߺ���ľ���
		VSREAL Distance(const VSSphere3 &Sphere,VSREAL &fLineParameter,VSVector3 & SpherePoint)const;
		//ֱ�ߺ�ƽ�����
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &LinePoint,VSVector3 &PlanePoint)const;
		//ֱ�ߺ�AABB����
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL &fLineParameter, VSREAL fAABBParameter[3])const;

		//ֱ�ߺͶ���ξ���
		VSREAL SquaredDistance(const VSPolygon3 & Polygon,VSREAL &fLineParameter,
								int& IndexTriangle,
								VSREAL fTriangleParameter[3])const;
	};

#include "VSLine3.inl"
};
#endif