#ifndef VSPHONESHADERFUNCTION_H
#define VSPHONESHADERFUNCTION_H
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for plastics
	class VSGRAPHIC_API VSPhoneShaderFunction : public VSShaderMainFunction
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
			IN_SPECULAR_POW,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		enum //Specular Type
		{
			ST_BlinnPhong,
			ST_Phong,
			ST_MAX
		};
		VSPhoneShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSPhoneShaderFunction();
		virtual bool GetFuntionString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_PHONE;
		}
		FORCEINLINE void SetSpecularType(unsigned char uiSpecularType)
		{
			m_uiSpecularType = uiSpecularType;
		}
		FORCEINLINE unsigned char GetSpecularType()const
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

	protected:
		VSPhoneShaderFunction();
		unsigned int m_uiSpecularType;
	DECLARE_INITIAL
	
	};
	DECLARE_Ptr(VSPhoneShaderFunction);
	VSTYPE_MARCO(VSPhoneShaderFunction);
}
#endif