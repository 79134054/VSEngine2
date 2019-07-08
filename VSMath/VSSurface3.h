#ifndef VSSURFACE3_H
#define VSSURFACE3_H
#include "VSVector3.h"
/*
曲面类
*/
namespace VSEngine2
{
	class VSMATH_API VSSurface3 : public VSMemObject
	{
	public:
		VSSurface3();
		virtual ~VSSurface3() = 0;
		//求点
		virtual VSVector3 GetPoint(VSREAL U,VSREAL V) = 0;
		//求偏导
		virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V) = 0;
		virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V) = 0;
		//得到法向量
		FORCEINLINE VSVector3 GetN(VSREAL U,VSREAL V);
	};

#include "VSSurface3.inl"
}
#endif