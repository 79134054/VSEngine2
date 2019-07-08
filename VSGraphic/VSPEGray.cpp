#include "VSPEGray.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPEGray,VSPostEffectFunction)
BEGIN_ADD_PROPERTY(VSPEGray,VSPostEffectFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPEGray)
IMPLEMENT_INITIAL_END
VSPEGray::VSPEGray (const VSUsedName & ShowName,VSPostEffectSet * pPostEffectSet)
:VSPostEffectFunction(ShowName,pPostEffectSet)
{

	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::PET_OUT,_T("InputColor"),this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	VSOutputNode * pOutNode = NULL;
	pOutNode = VS_NEW VSOutputNode(VSPutNode::PET_OUT,_T("OutColor"),this);
	VSMAC_ASSERT(pOutNode);
	m_pOutput.AddElement(pOutNode);
}
VSPEGray::~VSPEGray ()
{

}
VSPEGray::VSPEGray ()
{
}
VSPostEffectSceneRender * VSPEGray::CreateSceneRender()
{

	if (!m_pPostEffectRender)
	{
		m_pPostEffectRender = VS_NEW VSPEGraySceneRender();
	}
	
	return m_pPostEffectRender;
}
void VSPEGray::OnDraw(VSCuller & Culler,double dAppTime)
{
	VSRenderTarget * pTarget = GetRenderTarget(m_pInput[INPUT_COLOR]);
	if (!pTarget)
	{
		return ;
	}
	if (m_pPostEffectRender)
	{
		VSPostEffectSceneRender * pTemp = m_pPostEffectRender;
		((VSPEGraySceneRender*) pTemp)->SetSourceTarget(pTarget->GetCreateBy());
		m_pPostEffectRender->Draw(Culler,dAppTime);
	}
}