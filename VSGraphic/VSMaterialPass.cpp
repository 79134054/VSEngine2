#include "VSMaterialPass.h"
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
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMaterialPass,VSPass)
BEGIN_ADD_PROPERTY(VSMaterialPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMaterialPass)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(MaterialRenderPassDraw, )
#define  MAX_DYNAMIC_LIGHT 3
VSMaterialPass::VSMaterialPass()
{

	

}
VSMaterialPass::~VSMaterialPass()
{


}
bool VSMaterialPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(MaterialRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance || !m_Light.GetNum())
	{
		m_Light.Clear();
		return 0;
	}

	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.LightArray = m_Light;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	VSArray<VSLight*> LightTemp;

	
	m_RenderSceondPassUsed = pMaterial->GetRenderState(m_uiPassId);

	VSBlendDesc BlendDesc = m_RenderSceondPassUsed.GetBlendState()->GetBlendDesc();
	if (BlendDesc.bBlendEnable[0] == true)
	{
		BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_ONE;
	}
	else
	{
		BlendDesc.ucSrcBlend[0] = VSBlendDesc::BP_ONE;
		BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_ONE;
	}
	BlendDesc.bBlendEnable[0] = true;		
	BlendDesc.bAlphaBlendEnable[0] = false;

	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderSceondPassUsed.SetBlendState(pBlendState);




	for (unsigned int i = 0 ; i < m_Light.GetNum() ; i++)
	{
		LightTemp.AddElement(m_Light[i]);
		if (LightTemp.GetNum() >= MAX_DYNAMIC_LIGHT || i == m_Light.GetNum() - 1)
		{
			MSPara.LightArray = LightTemp;

			m_VShaderkey.Clear();
			m_PShaderkey.Clear();

			if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cMaterialVertex))
			{
				m_Light.Clear();
				return 0;
			}
			if (!GetPShader(MSPara,VSResourceManager::GetMaterialShaderMap(),pMaterial->GetResourceName()))
			{
				m_Light.Clear();
				return 0;
			}


			pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
			pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);


			pMaterial->SetGlobleValue(this,m_uiPassId,m_pMaterialInstance->m_pCurVShader[GetPassType()],m_pMaterialInstance->m_pCurPShader[GetPassType()]);


			if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderSceondPassUsed,
				m_pMaterialInstance->m_pCurVShader[GetPassType()],
				m_pMaterialInstance->m_pCurPShader[GetPassType()],
				m_pMaterialInstance->m_pCurGShader[GetPassType()]))
			{
				return false;
			}



			LightTemp.Clear();
		}

	}
	

	
	m_Light.Clear();
	return 1;
}

