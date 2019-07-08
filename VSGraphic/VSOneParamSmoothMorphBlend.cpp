#include "VSOneParamSmoothMorphBlend.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOneParamSmoothMorphBlend, VSOneParamMorphBlend)
BEGIN_ADD_PROPERTY(VSOneParamSmoothMorphBlend, VSOneParamMorphBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamSmoothMorphBlend)
IMPLEMENT_INITIAL_END
VSOneParamSmoothMorphBlend::~VSOneParamSmoothMorphBlend()
{

}
VSOneParamSmoothMorphBlend::VSOneParamSmoothMorphBlend()
{

}
VSOneParamSmoothMorphBlend::VSOneParamSmoothMorphBlend(const VSUsedName & ShowName, VSMorphTree * pMorphTree)
:VSOneParamMorphBlend(ShowName, pMorphTree)
{


}
bool VSOneParamSmoothMorphBlend::Update(double dAppTime)
{
	VSOneParamMorphBlend::Update(dAppTime);

	VSREAL fInternal = m_fParamMax - m_fParamMin;
	VSREAL fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return false;
	}

	unsigned int uiIndex1, uiIndex2;
	VSREAL fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
	uiIndex1 = (unsigned int)fTemp;

	if (uiIndex1 >= m_pInput.GetNum() - 1)
	{
		uiIndex2 = uiIndex1;
	}
	else
	{
		uiIndex2 = uiIndex1 + 1;
	}


	m_fWeight = fTemp - (VSREAL)uiIndex1;


	VSInputNode* pInputNode1 = GetInputNode(uiIndex1);
	VSInputNode* pInputNode2 = GetInputNode(uiIndex2);
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		m_pMorphBaseFunction1 = (VSMorphFunction *)pInputNode1->GetOutputLink()->GetOwner();
		m_pMorphBaseFunction2 = (VSMorphFunction *)pInputNode2->GetOutputLink()->GetOwner();
	}
	else if (pInputNode1->GetOutputLink())
	{
		m_pMorphBaseFunction1 = (VSMorphFunction *)pInputNode1->GetOutputLink()->GetOwner();
		m_pMorphBaseFunction2 = NULL;
	}
	else if (pInputNode2->GetOutputLink())
	{
		m_pMorphBaseFunction2 = (VSMorphFunction *)pInputNode2->GetOutputLink()->GetOwner();
		m_pMorphBaseFunction1 = NULL;
	}
	else
	{
		m_pMorphBaseFunction1 = NULL;
		m_pMorphBaseFunction2 = NULL;
	}
	return true;
}
void VSOneParamSmoothMorphBlend::UpdateVertexDate(unsigned int uiVertexIndex)
{
	VSOneParamMorphBlend::UpdateVertexDate(uiVertexIndex);

	if (m_pMorphBaseFunction1 || m_pMorphBaseFunction2)
	{
		LineBlendTwo(this, m_pMorphBaseFunction1, m_pMorphBaseFunction2, m_fWeight);
	}
	
	return;
}

