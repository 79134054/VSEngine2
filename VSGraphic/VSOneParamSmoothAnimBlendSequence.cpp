#include "VSOneParamSmoothAnimBlendSequence.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOneParamSmoothAnimBlendSequence, VSOneParamSmoothAnimBlend)
BEGIN_ADD_PROPERTY(VSOneParamSmoothAnimBlendSequence, VSOneParamSmoothAnimBlend)
REGISTER_PROPERTY(m_AnimSequenceFuncArray, AnimSequenceFuncArray, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamSmoothAnimBlendSequence)
IMPLEMENT_INITIAL_END
VSOneParamSmoothAnimBlendSequence::~VSOneParamSmoothAnimBlendSequence()
{

}
VSOneParamSmoothAnimBlendSequence::VSOneParamSmoothAnimBlendSequence()
{

}
VSOneParamSmoothAnimBlendSequence::VSOneParamSmoothAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSOneParamSmoothAnimBlend(ShowName, pAnimTree)
{
	//no need input
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		VSMAC_DELETE(m_pInput[i]);
	}
	m_pInput.Clear();

}
void VSOneParamSmoothAnimBlendSequence::SetAnimFrequency(unsigned int Index, double Frequency)
{
	if (Index >= m_AnimSequenceFuncArray.GetNum())
	{
		return ;
	}
	m_AnimSequenceFuncArray[Index]->m_dFrequency = Frequency;
}
void VSOneParamSmoothAnimBlendSequence::SetAnimPhase(unsigned int Index, double Phase)
{
	if (Index >= m_AnimSequenceFuncArray.GetNum())
	{
		return;
	}
	m_AnimSequenceFuncArray[Index]->m_dPhase = Phase;
}
bool VSOneParamSmoothAnimBlendSequence::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_AnimSequenceFuncArray.GetNum() == 0)
	{
		return false;
	}
	unsigned int Index1 = 0;
	unsigned int Index2 = 0;
	VSREAL fBlendWeight = 0.0f;
	if (m_AnimSequenceFuncArray.GetNum() > 1)
	{
		VSREAL fInternal = m_fParamMax - m_fParamMin;
		VSREAL fInternalSeg = fInternal / (m_AnimSequenceFuncArray.GetNum() - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		VSREAL fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
		Index1 = (unsigned int)fTemp;
		Index2 = Index1 + 1;
		fBlendWeight = fTemp - (VSREAL)Index1;


		if (Index2 >= m_AnimSequenceFuncArray.GetNum())
		{
			Index2 = m_AnimSequenceFuncArray.GetNum() - 1;
		}

	}

	VSREAL AnimTime1 = m_AnimSequenceFuncArray[Index1]->GetAnimTime();
	VSREAL AnimTime2 = m_AnimSequenceFuncArray[Index2]->GetAnimTime();
	VSREAL BlendTime = AnimTime1 + (AnimTime2 - AnimTime1) * fBlendWeight;
	VSREAL AnimTimeScale1 = AnimTime1 / BlendTime;
	VSREAL AnimTimeScale2 = AnimTime2 / BlendTime;
	m_AnimSequenceFuncArray[Index1]->m_fInnerTimeScale = AnimTimeScale1;
	m_AnimSequenceFuncArray[Index2]->m_fInnerTimeScale = AnimTimeScale2;

	m_AnimSequenceFuncArray[Index1]->ClearFlag();
	m_AnimSequenceFuncArray[Index2]->ClearFlag();
	m_AnimSequenceFuncArray[Index1]->Update(dAppTime);
	m_AnimSequenceFuncArray[Index2]->Update(dAppTime);

	m_AnimSequenceFuncArray[Index1]->m_fInnerTimeScale = 1.0f;
	m_AnimSequenceFuncArray[Index2]->m_fInnerTimeScale = 1.0f;

	VSAnimSequenceFunc* pAnimBaseFunction1 = m_AnimSequenceFuncArray[Index1];
	VSAnimSequenceFunc* pAnimBaseFunction2 = m_AnimSequenceFuncArray[Index2];

	LineBlendTwo(this, pAnimBaseFunction1, pAnimBaseFunction2, fBlendWeight);
	LineBlendTwo(m_RootAtom, pAnimBaseFunction1, pAnimBaseFunction2, fBlendWeight);
	return 1;
}
void VSOneParamSmoothAnimBlendSequence::CreateSlot(unsigned int uiWidth)
{
	m_AnimSequenceFuncArray.Clear();
	for (unsigned int i = 0; i < uiWidth; i++)
	{
		VSAnimSequenceFunc * pAnimSequenceFun = NULL;
		if (GetSkelectonMeshNode())
		{
			pAnimSequenceFun = VS_NEW VSAnimSequenceFunc(GetSkelectonMeshNode());
		}
		else
		{
			pAnimSequenceFun = VS_NEW VSAnimSequenceFunc();
		}
		m_AnimSequenceFuncArray.AddElement(pAnimSequenceFun);
	}
}
void VSOneParamSmoothAnimBlendSequence::SetAnim(unsigned int i, const VSUsedName &AnimName)
{
	if (i >= m_AnimSequenceFuncArray.GetNum())
	{
		return;
	}
	m_AnimSequenceFuncArray[i]->SetAnim(AnimName);
}
bool VSOneParamSmoothAnimBlendSequence::SetObject(VSObject * pObject)
{
	if (VSOneParamSmoothAnimBlend::SetObject(pObject))
	{
		for (unsigned int i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
		{
			if (m_AnimSequenceFuncArray[i])
			{
				m_AnimSequenceFuncArray[i]->SetObject(pObject);
			}
		}
		return true;
	}

	return false;
}