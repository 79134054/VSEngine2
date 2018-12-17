#include "VSOneParamSmoothAnimBlend.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOneParamSmoothAnimBlend, VSOneParamAnimBlend)
BEGIN_ADD_PROPERTY(VSOneParamSmoothAnimBlend, VSOneParamAnimBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamSmoothAnimBlend)
IMPLEMENT_INITIAL_END
VSOneParamSmoothAnimBlend::~VSOneParamSmoothAnimBlend()
{

}
VSOneParamSmoothAnimBlend::VSOneParamSmoothAnimBlend()
{

}
VSOneParamSmoothAnimBlend::VSOneParamSmoothAnimBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSOneParamAnimBlend(ShowName, pAnimTree)
{


}

bool VSOneParamSmoothAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	VSREAL fInternal = m_fParamMax - m_fParamMin;
	VSREAL fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return 0;
	}
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;

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


	VSREAL fWeight = fTemp - (VSREAL)uiIndex1;


	VSInputNode* pInputNode1 = GetInputNode(uiIndex1);
	VSInputNode* pInputNode2 = GetInputNode(uiIndex2);
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		VSAnimFunction *pAnimBaseFunction2 = (VSAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		LineBlendTwo(this, pAnimBaseFunction1, pAnimBaseFunction2, fWeight);
		LineBlendTwo(m_RootAtom, pAnimBaseFunction1, pAnimBaseFunction2, fWeight);
	}
	else if (pInputNode1->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();

		LineBlendTwo(this, pAnimBaseFunction1, NULL, 0.0f);
		LineBlendTwo(m_RootAtom, pAnimBaseFunction1, NULL, 0.0f);
	}
	else if (pInputNode2->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction2 = (VSAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		LineBlendTwo(this, NULL, pAnimBaseFunction2, 0.0f);
		LineBlendTwo(m_RootAtom, NULL, pAnimBaseFunction2, 0.0f);
	}
	return 1;
}
