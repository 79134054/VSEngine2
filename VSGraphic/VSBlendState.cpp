#include "VSBlendState.h"
#include "VSShader.h"
#include "VSMaterial.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSBlendState,VSBind)
VSPointer<VSBlendState> VSBlendState::Default;
BEGIN_ADD_PROPERTY(VSBlendState,VSBind)
REGISTER_PROPERTY(m_BlendDesc,BlendDesc,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBlendState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSBlendState::InitialDefaultState()
{
	VSBlendDesc BlendDesc;
	Default = VSResourceManager::CreateBlendState(BlendDesc);
	if(!Default)
		return 0;
	return 1;
}
bool VSBlendState::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
bool VSBlendState::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadBlendState(this,pID))
		return 0;
	return 1;
}
bool VSBlendState::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseBlendState(pID))
		return 0;
	return 1;
}
VSBlendState::VSBlendState()
{
	m_bIsStatic = 1;
}
VSBlendState::~VSBlendState()
{
	ReleaseResource();
}
IMPLEMENT_RTTI(VSBlendDesc,VSObject)
BEGIN_ADD_PROPERTY(VSBlendDesc,VSObject)
REGISTER_PROPERTY(bBlendEnable[0],BlendEnable0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[0],SrcBlend0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[0],DestBlend0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[0],BlendOp0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[0],AlphaBlendEnable0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[0],SrcBlendAlpha0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[0],DestBlendAlpha0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[0],BlendOpAlpha0,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[0],WriteMask0,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[1],BlendEnable1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[1],SrcBlend1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[1],DestBlend1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[1],BlendOp1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[1],AlphaBlendEnable1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[1],SrcBlendAlpha1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[1],DestBlendAlpha1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[1],BlendOpAlpha1,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[1],WriteMask1,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[2],BlendEnable2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[2],SrcBlend2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[2],DestBlend2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[2],BlendOp2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[2],AlphaBlendEnable2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[2],SrcBlendAlpha2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[2],DestBlendAlpha2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[2],BlendOpAlpha2,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[2],WriteMask2,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[3],BlendEnable3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[3],SrcBlend3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[3],DestBlend3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[3],BlendOp3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[3],AlphaBlendEnable3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[3],SrcBlendAlpha3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[3],DestBlendAlpha3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[3],BlendOpAlpha3,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[3],WriteMask3,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[4],BlendEnable4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[4],SrcBlend4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[4],DestBlend4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[4],BlendOp4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[4],AlphaBlendEnable4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[4],SrcBlendAlpha4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[4],DestBlendAlpha4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[4],BlendOpAlpha4,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[4],WriteMask4,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[5],BlendEnable5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[5],SrcBlend5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[5],DestBlend5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[5],BlendOp5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[5],AlphaBlendEnable5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[5],SrcBlendAlpha5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[5],DestBlendAlpha5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[5],BlendOpAlpha5,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[5],WriteMask5,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[6],BlendEnable6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[6],SrcBlend6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[6],DestBlend6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[6],BlendOp6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[6],AlphaBlendEnable6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[6],SrcBlendAlpha6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[6],DestBlendAlpha6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[6],BlendOpAlpha6,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[6],WriteMask6,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[7],BlendEnable7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[7],SrcBlend7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[7],DestBlend7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[7],BlendOp7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[7],AlphaBlendEnable7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[7],SrcBlendAlpha7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[7],DestBlendAlpha7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[7],BlendOpAlpha7,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[7],WriteMask7,VSProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bAlphaToCoverageEnable, AlphaToCoverageEnable, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bIndependentBlendEnable, IndependentBlendEnable, VSProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[0], BlendColor0, VSProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[1], BlendColor1, VSProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[2], BlendColor2, VSProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[3], BlendColor3, VSProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(ucSampleMask, SampleMask, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBlendDesc)
IMPLEMENT_INITIAL_END
