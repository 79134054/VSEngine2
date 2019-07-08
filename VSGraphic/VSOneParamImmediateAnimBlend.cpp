#include "VSOneParamImmediateAnimBlend.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOneParamImmediateAnimBlend, VSOneParamAnimBlend)
BEGIN_ADD_PROPERTY(VSOneParamImmediateAnimBlend, VSOneParamAnimBlend)
REGISTER_PROPERTY(m_NodeCrossFadingTime, NodeCrossFadingTime, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamImmediateAnimBlend)
IMPLEMENT_INITIAL_END
VSOneParamImmediateAnimBlend::~VSOneParamImmediateAnimBlend()
{

}
VSOneParamImmediateAnimBlend::VSOneParamImmediateAnimBlend()
{
	m_uiLastAnimIndex = VSMAX_INTEGER;
	m_fCrossFadingTime = 0.0f;
	m_uiCurAnimIndex = 0;
}
VSOneParamImmediateAnimBlend::VSOneParamImmediateAnimBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSOneParamAnimBlend(ShowName, pAnimTree)
{

	m_NodeCrossFadingTime.AddElement(0.0f);
	m_NodeCrossFadingTime.AddElement(0.0f);
	m_uiLastAnimIndex = VSMAX_INTEGER;
	m_fCrossFadingTime = 0.0f;
}
void VSOneParamImmediateAnimBlend::AddInputNode()
{
	VSString InputName = _T("Child");
	VSString ID = IntToString(m_pInput.GetNum());
	InputName += ID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	m_NodeCrossFadingTime.AddElement(0.0f);
}
void VSOneParamImmediateAnimBlend::DeleteInputNode()
{
	if (m_pInput.GetNum() > 2)
	{
		VSInputNode * pInputNode = m_pInput[m_pInput.GetNum() - 1];

		m_pInput.Erase(m_pInput.GetNum() - 1);
		if (!pInputNode)
		{
			return;
		}
		VSMAC_DELETE(pInputNode);
		m_NodeCrossFadingTime.Erase(m_NodeCrossFadingTime.GetNum() - 1);
	}
}
bool VSOneParamImmediateAnimBlend::Update(double dAppTime)
{
	if (!VSAnimBaseFunction::Update(dAppTime))
	{
		return 0;

	}
	if (!ComputePara(dAppTime))
		return 0;
	

	VSREAL fInternal = m_fParamMax - m_fParamMin;
	VSREAL fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return 0;
	}


	VSREAL fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
	m_uiCurAnimIndex = (unsigned int)fTemp;
	if (m_uiLastAnimIndex == VSMAX_INTEGER)
	{
		m_uiLastAnimIndex = m_uiCurAnimIndex;
	}
	if (m_uiLastAnimIndex != m_uiCurAnimIndex && m_fCrossFadingTime < 0.00001f)
	{
		VSArray<VSAnimBaseFunction *> CurLeafNodeArray;
		if (m_pInput[m_uiCurAnimIndex]->GetOutputLink())
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[m_uiCurAnimIndex]->GetOutputLink()->GetOwner();
			pAnimBaseFunction->GetLeafArray(CurLeafNodeArray);
		}	
		VSArray<VSAnimBaseFunction *> LastLeafNodeArray;
		if (m_pInput[m_uiLastAnimIndex]->GetOutputLink())
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[m_uiLastAnimIndex]->GetOutputLink()->GetOwner();
			pAnimBaseFunction->GetLeafArray(LastLeafNodeArray);
		}
		if (LastLeafNodeArray.GetNum() && CurLeafNodeArray.GetNum())
		{
			for (unsigned int i = 0; i < CurLeafNodeArray.GetNum(); i++)
			{
				bool bFound = false;
				for (unsigned int j = 0; j < LastLeafNodeArray.GetNum(); j++)
				{
					if (CurLeafNodeArray[i] == LastLeafNodeArray[j])
					{
						CurLeafNodeArray[i]->NoLeafStart();
						bFound = true;
						break;
					}
				}
				if (bFound == false)
				{
					CurLeafNodeArray[i]->EnableLeafStart();
				}
			}
		}
		
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink() )
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction)
			{
				if (i == m_uiLastAnimIndex || i == m_uiCurAnimIndex)
				{
					pAnimBaseFunction->Update(dAppTime);
				}
			}
		}
	}

	if (!ComputeOutBoneMatrix(dAppTime))
		return 0;
	return 1;
}
bool VSOneParamImmediateAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	


	if (m_uiLastAnimIndex != m_uiCurAnimIndex)
	{
		if (m_fCrossFadingTime < m_NodeCrossFadingTime[m_uiCurAnimIndex])
		{
			VSREAL fWeight = 0.0f;
// 			if (m_NodeCrossFadingTime[uiIndex1] > EPSILON_E4)
// 			{
			fWeight = m_fCrossFadingTime / m_NodeCrossFadingTime[m_uiCurAnimIndex];
		/*	}*/

			VSInputNode* pInputNode1 = GetInputNode(m_uiLastAnimIndex);
			VSInputNode* pInputNode2 = GetInputNode(m_uiCurAnimIndex);
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
				LineBlendTwo(this, NULL, pAnimBaseFunction2,0.0f);
				LineBlendTwo(m_RootAtom, NULL, pAnimBaseFunction2, 0.0f);
			}
			m_fCrossFadingTime += (VSREAL)m_dIntervalTime;
		}
		else
		{
			m_uiLastAnimIndex = m_uiCurAnimIndex;
			m_fCrossFadingTime = 0.0f;
			VSInputNode* pInputNode1 = GetInputNode(m_uiCurAnimIndex);
			if (pInputNode1->GetOutputLink())
			{
				VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
				LineBlendTwo(this, pAnimBaseFunction1, NULL, 0.0f);
				LineBlendTwo(m_RootAtom, pAnimBaseFunction1, NULL, 0.0f);
			}
		}
	}
	else
	{
		m_fCrossFadingTime = 0.0f;
		VSInputNode* pInputNode1 = GetInputNode(m_uiCurAnimIndex);
		if (pInputNode1->GetOutputLink())
		{
			VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
			LineBlendTwo(this, pAnimBaseFunction1, NULL, 0.0f);
			LineBlendTwo(m_RootAtom, pAnimBaseFunction1, NULL, 0.0f);
		}

	}

	return 1;
}