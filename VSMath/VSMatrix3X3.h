#ifndef VSMATRIX3X3_H
#define VSMATRIX3X3_H
#include "VSVector3.h"
#include "VSQuat.h"
namespace VSEngine2
{
	//矩阵
	class  VSMATH_API VSMatrix3X3 
	{
	public:
		union
		{
			VSREAL m[9];
			struct
			{
				VSREAL _00, _01, _02;
				VSREAL _10, _11, _12;
				VSREAL _20, _21, _22;
			};
			VSREAL M[3][3];
		};

		VSMatrix3X3(void);
		VSMatrix3X3(VSREAL m00,VSREAL m01,VSREAL m02,
					VSREAL m10,VSREAL m11,VSREAL m12,
					VSREAL m20,VSREAL m21,VSREAL m22);
		

		void CreateFromDirection(VSVector3 & Direction , const VSVector3 &Up = VSVector3(0,1,0));
		void CreateRotX(VSREAL a);								// 绕x旋转
		void CreateRotY(VSREAL a);								// 绕y旋转
		void CreateRotZ(VSREAL a);								// 绕z旋转
		void CreateEluer(VSREAL Roll,VSREAL Pitch, VSREAL Yaw);		// 绕z x and y构建，欧拉角  -pi <= y z <= pi  -pi/2 < x < pi/2
		void CreateAxisAngle(const VSVector3 &vAxis, VSREAL a);	//绕vAxis旋转a弧度	
		//创建旋转矩阵
		void CreateRot(const VSVector3 &U,const VSVector3 &V,const VSVector3 & N);
		//创建缩放矩阵
		void CreateScale(VSREAL fX,VSREAL fY,VSREAL fZ);

		void CreateScale(const VSVector3 & Axis,VSREAL fScale);
		//得到eluer角度
		void GetEluer(VSREAL &Yaw,VSREAL &Pitch,VSREAL &Roll)const;
		//得到旋转轴和旋转角
		void GetAxisAngle(VSVector3 & Axis,VSREAL & fAngle)const;
		//得到四元数
		VSQuat GetQuat()const;
		//线性插值
		void LineInterpolation(VSREAL t,const VSMatrix3X3 & M1, const VSMatrix3X3 &M2);
		//球形插值
		void Slerp(VSREAL t,const VSMatrix3X3 & M1, const VSMatrix3X3 &M2);
		VSMatrix3X3 operator *(VSREAL f)const;
		VSMatrix3X3 operator +(VSREAL f)const;
		VSMatrix3X3 operator -(VSREAL f)const;
		VSMatrix3X3 operator +(const VSMatrix3X3 &Matirx)const;
		VSMatrix3X3 operator -(const VSMatrix3X3 &Matirx)const;

		void operator *= (VSREAL f);
		void operator += (VSREAL f);
		void operator -= (VSREAL f);
		void operator += (const VSMatrix3X3 &Matirx);
		void operator -= (const VSMatrix3X3 &Matirx);

		bool operator ==(const VSMatrix3X3 &v)const;
		//按行获得向量
		void GetRowVector(VSVector3 Row[3])const;
		//按行列得向量
		void GetColumnVector(VSVector3 Column[3])const;

		void GetRowVector(VSVector3 &Row0,VSVector3 &Row1,VSVector3 &Row2)const;
		void GetColumnVector(VSVector3 &Column0,VSVector3 &Column1,VSVector3 &Column2)const;
		//按UVN获得
		void GetUVN(VSVector3 UVN[3])const;
		void GetUVN(VSVector3 & U,VSVector3 &V,VSVector3 &N)const;
		//求特征值，特征向量
		void GetEigenSystem(VSREAL EigenValue[3],VSVector3 Eigen[3])const;
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Identity(void);											//单位矩阵
		FORCEINLINE void TransposeOf(const VSMatrix3X3 &Matirx);					//转置
		FORCEINLINE void InverseOf(const VSMatrix3X3 &Matirx);					//求逆
		FORCEINLINE VSREAL Det()const;											//求判别式
		FORCEINLINE VSMatrix3X3 operator * (const VSMatrix3X3 &Matirx)const;		// 矩阵相乘
		FORCEINLINE VSVector3 operator * (const VSVector3 &vc)const;				// 矩阵和向量乘

		//一个行向量 一个列向量构造
		FORCEINLINE void CreateFromTwoVector(const VSVector3 & v1,const VSVector3 & v2);
		FORCEINLINE void SetZero();
		void GetScale(VSVector3 & Scale)const;
		void GetScaleAndRotated(VSVector3 & Scale);
		const static VSMatrix3X3 ms_CameraViewRight;
		const static VSMatrix3X3 ms_CameraViewLeft;
		const static VSMatrix3X3 ms_CameraViewUp;
		const static VSMatrix3X3 ms_CameraViewDown;
		const static VSMatrix3X3 ms_CameraViewFront;
		const static VSMatrix3X3 ms_CameraViewBack;
		const static VSMatrix3X3 ms_Identity;

	}; // class

#include "VSMatrix3X3.inl"
};
#endif