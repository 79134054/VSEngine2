#ifndef VSSAMPLERSTATE_H
#define VSSAMPLERSTATE_H
#include "VSBind.h"
#include "VSVector3W.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSSamplerDesc : public VSObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Coordinate Mode
		{
			CM_CLAMP,
			CM_WRAP,
			CM_MIRROR,					
			CM_BORDER,
			CM_MAX


		};
		enum //Filter Method
		{
			FM_NONE,
			FM_POINT,
			FM_LINE,
			FM_ANISOTROPIC,
			FM_MAX

		};
		VSSamplerDesc()
		{
			m_uiMag = FM_POINT;
			m_uiMin = FM_POINT;
			m_uiMip = FM_NONE;
			m_uiMipLevel = 0;
			m_uiAniLevel = 1;

			m_uiCoordU = CM_WRAP;
			m_uiCoordV = CM_WRAP;
			m_uiCoordW = CM_WRAP;
			m_BorderColor = VSColorRGBA(0.0f,0.0f,0.0f,0.0f);
			m_MipLODBias = 0.0f;
		}
		~VSSamplerDesc()
		{

		}

		

		unsigned char m_uiMag;
		unsigned char m_uiMin;
		unsigned char m_uiMip;
		unsigned char m_uiMipLevel;
		unsigned char m_uiAniLevel;

		unsigned char m_uiCoordU;
		unsigned char m_uiCoordV;
		unsigned char m_uiCoordW;
		VSREAL        m_MipLODBias;
		VSColorRGBA  m_BorderColor;
		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(VSSamplerDesc)-sizeof(VSObject);
			return (void *)&m_uiMag;
		}
	};
	VSTYPE_MARCO(VSSamplerDesc);
	class VSGRAPHIC_API VSSamplerState : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~VSSamplerState();
	protected:
		VSSamplerState();
		VSSamplerDesc m_SamplerDesc;
	protected:
		static VSPointer<VSSamplerState> Default;
		static VSPointer<VSSamplerState> TriLine;
		static VSPointer<VSSamplerState> DoubleLine;
		static VSPointer<VSSamplerState> ShadowMapSampler;
		static VSPointer<VSSamplerState> BorderARGB0Sampler;
		static VSPointer<VSSamplerState> LightFunctionSampler;
	public:
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		const VSSamplerDesc & GetSamplerDesc()const
		{

			return m_SamplerDesc;
		}
		static const VSSamplerState *GetDefault()
		{
			return Default;
		}
		static const VSSamplerState *GetTriLine()
		{
			return TriLine;
		}
		static const VSSamplerState *GetDoubleLine()
		{
			return DoubleLine;
		}
		static const VSSamplerState * GetShadowMapSampler()
		{
			return ShadowMapSampler;
		}
		static const VSSamplerState * GetBorderARGB0Sampler()
		{
			return BorderARGB0Sampler;
		}
		static const VSSamplerState * GetLightFunctionSampler()
		{
			return LightFunctionSampler;
		}
		friend class VSResourceManager;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
	};
	DECLARE_Ptr(VSSamplerState);
	VSTYPE_MARCO(VSSamplerState);
};
#endif