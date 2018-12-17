#ifndef VSSHIRLEYSHADERFUNCTION_H
#define VSSHIRLEYSHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	//good for metal and plastic materials
	class VSGRAPHIC_API VSShirleyShaderFunction : public VSShaderMainFunction
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
			IN_NU,
			IN_NV,
			IN_RS,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		VSShirleyShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSShirleyShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_SHIRLEY;
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
		VSShirleyShaderFunction();
		DECLARE_INITIAL

	};
	DECLARE_Ptr(VSShirleyShaderFunction);
	VSTYPE_MARCO(VSShirleyShaderFunction);
}
#endif