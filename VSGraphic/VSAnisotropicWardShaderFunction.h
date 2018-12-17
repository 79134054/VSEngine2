#ifndef VSANISOTROPICWARDSHADERFUNCTION_H
#define VSANISOTROPICWARDSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for plastics
	class VSGRAPHIC_API VSAnisotropicWardShaderFunction : public VSShaderMainFunction
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
			IN_ROUGHNESSX,
			IN_ROUGHNESSY,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};

		VSAnisotropicWardShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSAnisotropicWardShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_ANISOTROPICWARD;
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
	protected:
		VSAnisotropicWardShaderFunction();

		DECLARE_INITIAL

	};
	DECLARE_Ptr(VSAnisotropicWardShaderFunction);
	VSTYPE_MARCO(VSAnisotropicWardShaderFunction);
}
#endif