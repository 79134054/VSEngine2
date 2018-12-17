#ifndef VSMINNAERTSHADERFUNCTION_H
#define VSMINNAERTSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	//»ﬁ±Ì√Ê no specular
	class VSGRAPHIC_API VSMinnaertShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_POWER,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		VSMinnaertShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSMinnaertShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_MINNAERT;
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
		VSMinnaertShaderFunction();
		DECLARE_INITIAL

	};
	DECLARE_Ptr(VSMinnaertShaderFunction);
	VSTYPE_MARCO(VSMinnaertShaderFunction);
}
#endif