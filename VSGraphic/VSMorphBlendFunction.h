#ifndef VSMORPHBLENDFUNCTION_H
#define VSMORPHBLENDFUNCTION_H
#include "VSMorphFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSMorphBlendFunction : public VSMorphFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY

	public:
		VSMorphBlendFunction(const VSUsedName & ShowName, VSMorphTree * pMorphTree);
		virtual ~VSMorphBlendFunction() = 0;
		virtual bool Update(double dAppTime);
		static void LineBlendTwo(VSMorphFunction * pOut,
			VSMorphFunction * pMorphFunction1, VSMorphFunction * pMorphFunction2, VSREAL fWeight);
	protected:
		VSMorphBlendFunction();
		virtual bool ComputePara(double dAppTime) = 0;




	};
	DECLARE_Ptr(VSMorphBlendFunction);
	VSTYPE_MARCO(VSMorphBlendFunction);

}
#endif