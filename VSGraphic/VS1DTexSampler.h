#ifndef VS1DTEXSAMPLER_H
#define VS1DTEXSAMPLER_H
#include "VSTexSampler.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VS1DTexSampler : public VSTexSampler
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		VS1DTexSampler(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VS1DTexSampler();
		virtual void SetTexture(VSTexAllStateR * pTexAllState);


		virtual unsigned int GetTexType()const
		{
			return VSTexture::TT_1D;
		}

		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VS1DTexSampler();

	public:
		enum
		{
			IN_TEXCOORD,
			IN_MAX
		};
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VS1DTexSampler);
	VSTYPE_MARCO(VS1DTexSampler);
}
#endif