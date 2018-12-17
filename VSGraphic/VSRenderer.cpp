#include "VSRenderer.h"
#include "VSFont.h"
#include "VSVShader.h"
#include "VSPShader.h"
#include "VSGShader.h"
#include "VSDShader.h"
#include "VSHShader.h"
#include "VSVertexBuffer.h"
#include "VSGeometry.h"
#include "VSTexture.h"
#include "VS2DTexture.h"
#include "VSTexAllState.h"
#include "VSMaterial.h"
#include "VSDepthStencil.h"
#include "VSCubeTexture.h"
#include "VSVertexFormat.h"
#include "VSMeshDate.h"
#include "VSShaderStringFactory.h"
#include "VSAABB3.h"
#include "VSOBB3.h"
#include "VSSphere3.h"
#include "VSTriangleSet.h"
#include "VSLineSet.h"
#include "VSGraphicInclude.h"
#include "VSRenderTarget.h"
#include "VSProfiler.h"
using namespace VSEngine2;
VSRenderer * VSRenderer::ms_pRenderer = NULL;

VSString VSRenderer::ms_VShaderProgramMain = _T("VSMain");
VSString VSRenderer::ms_PShaderProgramMain = _T("PSMain");
VSString VSRenderer::ms_GShaderProgramMain = _T("GSMain");
unsigned int VSRenderer::ms_uiBytesPerPixel[SFT_MAX] =
{
	4,  
	2,
	4,
	4,
	16,
	8,
	4,
	2,
	4,
	4,
	4,
	8,
	2,
	1,
	16,
	16
};
unsigned int VSRenderer::ms_uiChannelPerPixel[SFT_MAX] =
{
	4,  
	1,
	2,
	1,
	4,
	4,
	2,
	1,
	1,
	4,
	2,
	2,
	3,
	1,
	4,
	4
};
VSRenderer::SCREEN_QUAD_TYPE VSRenderer::ms_FullScreen[4] = 
{
	{VSVector3(-1.0f,  1.0f, 0.0f)},
	{VSVector3( 1.0f,  1.0f, 0.0f)},
	{VSVector3( 1.0f, -1.0f, 0.0f)},
	{VSVector3(-1.0f, -1.0f, 0.0f)}	
	
};
VSUSHORT_INDEX VSRenderer::ms_FullScreenI[6] = { 0, 1, 3, 1, 2, 3 };
unsigned int VSRenderer::GetBytesPerPixel(unsigned int uiFormatType)
{
	if (uiFormatType >= SFT_MAX)
	{
		return 0;
	}
	return ms_uiBytesPerPixel[uiFormatType];
}
unsigned int VSRenderer::GetChannelPerPixel(unsigned int uiFormatType)
{
	if (uiFormatType >= SFT_MAX)
	{
		return 0;
	}
	return ms_uiChannelPerPixel[uiFormatType];
}
const VSString & VSRenderer::GetVShaderProgramMain()
{
	return ms_VShaderProgramMain;
}
const VSString & VSRenderer::GetPShaderProgramMain()
{
	return ms_PShaderProgramMain;
}
const VSString & VSRenderer::GetGShaderProgramMain()
{
	return ms_GShaderProgramMain;
}
VSRenderer::~VSRenderer()
{
	VSMAC_DELETEA(m_pChildWindowInfo);
	m_pVertexBuffer = NULL;
	m_pVertexFormat = NULL;
	m_pIndexBuffer = NULL;
	m_pVShader = NULL;
	m_pPshader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pFont		= NULL;
	m_pQuadVertexFormat = NULL;

	m_pBlendState = NULL;
	m_pDepthStencilState = NULL;
	m_pRasterizerState = NULL;

}
void VSRenderer::RenderStateToDefault()
{
	VSMAC_ASSERT(m_uiMaxTexture);

	for (unsigned int i = 0; i < m_uiMaxTexture; i++)
	{
		m_pPTex[i] = NULL;

		m_pPSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetSamplerState(NULL, i, true);
	}

	VSMAC_ASSERT(m_uiMaxVTexture);
	for (unsigned int i = 0; i < m_uiMaxVTexture; i++)
	{
		m_pVTex[i] = NULL;
		m_pVSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetVSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxGTexture; i++)
	{
		m_pGTex[i] = NULL;
		m_pGSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetGSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxDTexture; i++)
	{
		m_pDTex[i] = NULL;
		m_pDSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetDSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxHTexture; i++)
	{
		m_pHTex[i] = NULL;
		m_pHSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetHSamplerState(NULL, i, true);
	}

	m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	SetDepthStencilState(NULL, true);
	SetBlendState(NULL, true);
	SetRasterizerState(NULL, true);
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;
 	m_pVertexFormat = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pPshader = NULL;
	m_pVShader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	SetTexture(VSTexAllState::GetDefalut()->m_pTex, 0);
}
bool VSRenderer::SetDefaultValue()
{
	VSMAC_ASSERT(m_uiMaxTexture);

	for(unsigned int i = 0 ; i < m_uiMaxTexture ; i++)
	{
		m_pPTex[i] = NULL;

		m_pPSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetSamplerState(NULL, i, true);
	}
	VSMAC_ASSERT(m_uiMaxVTexture);

	for(unsigned int i = 0 ; i < m_uiMaxVTexture ; i++)
	{
		m_pVTex[i] = NULL;
		m_pVSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetVSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxGTexture; i++)
	{
		m_pGTex[i] = NULL;
		m_pGSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetGSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxDTexture; i++)
	{
		m_pDTex[i] = NULL;
		m_pDSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetDSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxHTexture; i++)
	{
		m_pHTex[i] = NULL;
		m_pHSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetHSamplerState(NULL, i, true);
	}
	m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
 	SetDepthStencilState(NULL, true);
 	SetBlendState(NULL, true);
 	SetRasterizerState(NULL, true);
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;

	

	m_pFont = VSResourceManager::LoadASYNFont(_T("Arial"),false);

	SetTexture(VSTexAllState::GetDefalut()->m_pTex,0);

	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> ForamtArray;
	VSVertexFormat::VERTEXFORMAT_TYPE Pos;
	Pos.DateType = VSDataBuffer::DT_FLOAT32_3;
	Pos.OffSet = 0;
	Pos.Semantics = VSVertexFormat::VF_POSITION;
	Pos.SemanticsIndex = 0;
	ForamtArray.AddElement(Pos);

	VSVertexFormat::VERTEXFORMAT_TYPE TextureCode;
	TextureCode.DateType = VSDataBuffer::DT_FLOAT32_2;
	TextureCode.OffSet = 12;
	TextureCode.Semantics = VSVertexFormat::VF_TEXCOORD;
	TextureCode.SemanticsIndex = 0;
	ForamtArray.AddElement(TextureCode);
	m_pQuadVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &ForamtArray);

	VSVertexFormat::VERTEXFORMAT_TYPE ColorCode;
	ColorCode.DateType = VSDataBuffer::DT_COLOR;
	ColorCode.OffSet = 20;
	ColorCode.Semantics = VSVertexFormat::VF_COLOR;
	ColorCode.SemanticsIndex = 0;
	ForamtArray.AddElement(ColorCode);
	m_pQuadVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &ForamtArray);
	VSResourceManager::LoadDefaultResource(GetRendererType());
	//VSCustomMaterial::LoadAllDeviceShader();
	
	return 1;
}
bool VSRenderer::SetRenderTargets(VSRenderTarget * pRenderTargets[], unsigned int uiNum)
{
	if (uiNum > 0)
	{
		if (pRenderTargets[0] == NULL)
		{
			return false;
		}
		for (unsigned int i = 0; i < uiNum; i++)
		{
			SetRenderTarget(pRenderTargets[i], i);
		}
		return true;
	}
	return false;

}
bool VSRenderer::EndRenderTargets(VSRenderTarget * pRenderTargets[], unsigned int uiNum)
{
	if (uiNum > 0)
	{
		if (pRenderTargets[0] == NULL)
		{
			return false;
		}
		for (unsigned int i = 0; i < uiNum; i++)
		{
			EndRenderTarget(pRenderTargets[i], i);
		}
		return true;
	}
	return false;
}
VSRenderer::ChildWindowInfo * VSRenderer::GetChildWindowInfo(int uiID)
{
	if (!m_bWindowed)
		return NULL;
	else if (uiID >= m_iNumChildWindow)
		return NULL;
	else if (uiID < 0)
	{
		return NULL;
	}
	return &m_pChildWindowInfo[uiID];
}

