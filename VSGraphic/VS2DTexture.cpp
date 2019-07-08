#include "VS2DTexture.h"
#include "VSGraphicInclude.h"
#include "VSRenderTarget.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VS2DTexture,VSTexture)
BEGIN_ADD_PROPERTY(VS2DTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS2DTexture)
IMPLEMENT_INITIAL_END


bool VS2DTexture:: SaveToFile(TCHAR * pSaveName)
{
	
	if(!m_uiHeight || !m_uiWidth)
		return 0;

	unsigned int uiHightSize = GetByteSize(0);
	if (!uiHightSize)
	{
		return false;
	}
	BITMAPFILEHEADER FileHeader;
	FileHeader.bfType = 0x4D42;
	FileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + uiHightSize;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER InfoHeader;
	InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	InfoHeader.biWidth = m_uiWidth;
	InfoHeader.biHeight = m_uiHeight;
	InfoHeader.biPlanes = 1;
	InfoHeader.biBitCount = 32;
	InfoHeader.biCompression = 0;
	InfoHeader.biSizeImage =  uiHightSize;
	InfoHeader.biXPelsPerMeter = 0;
	InfoHeader.biYPelsPerMeter = 0;
	InfoHeader.biClrUsed = 0;
	InfoHeader.biClrImportant = 0;

	
	

	unsigned char *pBufferTemp = NULL,*pBuffer = NULL;					//临时缓冲区
	pBufferTemp = VS_NEW unsigned char[uiHightSize];
	pBuffer = GetBuffer(0);
	if(!pBufferTemp)
		return 0;

	unsigned int  bytes_per_line = m_uiWidth * 4;
	// 按行翻转
	for (unsigned int index=0; index < m_uiHeight; index++)
		VSMemcpy(&pBufferTemp[((m_uiHeight - 1) - index) * bytes_per_line],
		&pBuffer[index * bytes_per_line], bytes_per_line);

	VSFile * pFile = NULL;
	pFile = VS_NEW VSFile();
	if (!pFile)
	{
		return 0;
	}
	
	if(!pFile->Open(pSaveName,VSFile::OM_WB))
	{
		VSMAC_DELETE(pFile);
		return 0;
	}
	pFile->Write(&FileHeader,sizeof(BITMAPFILEHEADER),1);
	pFile->Write(&InfoHeader,sizeof(BITMAPINFOHEADER),1);
	pFile->Write(pBufferTemp,uiHightSize,1);
	VSMAC_DELETEA(pBufferTemp);
	VSMAC_DELETE(pFile);
	return 1;
}
VS2DTexture::VS2DTexture(unsigned int uiWidth, unsigned int uiHeight,
						unsigned int uiFormatType,unsigned int uiMipLevel,bool bIsStatic)
						:VSTexture(uiFormatType,uiWidth,uiHeight,1,uiMipLevel,bIsStatic)
{
	SetMipLevel();
	m_bRenderTarget = false;
	m_bRtUsed = false;
	m_pCurRenderTarget = NULL;
}
VS2DTexture::VS2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMipLevel)
			:VSTexture(uiFormatType,uiWidth,uiHeight,1,uiMipLevel,true)
{
	//SetMipLevel();
	m_bRenderTarget = true;
	m_bRtUsed = false;
	m_pCurRenderTarget = NULL;
}
VS2DTexture::~VS2DTexture()
{
	
}
VS2DTexture::VS2DTexture()
{
	m_bRenderTarget = false;
}
void VS2DTexture::SetMipLevel()
{
	if (m_uiMipLevel == 1)
	{
		return;
	}
	VSMAC_ASSERT(m_uiWidth && m_uiHeight);
	
	if (!IsTwoPower(m_uiWidth) || !IsTwoPower(m_uiHeight))
	{
		m_uiMipLevel = 1;
		VSMAC_ASSERT(0);
	}

	unsigned int uiWidthLevel;
	unsigned int uiWidth = m_uiWidth;
	_asm bsr eax, uiWidth
	_asm mov uiWidthLevel, eax

	unsigned int uHeightLevel;
	unsigned int uiHeight = m_uiHeight;
	_asm bsr eax, uiHeight
	_asm mov uHeightLevel, eax

	unsigned int uiCurMipLevel = Max(uiWidthLevel,uHeightLevel) + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}
}

void VS2DTexture::CreateRAMDate()
{
	if (m_bRenderTarget)
	{
		return;
	}
	VSTexture::CreateRAMDate();
}