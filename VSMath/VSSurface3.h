#ifndef VSSURFACE3_H
#define VSSURFACE3_H
#include "VSVector3.h"
/*
������
*/
namespace VSEngine2
{
	class VSMATH_API VSSurface3 : public VSMemObject
	{
	public:
		VSSurface3();
		virtual ~VSSurface3() = 0;
		//���
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V) = 0;
		//��ƫ��
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V) = 0;
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V) = 0;
		//�õ�������
		FORCEINLINE VSVector3 GetN(VSREAL U,VSREAL V);
	};

#include "VSSurface3.inl"
}
#endif