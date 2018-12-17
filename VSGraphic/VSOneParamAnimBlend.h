#ifndef VSONEPARAMANIMBLEND_H
#define VSONEPARAMANIMBLEND_H
#include "VSAnimBlendFunction.h"
namespace VSEngine2
{
	/*
		CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
							 0		1		2


	*/
	class VSStream;
	class VSGRAPHIC_API VSOneParamAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

	public:
		virtual ~VSOneParamAnimBlend();

		VSOneParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();
	public:
		VSREAL m_fParam;
		VSREAL m_fParamMax;
		VSREAL m_fParamMin;
		
		VSOneParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual void SetPara(void * pPara)
		{
			m_fParam = *((VSREAL *)pPara);
		}
	};
	DECLARE_Ptr(VSOneParamAnimBlend);
	VSTYPE_MARCO(VSOneParamAnimBlend);
}
#endif
