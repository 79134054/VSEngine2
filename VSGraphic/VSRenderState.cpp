#include "VSRenderState.h"
#include "VSShader.h"
#include "VSMaterial.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSRenderState,VSObject)
BEGIN_ADD_PROPERTY(VSRenderState,VSObject)
REGISTER_PROPERTY(m_DepthStencilDesc,DepthStencilDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RasterizerDesc,RasterizerDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_BlendDesc,BlendDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RectArray,RectArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Plane,Plane,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRenderState)
IMPLEMENT_INITIAL_END
bool VSRenderState::PostLoad(void * pDate)
{
	VSObject::PostLoad(pDate);
	m_pDepthStencilState = VSResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	m_pBlendState = VSResourceManager::CreateBlendState(m_BlendDesc);
	m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
	return true;
}
bool VSRenderState::PostClone(VSObject * pObjectSrc)
{
	VSObject::PostClone(pObjectSrc);
	m_pDepthStencilState = VSResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	m_pBlendState = VSResourceManager::CreateBlendState(m_BlendDesc);
	m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
	return true;
}
void VSRenderState::Inherit(const VSRenderState * pRenderState,unsigned int uiInheritFlag)
{
	if (!uiInheritFlag)
	{
		return ;
	}
	VSMAC_ASSERT(pRenderState);
	bool bReCreateDepthStencil = false;
	bool bReCreateRasterizer = false;
	bool bReCreateBlend = false;
	if (uiInheritFlag & IF_WIRE_ENABLE)
	{
		if (m_pRasterizerState->GetRasterizerDesc().m_bWireEnable != 
			pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable)
		{
			bReCreateRasterizer = true;
			m_RasterizerDesc.m_bWireEnable = pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable;
		}
	}
	if (bReCreateRasterizer)
	{
		m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
	}
	if (bReCreateDepthStencil)
	{
		m_pDepthStencilState = VSResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	}
	if (bReCreateBlend)
	{
		m_pBlendState = VSResourceManager::CreateBlendState(m_BlendDesc);
	}
	return;
}
void VSRenderState::SwapCull()
{
	unsigned int uiChangeType[3] = {VSRasterizerDesc::CT_NONE,VSRasterizerDesc::CT_CCW,VSRasterizerDesc::CT_CW};
	m_RasterizerDesc.m_uiCullType = uiChangeType[m_RasterizerDesc.m_uiCullType];
	m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
}