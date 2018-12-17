#include "VSMorphBlendFunction.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMorphBlendFunction, VSMorphFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMorphBlendFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMorphBlendFunction, VSMorphFunction)
END_ADD_PROPERTY
VSMorphBlendFunction::VSMorphBlendFunction()
{

}
VSMorphBlendFunction::VSMorphBlendFunction(const VSUsedName & ShowName, VSMorphTree * pMorphTree)
:VSMorphFunction(ShowName, pMorphTree)
{


	VSString OutputName = _T("Output");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::AVT_MORPH, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);



}
VSMorphBlendFunction::~VSMorphBlendFunction()
{

}
bool VSMorphBlendFunction::Update(double dAppTime)
{
	if (!VSMorphFunction::Update(dAppTime))
	{
		return 0;

	}
	if (!ComputePara(dAppTime))
		return 0;

	return 1;
}
void VSMorphBlendFunction::LineBlendTwo(VSMorphFunction * pOut,
	VSMorphFunction * pMorphFunction1, VSMorphFunction * pMorphFunction2, VSREAL fWeight)
{
	if (!pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return;
	}
	if (pMorphFunction1 && pMorphFunction2)
	{
		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
		{
			VSVector3 * pVector1 = pMorphFunction1->GetPos(uiLevel);
			VSVector3 * pVector2 = pMorphFunction2->GetPos(uiLevel);
			if (pVector2 && pVector1)
			{
				VSVector3 Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetPos(Vec, uiLevel);
			}
		}

		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
		{
			VSVector3 * pVector1 = pMorphFunction1->GetNormal(uiLevel);
			VSVector3 * pVector2 = pMorphFunction2->GetNormal(uiLevel);
			if (pVector2 && pVector1)
			{
				VSVector3 Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetNormal(Vec, uiLevel);
			}
		}

		{
			VSVector3 * pVector1 = pMorphFunction1->GetBinormal();
			VSVector3 * pVector2 = pMorphFunction2->GetBinormal();
			if (pVector2 && pVector1)
			{
				VSVector3 Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetBinormal(Vec);
			}
		}

		{
			VSVector3W * pVector1 = pMorphFunction1->GetTangent();
			VSVector3W * pVector2 = pMorphFunction2->GetTangent();
			if (pVector2 && pVector1)
			{
				VSVector3W Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetTangent(Vec);
			}
		}
		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
		{
			DWORD * pVector1 = pMorphFunction1->GetColor(uiLevel);
			DWORD * pVector2 = pMorphFunction2->GetColor(uiLevel);
			if (pVector2 && pVector1)
			{
				VSColorRGBA Color1, Color2;
				Color1.CreateFromARGB(*pVector1);
				Color2.CreateFromARGB(*pVector2);
				VSColorRGBA Result = LineInterpolation(Color1, Color2, fWeight);
				pOut->SetColor(Result.GetDWARGB(), uiLevel);
			}
		}
	}
	else if (pMorphFunction1)
	{
		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
		{
			VSVector3 * pVector1 = pMorphFunction1->GetPos(uiLevel);
			if (pVector1)
			{
				pOut->SetPos(*pVector1, uiLevel);
			}
		}

		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
		{
			VSVector3 * pVector1 = pMorphFunction1->GetNormal(uiLevel);

			if (pVector1)
			{
				pOut->SetNormal(*pVector1, uiLevel);
			}
		}

		{
			VSVector3 * pVector1 = pMorphFunction1->GetBinormal();
			if (pVector1)
			{
				pOut->SetBinormal(*pVector1);
			}
		}

		{
			VSVector3W * pVector1 = pMorphFunction1->GetTangent();

			if (pVector1)
			{
				pOut->SetTangent(*pVector1);
			}
		}
		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
		{
			DWORD * pVector1 = pMorphFunction1->GetColor(uiLevel);
			if (pVector1)
			{
				pOut->SetColor(*pVector1, uiLevel);
			}
		}
	}
	else if (pMorphFunction2)
	{
		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
		{
			VSVector3 * pVector1 = pMorphFunction2->GetPos(uiLevel);
			if (pVector1)
			{
				pOut->SetPos(*pVector1, uiLevel);
			}
		}

		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
		{
			VSVector3 * pVector1 = pMorphFunction2->GetNormal(uiLevel);

			if (pVector1)
			{
				pOut->SetNormal(*pVector1, uiLevel);
			}
		}

		{
			VSVector3 * pVector1 = pMorphFunction2->GetBinormal();
			if (pVector1)
			{
				pOut->SetBinormal(*pVector1);
			}
		}

		{
			VSVector3W * pVector1 = pMorphFunction2->GetTangent();

			if (pVector1)
			{
				pOut->SetTangent(*pVector1);
			}
		}
		for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
		{
			DWORD * pVector1 = pMorphFunction2->GetColor(uiLevel);
			if (pVector1)
			{
				pOut->SetColor(*pVector1, uiLevel);
			}
		}
	}
}
