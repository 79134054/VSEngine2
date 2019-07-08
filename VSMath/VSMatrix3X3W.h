#ifndef VSMATRIX3X3W_H
#define VSMATRIX3X3W_H
#include "VSVector3.h"
#include "VSVector3W.h"
#include "VSMatrix3X3.h"
#include "VSFastFunction.h"
//#include "VSPlane3.h"
namespace VSEngine2
{
	//矩阵
	class  VSMATH_API VSMatrix3X3W 
	{
	public:
		union
		{
			VSREAL m[16];
			struct
			{
				VSREAL _00, _01, _02, _03;
				VSREAL _10, _11, _12, _13;
				VSREAL _20, _21, _22, _23;
				VSREAL _30, _31, _32, _33;
			};
			VSREAL M[4][4];
		};

		VSMatrix3X3W(void);
		VSMatrix3X3W(VSREAL m00,VSREAL m01,VSREAL m02,VSREAL m03,
					VSREAL m10,VSREAL m11,VSREAL m12,VSREAL m13,
					VSREAL m20,VSREAL m21,VSREAL m22,VSREAL m23,
					VSREAL m30,VSREAL m31,VSREAL m32,VSREAL m33);

		
		//用3*3矩阵创建
		void CreateFrom3X3(const VSMatrix3X3 & Mat);
		//平移矩阵
		void CreateTranslate(VSREAL dx, VSREAL dy, VSREAL dz);
		void CreateTranslate(const VSVector3 & V);
		//建立变换矩阵
		void CreateInWorldObject(const VSVector3 &U,const VSVector3 &V,const VSVector3 & N,const VSVector3 &Point);
		
		//建立广告牌变换矩阵
		void CreateFormBillboard(const VSVector3 &vcPos,					//广告牌位置
								const VSMatrix3X3 &CameraRotMatrix,		//相机或其他矩阵
								bool bAxisY);						//是否只绕Y轴旋转
		//够建相机矩阵(根据视见方向)
		bool CreateFromLookDir(const VSVector3 &vcPos,					//相机位置
					const VSVector3 &vcDir,								//观察方向
					const VSVector3 &vcWorldUp = VSVector3(0,1,0));
		//够建相机矩阵(根据目标位置)
		bool CreateFromLookAt(const VSVector3 &vcPos,									//相机位置
								const VSVector3 &vcLookAt,							//观察位置
								const VSVector3 &vcWorldUp = VSVector3(0,1,0));		//上方向
		//建立透视投影矩阵
		bool CreatePerspective(VSREAL fFov ,				//X方向张角
								VSREAL fAspect,				//宽高比
								VSREAL fZN ,				//近剪裁面
								VSREAL fZF);				//远剪裁面
		//建立正交投影矩阵
		bool CreateOrthogonal(VSREAL fW ,				//宽
							VSREAL fH,					//高
							VSREAL fZN ,				//近剪裁面
							VSREAL fZF)	;				//远剪裁面
		//建立视口矩阵
		bool CreateViewPort(VSREAL fX,VSREAL fY,VSREAL fWidth,VSREAL fHeight,VSREAL fMinz,VSREAL fMaxz);
		//void CreatePointLightPlaneShadow(const VSVector3 & LightPoint,const VSPlane3 &Plane);
		//void CreateInfiniteLightPlaneShadow(const VSVector3 & LightDir,const VSPlane3 &Plane);
		VSMatrix3X3W operator *(VSREAL f)const;
		VSMatrix3X3W operator +(VSREAL f)const;
		VSMatrix3X3W operator -(VSREAL f)const;
		VSMatrix3X3W operator +(const VSMatrix3X3W &Matirx)const;
		VSMatrix3X3W operator -(const VSMatrix3X3W &Matirx)const;

		void operator *= (VSREAL f);
		void operator += (VSREAL f);
		void operator -= (VSREAL f);
		void operator += (const VSMatrix3X3W &Matirx);
		void operator -= (const VSMatrix3X3W &Matirx);

		bool operator ==(const VSMatrix3X3W &v)const;

		//加入把3*3矩阵 旋转或者缩放
		void Add3X3(const VSMatrix3X3 & Mat);
		//添加平移
		void AddTranslate(const VSVector3 & V);
		void AddTranslate(VSREAL dx, VSREAL dy, VSREAL dz);
		/*********************************** FORCEINLINE *************************************/

		FORCEINLINE void Identity(void);											//单位矩阵
		FORCEINLINE void SetZero();
		FORCEINLINE void TransposeOf(const VSMatrix3X3W &Matrix);				//转置
		FORCEINLINE void InverseOf(const VSMatrix3X3W & Mat);					//求逆

		FORCEINLINE VSMatrix3X3W GetTranspose()const;										//转置
		FORCEINLINE VSMatrix3X3W GetInverse()const;											//求逆

		FORCEINLINE VSVector3 GetTranslation(void)const;							//得到平移量
		FORCEINLINE void Get3X3(VSMatrix3X3 & Mat)const;							//得到3*3部分
		FORCEINLINE VSMatrix3X3W operator * (const VSMatrix3X3W &Matirx)const;    // 矩阵相乘
		FORCEINLINE VSVector3 operator * (const VSVector3 &vc)const;				// 矩阵和向量乘
		FORCEINLINE VSVector3W operator * (const VSVector3W &vc)const;			// 矩阵和向量乘


		//应用3X3部分
		FORCEINLINE VSVector3 Apply3X3(const VSVector3 &v)const;
		//应用平移
		FORCEINLINE VSVector3 ApplyTranlaste(const VSVector3 &Point)const;

		//按行获得向量
		void GetRowVector(VSVector3W Row[4])const;
		//按行列得向量
		void GetColumnVector(VSVector3W Column[4])const;

		void GetRowVector(VSVector3W &Row0,VSVector3W &Row1,VSVector3W &Row2,VSVector3W &Row3)const;
		void GetColumnVector(VSVector3W &Column0,VSVector3W &Column1,VSVector3W &Column2,VSVector3W &Column3)const;
		static VSMatrix3X3W ms_Materix3X3WIdentity;
	}; // class

#include "VSMatrix3X3W.inl"
};
#endif