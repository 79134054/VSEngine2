#include "VS3DTexture.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VS3DTexture,VSTexture)
BEGIN_ADD_PROPERTY(VS3DTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS3DTexture)
IMPLEMENT_INITIAL_END
VS3DTexture::VS3DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiLength,
						 unsigned int uiFormatType,unsigned int uiMipLevel,bool bIsStatic)
						 :VSTexture(uiFormatType,uiWidth,uiHeight,uiLength,uiMipLevel,bIsStatic)
{

	SetMipLevel();

}
VS3DTexture::~VS3DTexture()
{

}
VS3DTexture::VS3DTexture()
{

}
void VS3DTexture::SetMipLevel()
{
	VSMAC_ASSERT(m_uiWidth && m_uiHeight);
	VSMAC_ASSERT(IsTwoPower(m_uiWidth) && IsTwoPower(m_uiHeight) && IsTwoPower(m_uiLenght));
	unsigned int uiWidthLevel;
	unsigned int uiWidth = m_uiWidth;
	_asm bsr eax, uiWidth
	_asm mov uiWidthLevel, eax
	
	
	unsigned int uHeightLevel;
	unsigned int uiHeight = m_uiHeight;
	_asm bsr eax, uiHeight
	_asm mov uHeightLevel, eax

	unsigned int uiLengthLevel;
	unsigned int uiLength = m_uiLenght;
	_asm bsr eax, uiLength
	_asm mov uiLengthLevel, eax

	unsigned int uiCurMipLevel = Max(Max(uiWidthLevel,uHeightLevel),uiLengthLevel) + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}

}