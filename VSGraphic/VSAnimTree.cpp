#include "VSAnimTree.h"
#include "VSAnimFunction.h"
#include "VSAnimMainFunction.h"
#include "VSAnimBaseFunction.h"
#include "VSAnimSequenceFunc.h"
#include "VSOneParamSmoothAnimBlend.h"
#include "VSRectAnimBlend.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSOneParamSmoothAnimBlendSequence.h"
#include "VSOneParamImmediateAnimBlend.h"
#include "VSPartialAnimBlend.h"
using namespace VSEngine2;
VSPointer<VSAnimTree> VSAnimTree::Default = NULL;
bool VSAnimTree::ms_bIsEnableASYNLoader = true;
bool VSAnimTree::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(VSAnimTree,VSController)
BEGIN_ADD_PROPERTY(VSAnimTree,VSController)
REGISTER_PROPERTY(m_pAnimMainFunction,pAnimMainFunction,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pAnimFunctionArray,pAnimFunctionArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimTree)
IMPLEMENT_INITIAL_END
VSAnimTree::~VSAnimTree()
{
	for(unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		VSAnimBaseFunction * pAnimFunction = m_pAnimFunctionArray[i];
		m_pAnimFunctionArray[i] = NULL;
		VSMAC_DELETE(pAnimFunction);
	}
	
	m_pAnimFunctionArray.Clear();

	m_pAnimMainFunction = NULL;
}
VSAnimTree::VSAnimTree(const VSUsedName &ShowName)
{
	m_ShowName = ShowName;

	m_pAnimFunctionArray.Clear();
	m_pAnimMainFunction = NULL;

	m_pAnimMainFunction = VS_NEW VSAnimMainFunction(_T("AnimTree"),this);
	VSMAC_ASSERT(m_pAnimMainFunction);



}

VSAnimTree::VSAnimTree()
{
	m_pAnimFunctionArray.Clear();
	m_pAnimMainFunction = NULL;

}
VSSkelectonMeshNode * VSAnimTree::GetSkelectonMeshNode()const
{
	return DynamicCast<VSSkelectonMeshNode>(m_pObject);
}
void VSAnimTree::AddAnimFunction(VSAnimBaseFunction * pAnimFunction)
{
	if(pAnimFunction)
	{
		m_pAnimFunctionArray.AddElement(pAnimFunction);
	}
}
void VSAnimTree::DeleteAnimFunction(VSAnimBaseFunction * pAnimFunction)
{
	if(pAnimFunction)
	{
		for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
		{
			if(m_pAnimFunctionArray[i] == pAnimFunction)
			{
				m_pAnimFunctionArray.Erase(i);
				return ;
			}
		}

	}
}
bool VSAnimTree::Update(double dAppTime)
{
	if (!VSController::Update(dAppTime))
		return false;
	const VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return false;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return false;
	}

	if (!pMesh->GetAnimSet())
	{
		return false;
	}
	for(unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		m_pAnimFunctionArray[i]->ClearFlag();
	}
	if(!m_pAnimMainFunction->Update(dAppTime))
		return false;
	return true;
}
bool VSAnimTree::SetObject(VSObject * pObject)
{
	VSSkelectonMeshNode* Temp = DynamicCast<VSSkelectonMeshNode>(pObject); 
	if(!Temp)
		return 0;
	m_pObject = pObject;
	for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		m_pAnimFunctionArray[i]->SetObject(pObject);
	}
	return 1;
}
void VSAnimTree::ResetAnimFunction()
{
	for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		VSAnimSequenceFunc * pAnimSequence = DynamicCast<VSAnimSequenceFunc>(m_pAnimFunctionArray[i]);
		if (pAnimSequence)
		{
			
			pAnimSequence->SetAnim(pAnimSequence->GetAnimName());

			
		}
	}
}
void VSAnimTree::SetNodePara(const VSUsedName & ShowName, void * pPara)
{
	VSAnimBaseFunction * pAnimNode = GetAnimFunctionFromShowName(ShowName);
	if (!pAnimNode)
	{
		return;
	}
	pAnimNode->SetPara(pPara);
}
VSAnimBaseFunction *  VSAnimTree::GetAnimFunctionFromShowName(const VSUsedName & ShowName)
{
	for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		if (m_pAnimFunctionArray[i]->GetShowName() == ShowName)
		{
			return m_pAnimFunctionArray[i];
		}
	}
	return NULL;
}

