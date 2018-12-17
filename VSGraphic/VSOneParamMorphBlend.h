#ifndef VSONEPARAMMORPHBLEND_H
#define VSONEPARAMMORPHBLEND_H
#include "VSMorphBlendFunction.h"
namespace VSEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
	class VSStream;
	class VSGRAPHIC_API VSOneParamMorphBlend : public VSMorphBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSOneParamMorphBlend();

		VSOneParamMorphBlend(const VSUsedName & ShowName, VSMorphTree * pMorphTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();
	public:
		VSREAL m_fParam;
		VSREAL m_fParamMax;
		VSREAL m_fParamMin;

		VSOneParamMorphBlend();
		virtual bool ComputePara(double dAppTime);
		virtual void SetPara(void * pPara)
		{
			m_fParam = *((VSREAL *)pPara);
		}
	protected:
		VSMorphFunction *m_pMorphBaseFunction1;
		VSMorphFunction *m_pMorphBaseFunction2;
		VSREAL m_fWeight;
	};
	DECLARE_Ptr(VSOneParamMorphBlend);
	VSTYPE_MARCO(VSOneParamMorphBlend);
}
#endif
