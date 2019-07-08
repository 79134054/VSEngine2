#ifndef VSCOOKTORRANCESHADERFUNCTION_H
#define VSCOOKTORRANCESHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for rough surfaces , no specular
	class VSGRAPHIC_API VSCookTorranceShaderFunction : public VSShaderMainFunction
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
			IN_C, //use in alternate blinn
			IN_R,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		enum //Specluar type
		{
			ST_BACKMANN_LOOKUP,
			ST_BACKMANN,
			ST_ALTERNATE_BLINN,
			ST_MAX

		};
		VSCookTorranceShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSCookTorranceShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_COOKTOORANCE;
		}
		FORCEINLINE void SetSpecularType(unsigned int uiSpecularType)
		{
			m_uiSpecularType = uiSpecularType;
		}
		FORCEINLINE unsigned int GetSpecularType()const
		{
			return m_uiSpecularType;
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
		VSCookTorranceShaderFunction();
		DECLARE_INITIAL
		unsigned int m_uiSpecularType;

	};
	DECLARE_Ptr(VSCookTorranceShaderFunction);
	VSTYPE_MARCO(VSCookTorranceShaderFunction);
}
#endif