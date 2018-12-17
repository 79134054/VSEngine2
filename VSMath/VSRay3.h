#ifndef VSRAY3_H
#define VSRAY3_H
#include "VSLine3.h"
namespace VSEngine2
{
	//����
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
		//�����������������Ƿ�λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fRayParameter,
						VSREAL fTriangleParameter[3])const;
		//����������ƽ��λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fRayParameter)const;
		//�������������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fRayParameter,
						VSREAL fRectangleParameter[2])const;

		//����������OBBλ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//����ֱ����AABBλ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//������������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//��������������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSPolygon3 &Polygon,VSREAL &fRayParameter ,
						bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const;
		/*************************************����************************************************/
		//������߾���
		VSREAL SquaredDistance(const VSVector3 &Point,VSREAL &fLineParameter)const;
		//ֱ�ߺ����߾���
		VSREAL SquaredDistance(const VSLine3 &Line,VSREAL &fRayParameter,VSREAL &fLineParameter)const;
		//���ߺ����߾���
		VSREAL SquaredDistance(const VSRay3 &Ray,VSREAL &fRay1Parameter,VSREAL &fRay2Parameter)const;
		//���ߺ��߶ξ���
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fRayParameter,VSREAL &fSegmentParameter)const;
		//���ߺ������ξ���
		VSREAL SquaredDistance(const VSTriangle3& Triangle,VSREAL &fRayParameter,VSREAL fTriangleParameter[3])const;
		//���ߺ;��ξ���
		VSREAL SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fRayParameter,VSREAL fRectangleParameter[2])const;
		//���ߺ�OBB����
		VSREAL SquaredDistance(const VSOBB3 & OBB,VSREAL &fRayParameter,VSREAL fOBBParameter[3])const;
		//���ߺ���ľ���
		VSREAL Distance(const VSSphere3 &Sphere,VSREAL &fRayParameter,VSVector3 & SpherePoint)const;
		//���ߺ�ƽ�����
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 &RayPoint,VSVector3 &PlanePoint)const;
		//���ߺ�AABB����
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL &fRayParameter, VSREAL fAABBParameter[3])const;
		//���ߺͶ���ξ���
		VSREAL SquaredDistance(const VSPolygon3 &Polygon,VSREAL &fRayParameter,
								int& IndexTriangle,
								VSREAL fTriangleParameter[3])const;
	}; // class
	/*----------------------------------------------------------------*/
#include "VSRay3.inl"

}
#endif