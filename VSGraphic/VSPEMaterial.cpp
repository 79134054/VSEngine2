#include "VSPEMaterial.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPEMaterial, VSPostEffectFunction)
BEGIN_ADD_PROPERTY(VSPEMaterial, VSPostEffectFunction)
REGISTER_PROPERTY(m_pMaterialInstance, MaterialInstance, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPEMaterial)
IMPLEMENT_INITIAL_END
VSPEMaterial::VSPEMaterial(const VSUsedName & ShowName, VSPostEffectSet * pPostEffectSet)
:VSPostEffectFunction(ShowName, pPostEffectSet)
{

	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::PET_OUT, _T("InputColor"), this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	VSOutputNode * pOutNode = NULL;
	pOutNode = VS_NEW VSOutputNode(VSPutNode::PET_OUT, _T("OutColor"), this);
	VSMAC_ASSERT(pOutNode);
	m_pOutput.AddElement(pOutNode);
	m_pMaterialInstance = NULL;
}
VSPEMaterial::~VSPEMaterial()
{

}
VSPEMaterial::VSPEMaterial()
{
	m_pMaterialInstance = NULL;
}
VSPostEffectSceneRender * VSPEMaterial::CreateSceneRender()
{

	if (!m_pPostEffectRender)
	{
		m_pPostEffectRender = VS_NEW VSPEMaterialSceneRender();
	}

	return m_pPostEffectRender;
}
void VSPEMaterial::SetMaterial(VSMaterialR * pMaterial)
{
	m_pMaterialInstance = VS_NEW VSMaterialInstance(pMaterial);
}
void VSPEMaterial::OnDraw(VSCuller & Culler, double dAppTime)
{
	VSRenderTarget * pTarget = GetRenderTarget(m_pInput[INPUT_COLOR]);
	if (!pTarget || !m_pMaterialInstance)
	{
		return;
	}
	if (m_pPostEffectRender)
	{
		VSPostEffectSceneRender * pTemp = m_pPostEffectRender;
		((VSPEMaterialSceneRender*)pTemp)->SetSourceTarget(pTarget->GetCreateBy());
		((VSPEMaterialSceneRender*)pTemp)->m_pMaterialInstacne = m_pMaterialInstance;
		m_pPostEffectRender->Draw(Culler, dAppTime);
	}
}