#ifndef VSCUBETEXTURE_H
#define VSCUBETEXTURE_H
#include "VSTexture.h"
#include "VS2DTexture.h"
namespace VSEngine2
{
	class VSRenderTarget;
	class VSGRAPHIC_API VSCubeTexture : public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//FACE
		{
			F_RIGHT,
			F_LEFT,	
			F_TOP,
			F_BOTTOM,
			F_FRONT,
			F_BACK,				
			F_MAX
		};
		VSCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,
					unsigned int uiMipLevel,bool bIsStatic);
		VSCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,unsigned int uiMipLevel = 1);
		VSCubeTexture(VS2DTexture * pTexture[VSCubeTexture::F_MAX]);
		virtual ~VSCubeTexture();

		virtual unsigned int GetTexType()const{return TT_CUBE;}

		unsigned char * GetFaceBuffer(unsigned int uiLevel,unsigned int uiFace)const;
		
		virtual unsigned int GetByteSize(unsigned int uiLevel)const;
		FORCEINLINE unsigned int GetFaceByteSize(unsigned int uiLevel)const
		{
			if (uiLevel > m_uiMipLevel)
			{
				return 0;
			}
			if (IsCompress())
			{
				return VSRenderer::GetBytesPerPixel(m_uiFormatType) 
					* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel)) 
					* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel)) 
					* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetLength(uiLevel));
			}
			return VSRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel) * GetHeight(uiLevel);
		}
		virtual void SetMipLevel();
		FORCEINLINE bool IsRenderTarget()const
		{
			return m_bRenderTarget;
		}
		
		bool m_bRtUsed;
		friend class VSRenderTarget;
		class VSRenderTarget *GetCurRenderTarget(unsigned int uiFace)const
		{
			return m_pCurRenderTarget[uiFace];
		}
		
	protected:
		VSCubeTexture();
		bool m_bRenderTarget;
		class VSRenderTarget * m_pCurRenderTarget[F_MAX];
		
	};
	DECLARE_Ptr(VSCubeTexture);
	VSTYPE_MARCO(VSCubeTexture);
}
#endif