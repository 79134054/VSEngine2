#include "VSTwoParamAnimBlendSequence.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTwoParamAnimBlendSequence, VSTwoParamAnimBlend)
BEGIN_ADD_PROPERTY(VSTwoParamAnimBlendSequence, VSTwoParamAnimBlend)
REGISTER_PROPERTY(m_AnimSequenceFuncArray, AnimSequenceFuncArray, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTwoParamAnimBlendSequence)
IMPLEMENT_INITIAL_END
VSTwoParamAnimBlendSequence::~VSTwoParamAnimBlendSequence()
{

}
VSTwoParamAnimBlendSequence::VSTwoParamAnimBlendSequence()
{

	
}
VSTwoParamAnimBlendSequence::VSTwoParamAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSTwoParamAnimBlend(ShowName, pAnimTree)
{
	//no need input
	m_pInput.Clear();
	
	
}
void VSTwoParamAnimBlendSequence::SetAnimFrequency(unsigned int i, unsigned int j, double Frequency)
{
	if (j >= m_uiWidth || i >= m_uiHeight)
	{
		return;
	}
	m_AnimSequenceFuncArray[i * m_uiWidth + j]->m_dFrequency = Frequency;
}
void VSTwoParamAnimBlendSequence::SetAnimPhase(unsigned int i, unsigned int j, double Phase)
{
	if (j >= m_uiWidth || i >= m_uiHeight)
	{
		return;
	}
	m_AnimSequenceFuncArray[i * m_uiWidth + j]->m_dPhase = Phase;
}
bool VSTwoParamAnimBlendSequence::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_AnimSequenceFuncArray.GetNum() == 0)
	{
		return false;
	}

	unsigned int Index1[2] = { 0 ,0};
	unsigned int Index2[2] = { 0, 0 };
	VSREAL fWeight[2] = {0.0f,0.0f};
	if (m_uiWidth > 1)
	{
		VSREAL fInternal = m_fParamMax[0] - m_fParamMin[0];
		VSREAL fInternalSeg = fInternal / (m_uiWidth - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		VSREAL fTemp = (m_fParam[0] - m_fParamMin[0]) / fInternalSeg;
		Index1[0] = (unsigned int)fTemp;
		Index2[0] = Index1[0] + 1;
		fWeight[0] = fTemp - (VSREAL)Index1[0];
	}
	if (m_uiHeight > 1)
	{
		VSREAL fInternal = m_fParamMax[1] - m_fParamMin[1];
		VSREAL fInternalSeg = fInternal / (m_uiHeight - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		VSREAL fTemp = (m_fParam[1] - m_fParamMin[1]) / fInternalSeg;
		Index1[1] = (unsigned int)fTemp;
		Index2[1] = Index1[1] + 1;
		fWeight[1] = fTemp - (VSREAL)Index1[1];
	}

	VSAnimSequenceFunc *pAnimBaseFunction1 = GetAnimSequenceFuntion(Index1[1], Index1[0]);
	VSAnimSequenceFunc *pAnimBaseFunction2 = GetAnimSequenceFuntion(Index1[1], Index2[0]);
	VSAnimSequenceFunc *pAnimBaseFunction3 = GetAnimSequenceFuntion(Index2[1], Index1[0]);
	VSAnimSequenceFunc *pAnimBaseFunction4 = GetAnimSequenceFuntion(Index2[1], Index2[0]);

	VSREAL AnimTime1 = pAnimBaseFunction1->GetAnimTime();
	VSREAL AnimTime2 = pAnimBaseFunction2->GetAnimTime();
	VSREAL BlendTime1 = AnimTime1 + (AnimTime2 - AnimTime1) * fWeight[0];

	VSREAL AnimTime3 = pAnimBaseFunction3->GetAnimTime();
	VSREAL AnimTime4 = pAnimBaseFunction4->GetAnimTime();
	VSREAL BlendTime2 = AnimTime3 + (AnimTime4 - AnimTime3) * fWeight[0];

	VSREAL BlendTime = BlendTime1 + (BlendTime2 - BlendTime1) * fWeight[1];

	VSREAL AnimTimeScale1 = AnimTime1 / BlendTime;
	VSREAL AnimTimeScale2 = AnimTime2 / BlendTime;
	VSREAL AnimTimeScale3 = AnimTime3 / BlendTime;
	VSREAL AnimTimeScale4 = AnimTime4 / BlendTime;

	pAnimBaseFunction1->m_fInnerTimeScale = AnimTimeScale1;
	pAnimBaseFunction2->m_fInnerTimeScale = AnimTimeScale2;
	pAnimBaseFunction3->m_fInnerTimeScale = AnimTimeScale3;
	pAnimBaseFunction4->m_fInnerTimeScale = AnimTimeScale4;

	pAnimBaseFunction1->ClearFlag();
	pAnimBaseFunction2->ClearFlag();
	pAnimBaseFunction3->ClearFlag();
	pAnimBaseFunction4->ClearFlag();

	pAnimBaseFunction1->Update(dAppTime);
	pAnimBaseFunction2->Update(dAppTime);
	pAnimBaseFunction3->Update(dAppTime);
	pAnimBaseFunction4->Update(dAppTime);



	pAnimBaseFunction1->m_fInnerTimeScale = 1.0f;
	pAnimBaseFunction2->m_fInnerTimeScale = 1.0f;
	pAnimBaseFunction3->m_fInnerTimeScale = 1.0f;
	pAnimBaseFunction4->m_fInnerTimeScale = 1.0f;

	
	LineBlendTwo(&m_BlendBoneMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
	LineBlendTwo(&m_BlendBoneMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
	LineBlendTwo(&m_BoneOutPut, &m_BlendBoneMatrix[0], &m_BlendBoneMatrix[1], fWeight[1]);

	LineBlendTwo(m_BlendRootMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
	LineBlendTwo(m_BlendRootMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
	m_RootAtom.Interpolation(m_BlendRootMatrix[0], m_BlendRootMatrix[1], fWeight[1]);
	return 1;
}
void VSTwoParamAnimBlendSequence::CreateSlot(unsigned int uiWidth, unsigned int uiHeight)
{
	m_AnimSequenceFuncArray.Clear();
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	for (unsigned int i = 0; i < uiWidth;i++)
	{
		for (unsigned int j = 0; j < uiHeight; j++)
		{
			VSAnimSequenceFunc * pAnimSequenceFun = VS_NEW VSAnimSequenceFunc(GetSkelectonMeshNode());
			m_AnimSequenceFuncArray.AddElement(pAnimSequenceFun);
		}
	}
}
void VSTwoParamAnimBlendSequence::SetAnim(unsigned int i , unsigned int j, const VSUsedName &AnimName)
{
	if ( j >= m_uiWidth || i >= m_uiHeight)
	{
		return;
	}
	m_AnimSequenceFuncArray[i * m_uiWidth + j]->SetAnim(AnimName);
}

VSAnimSequenceFunc * VSTwoParamAnimBlendSequence::GetAnimSequenceFuntion(unsigned int i, unsigned int j)
{
	return m_AnimSequenceFuncArray[i * m_uiWidth + j];
}