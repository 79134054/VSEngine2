#include "VSMorphTree.h"
#include "VSMorphMainFunction.h"
#include "VSMorphBaseFunction.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSOneParamSmoothMorphBlend.h"
#include "VSMorphSequenceFunc.h"
#include "VSSwitchNode.h"
using namespace VSEngine2;
VSPointer<VSMorphTree> VSMorphTree::Default = NULL;
bool VSMorphTree::ms_bIsEnableASYNLoader = true;
bool VSMorphTree::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(VSMorphTree,VSController)
BEGIN_ADD_PROPERTY(VSMorphTree,VSController)
REGISTER_PROPERTY(m_pMorphMainFunction,MorphMainFunction,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMorphFunctionArray,MorphFunctionArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ShowName, ShowName, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMorphTree)
IMPLEMENT_INITIAL_END
VSMorphTree::~VSMorphTree()
{
	for(unsigned int i = 0 ; i < m_pMorphFunctionArray.GetNum() ; i++)
	{
		VSMorphBaseFunction * pMorphFunction = m_pMorphFunctionArray[i];
		m_pMorphFunctionArray[i] = NULL;
		VSMAC_DELETE(pMorphFunction);
	}

	m_pMorphFunctionArray.Clear();
	m_pMorphMainFunction = NULL;
}
VSMorphTree::VSMorphTree()
{
	m_pMorphFunctionArray.Clear();
	m_pMorphMainFunction = NULL;
	m_bCompressData = false;
}
VSMorphTree::VSMorphTree(const VSUsedName &ShowName)
{
	m_ShowName = ShowName;
	m_pMorphFunctionArray.Clear();
	m_pMorphMainFunction = NULL;
	m_bCompressData = false;
	m_pMorphMainFunction = VS_NEW VSMorphMainFunction(_T("MorphTree"), this);
	VSMAC_ASSERT(m_pMorphMainFunction);
}
void VSMorphTree::AddMorphFunction(VSMorphBaseFunction * pMorphFunction)
{
	if(pMorphFunction)
	{
		m_pMorphFunctionArray.AddElement(pMorphFunction);
	}
}
void VSMorphTree::DeleteMorphFunction(VSMorphBaseFunction * pMorphFunction)
{
	if(pMorphFunction)
	{
		for (unsigned int i = 0 ; i < m_pMorphFunctionArray.GetNum() ; i++)
		{
			if(m_pMorphFunctionArray[i] == pMorphFunction)
			{
				m_pMorphFunctionArray.Erase(i);
				return ;
			}
		}

	}
}
void VSMorphTree::ClearChangeFlag()
{
	for (unsigned int i = 0; i < m_pMorphFunctionArray.GetNum(); i++)
	{
		m_pMorphFunctionArray[i]->ClearChangeFlag();
	}
}
bool VSMorphTree::Update(double dAppTime)
{
	if(!VSController::Update(dAppTime))
		return false;
	VSModelMeshNode* pMeshNode = GetMeshNode();

	if (!pMeshNode)
	{
		return false;
	}
	VSGeometryNode * pGeomeNode = NULL;
	if (pMeshNode->GetLodType() == VSModelMeshNode::LT_DLOD)
	{
		if (pMeshNode->GetDlodNode())
		{
			pGeomeNode = DynamicCast<VSGeometryNode>(pMeshNode->GetDlodNode()->GetActiveNode());
		}
	}
	else
	{
		pGeomeNode = DynamicCast<VSGeometryNode>(pMeshNode->GetChild(0));
	}
	if (!pGeomeNode)
	{
		return false;
	}
	for(unsigned int i = 0 ; i < m_pMorphFunctionArray.GetNum() ; i++)
	{
		m_pMorphFunctionArray[i]->SetCurGeometryNodeNode(pGeomeNode);
		m_pMorphFunctionArray[i]->ClearFlag();
	}
	if(!m_pMorphMainFunction->Update(dAppTime))
		return false;
	return true;
}
bool VSMorphTree::SetObject(VSObject * pObject)
{
	VSMeshNode* Temp = DynamicCast<VSMeshNode>(pObject); 
	if(!Temp)
		return 0;
	m_pObject = pObject;
	for (unsigned int i = 0; i < m_pMorphFunctionArray.GetNum(); i++)
	{
		m_pMorphFunctionArray[i]->SetObject(pObject);
	}

	VSModelMeshNode* pMeshNode = GetMeshNode();
	if (!pMeshNode)
	{
		return false;
	}
	VSGeometryNode * pGeomeNode = NULL;
	if (pMeshNode->GetLodType() == VSModelMeshNode::LT_DLOD)
	{
		if (pMeshNode->GetDlodNode())
		{
			pGeomeNode = DynamicCast<VSGeometryNode>(pMeshNode->GetDlodNode()->GetActiveNode());
		}
	}
	else
	{
		pGeomeNode = DynamicCast<VSGeometryNode>(pMeshNode->GetChild(0));
	}
	if (!pGeomeNode)
	{
		return false;
	}

	m_bCompressData = (pGeomeNode->GetNormalGeometry(0)->GetOriginMeshDate()->GetVertexBuffer()->GetNormalDate(0)->GetDT() == VSDataBuffer::DT_UBYTE4N);
	return 1;
}
void VSMorphTree::SetNodePara(const VSUsedName & ShowName, void * pPara)
{
	VSMorphBaseFunction * pAnimNode = GetMorphFunctionFromShowName(ShowName);
	pAnimNode->SetPara(pPara);
}
VSMorphBaseFunction *  VSMorphTree::GetMorphFunctionFromShowName(const VSUsedName & ShowName)
{
	for (unsigned int i = 0; i < m_pMorphFunctionArray.GetNum(); i++)
	{
		if (m_pMorphFunctionArray[i]->GetShowName() == ShowName)
		{
			return m_pMorphFunctionArray[i];
		}
	}
	return NULL;
}
IMPLEMENT_RTTI(VSMorphTreeTwoSmoothBlendAnim, VSMorphTree)
BEGIN_ADD_PROPERTY(VSMorphTreeTwoSmoothBlendAnim, VSMorphTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMorphTreeTwoSmoothBlendAnim)
IMPLEMENT_INITIAL_END
VSMorphTreeTwoSmoothBlendAnim::VSMorphTreeTwoSmoothBlendAnim()
{

}
VSMorphTreeTwoSmoothBlendAnim::VSMorphTreeTwoSmoothBlendAnim(const VSUsedName &ShowName, const VSString &MorphName)
:VSMorphTree(ShowName)
{
	VSOneParamSmoothMorphBlend* pMorphBlend = VS_NEW VSOneParamSmoothMorphBlend(_T("OneParamSmoothMorphBlend"), this);
	
	VSMorphSequenceFunc * pMorphS1 = VS_NEW VSMorphSequenceFunc(_T("Squence1"), this);
	VSMorphSequenceFunc * pMorphS2 = VS_NEW VSMorphSequenceFunc(_T("Squence2"), this);
	pMorphS2->SetMorph(MorphName);

	pMorphBlend->GetInputNode(0)->Connection(pMorphS1->GetOutputNode(0));
	pMorphBlend->GetInputNode(1)->Connection(pMorphS2->GetOutputNode(0));
	m_pMorphMainFunction->GetInputNode(_T("Morph"))->Connection(pMorphBlend->GetOutputNode(0));

}
VSMorphTreeTwoSmoothBlendAnim::~VSMorphTreeTwoSmoothBlendAnim()
{

}