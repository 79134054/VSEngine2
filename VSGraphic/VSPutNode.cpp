#include "VSPutNode.h"
#include "VSShaderFunction.h"
#include "VSStream.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSPutNode,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSPutNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSPutNode,VSObject)
REGISTER_PROPERTY(m_NodeName,NodeName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiValueType,ValueType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pOwner,Owner,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
VSPutNode::VSPutNode(unsigned int uiValueType,const VSUsedName & NodeName,VSObject * pObject)
{
	m_pOwner = pObject;
	m_NodeName = NodeName;
	m_uiValueType = uiValueType;
}
VSPutNode::~VSPutNode()
{
	m_pOwner = NULL;
}
VSPutNode::VSPutNode()
{
	m_pOwner = NULL;
	m_NodeName = _T("");
	m_uiValueType = VT_1;

}
IMPLEMENT_RTTI(VSInputNode,VSPutNode)
BEGIN_ADD_PROPERTY(VSInputNode,VSPutNode)
REGISTER_PROPERTY(m_pOutputLink,OutputLink,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSInputNode)
IMPLEMENT_INITIAL_END

VSInputNode::VSInputNode(unsigned int uiValueType,const VSUsedName &NodeName,VSObject * pObject)
			:VSPutNode(uiValueType,NodeName,pObject)
{
	m_pOutputLink = NULL;

}
VSInputNode::~VSInputNode()
{
	m_pOutputLink = NULL;
}
VSInputNode::VSInputNode()
{
	m_pOutputLink = NULL;
}


IMPLEMENT_RTTI(VSOutputNode,VSPutNode)
BEGIN_ADD_PROPERTY(VSOutputNode,VSPutNode)
REGISTER_PROPERTY(m_pInputLink,InputLink,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOutputNode)
IMPLEMENT_INITIAL_END


VSOutputNode::VSOutputNode(unsigned int uiValueType,const VSUsedName &NodeName,VSObject * pObject)
			:VSPutNode(uiValueType,NodeName,pObject)
{

	m_pInputLink.Clear();
}
VSOutputNode::~VSOutputNode()
{
	m_pInputLink.Clear();
}
VSOutputNode::VSOutputNode()
{
	m_pInputLink.Clear();
}
bool VSOutputNode::CheckIsConnection(VSInputNode * pInputNode)const
{
	if(!pInputNode)
		return 0;
	for(unsigned int i = 0 ; i < m_pInputLink.GetNum() ; i++)
	{
		if(m_pInputLink[i] == pInputNode)
			return 0;
	
	}
	return 1;
}
bool VSInputNode::Connection(VSOutputNode * pOutputNode,bool bIsJudgeType)
{
	if(!pOutputNode)
		return 0;
	if(bIsJudgeType)
	{
		if (m_uiValueType != pOutputNode->GetValueType())
		{
			return 0;
		}
	}
	m_pOutputLink = pOutputNode;
	if(pOutputNode->CheckIsConnection(this))
		pOutputNode->m_pInputLink.AddElement(this);
	return 1;
}
bool VSOutputNode::Connection(VSInputNode * pInputNode,bool bIsJudgeType)
{
	if(!pInputNode)
		return 0;
	if(bIsJudgeType)
	{
		if (m_uiValueType != pInputNode->GetValueType())
		{
			return 0;
		}
	}
	if(CheckIsConnection(pInputNode))
		m_pInputLink.AddElement(pInputNode);
	pInputNode->m_pOutputLink = this;
	return 1;
}
void VSInputNode::UnConnection()
{
	if(!m_pOutputLink)
		return;
	for(unsigned int i = 0 ; i < m_pOutputLink->m_pInputLink.GetNum() ; i++)
	{
		if(m_pOutputLink->m_pInputLink[i] == this)
		{
			m_pOutputLink->m_pInputLink.Erase(i);
			break;
		}
	}
	m_pOutputLink = NULL;
}
void VSOutputNode::UnConnection()
{
	if(!m_pInputLink.GetNum())
		return ;
	for(unsigned int i = 0 ; i < m_pInputLink.GetNum() ; i++)
	{
		if(m_pInputLink[i])
			m_pInputLink[i]->m_pOutputLink = NULL;
	}
	m_pInputLink.Clear();
}

