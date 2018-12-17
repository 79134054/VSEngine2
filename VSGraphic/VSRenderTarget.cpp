#include "VSRenderTarget.h"
#include "VSVertexFormat.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSRenderTarget,VSBind)
BEGIN_ADD_PROPERTY(VSRenderTarget,VSBind)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRenderTarget)
IMPLEMENT_INITIAL_END
VSRenderTarget::VSRenderTarget()
{
	m_uiWidth = 512;
	m_uiHeight = 512;
	m_uiFormatType = VSRenderer::SFT_A8R8G8B8;
	m_uiMulSample = VSRenderer::MS_NONE;
	m_pCreateBy = NULL;
	m_bUsed = false;
}
VSRenderTarget::VSRenderTarget(unsigned int uiWidth, unsigned int uiHeight,
							   unsigned int uiFormatType, unsigned int uiMulSample)
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiFormatType = uiFormatType;
	m_uiMulSample = uiMulSample;
	m_pCreateBy = NULL;
	m_uiLevel = 0;
	m_uiParam = 0;
	m_bUsed = false;
}
VSRenderTarget::VSRenderTarget(VSTexture * pCreateBy,unsigned int uiMulSample,unsigned int uiLevel,unsigned int Param)
{
	if (pCreateBy->GetTexType() == VSTexture::TT_2D && ((VS2DTexture *)pCreateBy)->IsRenderTarget())
	{
		VSMAC_ASSERT(((VS2DTexture *)pCreateBy)->IsRenderTarget());
		((VS2DTexture *)pCreateBy)->m_pCurRenderTarget = this;
	}
	else if (pCreateBy->GetTexType() == VSTexture::TT_CUBE && ((VSCubeTexture *)pCreateBy)->IsRenderTarget())
	{
		VSMAC_ASSERT(((VS2DTexture *)pCreateBy)->IsRenderTarget());
		((VSCubeTexture *)pCreateBy)->m_pCurRenderTarget[Param] = this;
	}
	else
	{
		VSMAC_ASSERT(0);
	}
	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = uiMulSample;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiParam = Param;
	m_uiWidth = m_pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = m_pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
}
VSRenderTarget::~VSRenderTarget()
{
	ReleaseResource();
	m_pCreateBy = NULL;
}
bool VSRenderTarget::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadRenderTarget(this,pID))
		return 0;
	return 1;
}
bool VSRenderTarget::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseRenderTarget(pID))
		return 0;
	return 1;
}