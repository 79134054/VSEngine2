#ifndef VSISOTROPICWARDSHADERFUNCTION_H
#define VSISOTROPICWARDSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for plastics
	class VSGRAPHIC_API VSIsotropicWardShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_SPECULAR_COLOR,
			IN_ROUGHNESS,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};

		VSIsotropicWardShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSIsotropicWardShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_ISOTROPICWARD;
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
		bool UseLookUpTable()const
		{
			return true;
		}
		virtual void SetGlobleValue(unsigned int uiOutPutStringType , VSVShader * pVShader , VSPShader * pPShader);
	protected:
		VSIsotropicWardShaderFunction();

		DECLARE_INITIAL

	};
	DECLARE_Ptr(VSIsotropicWardShaderFunction);
	VSTYPE_MARCO(VSIsotropicWardShaderFunction);
}
#endif