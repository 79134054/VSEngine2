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
	//向量
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
		FORCEINLINE VSREAL GetLength(void)const;                   // 长度
		FORCEINLINE VSREAL GetSqrLength(void) const;          // 长度平方
		FORCEINLINE void  Negate(void);                      // 乘以-1
		FORCEINLINE void  Normalize(void);                   // 单位化
		FORCEINLINE void Cross(const VSVector3 &v1,          //叉积
						const VSVector3 &v2); 

		VSREAL AngleWith( VSVector3 &v);          // 两个向量的夹角(弧度)
		void  Create(const VSVector3 &v1,			// 构造向量从点v1到v2
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

		
		VSREAL Dot(const VSVector3 &v)const;//点积
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
		/********************************距离******************************************/
		//点和点距离
		VSREAL SquaredDistance(const VSVector3 &Point)const;
		//点到直线距离
		VSREAL SquaredDistance(const VSLine3 & Line,VSREAL &fLineParameter)const;
		//点和射线距离
		VSREAL SquaredDistance(const VSRay3 & Ray,VSREAL &fRayParameter)const;
		//点和线段距离
		VSREAL SquaredDistance(const VSSegment3 & Segment,VSREAL &fSegmentParameter)const;
		//点和三角形距离
		VSREAL SquaredDistance(const VSTriangle3 &Triangle,VSREAL fTriangleParameter[3])const;
		//点和矩形距离
		VSREAL SquaredDistance(const VSRectangle3 &Rectangle,VSREAL fRectangleParameter[2])const;
		//点和OBB距离
		VSREAL SquaredDistance(const VSOBB3 &OBB,VSREAL fOBBParameter[3])const;
		//点和球的距离
		VSREAL Distance(const VSSphere3& Sphere,VSVector3 & SpherePoint)const;
		//点和平面距离
		VSREAL Distance(const VSPlane3 & Plane,VSVector3 &PlanePoint)const;
		//点和AABB距离
		VSREAL SquaredDistance(const VSAABB3 &AABB,VSREAL fAABBParameter[3])const;
		//点和多边形距离
		VSREAL SquaredDistance(const VSPolygon3 & Polygon , int& IndexTriangle,VSREAL fTriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//点和球位置关系
		//VSIN VSOUT VSON	
		int RelationWith(const VSSphere3 & Sphere)const;
		//点和平面位置关系
		//VSFRONT VSBACK VSON
		int RelationWith(const VSPlane3 &Plane)const;
		//点和OBB位置关系
		//VSIN VSOUT VSON
		int RelationWith(const VSOBB3 &OBB)const; 
		//点和AABB位置关系
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