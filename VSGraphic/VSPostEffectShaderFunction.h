#ifndef VSPOSTSHADERSHADERFUNCTION_H
#define VSPOSTSHADERSHADERFUNCTION_H
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSPostEffectShaderFunction : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		enum
		{
			IN_COLOR,
			IN_MAX
		};
		VSPostEffectShaderFunction(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSPostEffectShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName(){};
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction);

	protected:
		VSPostEffectShaderFunction();
	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};

	};
	DECLARE_Ptr(VSPostEffectShaderFunction);
	VSTYPE_MARCO(VSPostEffectShaderFunction);
}
#endif