#ifndef VSSPHERE3_H
#define VSSPHERE3_H
#include "VSVector3.h"
namespace VSEngine2
{
	class VSMATH_API VSSphere3
	{
	public:
		VSVector3 m_Center;		//����
		VSREAL m_fRadius;		//�뾶
		VSSphere3();
		~VSSphere3();
		VSSphere3(const VSVector3 & Center, VSREAL fRadius);
		//��������������Χ��
		void CreateSphere(const VSVector3 *pPointArray,unsigned int uiPointNum);
		//��ϰ�Χ��
		VSSphere3 MergSpheres(const VSSphere3 &Sphere)const;
		//�任����
		void Transform(const VSSphere3 & Sphere,const VSMatrix3X3W &Mat); 
		VSAABB3 GetAABB()const;
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void Set(const VSVector3 & Center, VSREAL fRadius);
		
		/*************************************����************************************************/
		//�����ľ���
		VSREAL Distance(const VSVector3 & Point,VSVector3 & SpherePoint)const;
		//�ߺ���ľ���
		VSREAL Distance(const VSLine3 & Line,VSVector3 & SpherePoint,VSREAL &fLineParameter)const;
		//���ߺ���ľ���
		VSREAL Distance(const VSRay3 & Ray,VSVector3 & SpherePoint,VSREAL &fRayParameter)const;
		//�߶κ���ľ���
		VSREAL Distance(const VSSegment3 & Segment,VSVector3 & SpherePoint,VSREAL &fSegmentParameter)const;
		//OBB����ľ���
		VSREAL Distance(const VSOBB3 &OBB,VSVector3 & SpherePoint,VSREAL fOBBParameter[3])const;
		//ƽ�����ľ���
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 & SpherePoint)const;
		//���κ���ľ���
		VSREAL Distance(const VSRectangle3 & Rectangle,VSVector3 & SpherePoint,VSREAL fRectangleParameter[2])const;
		//�����κ���ľ���
		VSREAL Distance(const VSTriangle3 Triangle,VSVector3 & SpherePoint,VSREAL fTriangleParameter[3])const;
		//AABB����ľ���
		VSREAL Distance(const VSAABB3 &AABB,VSVector3 & SpherePoint,VSREAL fAABBParameter[3])const;

		//����κ���ľ���
		VSREAL Distance(const VSPolygon3 &Polygon,VSVector3 & SpherePoint,int& IndexTriangle,
						VSREAL fTriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//�����λ�ù�ϵ
		//VSIN VSOUT VSON	
		int RelationWith(const VSVector3 &Point)const;
		//ֱ������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSLine3 &Line, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//��������λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT
		int RelationWith(const VSRay3 &Ray, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//�߶�����λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT VSIN
		int RelationWith(const VSSegment3 &Segment, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const;
		//ƽ�������λ�ù�ϵ
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPlane3 &Plane)const;
		//�����κ�Բλ�ù�ϵ
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSTriangle3 Triangle)const;
		//���κ�Բλ�ù�ϵ
		//VSNOINTERSECT VSINTERSECT VSIN
		int RelationWith(const VSRectangle3 &Rectangle)const;

		//OBB��Բλ�ù�ϵ
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSOBB3 &OBB)const;
		//԰��Բλ�ù�ϵ
		//VSNOINTERSECT VSINTERSECT
		int RelationWith(const VSSphere3 &Sphere)const;
	};

#include "VSSphere3.inl"
};
#endif