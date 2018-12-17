#include "VSDepthStencilState.h"
#include "VSShader.h"
#include "VSMaterial.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDepthStencilState,VSBind)
VSPointer<VSDepthStencilState> VSDepthStencilState::Default;
BEGIN_ADD_PROPERTY(VSDepthStencilState,VSBind)
REGISTER_PROPERTY(m_DepthStencilDesc,DepthStencilDesc,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDepthStencilState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSDepthStencilState::InitialDefaultState()
{
	VSDepthStencilDesc DepthStencilDesc;
	Default = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
	if(!Default)
		return 0;
	return 1;
}
bool VSDepthStencilState::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
bool VSDepthStencilState::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadDepthStencilState(this,pID))
		return 0;
	return 1;
}
bool VSDepthStencilState::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseDepthStencilState(pID))
		return 0;
	return 1;
}
VSDepthStencilState::VSDepthStencilState()
{
	m_bIsStatic = 1;
}
VSDepthStencilState::~VSDepthStencilState()
{
	ReleaseResource();
}
IMPLEMENT_RTTI(VSDepthStencilDesc,VSObject)
BEGIN_ADD_PROPERTY(VSDepthStencilDesc,VSObject)
REGISTER_PROPERTY(m_bDepthEnable,DepthEnable,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bDepthWritable,DepthWritable,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDepthCompareMethod,DepthCompareMethod,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bStencilEnable,StencilEnable,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiStencilCompareMethod,StencilCompareMethod,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiReference,Reference,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMask,Mask,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiWriteMask,WriteMask,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSPassZPassOP,SPassZPassOP,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSPassZFailOP,SPassZFailOP,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSFailZPassOP,SFailZPassOP,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_SPassZPassOP,CCW_SPassZPassOP,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_SPassZFailOP,CCW_SPassZFailOP,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_SFailZPassOP,CCW_SFailZPassOP,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bTwoSideStencilMode,TwoSideStencilMode,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_StencilCompareMethod,CCW_StencilCompareMethod,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDepthStencilDesc)
IMPLEMENT_INITIAL_END
