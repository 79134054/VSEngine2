#ifndef VSMATRIX3X3_H
#define VSMATRIX3X3_H
#include "VSVector3.h"
#include "VSQuat.h"
namespace VSEngine2
{
	//����
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
		void CreateRotX(VSREAL a);								// ��x��ת
		void CreateRotY(VSREAL a);								// ��y��ת
		void CreateRotZ(VSREAL a);								// ��z��ת
		void CreateEluer(VSREAL Roll,VSREAL Pitch, VSREAL Yaw);		// ��z x and y������ŷ����  -pi <= y z <= pi  -pi/2 < x < pi/2
		void CreateAxisAngle(const VSVector3 &vAxis, VSREAL a);	//��vAxis��תa����	
		//������ת����
		void CreateRot(const VSVector3 &U,const VSVector3 &V,const VSVector3 & N);
		//�������ž���
		void CreateScale(VSREAL fX,VSREAL fY,VSREAL fZ);

		void CreateScale(const VSVector3 & Axis,VSREAL fScale);
		//�õ�eluer�Ƕ�
		void GetEluer(VSREAL &Yaw,VSREAL &Pitch,VSREAL &Roll)const;
		//�õ���ת�����ת��
		void GetAxisAngle(VSVector3 & Axis,VSREAL & fAngle)const;
		//�õ���Ԫ��
		VSQuat GetQuat()const;
		//���Բ�ֵ
		void LineInterpolation(VSREAL t,const VSMatrix3X3 & M1, const VSMatrix3X3 &M2);
		//���β�ֵ
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
		//���л������
		void GetRowVector(VSVector3 Row[3])const;
		//�����е�����
		void GetColumnVector(VSVector3 Column[3])const;

		void GetRowVector(VSVector3 &Row0,VSVector3 &Row1,VSVector3 &Row2)const;
		void GetColumnVector(VSVector3 &Column0,VSVector3 &Column1,VSVector3 &Column2)const;
		//��UVN���
		void GetUVN(VSVector3 UVN[3])const;
		void GetUVN(VSVector3 & U,VSVector3 &V,VSVector3 &N)const;
		//������ֵ����������
		void GetEigenSystem(VSREAL EigenValue[3],VSVector3 Eigen[3])const;
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Identity(void);											//��λ����
		FORCEINLINE void TransposeOf(const VSMatrix3X3 &Matirx);					//ת��
		FORCEINLINE void InverseOf(const VSMatrix3X3 &Matirx);					//����
		FORCEINLINE VSREAL Det()const;											//���б�ʽ
		FORCEINLINE VSMatrix3X3 operator * (const VSMatrix3X3 &Matirx)const;		// �������
		FORCEINLINE VSVector3 operator * (const VSVector3 &vc)const;				// �����������

		//һ�������� һ������������
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