#ifndef VSCUSTOMSHADERFUNCTION_H
#define VSCUSTOMSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for plastics
	class VSGRAPHIC_API VSCustomShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_EMISSIVE_COLOR,
			IN_NORMAL,
			IN_ALPHA,
			IN_CUSTOM,
			IN_DIFFUSE_COLOR,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		VSCustomShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSCustomShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual bool GetShaderTreeString(VSString &OutString,unsigned int uiOutPutStringType);
		virtual bool GetInputValueString(VSString &OutString,unsigned int uiOutPutStringType)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_CUSTOM;
		}
		virtual VSInputNode * GetDiffuseNode()const
		{
			return m_pInput[IN_DIFFUSE_COLOR];
		}
		virtual VSInputNode * GetNormalNode()const
		{
			return m_pInput[IN_NORMAL];
		}
		virtual VSInputNode * GetAlphaNode()const
		{
			return m_pInput[IN_ALPHA];
		}
		virtual VSInputNode * GetEmissiveNode()const
		{
			return m_pInput[IN_EMISSIVE_COLOR];
		}
		virtual VSInputNode * GetReflectMipNode()const
		{
			return m_pInput[IN_REFLECT_MIP];
		}
		virtual VSInputNode * GetReflectPowNode()const
		{
			return m_pInput[IN_REFLECT_POW];
		}
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction);

		virtual void CreatLightFunctionString(unsigned int uiOutPutStringType);

		VSString m_LightFunctionString;
		VSString m_CustomContentString;
		VSString m_CustomDefine;
	protected:
		VSCustomShaderFunction();

		DECLARE_INITIAL

	};
	DECLARE_Ptr(VSCustomShaderFunction);
	VSTYPE_MARCO(VSCustomShaderFunction);
}
#endif