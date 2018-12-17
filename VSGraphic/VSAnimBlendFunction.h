#ifndef VSANIMBLENDFUNCTION_H
#define VSANIMBLENDFUNCTION_H
#include "VSAnimFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSAnimBlendFunction : public VSAnimFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY

	public:
		
		virtual ~VSAnimBlendFunction() = 0;
		virtual bool Update(double dAppTime);

		VSAnimBlendFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		static void LineBlendTwo(VSAnimFunction * pOut,
						VSAnimFunction * pAnimFunction1,VSAnimFunction * pAnimFunction2,VSREAL fWeight);


		static void LineBlendTwo(VSArray<VSAnimAtom> *pOut,
			const VSArray<VSAnimAtom> * pIn1,const VSArray<VSAnimAtom> *pIn2,VSREAL fWeight);


		static void LineBlendTwo(VSArray<VSAnimAtom> *pOut,
			VSAnimFunction * pAnimFunction1,VSAnimFunction * pAnimFunction2,VSREAL fWeight);


		static void LineBlendTwo(VSAnimAtom &pOut,
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight);
		static void AdditiveBlend(VSAnimFunction * pOut, VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2);
	protected:
		VSAnimBlendFunction();
		virtual bool ComputePara(double dAppTime) = 0;
		virtual bool ComputeOutBoneMatrix(double dAppTime) = 0;

		

	};
	DECLARE_Ptr(VSAnimBlendFunction);
	VSTYPE_MARCO(VSAnimBlendFunction);

}
#endif