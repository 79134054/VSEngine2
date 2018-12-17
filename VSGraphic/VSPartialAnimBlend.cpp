#include "VSPartialAnimBlend.h"
#include "VSSkelectonMeshNode.h"
#include "VSGraphicInclude.h"
#include "VSBoneNode.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPartialAnimBlend,VSAnimBlendFunction)
BEGIN_ADD_PROPERTY(VSPartialAnimBlend,VSAnimBlendFunction)
REGISTER_PROPERTY(m_BoneWeight, BoneWeight, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPartialAnimBlend)
IMPLEMENT_INITIAL_END
VSPartialAnimBlend::~VSPartialAnimBlend()
{
	
	m_BoneWeight.Clear();
}
VSPartialAnimBlend::VSPartialAnimBlend()
{
	m_BoneWeight.Clear();
}
VSPartialAnimBlend::VSPartialAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBlendFunction(ShowName,pAnimTree)
{
	
	VSString InputName0 = _T("Child0");
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName0,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);



	VSString InputName1 = _T("Child1");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName1,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


}
void VSPartialAnimBlend::AddInputNode()
{
	VSString InputName = _T("Child");
	VSString ID = IntToString(m_pInput.GetNum()); 
	InputName += ID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
	{
		m_BoneWeight[i].Value.AddElement(0.0f);
	}

}
void VSPartialAnimBlend::DeleteInputNode()
{
	if (m_pInput.GetNum() > 2)
	{
		VSInputNode * pInputNode = m_pInput[m_pInput.GetNum() - 1];

		m_pInput.Erase(m_pInput.GetNum() - 1);
		if (!pInputNode)
		{
			return ;
		}
		VSMAC_DELETE(pInputNode);

		for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
		{
			m_BoneWeight[i].Value.Erase(m_BoneWeight[i].Value.GetNum() - 1);
		}
	}
}
void VSPartialAnimBlend::AddControllBone(const VSUsedName & BoneName)
{
	VSArray<VSREAL> Temp;
	Temp.SetBufferNum(m_pInput.GetNum());
	for (unsigned int i = 0; i < Temp.GetNum(); i++)
	{
		if (i == 0)
		{
			Temp[i] = 1.0f;
		}
		else
		{
			Temp[i] = 0.0f;
		}
	}
	m_BoneWeight.AddElement(BoneName, Temp);

}
void VSPartialAnimBlend::SetBoneWeight(unsigned int i, const VSUsedName & BoneName, VSREAL Weight)
{
	unsigned int uiIndex = m_BoneWeight.Find(BoneName);
	if (uiIndex != m_BoneWeight.GetNum())
	{
		if (i >= m_BoneWeight[uiIndex].Value.GetNum())
		{
			return;
		}
		m_BoneWeight[uiIndex].Value[i] = Weight;
	}
	
}
bool VSPartialAnimBlend::SetObject(VSObject * pObject)
{
	if (VSAnimFunction::SetObject(pObject))
	{
		ComputeWeight();
#ifdef FAST_BLEND_PARIAL_ANIMITION
		m_FastTempMatrix.SetBufferNum(m_BoneOutPut.GetNum());
#endif
		return true;
	}

	return false;
}
bool VSPartialAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{

	if (m_Weight.GetNum() == 0)
	{
		VSMAC_ASSERT(0);
	}
	
	for ( unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		VSInputNode* pInputNode = GetInputNode(i);
		VSAnimFunction *pAnimBaseFunction = (VSAnimFunction *)pInputNode->GetOutputLink()->GetOwner();
		if (!pAnimBaseFunction)
		{
			return false;
		}
#ifdef FAST_BLEND_PARIAL_ANIMITION
		if (i == 0)
		{
			pAnimBaseFunction->m_RootAtom.GetMatrix(m_FastRootMatrix);
			m_FastRootMatrix *= m_Weight[i][0];
		}
		else
		{
			VSMatrix3X3W Temp;
			pAnimBaseFunction->m_RootAtom.GetMatrix(Temp);
			m_FastRootMatrix += Temp * m_Weight[i][0];
		}
#else
		if (i == 0)
		{
			m_RootAtom = pAnimBaseFunction->m_RootAtom * m_Weight[i][0];
		}
		else
		{
			m_RootAtom.BlendWith(pAnimBaseFunction->m_RootAtom * m_Weight[i][0]);
		}
#endif
		for (unsigned int j = 0; j < m_BoneOutPut.GetNum(); j++)
		{

#ifdef FAST_BLEND_PARIAL_ANIMITION
			if (i == 0)
			{
				pAnimBaseFunction->m_BoneOutPut[j].GetMatrix(m_FastTempMatrix[j]);
				m_FastTempMatrix[j] *= m_Weight[i][j];

				
			}
			else
			{
				VSMatrix3X3W Temp;
				pAnimBaseFunction->m_BoneOutPut[j].GetMatrix(Temp);
				m_FastTempMatrix[j] += Temp * m_Weight[i][j];

			}
#else
			if (i == 0)
			{
				m_BoneOutPut[j] = pAnimBaseFunction->m_BoneOutPut[j] * m_Weight[i][j];
			}
			else
			{
				m_BoneOutPut[j].BlendWith(pAnimBaseFunction->m_BoneOutPut[j] * m_Weight[i][j]);
			}
#endif
		}
	}
#ifdef FAST_BLEND_PARIAL_ANIMITION
	for (unsigned int j = 0; j < m_BoneOutPut.GetNum(); j++)
	{
		m_BoneOutPut[j].FromMatrix(m_FastTempMatrix[j]);
	}
	m_RootAtom.FromMatrix(m_FastRootMatrix);
#endif
	return 1;
}

