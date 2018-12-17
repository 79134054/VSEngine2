#include "VSCubeTexture.h"
#include "VSGraphicInclude.h"
#include "VSRenderTarget.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCubeTexture,VSTexture)
BEGIN_ADD_PROPERTY(VSCubeTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCubeTexture)
IMPLEMENT_INITIAL_END
VSCubeTexture::VSCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,
							 unsigned int uiMipLevel,bool bIsStatic)
						 :VSTexture(uiFormatType,uiWidth,uiWidth,1,uiMipLevel,bIsStatic)
{
	
	SetMipLevel();
	m_bRenderTarget = false;
	m_bRtUsed = false;
	for (unsigned int i = 0; i < F_MAX; i++)
	{
		m_pCurRenderTarget[i] = NULL;
	}
}
VSCubeTexture::VSCubeTexture(VS2DTexture * pTexture[VSCubeTexture::F_MAX])
{
	m_bRenderTarget = false;
	bool bFind = false;
	unsigned int uiWidth = 0;
	unsigned int uiHeight = 0;
	unsigned int uiFormat = 0;
	unsigned int uiMipLevel = 0;
	bool bIsRenderTarge = false;
	bool bStatic = true;
	for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ;i++)
	{
		if (pTexture[i])
		{
			if (!bFind)
			{
				uiWidth = pTexture[i]->GetWidth(0);
				uiHeight = pTexture[i]->GetHeight(0);
				VSMAC_ASSERT(uiWidth == uiHeight);
				uiFormat = pTexture[i]->GetFormatType();
				bIsRenderTarge = pTexture[i]->IsRenderTarget();
				uiMipLevel = pTexture[i]->GetMipLevel();
				bStatic = pTexture[i]->IsStatic();
				bFind = true;

				
			}
			else
			{
				if (uiWidth != pTexture[i]->GetWidth(0) 
					|| uiFormat != pTexture[i]->GetFormatType() 
					|| bIsRenderTarge != pTexture[i]->IsRenderTarget()
					|| uiMipLevel != pTexture[i]->GetMipLevel()
					|| bStatic != pTexture[i]->IsStatic())
				{
					VSMAC_ASSERT(0);
				}
			}
					
		}
	}

	

	VSMAC_ASSERT(uiWidth);
	VSMAC_ASSERT(uiFormat < VSRenderer::SFT_MAX);
	m_uiFormatType = uiFormat;
	m_uiHeight = uiWidth;
	m_uiWidth = uiWidth;
	m_uiLenght = 1;


	m_bIsStatic = true;
	m_uiMipLevel = uiMipLevel;

	if (!bIsRenderTarge)
	{		
		SetMipLevel();

		CreateRAMDate();
		for (unsigned int i = 0 ; i < F_MAX ;i++)
		{
			if (pTexture[i])
			{
				for (unsigned int j = 0 ; j < m_uiMipLevel ; j++)
				{
					VSMemcpy(GetFaceBuffer(j,i),pTexture[i]->GetBuffer(j),pTexture[i]->GetByteSize(j));
				}
				
			}
			else
			{
				for (unsigned int j = 0 ; j < m_uiMipLevel ; j++)
				{
					VSMemset(GetFaceBuffer(j,i),0,GetFaceByteSize(j));
				}
			}
		}
		
		
	}
	else
	{
		VSMAC_ASSERT(0);
	}
	m_bRtUsed = false;
	for (unsigned int i = 0; i < F_MAX; i++)
	{
		m_pCurRenderTarget[i] = NULL;
	}
}
VSCubeTexture::VSCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,unsigned int uiMipLevel)
:VSTexture(uiFormatType,uiWidth,uiWidth,1,uiMipLevel,true)
{

	m_bRenderTarget = true;
	m_bRtUsed = false;
	for (unsigned int i = 0; i < F_MAX; i++)
	{
		m_pCurRenderTarget[i] = NULL;
	}
}
VSCubeTexture::~VSCubeTexture()
{

}
VSCubeTexture::VSCubeTexture()
{

}
unsigned int VSCubeTexture::GetByteSize(unsigned int uiLevel)const
{
	return GetFaceByteSize(uiLevel) * F_MAX;
}
unsigned char * VSCubeTexture::GetFaceBuffer(unsigned int uiLevel,unsigned int uiFace)const
{
	if(uiLevel < m_uiMipLevel && uiFace < F_MAX)
	{
		if (IsCompress())
		{
			return GetBuffer(uiLevel) + VSRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel)) 
					* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel)) 
					* uiFace * VSRenderer::GetBytesPerPixel(m_uiFormatType);
		}
		else
		{
			return GetBuffer(uiLevel) + GetWidth(uiLevel) * GetHeight(uiLevel) * uiFace * VSRenderer::GetBytesPerPixel(m_uiFormatType);
		}
		
	}
	else
		return NULL;
}
void VSCubeTexture::SetMipLevel()
{
	VSMAC_ASSERT(m_uiWidth && m_uiHeight);
	VSMAC_ASSERT(IsTwoPower(m_uiWidth));
	unsigned int uiWidthLevel;
	unsigned int uiWidth = m_uiWidth;
	_asm bsr eax, uiWidth
	_asm mov uiWidthLevel, eax


	unsigned int uiCurMipLevel = uiWidthLevel + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}
	
}