IMPLEMENT_RTTI(VSAnimTreeOneAnim,VSAnimTree)
BEGIN_ADD_PROPERTY(VSAnimTreeOneAnim,VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimTreeOneAnim)
IMPLEMENT_INITIAL_END
VSAnimTreeOneAnim::VSAnimTreeOneAnim()
{

}
VSAnimTreeOneAnim::VSAnimTreeOneAnim(const VSUsedName &ShowName,const VSString& AnimName)
:VSAnimTree(ShowName)
{
	VSAnimSequenceFunc* pAnimSequenceFunc = VS_NEW VSAnimSequenceFunc(_T("Squence"),this);
	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimSequenceFunc->GetOutputNode(0));
	pAnimSequenceFunc->SetAnim(AnimName);

}
VSAnimTreeOneAnim::~VSAnimTreeOneAnim()
{

}
IMPLEMENT_RTTI(VSAnimTreeTwoSmoothBlendAnim,VSAnimTree)
BEGIN_ADD_PROPERTY(VSAnimTreeTwoSmoothBlendAnim,VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimTreeTwoSmoothBlendAnim)
IMPLEMENT_INITIAL_END
VSAnimTreeTwoSmoothBlendAnim::VSAnimTreeTwoSmoothBlendAnim()
{

}
VSAnimTreeTwoSmoothBlendAnim::VSAnimTreeTwoSmoothBlendAnim(const VSUsedName &ShowName,const VSString &AnimName1,const VSString &AnimName2)
:VSAnimTree(ShowName)
{
	VSOneParamSmoothAnimBlendSequence* pAnimBlend = VS_NEW VSOneParamSmoothAnimBlendSequence(_T("OneParamSmoothAnimBlendSequence"), this);
 	pAnimBlend->CreateSlot(2);
 	pAnimBlend->SetAnim(0,AnimName1);
	pAnimBlend->SetAnim(1, AnimName2);
	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
VSAnimTreeTwoSmoothBlendAnim::~VSAnimTreeTwoSmoothBlendAnim()
{

}

IMPLEMENT_RTTI(VSAnimTreeTwoImmediateAnim, VSAnimTree)
BEGIN_ADD_PROPERTY(VSAnimTreeTwoImmediateAnim, VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimTreeTwoImmediateAnim)
IMPLEMENT_INITIAL_END
VSAnimTreeTwoImmediateAnim::VSAnimTreeTwoImmediateAnim()
{

}
VSAnimTreeTwoImmediateAnim::VSAnimTreeTwoImmediateAnim(const VSUsedName &ShowName, const VSString &AnimName1, const VSString &AnimName2)
:VSAnimTree(ShowName)
{
	VSAnimSequenceFunc* pAnimSequenceFunc1 = VS_NEW VSAnimSequenceFunc(_T("Squence1"), this);

	pAnimSequenceFunc1->SetAnim(AnimName1);

	VSAnimSequenceFunc* pAnimSequenceFunc2 = VS_NEW VSAnimSequenceFunc(_T("Squence2"), this);

	pAnimSequenceFunc2->SetAnim(AnimName2);

	VSOneParamImmediateAnimBlend * pAnimBlend = VS_NEW VSOneParamImmediateAnimBlend(_T("OneParamBlend"), this);

	pAnimBlend->SetNodeCrossFadingTime(0, 1000.0f);
	pAnimBlend->SetNodeCrossFadingTime(1, 1000.0f);

	pAnimBlend->GetInputNode(0)->Connection(pAnimSequenceFunc1->GetOutputNode(0));
	pAnimBlend->GetInputNode(1)->Connection(pAnimSequenceFunc2->GetOutputNode(0));

	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
VSAnimTreeTwoImmediateAnim::~VSAnimTreeTwoImmediateAnim()
{

}

IMPLEMENT_RTTI(VSAnimTreePartialAnim, VSAnimTree)
BEGIN_ADD_PROPERTY(VSAnimTreePartialAnim, VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimTreePartialAnim)
IMPLEMENT_INITIAL_END
VSAnimTreePartialAnim::VSAnimTreePartialAnim()
{

}
VSAnimTreePartialAnim::VSAnimTreePartialAnim(const VSUsedName &ShowName, const VSString &AnimName1, const VSString &AnimName2)
:VSAnimTree(ShowName)
{
	VSAnimSequenceFunc* pAnimSequenceFunc1 = VS_NEW VSAnimSequenceFunc(_T("Squence1"), this);

	pAnimSequenceFunc1->SetAnim(AnimName1);

	VSAnimSequenceFunc* pAnimSequenceFunc2 = VS_NEW VSAnimSequenceFunc(_T("Squence2"), this);

	pAnimSequenceFunc2->SetAnim(AnimName2);

	VSPartialAnimBlend * pAnimBlend = VS_NEW VSPartialAnimBlend(_T("PartialAnimBlend"), this);

	

	pAnimBlend->GetInputNode(0)->Connection(pAnimSequenceFunc1->GetOutputNode(0));
	pAnimBlend->GetInputNode(1)->Connection(pAnimSequenceFunc2->GetOutputNode(0));

	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
VSAnimTreePartialAnim::~VSAnimTreePartialAnim()
{

}

IMPLEMENT_RTTI(VSAnimTreeRectBlendAnim,VSAnimTree)
BEGIN_ADD_PROPERTY(VSAnimTreeRectBlendAnim,VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimTreeRectBlendAnim)
IMPLEMENT_INITIAL_END
VSAnimTreeRectBlendAnim::VSAnimTreeRectBlendAnim()
{

}
VSAnimTreeRectBlendAnim::VSAnimTreeRectBlendAnim(const VSUsedName &ShowName,
												 const VSString &AnimName1,
												 const VSString &AnimName2,
												 const VSString &AnimName3,
												 const VSString &AnimName4)
:VSAnimTree(ShowName)
{
	VSAnimSequenceFunc* pAnimSequenceFunc1 = VS_NEW VSAnimSequenceFunc(_T("Squence1"),this);

	pAnimSequenceFunc1->SetAnim(AnimName1);

	VSAnimSequenceFunc* pAnimSequenceFunc2 = VS_NEW VSAnimSequenceFunc(_T("Squence2"),this);

	pAnimSequenceFunc2->SetAnim(AnimName2);

	VSAnimSequenceFunc* pAnimSequenceFunc3 = VS_NEW VSAnimSequenceFunc(_T("Squence3"),this);

	pAnimSequenceFunc1->SetAnim(AnimName3);

	VSAnimSequenceFunc* pAnimSequenceFunc4 = VS_NEW VSAnimSequenceFunc(_T("Squence4"),this);

	pAnimSequenceFunc2->SetAnim(AnimName4);


	VSRectAnimBlend * pAnimBlend = VS_NEW VSRectAnimBlend(_T("RectBlend"),this);


	pAnimBlend->GetInputNode(0)->Connection(pAnimSequenceFunc1->GetOutputNode(0));
	pAnimBlend->GetInputNode(1)->Connection(pAnimSequenceFunc2->GetOutputNode(0));
	pAnimBlend->GetInputNode(2)->Connection(pAnimSequenceFunc3->GetOutputNode(0));
	pAnimBlend->GetInputNode(3)->Connection(pAnimSequenceFunc4->GetOutputNode(0));
	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
VSAnimTreeRectBlendAnim::~VSAnimTreeRectBlendAnim()
{

}