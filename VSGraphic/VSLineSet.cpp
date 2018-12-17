#include "VSLineSet.h"
#include "VSCreateClodFactory.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSLineSet,VSMeshDate)
BEGIN_ADD_PROPERTY(VSLineSet,VSMeshDate)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSLineSet)
IMPLEMENT_INITIAL_END
VSLineSet::VSLineSet()
{

}
VSLineSet::~VSLineSet()
{

}
bool VSLineSet::CreateIndex(unsigned int uiLineType)
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 2)
		return 0;
	if(uiLineType == LT_OPEN)
	{
		Num = (Num - 1) * 2;

	}
	else if(uiLineType == LT_CLOSE)
	{
		Num = Num * 2;
	}
	else if(uiLineType == LT_SEGMENT)
	{
		if(Num % 2)
		{
			return 0;
		}

	}
	else
		 return 0;
	VSDataBuffer * pIndex = NULL;
	pIndex = new VSDataBuffer();
	if(!pIndex)
		return 0;
	
	
	if(!pIndex->CreateEmptyBuffer(Num,VSDataBuffer::DT_USHORT))
		return 0;

	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndex->GetDate();
	if(!pIndexBuffer)
		return 0;

	if(uiLineType == LT_OPEN)
	{
		for(unsigned int i = 0 ; i < Num / 2 ; i++)
		{
			pIndexBuffer[2 * i] = i;
			pIndexBuffer[2 * i + 1] = i + 1;
		}
	}
	else if(uiLineType == LT_CLOSE)
	{	
		unsigned int i;
		for( i = 0; i < Num / 2 ; i++)
		{
			pIndexBuffer[2 * i] = i;
			pIndexBuffer[2 * i + 1] = i + 1;
		}
		//i已经递增，最后一个索引应该是0，所以修改 2 * ( i - 1) + 1 =  2 * i - 1
		pIndexBuffer[2 * i - 1] = 0;
	}
	else if(uiLineType == LT_SEGMENT)
	{
		for(unsigned int i = 0 ; i < Num ; i++)
		{
			pIndexBuffer[i] = i;

		}
	}
	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();
		if (!m_pIndexBuffer)
		{
			return 0;
		}
	}

	m_pIndexBuffer->SetDate(pIndex);
	
	return 1;

}
unsigned int VSLineSet::GetTotleNum()const
{	
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 2;
	}
	else
		return m_pIndexBuffer->GetNum() / 2;
}
unsigned int VSLineSet::GetGirdNum(unsigned int uiInputNum)const
{
	return uiInputNum / 2;
}
bool VSLineSet::CreateCollapseRecord()
{
	if (!m_pIndexBuffer)
	{
		return false;
	}
	m_pCollapseRecord = NULL;
	m_pCollapseRecord = VSCreateClodLineFactory::CreateClodLine(this);
	if (m_pCollapseRecord)
	{
		m_pIndexBuffer->SetStatic(false);
		return 1;
	}
	else
		return 0;
}
void VSLineSet::UpDateClodMesh()
{
	if (!m_pCollapseRecord)
	{
		return;
	}
	if (!m_pIndexBuffer)
	{
		return ;
	}
	
	VSCollapseRecord * pCollapseRecord = m_pCollapseRecord;
	if (pCollapseRecord->m_uiCurRecordID == pCollapseRecord->DesireRecordID())
	{
		return;
	}
	if (!m_pIndexBuffer->GetIdentifier())
	{
		return;
	}
	VSUSHORT_INDEX *pIndex = (VSUSHORT_INDEX *)m_pIndexBuffer->Lock();

	if (!pIndex)
	{
		return;
	}

	while(pCollapseRecord->m_uiCurRecordID < pCollapseRecord->DesireRecordID())
	{
		pCollapseRecord->m_uiCurRecordID++;
		const RecordLineInfo& Record = ((VSCollapseRecordLine *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];	
		pIndex[Record.m_MapIndex] = Record.m_uiKeep;		

	}
	while(pCollapseRecord->m_uiCurRecordID > pCollapseRecord->DesireRecordID())
	{

		const RecordLineInfo& Record = ((VSCollapseRecordLine *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		pIndex[Record.m_MapIndex] = Record.m_uiThrow;
		pCollapseRecord->m_uiCurRecordID--;
	}
	m_pIndexBuffer->UnLock();
	//pCollapseRecord->m_uiCurRecordID = pCollapseRecord->DesireRecordID();
}
unsigned int VSLineSet::GetActiveNum()const
{
	if (m_pCollapseRecord)
	{
		VSCollapseRecord * pCollapseRecord = m_pCollapseRecord;
		const RecordLineInfo& Record = ((VSCollapseRecordLine *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		return Record.m_uiLeftEdgeNum;
	}
	else
		return GetTotleNum();
}