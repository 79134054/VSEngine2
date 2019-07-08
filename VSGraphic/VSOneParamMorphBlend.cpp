#include "VSOneParamMorphBlend.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSOneParamMorphBlend, VSMorphBlendFunction)
BEGIN_ADD_PROPERTY(VSOneParamMorphBlend, VSMorphBlendFunction)
REGISTER_PROPERTY(m_fParamMax, ParamMax, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fParamMin, ParamMin, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamMorphBlend)
IMPLEMENT_INITIAL_END
VSOneParamMorphBlend::~VSOneParamMorphBlend()
{

}
VSOneParamMorphBlend::VSOneParamMorphBlend()
{
	m_fParam = 0.0f;
	m_fParamMax = 1.0f;
	m_fParamMin = -1.0f;
	m_pMorphBaseFunction1 = NULL;
	m_pMorphBaseFunction2 = NULL;
	m_fWeight = 0.0f;
}
VSOneParamMorphBlend::VSOneParamMorphBlend(const VSUsedName & ShowName, VSMorphTree * pMorphTree)
:VSMorphBlendFunction(ShowName, pMorphTree)
{
	m_fParam = 0.0f;
	m_fParamMax = 1.0f;
	m_fParamMin = -1.0f;

	m_pMorphBaseFunction1 = NULL;
	m_pMorphBaseFunction2 = NULL;
	m_fWeight = 0.0f;
	VSString InputName0 = _T("Child0");
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName0, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	VSString InputName1 = _T("Child1");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName1, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

}
void VSOneParamMorphBlend::AddInputNode()
{
	VSString InputName = _T("Child");
	VSString ID = IntToString(m_pInput.GetNum());
	InputName += ID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

}
void VSOneParamMorphBlend::DeleteInputNode()
{
	if (m_pInput.GetNum() > 2)
	{
		VSInputNode * pInputNode = m_pInput[m_pInput.GetNum() - 1];

		m_pInput.Erase(m_pInput.GetNum() - 1);
		if (!pInputNode)
		{
			return;
		}
		VSMAC_DELETE(pInputNode);
	}
}
bool VSOneParamMorphBlend::ComputePara(double dAppTime)
{
	if (m_fParam < m_fParamMin)
	{
		m_fParam = m_fParamMin;
	}
	else if (m_fParam > m_fParamMax)
	{
		m_fParam = m_fParamMax;
	}
	return 1;
}