void VSPartialAnimBlend::ComputeWeight()
{

	const VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return ;
	}

	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return ;
	}
	m_Weight.SetBufferNum(m_pInput.GetNum());
	for (unsigned int i = 0; i < m_Weight.GetNum(); i++)
	{
		m_Weight[i].SetBufferNum(pSkelecton->GetBoneNum());
		if (i == 0)
		{
			for (unsigned int j = 0; j < pSkelecton->GetBoneNum(); j++)
			{
				m_Weight[i][j] = 1.0f;
			}
		}
		else
		{
			for (unsigned int j = 0; j < pSkelecton->GetBoneNum(); j++)
			{
				m_Weight[i][j] = 0.0f;
			}
		}
	}


	for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
	{
		VSREAL Sum = 0.0f;
		for (unsigned int j = 0; j < m_BoneWeight[i].Value.GetNum(); j++)
		{
			Sum += m_BoneWeight[i].Value[j];
		}

		for (unsigned int j = 0; j < m_BoneWeight[i].Value.GetNum(); j++)
		{
			m_BoneWeight[i].Value[j] /= Sum;
		}
	}


	for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
	{
			
		VSBoneNode * pBone = pSkelecton->GetBoneNode(m_BoneWeight[i].Key);

		VSArray<VSUsedName> AllBoneName;

		VSArray<VSBoneNode *> AllBoneArray;
		pBone->GetAllBoneArray(AllBoneArray);

		for (unsigned int m = 0; m < AllBoneArray.GetNum(); m++)
		{
			AllBoneName.AddElement(AllBoneArray[m]->m_cName);
		}

		VSArray<unsigned int> BoneIndex;
		BoneIndex.SetBufferNum(AllBoneName.GetNum());
		for (unsigned int s = 0; s < AllBoneName.GetNum(); s++)
		{
			for (unsigned int j = 0; j < pSkelecton->GetBoneNum(); j++)
			{
				VSBoneNode * pBone = pSkelecton->GetBoneNode(j);

				if (pBone->m_cName == AllBoneName[s])
				{
					BoneIndex[s] = j;
				}
			}
		}	

		for (unsigned int s = 0; s < m_Weight.GetNum(); s++)
		{
			for (unsigned int j = 0; j < BoneIndex.GetNum(); j++)
			{

				m_Weight[s][BoneIndex[j]] = m_BoneWeight[i].Value[s];
			}
		}
	}		
}
bool VSPartialAnimBlend::ComputePara(double dAppTime)
{
	return true;
}