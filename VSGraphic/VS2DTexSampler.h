#ifndef VS2DTEXSAMPLER_H
#define VS2DTEXSAMPLER_H
#include "VSTexSampler.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VS2DTexSampler : public VSTexSampler
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VS2DTexSampler(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VS2DTexSampler();
		virtual void SetTexture(VSTexAllStateR * pTexAllState);
		
		
		virtual unsigned int GetTexType()const
		{
			return VSTexture::TT_2D;
		}

		virtual bool GetFuntionString(VSString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(VSString &OutString)const;
	protected:
		VS2DTexSampler();
		
	public:
		enum
		{
			IN_TEXCOORD,
			IN_MAX
		};
		enum
		{
			OUT_COLOR,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_COLOR_A,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VS2DTexSampler);
	VSTYPE_MARCO(VS2DTexSampler);
}
#endif