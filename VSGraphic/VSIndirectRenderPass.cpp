#include "VSIndirectRenderPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
#include "VSSortLight.h"
#include "VSSkyLight.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSIndirectRenderPass,VSPass)
BEGIN_ADD_PROPERTY(VSIndirectRenderPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSIndirectRenderPass)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(IndirectRenderPassDraw, )
VSIndirectRenderPass::VSIndirectRenderPass()
{



}
VSIndirectRenderPass::~VSIndirectRenderPass()
{

}
bool VSIndirectRenderPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(IndirectRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
	{
		m_Light.Clear();
		return 0;
	}

	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}
	
	VSColorRGBA SkyLightUpColor = VSColorRGBA(0.0f,0.0f,0.0f,0.0f);
	VSColorRGBA SkyLightDownColor = VSColorRGBA(0.0f,0.0f,0.0f,0.0f);

	for (unsigned int i = 0 ; i < m_Light.GetNum() ; i++)
	{
		if (m_Light[i]->GetLightType() == VSLight::LT_SKY)
		{
			SkyLightUpColor += ((VSSkyLight *)m_Light[i])->m_UpColor;
			SkyLightDownColor +=((VSSkyLight *)m_Light[i])->m_DownColor;
		}
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.LightArray = m_Light;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.m_SkyLightUpColor = SkyLightUpColor;
	MSPara.m_SkyLightDownColor = SkyLightDownColor;

	
	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara,VSResourceManager::GetInnerVertexShaderMap(),VSUsedName::ms_cMaterialVertex))
	{
		m_Light.Clear();
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetIndirectShaderMap(),pMaterial->GetResourceName()))
	{
		m_Light.Clear();
		return 0;
	}


	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);




	pMaterial->SetGlobleValue(this,m_uiPassId,m_pMaterialInstance->m_pCurVShader[GetPassType()],m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&pMaterial->GetRenderState(m_uiPassId),
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	


	m_Light.Clear();
	return 1;
}

