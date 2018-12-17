#ifndef VSVECTOR3_H
#define VSVECTOR3_H
#include "VSMath.h"
#include "VSFastFunction.h"
namespace VSEngine2
{
	class VSMatrix3X3;
	class VSQuat;
	class VSRay3;
	class VSLine3;
	class VSPlane3;
	class VSRay3;
	class VSSegment3;
	class VSTriangle3;
	class VSRectangle3;
	class VSOBB3;
	class VSSphere3;
	class VSAABB3;
	class VSMatrix3X3W;
	class VSPolygon3;
	//����
	class  VSMATH_API VSVector3 
	{
	public:
		union
		{
			VSREAL m[3];
			struct  
			{
				VSREAL x, y, z;
			};
		};


		VSVector3(void);
		VSVector3(VSREAL _x, VSREAL _y, VSREAL _z);

		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void  Set(VSREAL _x, VSREAL _y, VSREAL _z);
		FORCEINLINE VSREAL GetLength(void)const;                   // ����
		FORCEINLINE VSREAL GetSqrLength(void) const;          // ����ƽ��
		FORCEINLINE void  Negate(void);                      // ����-1
		FORCEINLINE void  Normalize(void);                   // ��λ��
		FORCEINLINE void Cross(const VSVector3 &v1,          //���
						const VSVector3 &v2); 

		VSREAL AngleWith( VSVector3 &v);          // ���������ļн�(����)
		void  Create(const VSVector3 &v1,			// ���������ӵ�v1��v2
							const VSVector3 &v2);


		/*            N     _
				 \	  /|\   /|
			Dir	  \	   |   /  Reflect		 
		          _\|  |  /
		--------------------------
		*/
		VSVector3 ReflectDir(const VSVector3 &N)const;

		VSVector3 & operator += (const VSVector3 &v);
		VSVector3 & operator -= (const VSVector3 &v);

		VSVector3 & operator *= (VSREAL f);
		VSVector3 & operator /= (VSREAL f);
		VSVector3 & operator += (VSREAL f);
		VSVector3 & operator -= (VSREAL f);

		
		VSREAL Dot(const VSVector3 &v)const;//���
		bool operator ==(const VSVector3 &v)const;


		VSVector3 operator * (VSREAL f)const;            
		VSVector3 operator / (VSREAL f)const;            
		VSVector3 operator + (VSREAL f)const;            
		VSVector3 operator - (VSREAL f)const;            

		VSQuat   operator * (const VSQuat   &q)const; 
		VSVector3 operator * (const VSMatrix3X3 &m)const; 
		VSVector3 operator * (const VSMatrix3X3W &m)const;
		VSVector3 operator + (const VSVector3 &v)const; 
		VSVector3 operator - (const VSVector3 &v)const; 
		VSVector3 operator / (const VSVector3 &v)const;
		VSVector3 operator * (const VSVector3 &v)const;

		VSVector3 & operator /= (const VSVector3 &v);
		VSVector3 & operator *= (const VSVector3 &v);
		bool IsParallel(const VSVector3 & Vector)const;
		/********************************����******************************************/
		//��͵����
		VSREAL SquaredDistance(const VSVector3 &Point)const;
		//�㵽ֱ�߾���
		VSREAL SquaredDistance(const VSLine3 & Line,VSREAL &fLineParameter)const;
		//������߾���
		VSREAL SquaredDistance(const VSRay3 & Ray,VSREAL &fRayParameter)const;
		//����߶ξ���
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fSegmentParameter)const;
		//��������ξ���
		VSREAL SquaredDistance(const VSTriangle3 &Triangle,VSREAL fTriangleParameter[3])const;
		//��;��ξ���
		VSREAL SquaredDistance(const VSRectangle3 &Rectangle,VSREAL fRectangleParameter[2])const;
		//���OBB����
		VSREAL SquaredDistance(const VSOBB3 &OBB,VSREAL fOBBParameter[3])const;
		//�����ľ���
		VSREAL Distance(const VSSphere3& Sphere,VSVector3 & SpherePoint)const;
		//���ƽ�����
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 &PlanePoint)const;
		//���AABB����
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL fAABBParameter[3])const;
		//��Ͷ���ξ���
		VSREAL SquaredDistance(const VSPolygon3 & Polygon , int& IndexTriangle,VSREAL fTriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//�����λ�ù�ϵ
		//VSIN VSOUT VSON	
		int RelationWith(const VSSphere3 & Sphere)const;
		//���ƽ��λ�ù�ϵ
		//VSFRONT VSBACK VSON
		int RelationWith(const VSPlane3 &Plane)const;
		//���OBBλ�ù�ϵ
		//VSIN VSOUT VSON
		int RelationWith(const VSOBB3 &OBB)const; 
		//���AABBλ�ù�ϵ
		//VSIN VSOUT VSON
		int RelationWith(const VSAABB3 &AABB)const; 
		const static VSVector3 ms_Up;
		const static VSVector3 ms_Right;
		const static VSVector3 ms_Front;
		const static VSVector3 ms_Zero;
		const static VSVector3 ms_One;
	};  // class
	#include "VSVector3.inl"
}
#endif