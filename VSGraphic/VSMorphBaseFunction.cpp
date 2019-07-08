#include "VSMorphBaseFunction.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMorphBaseFunction,VSController)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMorphBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMorphBaseFunction,VSController)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pInput,pInput,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOutput,Output,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOwner,Owner,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSMorphBaseFunction::VSMorphBaseFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_pOwner = NULL;
	m_bIsVisited = 0;
}
VSMorphBaseFunction::VSMorphBaseFunction(const VSUsedName & ShowName,VSMorphTree * pMorphTree)
{
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	VSMAC_ASSERT(pMorphTree);
	m_pOwner = pMorphTree;
	m_pOwner->AddMorphFunction(this);
	m_bIsVisited = 0;

}
VSMorphBaseFunction::~VSMorphBaseFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		VSMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		VSMAC_DELETE(m_pOutput[i]);
	}
	m_pOwner->DeleteMorphFunction(this);
}
bool VSMorphBaseFunction::Update(double dAppTime)
{
	if(!VSController::Update(dAppTime))
		return false;
	if(m_bIsVisited)
		return false;
	m_bIsVisited = 1;
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			VSMorphBaseFunction *pMorphBaseFunction = (VSMorphBaseFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
			if(pMorphBaseFunction)
			{
				pMorphBaseFunction->Update(dAppTime);
			}
		}
	}
	return true;

}
VSInputNode * VSMorphBaseFunction::GetInputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];

}
VSInputNode * VSMorphBaseFunction::GetInputNode(const VSString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

VSOutputNode * VSMorphBaseFunction::GetOutputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pOutput.GetNum())
		return NULL;
	return m_pOutput[uiNodeID];
}
VSOutputNode * VSMorphBaseFunction::GetOutputNode(const VSString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}
void VSMorphBaseFunction::ClearFlag()
{
	m_bIsVisited = 0;
}