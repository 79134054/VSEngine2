#include "VSMeshDate.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMeshDate,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMeshDate)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMeshDate,VSObject)
REGISTER_PROPERTY(m_pVertexBuffer,VertexBuffer,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pIndexBuffer,IndexBuffer,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pCollapseRecord,CollapseRecord,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSMeshDate::VSMeshDate()
{

	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pCollapseRecord = NULL;

}
VSMeshDate::~VSMeshDate()
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pCollapseRecord = NULL;
}
bool VSMeshDate::SetIndexBuffer(VSIndexBuffer * pIndexBuffer)
{
	if(!pIndexBuffer)
		return 0;

	if(pIndexBuffer->GetNum())
	{
		m_pIndexBuffer = pIndexBuffer;
		return 1;

	}
	else
	{
		return 0;
	}
}
bool VSMeshDate::SetVertexBuffer(VSVertexBuffer *pVertexBuffer)
{
	if(!pVertexBuffer)
		return 0;
	if(pVertexBuffer->GetVertexNum())
	{
		m_pVertexBuffer = pVertexBuffer;
		return 1;
	}
	else
	{
		return 0;
	}
}
void VSMeshDate::SetLodDesirePercent(VSREAL fDesirePercent)
{
	if (m_pCollapseRecord)
	{
		if (m_pCollapseRecord->GetRecordNum() > 0)
		{
			unsigned int uiDesireRecordID = (unsigned int )( (1.0f- fDesirePercent) * (m_pCollapseRecord->GetRecordNum() - 1));
			m_pCollapseRecord->SetDesireRecordID(uiDesireRecordID);
		}
		
	}

}