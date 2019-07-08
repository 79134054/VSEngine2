#include "VSAnimBlendFunction.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSAnimBlendFunction,VSAnimFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSAnimBlendFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSAnimBlendFunction,VSAnimFunction)
END_ADD_PROPERTY
VSAnimBlendFunction::VSAnimBlendFunction()
{

}
VSAnimBlendFunction::VSAnimBlendFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimFunction(ShowName,pAnimTree)
{


	VSString OutputName = _T("Output");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::AVT_ANIM,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	

}
VSAnimBlendFunction::~VSAnimBlendFunction()
{
	
}
bool VSAnimBlendFunction::Update(double dAppTime)
{
	if (!VSAnimFunction::Update(dAppTime))
	{
		return 0;

	}
	if(!ComputePara(dAppTime))
		return 0;
	if(!ComputeOutBoneMatrix(dAppTime))
		return 0;
	return 1;
}
void VSAnimBlendFunction::AdditiveBlend(VSAnimFunction * pOut, VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2)
{
	if (!pOut)
	{
		return;
	}
	unsigned int uiBoneNum = pOut->m_BoneOutPut.GetNum();
	if (!uiBoneNum)
	{
		return;
	}

	if (pAnimFunction1 && pAnimFunction2)
	{

		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return;
		}
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return;
		}

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutPut[i].AddTwo(pAnimFunction1->m_BoneOutPut[i], pAnimFunction2->m_BoneOutPut[i]);
		}
	}
	else if (pAnimFunction1)
	{
		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return;
		}

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction1->m_BoneOutPut[i];
		}
	}
	else if (pAnimFunction2)
	{
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return;
		}


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction2->m_BoneOutPut[i];
		}
	}
}
void VSAnimBlendFunction::LineBlendTwo(VSAnimFunction * pOut,
									   VSAnimFunction * pAnimFunction1,VSAnimFunction * pAnimFunction2,
									   VSREAL fWeight)
{
	if (!pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return ;
	}
	unsigned int uiBoneNum = pOut->m_BoneOutPut.GetNum();
	if (!uiBoneNum)
	{
		return ;
	}
	
	if (pAnimFunction1 && pAnimFunction2)
	{

		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutPut[i].Interpolation(pAnimFunction1->m_BoneOutPut[i],pAnimFunction2->m_BoneOutPut[i],fWeight);
		}
	}
	else if (pAnimFunction1)
	{
		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction1->m_BoneOutPut[i];
		}
	}
	else if (pAnimFunction2)
	{
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}


		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction2->m_BoneOutPut[i];
		}
	}
}

void VSAnimBlendFunction::LineBlendTwo(VSArray<VSAnimAtom> *pOut,
						 const VSArray<VSAnimAtom> * pIn1,const VSArray<VSAnimAtom> *pIn2,
						 VSREAL fWeight)
{

	if (pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return ;
	}
	unsigned int uiBoneNum = pOut->GetNum();
	if (!uiBoneNum)
	{
		return ;
	}

	if (pIn1 && pIn2)
	{

		if (uiBoneNum != pIn1->GetNum())
		{
			return ;
		}
		if (uiBoneNum != pIn2->GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i].Interpolation((*pIn1)[i],(*pIn2)[i],fWeight); 
		}
	}
	else if (pIn1)
	{
		if (uiBoneNum != pIn1->GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = (*pIn1)[i];
		}
	}
	else if (pIn2)
	{
		if (uiBoneNum != pIn2->GetNum())
		{
			return ;
		}


		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = (*pIn2)[i];
		}
	}
}

void VSAnimBlendFunction::LineBlendTwo(VSAnimAtom &pOut,
	VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight)
{
	if (fWeight < 0.0f || fWeight > 1.0f)
	{
		return;
	}


	if (pAnimFunction1 && pAnimFunction2)
	{

		pOut.Interpolation(pAnimFunction1->m_RootAtom, pAnimFunction2->m_RootAtom, fWeight);

	}
	else if (pAnimFunction1)
	{

		pOut = pAnimFunction1->m_RootAtom;
		
	}
	else if (pAnimFunction2)
	{


		pOut = pAnimFunction2->m_RootAtom;
	}
}
void VSAnimBlendFunction::LineBlendTwo(VSArray<VSAnimAtom> *pOut,
									   VSAnimFunction * pAnimFunction1,VSAnimFunction * pAnimFunction2,
									   VSREAL fWeight)
{
	if (!pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return ;
	}
	unsigned int uiBoneNum = pOut->GetNum();
	if (!uiBoneNum)
	{
		return ;
	}

	if (pAnimFunction1 && pAnimFunction2)
	{

		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i].Interpolation(pAnimFunction1->m_BoneOutPut[i],pAnimFunction2->m_BoneOutPut[i],fWeight);
		}
	}
	else if (pAnimFunction1)
	{
		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = pAnimFunction1->m_BoneOutPut[i];
		}
	}
	else if (pAnimFunction2)
	{
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}


		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = pAnimFunction2->m_BoneOutPut[i];
		}
	}
}

