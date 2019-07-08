#include "VSSamplerState.h"
#include "VSShader.h"
#include "VSMaterial.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSamplerState,VSBind)
VSPointer<VSSamplerState> VSSamplerState::Default;
VSPointer<VSSamplerState> VSSamplerState::DoubleLine;
VSPointer<VSSamplerState> VSSamplerState::TriLine;
VSPointer<VSSamplerState> VSSamplerState::ShadowMapSampler;
VSPointer<VSSamplerState> VSSamplerState::BorderARGB0Sampler;
VSPointer<VSSamplerState> VSSamplerState::LightFunctionSampler;
BEGIN_ADD_PROPERTY(VSSamplerState,VSBind)
REGISTER_PROPERTY(m_SamplerDesc,SamplerDesc,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSamplerState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSSamplerState::InitialDefaultState()
{
	VSSamplerDesc SamplerDesc;
	Default = VSResourceManager::CreateSamplerState(SamplerDesc);
	if(!Default)
		return 0;


	SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	DoubleLine = VSResourceManager::CreateSamplerState(SamplerDesc);
	if(!DoubleLine)
		return 0;


	SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
	TriLine = VSResourceManager::CreateSamplerState(SamplerDesc);
	if(!TriLine)
		return 0;


	VSSamplerDesc ShadowSamplerDesc;
	ShadowSamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	ShadowSamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	ShadowSamplerDesc.m_uiCoordU = VSSamplerDesc::CM_BORDER;
	ShadowSamplerDesc.m_uiCoordV = VSSamplerDesc::CM_BORDER;
	ShadowSamplerDesc.m_BorderColor = VSColorRGBA(1.0f,1.0f,1.0f,1.0f);
	ShadowMapSampler = VSResourceManager::CreateSamplerState(ShadowSamplerDesc);
	if (!ShadowMapSampler)
		return 0;

	VSSamplerDesc BorderARGB0Desc;
	BorderARGB0Desc.m_uiMag = VSSamplerDesc::FM_LINE;
	BorderARGB0Desc.m_uiMin = VSSamplerDesc::FM_LINE;
	BorderARGB0Desc.m_uiCoordU = VSSamplerDesc::CM_BORDER;
	BorderARGB0Desc.m_uiCoordV = VSSamplerDesc::CM_BORDER;
	BorderARGB0Desc.m_BorderColor = VSColorRGBA(1.0f,0.0f,0.0f,0.0f);
	BorderARGB0Sampler = VSResourceManager::CreateSamplerState(BorderARGB0Desc);
	if (!BorderARGB0Sampler)
		return 0;

	VSSamplerDesc LightFunctionDesc;
	LightFunctionDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	LightFunctionDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	LightFunctionDesc.m_uiCoordU = VSSamplerDesc::CM_BORDER;
	LightFunctionDesc.m_uiCoordV = VSSamplerDesc::CM_BORDER;
	LightFunctionSampler = VSResourceManager::CreateSamplerState(LightFunctionDesc);
	if (!LightFunctionSampler)
		return 0;
	return 1;
}
bool VSSamplerState::TerminalDefaultState()
{
	Default = NULL;
	DoubleLine = NULL;
	TriLine = NULL;
	ShadowMapSampler = NULL;
	BorderARGB0Sampler = NULL;
	LightFunctionSampler = NULL;
	return 1;
}
bool VSSamplerState::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadSamplerState(this,pID))
		return 0;
	return 1;
}
bool VSSamplerState::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseSamplerState(pID))
		return 0;
	return 1;
}
VSSamplerState::VSSamplerState()
{
	m_bIsStatic = 1;
}
VSSamplerState::~VSSamplerState()
{
	ReleaseResource();
}

IMPLEMENT_RTTI(VSSamplerDesc,VSObject)
BEGIN_ADD_PROPERTY(VSSamplerDesc,VSObject)
REGISTER_PROPERTY(m_uiMag,Mag,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMin,Min,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMip,Mip,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMipLevel,MipLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiAniLevel,AniLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordU,CoordU,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordV,CoordV,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordW,CoordW,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_BorderColor,BorderColor,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MipLODBias, MipLODBias, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSamplerDesc)
IMPLEMENT_INITIAL_END
