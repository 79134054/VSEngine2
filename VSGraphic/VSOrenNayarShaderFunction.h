#ifndef VSORENNAYARSHADERFUNCTION_H
#define VSORENNAYARSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for rough surfaces , no specular
	class VSGRAPHIC_API VSOrenNayarShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_ROUGHNESS_SQUARED,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		VSOrenNayarShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSOrenNayarShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_OREN_NAYAR;
		}
		bool UseLookUpTable()const
		{
			return true;
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

		virtual void SetGlobleValue(unsigned int uiOutPutStringType , VSVShader * pVShader , VSPShader * pPShader);
	protected:
		VSOrenNayarShaderFunction();
		DECLARE_INITIAL
	
	};
	DECLARE_Ptr(VSOrenNayarShaderFunction);
	VSTYPE_MARCO(VSOrenNayarShaderFunction);
}
#endif