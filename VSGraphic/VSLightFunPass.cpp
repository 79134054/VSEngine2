#include "VSLightFunPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSLightFunPass,VSPass)
BEGIN_ADD_PROPERTY(VSLightFunPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSLightFunPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(LightFunPRenderPassDraw, )
bool VSLightFunPass::InitialDefaultState()
{

	return 1;
}
bool VSLightFunPass::TerminalDefaultState()
{
	return 1;
}
VSLightFunPass::VSLightFunPass()
{



}
VSLightFunPass::~VSLightFunPass()
{


}


bool VSLightFunPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(LightFunPRenderPassDraw)
	if(!pRenderer || !m_pSpatial || !m_pMaterialInstance)
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
	MSPara.pShadowLight = m_pLight;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cLightFunVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetMaterialShaderMap(),pMaterial->GetResourceName()))
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

