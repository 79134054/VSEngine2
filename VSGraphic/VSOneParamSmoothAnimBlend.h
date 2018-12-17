#ifndef VSONEPARAMSMOOTHANIMBLEND_H
#define VSONEPARAMSMOOTHANIMBLEND_H
#include "VSOneParamAnimBlend.h"
namespace VSEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
	class VSStream;
	class VSGRAPHIC_API VSOneParamSmoothAnimBlend : public VSOneParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSOneParamSmoothAnimBlend();

		VSOneParamSmoothAnimBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

	public:

		VSOneParamSmoothAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	};
	DECLARE_Ptr(VSOneParamSmoothAnimBlend);
	VSTYPE_MARCO(VSOneParamSmoothAnimBlend);
}
#endif