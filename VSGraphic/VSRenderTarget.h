#ifndef VSRENDERTARGET_H
#define VSRENDERTARGET_H
#include "VSBind.h"
#include "VSTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSRenderTarget : public VSBind
	{

		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
	public:
		friend class VS2DTexture;
		friend class VSCubeTexture;
		friend class VSResourceManager;
		
		virtual ~VSRenderTarget();
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{

			return m_uiHeight;
		}
		FORCEINLINE unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		FORCEINLINE unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		FORCEINLINE unsigned int GetLevel()const
		{
			return m_uiLevel;
		}
		FORCEINLINE unsigned int GetParam()const
		{
			return m_uiParam;
		}
		FORCEINLINE VSTexture * GetCreateBy()const
		{
			return m_pCreateBy;
		}
		bool m_bUsed;
	protected:
		VSRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMulSample);
		VSRenderTarget(VSTexture * pCreateBy,unsigned int uiMulSample = VSRenderer::MS_NONE
			,unsigned int uiLevel = 0,unsigned int Param = 0);
		VSRenderTarget();
		VSTexturePtr m_pCreateBy; 
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiFormatType;
		unsigned int m_uiMulSample;
		unsigned int m_uiLevel;
		unsigned int m_uiParam;
		DECLARE_INITIAL
	};
	DECLARE_Ptr(VSRenderTarget);
	VSTYPE_MARCO(VSRenderTarget);
}
#endif