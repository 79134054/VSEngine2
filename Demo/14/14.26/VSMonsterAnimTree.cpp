#include "VSMonsterAnimTree.h"
#include "VSGraphicInclude.h"
#include "VSOneParamImmediateAnimBlend.h"
#include "VSPartialAnimBlend.h"
#include "VSAnimMainFunction.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMonsterAnimTree1, VSAnimTree)
BEGIN_ADD_PROPERTY(VSMonsterAnimTree1, VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMonsterAnimTree1)
IMPLEMENT_INITIAL_END
VSMonsterAnimTree1::VSMonsterAnimTree1()
{

}
VSMonsterAnimTree1::VSMonsterAnimTree1(const VSUsedName &ShowName)
:VSAnimTree(ShowName)
{

	VSAnimSequenceFunc* pAnimSequenceAttack = VS_NEW VSAnimSequenceFunc(_T("Attack"), this);
	pAnimSequenceAttack->SetAnim(_T("Attack2"));
	VSAnimSequenceFunc* pAnimSequenceIdle = VS_NEW VSAnimSequenceFunc(_T("Idle"), this);
	pAnimSequenceIdle->SetAnim(_T("Idle"));
	VSOneParamImmediateAnimBlend * pIdle_Attack = VS_NEW VSOneParamImmediateAnimBlend(_T("Idle_Attack"), this);

	pIdle_Attack->GetInputNode(0)->Connection(pAnimSequenceIdle->GetOutputNode(0));
	pIdle_Attack->GetInputNode(1)->Connection(pAnimSequenceAttack->GetOutputNode(0));

	pIdle_Attack->m_fParamMax = 1.0f;
	pIdle_Attack->m_fParamMin = 0.0f;

	pIdle_Attack->SetNodeCrossFadingTime(0, 1000.0f);
	pIdle_Attack->SetNodeCrossFadingTime(1, 1000.0f);

	VSAnimSequenceFunc* pAnimSequenceWalk = VS_NEW VSAnimSequenceFunc(_T("Walk"), this);
	pAnimSequenceWalk->SetAnim(_T("Walk"));

	VSPartialAnimBlend * pWalk_Attack = VS_NEW VSPartialAnimBlend(_T("Walk_Attack"), this);

	pWalk_Attack->GetInputNode(0)->Connection(pAnimSequenceWalk->GetOutputNode(0));
	pWalk_Attack->GetInputNode(1)->Connection(pAnimSequenceAttack->GetOutputNode(0));
	pWalk_Attack->AddControllBone(_T("Bip01 Neck"));
	pWalk_Attack->SetBoneWeight(0, _T("Bip01 Neck"), 0.0f);
	pWalk_Attack->SetBoneWeight(1, _T("Bip01 Neck"), 1.0f);

	VSOneParamImmediateAnimBlend * pFinal = VS_NEW VSOneParamImmediateAnimBlend(_T("Final"), this);
	pFinal->AddInputNode();
	pFinal->GetInputNode(0)->Connection(pIdle_Attack->GetOutputNode(0));
	pFinal->GetInputNode(1)->Connection(pAnimSequenceWalk->GetOutputNode(0));
	pFinal->GetInputNode(2)->Connection(pWalk_Attack->GetOutputNode(0));
	pFinal->m_fParamMax = 2.0f;
	pFinal->m_fParamMin = 0.0f;
	pFinal->SetNodeCrossFadingTime(0, 100.0f);
	pFinal->SetNodeCrossFadingTime(1, 100.0f);
	pFinal->SetNodeCrossFadingTime(2, 100.0f);

	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pFinal->GetOutputNode(0));

}
VSMonsterAnimTree1::~VSMonsterAnimTree1()
{

}

IMPLEMENT_RTTI(VSMonsterAnimTree2, VSAnimTree)
BEGIN_ADD_PROPERTY(VSMonsterAnimTree2, VSAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMonsterAnimTree2)
IMPLEMENT_INITIAL_END
VSMonsterAnimTree2::VSMonsterAnimTree2()
{

}
VSMonsterAnimTree2::VSMonsterAnimTree2(const VSUsedName &ShowName)
:VSAnimTree(ShowName)
{
	VSAnimSequenceFunc* pAnimSequenceAttack = VS_NEW VSAnimSequenceFunc(_T("Attack"), this);
	pAnimSequenceAttack->SetAnim(_T("Attack2"));
	VSAnimSequenceFunc* pAnimSequenceIdle = VS_NEW VSAnimSequenceFunc(_T("Idle"), this);
	pAnimSequenceIdle->SetAnim(_T("Idle"));
	VSAnimSequenceFunc* pAnimSequenceWalk = VS_NEW VSAnimSequenceFunc(_T("Walk"), this);
	pAnimSequenceWalk->SetAnim(_T("Walk"));

	VSOneParamImmediateAnimBlend * pIdle_Walk = VS_NEW VSOneParamImmediateAnimBlend(_T("Idle_Walk"), this);

	pIdle_Walk->GetInputNode(0)->Connection(pAnimSequenceIdle->GetOutputNode(0));
	pIdle_Walk->GetInputNode(1)->Connection(pAnimSequenceWalk->GetOutputNode(0));
	pIdle_Walk->m_fParamMax = 1.0f;
	pIdle_Walk->m_fParamMin = 0.0f;
	pIdle_Walk->SetNodeCrossFadingTime(0, 100.0f);
	pIdle_Walk->SetNodeCrossFadingTime(1, 100.0f);

	VSPartialAnimBlend * pBlend_Attack = VS_NEW VSPartialAnimBlend(_T("Blend_Attack"), this);

	pBlend_Attack->GetInputNode(0)->Connection(pIdle_Walk->GetOutputNode(0));
	pBlend_Attack->GetInputNode(1)->Connection(pAnimSequenceAttack->GetOutputNode(0));

	pBlend_Attack->AddControllBone(_T("Bip01 Neck"));
	pBlend_Attack->SetBoneWeight(0, _T("Bip01 Neck"), 0.0f);
	pBlend_Attack->SetBoneWeight(1, _T("Bip01 Neck"), 1.0f);


	VSOneParamImmediateAnimBlend * pFinal = VS_NEW VSOneParamImmediateAnimBlend(_T("Final"), this);

	pFinal->GetInputNode(0)->Connection(pIdle_Walk->GetOutputNode(0));
	pFinal->GetInputNode(1)->Connection(pBlend_Attack->GetOutputNode(0));

	pFinal->SetNodeCrossFadingTime(0, 100.0f);
	pFinal->SetNodeCrossFadingTime(1, 100.0f);

	pFinal->m_fParamMax = 1.0f;
	pFinal->m_fParamMin = 0.0f;

	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pFinal->GetOutputNode(0));

}
VSMonsterAnimTree2::~VSMonsterAnimTree2()
{

}