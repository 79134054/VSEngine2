#include "VSDepthStencil.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSDepthStencil,VSBind)
BEGIN_ADD_PROPERTY(VSDepthStencil,VSBind)
REGISTER_PROPERTY(m_uiWidth,Width,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiHeight,Height,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMulSample,MulSample,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiFormatType,FormatType,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSDepthStencil)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSDepthStencil::VSDepthStencil()
{
	m_bUsed = false;
}
VSDepthStencil::VSDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
							   unsigned int uiMulSample,unsigned int uiFormatType)
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	VSMAC_ASSERT(uiFormatType < VSRenderer::SFT_MAX)
	m_uiMulSample = uiMulSample;
	m_uiFormatType = uiFormatType;
	m_bIsStatic = 1;
	m_bUsed = false;
}
VSDepthStencil::~VSDepthStencil()
{
	ReleaseResource();
}
bool VSDepthStencil::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadDepthStencil (this,pID))
		return 0;
	return 1;
}
bool VSDepthStencil::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseDepthStencil(pID))
		return 0;
	return 1;
}