#include "VSPostEffectPass.h"
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
IMPLEMENT_RTTI(VSPostEffectPass, VSPass)
BEGIN_ADD_PROPERTY(VSPostEffectPass, VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPostEffectPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(PostEffectRenderPassDraw, )
bool VSPostEffectPass::InitialDefaultState()
{

	return 1;
}
bool VSPostEffectPass::TerminalDefaultState()
{
	return 1;
}
VSPostEffectPass::VSPostEffectPass()
{


	MSPara.m_VSShaderPath = _T("PostEffectVShader.txt");
	MSPara.m_VMainFunName = _T("VSMain");

}
VSPostEffectPass::~VSPostEffectPass()
{


}


bool VSPostEffectPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(PostEffectRenderPassDraw)
	if (!pRenderer || !m_pSpatial || !m_pMaterialInstance)
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
	if (!GetVShader(MSPara, VSResourceManager::GetVertexShaderMap(), VSUsedName::ms_cPostEffectVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara, VSResourceManager::GetMaterialShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara, GetPassType(), m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara, GetPassType(), m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	SetCustomConstant(MSPara, m_pMaterialInstance->m_pCurVShader[GetPassType()], m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	if (!pRenderer->DrawMesh((VSGeometry *)m_pSpatial, &m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
void VSPostEffectPass::SetCustomConstant(MaterialShaderPara &MSPara, VSVShader * pVShader, VSPShader * pPShader)
{
	//VS
	VSREAL Inv_Width = 1.0f / m_uiRTWidth;
	VSREAL Inv_Height = 1.0f / m_uiRTHeight;
	pVShader->SetParam(VSUsedName::ms_cPostInv_Width, &Inv_Width);
	pVShader->SetParam(VSUsedName::ms_cPostInv_Height, &Inv_Height);
	static VSUsedName PSColorBufferSampler = VSShaderStringFactory::ms_PSColorBufferSampler;
	pPShader->SetParam(PSColorBufferSampler, m_PColorBuffer);
}

