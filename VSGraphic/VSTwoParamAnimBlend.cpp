#include "VSTwoParamAnimBlend.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTwoParamAnimBlend, VSAnimBlendFunction)
IMPLEMENT_INITIAL_BEGIN(VSTwoParamAnimBlend)
IMPLEMENT_INITIAL_END
BEGIN_ADD_PROPERTY(VSTwoParamAnimBlend,VSAnimBlendFunction)
REGISTER_PROPERTY(m_fParamMax[0],ParamMaxX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMax[1],ParamMaxY,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMin[0],ParamMinX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMin[1],ParamMinY,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiWidth, uiWidth, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiHeight, uiHeight, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSTwoParamAnimBlend::~VSTwoParamAnimBlend()
{

}
VSTwoParamAnimBlend::VSTwoParamAnimBlend()
{
	m_fParam[0] = m_fParam[1] =0.0f;
	m_fParamMax[0] = m_fParamMax[0] =1.0f;
	m_fParamMin[1] = m_fParamMin[1] =-1.0f;
	m_uiWidth = 0;
	m_uiHeight = 0;
}
VSTwoParamAnimBlend::VSTwoParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBlendFunction(ShowName,pAnimTree)
{
	m_fParam[0] = m_fParam[1] =0.0f;
	m_fParamMax[0] = m_fParamMax[0] =1.0f;
	m_fParamMin[1] = m_fParamMin[1] =-1.0f;
	m_uiWidth = 0;
	m_uiHeight = 0;
	unsigned int BoneNum = GetSkelectonMeshNode()->GetSkelecton()->GetBoneNum();
	m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
	m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
}
bool VSTwoParamAnimBlend::ComputePara(double dAppTime)
{
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		if (m_fParam[i] < m_fParamMin[i])
		{
			m_fParam[i] = m_fParamMin[i];
		}
		else if (m_fParam[i] > m_fParamMax[i])
		{
			m_fParam[i] = m_fParamMax[i];
		}
	}
	
	return 1;
}
bool VSTwoParamAnimBlend::SetObject(VSObject * pObject)
{
	if (VSAnimBlendFunction::SetObject(pObject))
	{
		const VSSkelectonMeshNode* pMesh = GetSkelectonMeshNode();
		VSMAC_ASSERT(pMesh);

		VSSkelecton * pSkelecton = pMesh->GetSkelecton();
		VSMAC_ASSERT(pSkelecton);

		unsigned int BoneNum = pSkelecton->GetBoneNum();
		m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
		m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
		return true;
	}


	return false;
}
void VSTwoParamAnimBlend::CreateSlot(unsigned int uiWidth, unsigned int uiHeight)
{
	m_pInput.Clear();
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	for (unsigned int i = 0; i < uiWidth; i++)
	{
		for (unsigned int j = 0; j < uiHeight; j++)
		{
			VSString InputName = _T("Child");
			VSString ID = IntToString(i);
			InputName += ID;
			ID = IntToString(j);
			InputName += ID;
			VSInputNode * pInputNode = NULL;
			pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName, this);
			VSMAC_ASSERT(pInputNode);
			m_pInput.AddElement(pInputNode);
		}
	}
}
bool VSTwoParamAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_pInput.GetNum() == 0)
	{
		return false;
	}

	unsigned int Index1[2] = { 0, 0 };
	unsigned int Index2[2] = { 0, 0 };
	VSREAL fWeight[2] = { 0.0f, 0.0f };
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

	VSAnimFunction *pAnimBaseFunction1 = GetAnimFuntion(Index1[1], Index1[0]);
	VSAnimFunction *pAnimBaseFunction2 = GetAnimFuntion(Index1[1], Index2[0]);
	VSAnimFunction *pAnimBaseFunction3 = GetAnimFuntion(Index2[1], Index1[0]);
	VSAnimFunction *pAnimBaseFunction4 = GetAnimFuntion(Index2[1], Index2[0]);





	LineBlendTwo(&m_BlendBoneMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
	LineBlendTwo(&m_BlendBoneMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
	LineBlendTwo(&m_BoneOutPut, &m_BlendBoneMatrix[0], &m_BlendBoneMatrix[1], fWeight[1]);

	LineBlendTwo(m_BlendRootMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
	LineBlendTwo(m_BlendRootMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
	m_RootAtom.Interpolation(m_BlendRootMatrix[0], m_BlendRootMatrix[1], fWeight[1]);
	return 1;
}
VSAnimFunction * VSTwoParamAnimBlend::GetAnimFuntion(unsigned int i, unsigned int j)
{
	VSInputNode* pInputNode1 = GetInputNode(i * m_uiWidth + j);
	if (pInputNode1)
	{
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		return pAnimBaseFunction1;
	}
	return NULL;
}