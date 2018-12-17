#ifndef VSSTRAUSSSHADERFUNCTION_H
#define VSSTRAUSSSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	//smoothness and metalness , awide range of materials
	class VSGRAPHIC_API VSStraussShaderFunction : public VSShaderMainFunction
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
			IN_SMOOTHNESS,
			IN_METALNESS,
			IN_TRANSPARENCY,
			IN_K,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		VSStraussShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSStraussShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_STRAUSS;
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
		bool UseLookUpTable()const
		{
			return true;
		}
	protected:
		VSStraussShaderFunction();
		DECLARE_INITIAL

	};
	DECLARE_Ptr(VSStraussShaderFunction);
	VSTYPE_MARCO(VSStraussShaderFunction);
}
#endif