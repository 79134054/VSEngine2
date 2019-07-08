#ifndef VSTWOPARAMANIMBLEND_H
#define VSTWOPARAMANIMBLEND_H
#include "VSAnimBlendFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTwoParamAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		virtual ~VSTwoParamAnimBlend();

		VSTwoParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		void CreateSlot(unsigned int uiWidth, unsigned int uiHeight);
		virtual bool SetObject(VSObject * pObject);
	protected:
		VSREAL m_fParam[2];
		VSREAL m_fParamMax[2];
		VSREAL m_fParamMin[2];
		VSAnimAtom m_BlendRootMatrix[2];
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		VSArray<VSAnimAtom> m_BlendBoneMatrix[2];
		VSTwoParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		VSAnimFunction * GetAnimFuntion(unsigned int i, unsigned int j);
	};
	DECLARE_Ptr(VSTwoParamAnimBlend);
	VSTYPE_MARCO(VSTwoParamAnimBlend);
}
#endif