#ifndef VSPLANE3_H
#define VSPLANE3_H
#include "VSVector3.h"
#include "VSMatrix3X3.h"

namespace VSEngine2
{
	class VSLine3;
	class VSPlane3;
	class VSSphere3;
	class VSCylinder3;
	class  VSMATH_API VSPlane3 
	{
	protected:
		VSVector3	m_N;       //ƽ�淨����
		VSREAL		m_fD;       // ��ԭ��ľ���(ax+by+cz+d=0)
	public:
		VSPlane3();
		VSPlane3(const VSVector3 &N, const VSVector3 &P);
		VSPlane3(const VSVector3 &P0,  const VSVector3 &P1, const VSVector3 &P2);
		VSPlane3(const VSVector3 Point[3]);
		VSPlane3(const VSVector3 &N , VSREAL fD);
		void Transform(const VSPlane3 &Plane,const VSMatrix3X3W &Mat);
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE const VSVector3 & GetN()const;
		FORCEINLINE VSVector3 GetPoint()const;
		FORCEINLINE VSREAL GetfD()const;
		FORCEINLINE void  Set(const VSVector3 &N, const VSVector3 &P);
		FORCEINLINE void  Set(const VSVector3 &N , VSREAL fD);
		FORCEINLINE void  Set(const VSVector3 &P0,  const VSVector3 &P1, const VSVector3 &P2);
		FORCEINLINE void  Set(const VSVector3 Point[3]);
		FORCEINLINE VSPlane3 GetPlane()const;
		VSVector3 ReflectDir(const VSVector3 & Dir)const;
		/*************************************����************************************************/
		//�㵽ƽ�����
		VSREAL Distance(const VSVector3 &Point,VSVector3 &PlanePoint)const;
		//ƽ�����ľ���
		VSREAL Distance(const VSSphere3 &Sphere,VSVector3 & SpherePoint)const;
		//ֱ�ߺ�ƽ�����
		VSREAL Distance(const VSLine3 &Line,VSVector3 &PlanePoint,VSVector3 &LinePoint)const;
		//���ߺ�ƽ�����
		VSREAL Distance(const VSRay3 & Ray,VSVector3 &PlanePoint,VSVector3 &RayPoint)const;
		//�߶κ�ƽ�����
		VSREAL Distance(const VSSegment3 & Segment,VSVector3 &PlanePoint,VSVector3 &SegmentPoint)const;

		//������뺯����û�п����ཻ������¼���ģ���Ϊ�ཻ������¾���Ϊ0������RelationWith�ж�λ�ù�ϵ�������������������
		//ƽ���ƽ�����
		VSREAL Distance(const VSPlane3 &Plane,VSVector3 &Plane1Point,VSVector3 &Plane2Point)const;
		//ƽ��������ξ���
		VSREAL Distance(const VSTriangle3 &Triangle,VSVector3 &PlanePoint,VSVector3 &TrianglePoint)const;
		//���κ�ƽ�����
		VSREAL Distance(const VSRectangle3 &Rectangle,VSVector3 &PlanePoint,VSVector3 &RectanglePoint)const;
		//OBB��ƽ�����
		VSREAL Distance(const VSOBB3& OBB,VSVector3 &PlanePoint,VSVector3 &OBBPoint)const;
		//AABB��ƽ�����
		VSREAL Distance(const VSAABB3 &AABB,VSVector3 &PlanePoint,VSVector3 &AABBPoint)const;
		//ƽ��Ͷ���ξ���
		VSREAL Distance(const VSPolygon3 &Polygon,VSVector3 &PlanePoint,int& IndexTriangle,
						VSVector3 &TrianglePoint)const;
		/********************************RelationWith******************************************/
		//���ƽ���λ�ù�ϵ(VSFRONT VSBACK VSPLANAR)
		int RelationWith(const VSVector3 &Point)const;
		//����ֱ����ƽ��λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSLine3 &Line, bool bCull,VSREAL &fLineParameter)const;
		//����������ƽ��λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSRay3 &Ray, bool bCull,VSREAL &fRayParameter)const;
		//�����߶���ƽ��λ�ù�ϵ
		//VSNOINTERSECT VSNTERSECT VSON VSBACK VSFRONT
		int RelationWith(const VSSegment3 &Segment, bool bCull,VSREAL &fSegmentParameter)const;
		//����ƽ���OBBλ�ù�ϵ
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSOBB3 &OBB)const;
		//����ƽ���AABBλ�ù�ϵ
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSAABB3 &AABB)const;
		//ƽ�������λ�ù�ϵ
		//VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSSphere3 &Sphere)const;
		//ƽ���������λ�ù�ϵ
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSTriangle3 &Triangle)const;
		int RelationWith(const VSTriangle3 &Triangle ,VSSegment3 & Segment)const;
		//������Ϊ����ƽ���ƽ��λ�ù�ϵ
		//VSNOINTERSECT VSINTERSECT 
		int RelationWith(const VSPlane3 &Plane)const;
		int RelationWith(const VSPlane3 &Plane,VSLine3 &Line)const;
		//ƽ��;���λ�ù�ϵ
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSRectangle3 & Rectangle)const;		
		int RelationWith(const VSRectangle3 &Rectangle,VSSegment3 &Segment)const;

		//ƽ��Ͷ����λ�ù�ϵ
		//VSON VSFRONT VSBACK VSINTERSECT
		int RelationWith(const VSPolygon3 &Polygon)const;
		int RelationWith(const VSPolygon3 &Polygon,VSSegment3 & Segment)const;

		//ƽ���Բ��λ�ù�ϵ
		int RelationWith(const VSCylinder3 &Cylinder3)const;

	}; // class
	/*----------------------------------------------------------------*/
#include "VSPlane3.inl"

};
#endif