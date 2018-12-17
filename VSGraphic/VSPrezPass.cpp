#include "VSPrezPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPrezPass,VSPass)
VSPointer<VSPrezPass> VSPrezPass::Default;
BEGIN_ADD_PROPERTY(VSPrezPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPrezPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(PrezRenderPassDraw, )
bool VSPrezPass::InitialDefaultState()
{
	Default = VS_NEW VSPrezPass();
	if(!Default)
		return 0;
	return 1;
}
bool VSPrezPass::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
VSPrezPass::VSPrezPass()
{


	VSBlendDesc BlendDesc;
	BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);

}
VSPrezPass::~VSPrezPass()
{


}


bool VSPrezPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(PrezRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara,VSResourceManager::GetInnerVertexShaderMap(),VSUsedName::ms_cPrezVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetInnerPixelShaderMap(),VSUsedName::ms_cPrezPiexl))
	{
		return 0;
	}
	
	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}

