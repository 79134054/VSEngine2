#include "VSNormalDepthPass.h"
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
IMPLEMENT_RTTI(VSNormalDepthPass,VSPass)
BEGIN_ADD_PROPERTY(VSNormalDepthPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSNormalDepthPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(NormalDepthRenderPassDraw, )
bool VSNormalDepthPass::InitialDefaultState()
{

	return 1;
}
bool VSNormalDepthPass::TerminalDefaultState()
{
	return 1;
}
VSNormalDepthPass::VSNormalDepthPass()
{


	VSBlendDesc BlendDesc;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);
	
}
VSNormalDepthPass::~VSNormalDepthPass()
{


}


bool VSNormalDepthPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(NormalDepthRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;
	
	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cNormalDepthVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetNormalDepthShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}
	
	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	pMaterial->SetGlobleValue(this,m_uiPassId,m_pMaterialInstance->m_pCurVShader[GetPassType()],m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}