bool VSRenderer::ReleaseDefaultValue()
{
	ReleaseBindObjectResource();


	return 1;
}
bool VSRenderer::ReleaseBindObjectResource()
{
	for (unsigned int i = 0; i < VSBind::ms_BindArray.GetNum(); i++)
	{
		VSBind * pBind = VSBind::ms_BindArray[i];
		if (!pBind)
		{
			continue;
		}
		if (!pBind->ReleaseResource())
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	return true;
}

void VSRenderer::SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_UseState.GetAll(&RenderState);
	m_uiRenderStateInheritFlag = uiRenderStateInheritFlag;
}
void VSRenderer::ClearUseState()
{
	m_uiRenderStateInheritFlag = 0;
}
void VSRenderer::SetRenderState(VSRenderState & RenderState)
{

	SetBlendState(RenderState.m_pBlendState);

	SetDepthStencilState(RenderState.m_pDepthStencilState);

	SetRasterizerState(RenderState.m_pRasterizerState);

	if (RenderState.m_RasterizerDesc.m_bClipPlaneEnable && RenderState.m_Plane.GetNum() > 0)
	{
		m_bClipPlaneEnable = true;
		SetClipPlane(RenderState.m_Plane, m_bClipPlaneEnable);
	}
	else
	{
		if (m_bClipPlaneEnable)
		{
			m_bClipPlaneEnable = false;
			SetClipPlane(RenderState.m_Plane, m_bClipPlaneEnable);
		}
	}
	if (RenderState.m_RasterizerDesc.m_bScissorTestEnable && RenderState.m_RectArray.GetNum() > 0)
	{
		m_bScissorRectEnable = true;
		SetScissorRect(RenderState.m_RectArray, m_bScissorRectEnable);
	}
	else
	{
		if (m_bScissorRectEnable)
		{
			m_bScissorRectEnable = false;
			SetScissorRect(RenderState.m_RectArray, m_bScissorRectEnable);
		}
	}


}
bool VSRenderer::SetVShaderProgram(VSVShader*	pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;
	if (!LoadVShaderProgram(pVShaderProgram))
		return 0;

	/*VSResourceIdentifier * pID = pVShaderProgram->GetIdentifier(this);
	if(!pID)
	return 0;*/

	return 1;
}
bool VSRenderer::SetGShaderProgram(VSGShader* pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;
	if (!LoadGShaderProgram(pGShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetDShaderProgram(VSDShader* pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;
	if (!LoadDShaderProgram(pDShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetHShaderProgram(VSHShader* pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;
	if (!LoadHShaderProgram(pHShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetPShaderProgram(VSPShader*	pPShaderProgram)
{
	if (!pPShaderProgram)
		return 0;
	if (!LoadPShaderProgram(pPShaderProgram))
		return 0;

	/*VSResourceIdentifier * pID = pPShaderProgram->GetIdentifier(this);
	if(!pID)
	return 0;*/


	return 1;
}
void VSRenderer::EnableTextureRTUsed(VSTexture * pTexture)
{
	if (!pTexture)
	{
		return;
	}
	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = true;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = true;
	}
}
void VSRenderer::DisableTextureRTUsed(VSTexture * pTexture)
{
	if (!pTexture)
	{
		return;
	}
	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = false;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = false;
	}
}
bool VSRenderer::SetRenderTarget(VSRenderTarget *pRenderTarget, unsigned int i)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	if (i >= m_uiMaxRTNum)
	{
		return 0;
	}

	if (!LoadRenderTarget(pRenderTarget))
		return 0;
	VSTexture * pTexture = pRenderTarget->GetCreateBy();
	if (pTexture->HasAnySlot())
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (m_uiCurRTWidth != pRenderTarget->GetWidth()
		|| m_uiCurRTHeight != pRenderTarget->GetHeight()
		|| m_uiCurRTMultisampler != pRenderTarget->GetMulSample())
	{
		if (i == 0 && m_uiBackMacthType == BMT_NONE)
		{
			m_uiBackMacthType = BMT_RENDER_TARGET;
			m_uiCurRTWidth = pRenderTarget->GetWidth();
			m_uiCurRTHeight = pRenderTarget->GetHeight();
			m_uiCurRTMultisampler = pRenderTarget->GetMulSample();
		}
		else
		{
			VSMAC_ASSERT(false);
			return false;
		}
		
	}
	
	EnableTextureRTUsed(pTexture);
	return 1;
}
bool VSRenderer::EndRenderTarget(VSRenderTarget *pRenderTarget, unsigned int i)
{
	if (!pRenderTarget)
		return 0;

	if (i >= m_uiMaxRTNum)
	{
		return 0;
	}

	VSTexture * pTexture = pRenderTarget->GetCreateBy();
	if (pTexture->HasAnySlot())
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (!LoadRenderTarget(pRenderTarget))
		return 0;
	m_uiCurRTWidth = m_uiScreenWidth;
	m_uiCurRTHeight = m_uiScreenHeight;
	m_uiCurRTMultisampler = m_uiCurMultisample;
	DisableTextureRTUsed(pTexture);
	if (m_uiBackMacthType == BMT_RENDER_TARGET)
	{
		m_uiBackMacthType = BMT_NONE;
	}
	return 1;
}
bool VSRenderer::SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{
	if (!pDepthStencilBuffer)
		return 0;
	if (m_uiCurRTWidth != pDepthStencilBuffer->GetWidth()
		|| m_uiCurRTHeight != pDepthStencilBuffer->GetHeight()
		|| m_uiCurRTMultisampler != pDepthStencilBuffer->GetMulSample())
	{
		VSMAC_ASSERT(false);
		return false;	
	}
	if (!LoadDepthStencil(pDepthStencilBuffer))
		return 0;
	return 1;
}
bool VSRenderer::EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{
	if (!pDepthStencilBuffer)
		return 0;

	if (!LoadDepthStencil(pDepthStencilBuffer))
		return 0;
	return 1;
}
unsigned int VSRenderer::SetVBuffer(VSVertexBuffer * pVBuffer)
{
	if (m_pVertexBuffer == pVBuffer)
	{
		if (!pVBuffer)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pVBuffer  && pVBuffer->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}
	}

	if (!pVBuffer)
	{
		m_pVertexBuffer = NULL;
		SetVertexFormat(NULL);
		return FRI_FAIL;
	}
	if (!LoadVBuffer(pVBuffer))
	{
		m_pVertexBuffer = NULL;
		SetVertexFormat(NULL);
		return FRI_FAIL;
	}
	SetVertexFormat(pVBuffer->GetVertexFormat());
	m_pVertexBuffer = pVBuffer;

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetVertexFormat(VSVertexFormat * pVertexFormat)
{
	if (m_pVertexFormat == pVertexFormat)
		return FRI_SAMERESOURCE;
	m_pVertexFormat = pVertexFormat;
	if (!pVertexFormat)
	{
		return FRI_FAIL;
	}
	return FRI_SUCCESS;
}
bool VSRenderer::CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, unsigned int uiFace)
{
	if (!pSource || !pDest || uiFace >= VSCubeTexture::F_MAX)
	{
		return false;
	}
	if (pSource->GetWidth(0) != pSource->GetHeight(0) && pSource->GetWidth(0) != pDest->GetWidth(0))
	{
		return false;
	}
	if (pSource->GetMipLevel() != pDest->GetMipLevel())
	{
		return false;
	}
	if (pSource->GetFormatType() != pDest->GetFormatType())
	{
		return false;
	}
	return true;
}
unsigned int VSRenderer::SetIBuffer(VSIndexBuffer* pIBuffer)
{
	if (m_pIndexBuffer == pIBuffer)
	{
		if (!pIBuffer)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pIBuffer && pIBuffer->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pIBuffer)
	{
		m_pIndexBuffer = NULL;
		return FRI_FAIL;
	}
	if (!LoadIBuffer(pIBuffer))
	{
		m_pIndexBuffer = NULL;
		return FRI_FAIL;
	}

	m_pIndexBuffer = pIBuffer;

	return FRI_SUCCESS;
}

bool VSRenderer::LoadVShaderProgram(VSVShader * pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;

	return pVShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseVShaderProgram(VSVShader * pVShaderProgram)
{


	if (!pVShaderProgram)
		return 0;
	return pVShaderProgram->ReleaseResource();
}

bool VSRenderer::LoadPShaderProgram(VSPShader* pPShaderProgram)
{
	if (!pPShaderProgram)
		return 0;

	return pPShaderProgram->LoadResource(this);

}
bool VSRenderer::ReleasePShaderProgram(VSPShader* pPShaderProgram)
{

	if (!pPShaderProgram)
		return 0;

	return pPShaderProgram->ReleaseResource();

}
bool VSRenderer::LoadGShaderProgram(VSGShader * pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;

	return pGShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseGShaderProgram(VSGShader * pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;

	return pGShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadDShaderProgram(VSDShader * pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;

	return pDShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseDShaderProgram(VSDShader * pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;

	return pDShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadHShaderProgram(VSHShader * pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;

	return pHShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseHShaderProgram(VSHShader * pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;

	return pHShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadTexture(VSTexture * pTexture)
{
	if (!pTexture)
		return 0;

	return pTexture->LoadResource(this);
}
bool VSRenderer::ReleaseTexture(VSTexture * pTexture)
{

	if (!pTexture)
		return 0;

	return pTexture->ReleaseResource();

}
bool VSRenderer::LoadDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
		return 0;

	return pDepthStencil->LoadResource(this);
}
bool VSRenderer::ReleaseDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
		return 0;

	return pDepthStencil->ReleaseResource();
}
bool VSRenderer::LoadRenderTarget(VSRenderTarget  * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	pRenderTarget->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseRenderTarget(VSRenderTarget * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	pRenderTarget->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadBlendState(VSBlendState * pBlendState)
{
	if (!pBlendState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pBlendState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseBlendState(VSBlendState * pBlendState)
{
	if (!pBlendState)
	{
		return 0;
	}
	pBlendState->ReleaseResource();
	return 1;
}

bool VSRenderer::LoadDepthStencilState(VSDepthStencilState * pDepthStencilState)
{
	if (!pDepthStencilState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pDepthStencilState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseDepthStencilState(VSDepthStencilState * pDepthStencilState)
{
	if (!pDepthStencilState)
	{
		return 0;
	}
	pDepthStencilState->ReleaseResource();
	return 1;
}

bool VSRenderer::LoadRasterizerState(VSRasterizerState * pRasterizerState)
{
	if (!pRasterizerState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pRasterizerState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseRasterizerState(VSRasterizerState * pRasterizerState)
{
	if (!pRasterizerState)
	{
		return 0;
	}
	pRasterizerState->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadSamplerState(VSSamplerState * pSamplerState)
{
	if (!pSamplerState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pSamplerState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseSamplerState(VSSamplerState * pSamplerState)
{
	if (!pSamplerState)
	{
		return 0;
	}
	pSamplerState->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadVBuffer(VSVertexBuffer * pVBuffer)
{
	if (!pVBuffer)
		return 0;

	return pVBuffer->LoadResource(this);
}
bool VSRenderer::ReleaseVBuffer(VSVertexBuffer * pVBuffer)
{

	if (!pVBuffer)
		return 0;

	return pVBuffer->ReleaseResource();
}
bool VSRenderer::LoadIBuffer(VSIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
		return 0;

	return pIBuffer->LoadResource(this);

}
bool VSRenderer::ReleaseIBuffer(VSIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
		return 0;

	return pIBuffer->ReleaseResource();

}
unsigned int VSRenderer::SetVShader(VSVShader * pVShader)
{
	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetPShader(VSPShader * pPShader)
{

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetGShader(VSGShader * pGShader)
{

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetDShader(VSDShader * pDShader)
{

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetHShader(VSHShader * pHShader)
{

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetMesh(VSMeshDate *pMeshDate)
{

	VSMAC_ASSERT(pMeshDate);
	if (!pMeshDate)
	{
		return FRI_FAIL;
	}


	if (!SetVBuffer(pMeshDate->GetVertexBuffer()))
		return 0;
	if (!SetIBuffer(pMeshDate->GetIndexBuffer()))
		return 0;
	return FRI_SUCCESS;

}
bool VSRenderer::DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, VSGShader * pGShader)
{

	if (!pGeometry || !pGeometry->GetMeshDate())
		return 0;

	m_LocalRenderState.GetAll(pRenderState);

	if (pGeometry->IsSwapCull())
	{
		m_LocalRenderState.SwapCull();
	}
	if (m_uiRenderStateInheritFlag)
	{

		m_LocalRenderState.Inherit(&m_UseState, m_uiRenderStateInheritFlag);


	}
	SetRenderState(m_LocalRenderState);
	if (!SetVShader(pVShader))
	{
		return 0;
	}
	SetGShader(pGShader);

	if (!SetPShader(pPShader))
	{
		return 0;
	}
	
	if (SetMesh(pGeometry->GetMeshDate()) == FRI_FAIL)
		return 0;

	return 1;
}
void VSRenderer::SetVTexAllState(VSTexAllState * pTexAllState, unsigned int i)
{
	if (i >= m_uiMaxVTexture)
		return;
	if (pTexAllState)
	{
		SetVTexture(pTexAllState->m_pTex, i);
		SetVSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetVTexture(NULL, i);
		SetVSamplerState(NULL, i);
	}
}
unsigned int VSRenderer::SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pGSamplerState[i])
	{
		m_pGSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pGSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pDSamplerState[i])
	{
		m_pDSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pDSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pHSamplerState[i])
	{
		m_pHSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pHSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pVSamplerState[i])
	{
		m_pVSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pVSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
void VSRenderer::SetGTexAllState(VSTexAllState * pTexAllState, unsigned int i)
{
	VSMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxGTexture)
		return;
	if (pTexAllState)
	{
		SetGTexture(pTexAllState->m_pTex, i);
		SetGSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetGTexture(NULL, i);
		SetGSamplerState(NULL, i);
	}
}
void VSRenderer::SetDTexAllState(VSTexAllState * pTexAllState, unsigned int i)
{
	VSMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxDTexture)
		return;
	if (pTexAllState)
	{
		SetDTexture(pTexAllState->m_pTex, i);
		SetDSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetDTexture(NULL, i);
		SetDSamplerState(NULL, i);
	}
}
void VSRenderer::SetHTexAllState(VSTexAllState * pTexAllState, unsigned int i)
{
	VSMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxHTexture)
		return;
	if (pTexAllState)
	{
		SetHTexture(pTexAllState->m_pTex, i);
		SetHSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetHTexture(NULL, i);
		SetHSamplerState(NULL, i);
	}
}
void VSRenderer::SetTexAllState(VSTexAllState * pTexAllState, unsigned int i)
{
	VSMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxTexture)
		return;
	if (pTexAllState)
	{
		SetTexture(pTexAllState->m_pTex, i);
		SetSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetTexture(NULL, i);
		SetSamplerState(NULL, i);
	}
}
void VSRenderer::SetVSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_VSTexSlot.Set(uiSlot, b);
}
void VSRenderer::SetPSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_PSTexSlot.Set(uiSlot, b);
}
void VSRenderer::SetGSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_GSTexSlot.Set(uiSlot, b);
}
void VSRenderer::SetDSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_DSTexSlot.Set(uiSlot, b);
}
void VSRenderer::SetHSTextureRTTextureUsed(VSTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_HSTexSlot.Set(uiSlot, b);
}
bool VSRenderer::CheckIsTextureCanSet(VSTexture * pTexture)
{
	if (!pTexture)
	{
		return true;
	}
	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		if (((VS2DTexture*)pTexture)->IsRenderTarget())
		{
			if (((VS2DTexture*)pTexture)->m_bRtUsed == true)
			{
				VSMAC_ASSERT(0);
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		if (((VSCubeTexture*)pTexture)->m_bRtUsed == true)
		{
			VSMAC_ASSERT(0);
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
unsigned int VSRenderer::SetVTexture(VSTexture* pTexture, unsigned int i)
{

	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pVTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}
	}
	if (!pTexture)
	{
		SetVSTextureRTTextureUsed(m_pVTex[i],i,false);
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	SetVSTextureRTTextureUsed(m_pVTex[i],i,false);
	SetVSTextureRTTextureUsed(pTexture,i,true);
	m_pVTex[i] = pTexture;
	return 1;
}
unsigned int VSRenderer::SetDTexture(VSTexture* pTexture, unsigned int i)
{

	VSMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pDTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetDSTextureRTTextureUsed(m_pDTex[i], i, false);
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}

	SetDSTextureRTTextureUsed(m_pDTex[i], i, false);
	SetDSTextureRTTextureUsed(pTexture, i, true);

	m_pDTex[i] = pTexture;
	return 1;
}
unsigned int VSRenderer::SetHTexture(VSTexture* pTexture, unsigned int i)
{

	VSMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pHTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetHSTextureRTTextureUsed(m_pHTex[i], i, false);
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}

	SetHSTextureRTTextureUsed(m_pHTex[i], i, false);
	SetHSTextureRTTextureUsed(pTexture, i, true);

	m_pHTex[i] = pTexture;
	return 1;
}
unsigned int VSRenderer::SetGTexture(VSTexture* pTexture, unsigned int i)
{

	VSMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pGTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetGSTextureRTTextureUsed(m_pGTex[i], i, false);
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}

	SetGSTextureRTTextureUsed(m_pGTex[i], i, false);
	SetGSTextureRTTextureUsed(pTexture, i, true);

	m_pGTex[i] = pTexture;
	return 1;
}
unsigned int VSRenderer::SetTexture(VSTexture* pTexture, unsigned int i)
{

	VSMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pPTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetPSTextureRTTextureUsed(m_pPTex[i],i,false);
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}

	SetPSTextureRTTextureUsed(m_pPTex[i],i,false);
	SetPSTextureRTTextureUsed(pTexture,i,true);
	
	m_pPTex[i] = pTexture;
	return 1;
}
unsigned int VSRenderer::SetSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pPSamplerState[i])
	{
		m_pPSamplerState[i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pPSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetBlendState(VSBlendState * pBlendState, bool bForceSet)
{
	if (!pBlendState)
	{
		pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	}
	if (!m_pBlendState)
	{
		m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	}
	if (m_pBlendState == pBlendState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadBlendState(pBlendState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet)
{
	if (!pDepthStencilState)
	{
		pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	}
	if (!m_pDepthStencilState)
	{
		m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	}
	if (m_pDepthStencilState == pDepthStencilState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadDepthStencilState(pDepthStencilState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int VSRenderer::SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet)
{
	if (!pRasterizerState)
	{
		pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	}
	if (!m_pRasterizerState)
	{
		m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	}
	if (m_pRasterizerState == pRasterizerState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadRasterizerState(pRasterizerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
void VSRenderer::SetClipPlane(VSArray<VSPlane3> &Plane, bool bEnable)
{

}
void VSRenderer::SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable)
{

}
VSRenderer::VSRenderer()
{

	m_pChildWindowInfo = NULL;
	m_ClearColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_fClearDepth = 1.0f;
	m_uiClearStencil = 0;

	m_iNumChildWindow = 0;
	m_iCurWindowID = -1;
	m_bIsRendering = false;



	m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;
	m_pFont = NULL;//VSFont::Default;

	m_pVertexBuffer = NULL;
	m_pVertexFormat = NULL;
	m_pIndexBuffer = NULL;
	m_pVShader = NULL;
	m_pPshader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pQuadVertexFormat = NULL;
	m_uiRenderStateInheritFlag = 0;
	VSRenderer::ms_pRenderer = this;
	m_uiBackMacthType = BMT_NONE;
}
bool VSRenderer::UseWindow(int uiWindowID)
{
	if (!m_bWindowed)
		return 0;
	else if (uiWindowID >= m_iNumChildWindow)
		return 0;
	else if (uiWindowID < -1)
	{
		return 0;
	}
	m_iCurWindowID = uiWindowID;
	return 1;
}
bool VSRenderer::BeginRendering()
{
	ClearBuffers();
	m_bIsRendering = true;
	m_FontVertex.Clear();
	return true;
}
bool VSRenderer::EndRendering()
{
	if (m_FontVertex.GetNum() > 0)
	{
		SetViewPort(NULL);
		VSFont * pUseFont = m_pFont->GetResource();
		VSCustomMaterial *pFontMaterial = VSCustomMaterial::GetFont();
		pFontMaterial->SetPShaderTexture(VSUsedName::ms_cPostInputTexture, pUseFont->GetTexAllState());
		pFontMaterial->UseMaterial();
		unsigned int uiLength = m_FontVertex.GetNum() >> 2;
		DrawScreenFont(m_FontVertex.GetBuffer(), uiLength * 4, m_FontIndex.GetBuffer(), uiLength * 2 * 3);
	}	
	m_bIsRendering = false;
	VSMAC_ASSERT(m_uiBackMacthType == BMT_NONE);
	m_uiBackMacthType = BMT_NONE;
	return true;
}
void VSRenderer::DeviceLost()
{

}
void VSRenderer::ResetDevice()
{

}
bool VSRenderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow)
{
	if (m_uiScreenHeight == uiHeight && m_uiScreenWidth == uiWidth && m_bWindowed == bWindow)
	{


		return false;
	}
	if (!bWindow)
	{
		unsigned int uiAdpterInfoNum = 0;
		const VSAdapterInfo * pAdapterInfo = GetAdapterInfo(uiAdpterInfoNum);
		if (!pAdapterInfo || !uiAdpterInfoNum)
		{
			return false;
		}
		bool bFlag = false;
		for (unsigned int i = 0; i < pAdapterInfo[0].DisplayMode.GetNum(); i++)
		{
			if (pAdapterInfo[0].DisplayMode[i].uiHeight == uiHeight
				&& pAdapterInfo[0].DisplayMode[i].uiWidth == uiWidth
				&& pAdapterInfo[0].DisplayMode[i].uiBitFormat == VSRenderer::SFT_A8R8G8B8)
			{
				bFlag = true;
				break;
			}
		}
		if (!bFlag)
		{
			return false;
		}
	}


	m_bWindowed = bWindow;
	m_uiScreenHeight = uiHeight;
	m_uiScreenWidth = uiWidth;
	m_uiCurRTWidth = uiWidth;
	m_uiCurRTHeight = uiHeight;
	return true;
}
bool VSRenderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
bool VSRenderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	VSMAC_ASSERT(pScreenBuffer && uiVertexNum && pIndexBuffer && uiIndexNum);
	if (!pScreenBuffer || !uiVertexNum || !pIndexBuffer || !uiIndexNum)
	{
		return false;
	}
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
bool VSRenderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	VSMAC_ASSERT(pScreenFontBuffer && uiVertexNum && pIndexBuffer && uiIndexNum);
	if (!pScreenFontBuffer || !uiVertexNum || !pIndexBuffer || !uiIndexNum)
	{
		return false;
	}
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
VSString VSRenderer::Float(unsigned int uiIndex)const
{
	if (uiIndex == FI_1)
	{
		return Float();
	}
	else if (uiIndex == FI_2)
	{
		return Float2();
	}
	else if (uiIndex == FI_3)
	{
		return Float3();
	}
	else if (uiIndex == FI_4)
	{
		return Float4();
	}
	else
		VSMAC_ASSERT(1);
	return VSString();


}
bool VSRenderer::SetViewPort(VSViewPort *pViewPort)
{
	if (!pViewPort)
	{
		m_CurViewPort.Reset();
	}
	return true;
}
void VSRenderer::DrawText(int iX, int iY, const DWORD rColor,
	const TCHAR* acText, ...)
{
	if (!acText || !m_pFont)
		return;
	VSFont * pUseFont = m_pFont->GetResource();
	TCHAR cch[1024];
	char *pArgs;
	pArgs = (char*)&acText + sizeof(acText);
	VSSprintf(cch, 1024, acText, pArgs);
	VSString String = cch;
	
	unsigned int uiLength = String.GetLength();
	unsigned int uiRTWidth = GetCurRTWidth();
	unsigned int uiRTHeight = GetCurRTHeight();
	
	VSREAL InvRTWidth = 1.0f / uiRTWidth;
	VSREAL InvRTHeight = 1.0f / uiRTHeight;
	VSREAL fLeft = iX  * InvRTWidth;
	VSREAL fTop = 1.0f - iY * InvRTHeight;

	fLeft = fLeft * 2.0f - 1.0f;
	fTop = fTop * 2.0f - 1.0f;

	VSREAL fOriginalLeft = fLeft;
	VSREAL fGlyphSizeY = pUseFont->GetTexAllState()->m_pTex->GetHeight(0) * InvRTHeight/* * 2.0f*/;

	VSREAL fTextureWidth = pUseFont->GetTexAllState()->m_pTex->GetWidth(0) * 1.0f;
	
	for (unsigned int i = 0; i < uiLength; i++)
	{
		if (String[i] < VSFont::ASCII_START || String[i] > VSFont::ASCII_END)
		{
			continue;
		}
		if (String[i] == '\n')
		{
			fLeft = fOriginalLeft;
			fTop -= fGlyphSizeY;
			continue;
		}
		unsigned char CharIndex = String[i] - VSFont::ASCII_START;

		SCREEN_FONT_TYPE Vertex[4];
		VSREAL Left = pUseFont->GetFontData(CharIndex);
		VSREAL Right = pUseFont->GetFontData(CharIndex + 1);
		float fGlyphSizeX = (Right - Left) * fTextureWidth * InvRTWidth /**2.0f*/;  // in pixels
		Vertex[0].Point = VSVector3(fLeft, fTop, 0.0f);
		Vertex[1].Point = VSVector3(fLeft + fGlyphSizeX, fTop, 0.0f);
		Vertex[2].Point = VSVector3(fLeft + fGlyphSizeX, fTop - fGlyphSizeY, 0.0f);
		Vertex[3].Point = VSVector3(fLeft, fTop - fGlyphSizeY, 0.0f);

		
		fLeft += fGlyphSizeX;

		Vertex[0].UV = VSVector2(Left,0.0f);
		Vertex[1].UV = VSVector2(Right, 0.0f);
		Vertex[2].UV = VSVector2(Right, 1.0f);
		Vertex[3].UV = VSVector2(Left, 1.0f);;

		Vertex[0].Color = rColor;
		Vertex[1].Color = rColor;
		Vertex[2].Color = rColor;
		Vertex[3].Color = rColor;

		m_FontVertex.AddElement(Vertex[0]);
		m_FontVertex.AddElement(Vertex[1]);
		m_FontVertex.AddElement(Vertex[2]);
		m_FontVertex.AddElement(Vertex[3]);
		
	}
	unsigned int IndexFontNum = m_FontIndex.GetNum() / 6;
	if (IndexFontNum < (m_FontVertex.GetNum() >> 2))
	{
		for (unsigned int i = IndexFontNum; i < (m_FontVertex.GetNum() >> 2); i++)
		{
			unsigned int Index = i * 4;
			m_FontIndex.AddElement(Index);
			m_FontIndex.AddElement(Index + 1);
			m_FontIndex.AddElement(Index + 3);

			m_FontIndex.AddElement(Index + 1);
			m_FontIndex.AddElement(Index + 2);
			m_FontIndex.AddElement(Index + 3);
		}
	}
	
}
void VSRenderer::SetFont(VSFontR * pFont)
{
	if (!pFont)
	{
		return;	
	}
	else
	{
		m_pFont = pFont;
	}
}