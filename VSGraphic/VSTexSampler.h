#ifndef VSTEXSAMPLER_H
#define VSTEXSAMPLER_H
#include "VSShaderFunction.h"
#include "VSTexAllState.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTexSampler : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:

		virtual ~VSTexSampler() = 0;
		virtual unsigned int GetTexType()const = 0 ;
		VSTexSampler(const VSUsedName & ShowName,VSMaterial * pMaterial);
		FORCEINLINE VSTexAllStateR* GetTexture()const
		{
			return m_pTexAllState;
		}
		FORCEINLINE void SetTexture(VSTexAllStateR* pTexture)
		{
			m_pTexAllState = pTexture;
		}
		virtual bool GetInputValueString(VSString &InputString)const;
		virtual bool GetDeclareString(VSString &OutString,unsigned int uiRegisterID)const;
		virtual void SetTexCoordLevel(unsigned int uiTexCoordLevel)
		{
			m_uiTexCoordLevel = uiTexCoordLevel;
		}
		FORCEINLINE unsigned int GetTexCoordLevel()const
		{
			return m_uiTexCoordLevel;
		}
		FORCEINLINE void SetVEDecode(unsigned char uiVEDecode)
		{
			m_uiVEDecode = uiVEDecode;
		}
		FORCEINLINE void SetVESRGB(unsigned char uiVESRGB )
		{
			m_uiVESRGB = uiVESRGB ;
		}
		
		FORCEINLINE unsigned char GetVEDecode()const
		{
			return m_uiVEDecode;
		}
		FORCEINLINE unsigned char GetVESRGB()const
		{
			return m_uiVESRGB;
		}	
		virtual bool GetOutPutValueString(VSString &OutString)const = 0;
	protected:
		
		VSTexSampler();
	protected:
		VSTexAllStateRPtr m_pTexAllState;
		unsigned char m_uiTexCoordLevel;
		unsigned char m_uiVEDecode;
		unsigned char m_uiVESRGB;

	};
	DECLARE_Ptr(VSTexSampler);
	VSTYPE_MARCO(VSTexSampler);
}
#endif