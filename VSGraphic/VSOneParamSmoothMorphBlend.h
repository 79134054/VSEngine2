#ifndef VSONEPARAMSMOOTHMORPHBLEND_H
#define VSONEPARAMSMOOTHMORPHBLEND_H
#include "VSOneParamMorphBlend.h"
namespace VSEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
	class VSStream;
	class VSGRAPHIC_API VSOneParamSmoothMorphBlend : public VSOneParamMorphBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSOneParamSmoothMorphBlend();

		VSOneParamSmoothMorphBlend(const VSUsedName & ShowName, VSMorphTree * pMorphTree);
		virtual void UpdateVertexDate(unsigned int uiVertexIndex);
		virtual bool Update(double dAppTime);
	public:

		VSOneParamSmoothMorphBlend();
	};
	DECLARE_Ptr(VSOneParamSmoothMorphBlend);
	VSTYPE_MARCO(VSOneParamSmoothMorphBlend);
}
#endif