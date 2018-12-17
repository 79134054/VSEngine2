#include "VSMorphSequenceFunc.h"
#include "VSMorphSet.h"
#include "VSGeometryNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMorphSequenceFunc,VSMorphFunction)
BEGIN_ADD_PROPERTY(VSMorphSequenceFunc,VSMorphFunction)
REGISTER_PROPERTY(m_MorphName,MorphName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMorphSequenceFunc)
IMPLEMENT_INITIAL_END
VSMorphSequenceFunc::VSMorphSequenceFunc(const VSUsedName & ShowName,VSMorphTree * pMorphTree)
:VSMorphFunction(ShowName,pMorphTree)
{
	

	VSString OutputName = _T("Output");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::AVT_MORPH,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	m_uiRepeatType = VSController::RT_CYCLE;

	m_pVertexBuffer = NULL;
	m_pGeomeNode = NULL;
}
VSMorphSequenceFunc::VSMorphSequenceFunc()
{

	m_uiRepeatType = VSController::RT_CYCLE;
	m_pVertexBuffer = NULL;
	m_pGeomeNode = NULL;
}
VSMorphSequenceFunc::~VSMorphSequenceFunc()
{

}
void VSMorphSequenceFunc::SetMorph(const VSUsedName & MorphName)
{
	m_MorphName = MorphName;
}
void VSMorphSequenceFunc::ClearChangeFlag()
{
	VSMorphFunction::ClearChangeFlag();
	m_pVertexBuffer = NULL;
	for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
	{
		pPosDate[uiLevel] = NULL;
	}
	for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
	{
		pNormalDate[uiLevel] = NULL;
	}
	pTangentDate = NULL;
	pBinormalDate = NULL;
	for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
	{
		pColorDate[uiLevel] = NULL;
	}
}
void VSMorphSequenceFunc::UpdateGeometryDate(unsigned int GeometryIndex)
{
	if (!m_pGeomeNode)
	{
		return;
	}
	if (m_MorphName.GetBuffer())
	{
		const VSMorphSet * pMorphSet = m_pGeomeNode->GetMorphSet();
		if (!pMorphSet)
		{
			return;
		}

		VSMorph * pMorph = pMorphSet->GetMorph(m_MorphName);
		if (!pMorph)
		{
			return;
		}

		if (!pMorph->GetVertexNum(GeometryIndex))
		{
			return;
		}

		m_pVertexBuffer = pMorph->GetBuffer(GeometryIndex);
	}
	else
	{
		VSGeometry  * pGeometry = m_pGeomeNode->GetNormalGeometry(GeometryIndex);
		VSMeshDate * pMeshData = pGeometry->GetOriginMeshDate();
		m_pVertexBuffer = pMeshData->GetVertexBuffer();
	}

	if (!m_pVertexBuffer)
	{
		return;
	}

	for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_POS3; uiLevel++)
	{
		pPosDate[uiLevel] = m_pVertexBuffer->GetPositionDate(uiLevel);
	}
	for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
	{
		pNormalDate[uiLevel] = m_pVertexBuffer->GetNormalDate(uiLevel);
	}
	pTangentDate = m_pVertexBuffer->GetTangentDate();
	pBinormalDate = m_pVertexBuffer->GetBinormalDate();
	for (unsigned int uiLevel = 0; uiLevel < VSMorphFunction::MAX_NUM_COLOR; uiLevel++)
	{
		pColorDate[uiLevel] = m_pVertexBuffer->GetColorDate(uiLevel);
	}
}
void VSMorphSequenceFunc::UpdateVertexDate(unsigned int uiVertexIndex)
{
	
	if (!m_pVertexBuffer)
	{
		return;
	}

	
	for (unsigned int uiLevel = 0 ; uiLevel < VSMorphFunction::MAX_NUM_POS3 ; uiLevel++)
	{
		VSDataBuffer * pDate = pPosDate[uiLevel];
		if (pDate)
		{
			VSVector3 * pPos = (VSVector3 *)pDate->GetDate();
			if (pPos)
			{
				pPos +=uiVertexIndex;
				SetPos(*pPos,uiLevel);
			}
			
		}
	}

	for (unsigned int uiLevel = 0 ; uiLevel < VSMorphFunction::MAX_NUM_NORMAL3 ; uiLevel++)
	{
		VSDataBuffer * pDate = pNormalDate[uiLevel];
		if (pDate)
		{
			if (pDate->GetDT() == VSDataBuffer::DT_UBYTE4N)
			{
				DWORD * pNormal = (DWORD *)pDate->GetDate();
				if (pNormal)
				{
					pNormal += uiVertexIndex;
					
					VSVector3W Temp;
					Temp.CreateFormABGR(*pNormal);
					//所有的混合都是线性的，所以这里 * 2 - 1省略
					SetNormal(Temp.GetV3(), uiLevel);
				}
			}
			else
			{
				VSVector3 * pNormal = (VSVector3 *)pDate->GetDate();
				if (pNormal)
				{
					pNormal += uiVertexIndex;
					SetNormal(*pNormal, uiLevel);
				}
			}
			

		}
	}
	
	{
		VSDataBuffer * pDate = pTangentDate;
		if (pDate)
		{
			if (pDate->GetDT() == VSDataBuffer::DT_UBYTE4N)
			{
				DWORD * pTangent = (DWORD *)pDate->GetDate();
				if (pTangent)
				{
					pTangent += uiVertexIndex;

					VSVector3W Temp;
					Temp.CreateFormABGR(*pTangent);
					//所有的混合都是线性的，所以这里 * 2 - 1省略
					SetTangent(Temp);
				}
			}
			else
			{
				VSVector3 * pTangent = (VSVector3 *)pDate->GetDate();
				if (pTangent)
				{
					pTangent += uiVertexIndex;
					VSVector3W Temp(*pTangent);
					SetTangent(Temp);
				}
			}
			

		}
	
	}

	{
		VSDataBuffer * pDate = pBinormalDate;
		if (pDate)
		{
			VSVector3 * pBinormal = (VSVector3 *)pDate->GetDate();
			if (pBinormal)
			{
				pBinormal +=uiVertexIndex;
				SetBinormal(*pBinormal);
			}

		}

	}


	for (unsigned int uiLevel = 0 ; uiLevel < VSMorphFunction::MAX_NUM_COLOR ; uiLevel++)
	{
		VSDataBuffer * pDate = pColorDate[uiLevel];
		if (pDate)
		{
			DWORD * pColor = (DWORD *)pDate->GetDate();
			if (pColor)
			{
				pColor +=uiVertexIndex;
				SetColor(*pColor,uiLevel);
			}

		}
	}

}