#include "VSTriangleSet.h"
#include "VSNode.h"
#include "VSCreateClodFactory.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTriangleSet,VSMeshDate)
BEGIN_ADD_PROPERTY(VSTriangleSet,VSMeshDate)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTriangleSet)
IMPLEMENT_INITIAL_END
VSTriangleSet::VSTriangleSet()
{

}
VSTriangleSet::~VSTriangleSet()
{

}
bool VSTriangleSet::CreateFanIndex()
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetPositionDate(0)->GetNum();
	if(Num < 3)
		return 0;
	Num = (Num - 1) * 3;
	VSDataBuffer * pIndex = NULL;
	pIndex = new VSDataBuffer();
	if(!pIndex)
		return 0;
	if(!pIndex->CreateEmptyBuffer(Num,VSDataBuffer::DT_USHORT))
		return 0;

	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndex->GetDate();
	if(!pIndexBuffer)
		return 0;
	for(unsigned int i = 0 ; i < Num / 3; i++)
	{
		pIndexBuffer[i * 3] = 0;
		pIndexBuffer[i * 3 + 1] = i + 1;
		pIndexBuffer[i * 3 + 2] = i + 2;
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
bool VSTriangleSet::CreateNormal(unsigned int uiVertexLevel)
{
	if(m_pVertexBuffer == NULL)
		return 0;
	if(m_pVertexBuffer->GetNormalDate(uiVertexLevel))
		return 1;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 3)
		return 0;

	if(m_pIndexBuffer == NULL)
		return 0;

	//Noraml
	VSDataBuffer * pNormal= NULL;
	pNormal = new VSDataBuffer();
	if(!pNormal)
		return 0;
	if(!pNormal->CreateEmptyBuffer(Num,VSDataBuffer::DT_FLOAT32_3))
		return 0;

	VSVector3 * pNormalBuffer = (VSVector3 *)pNormal->GetDate();

	//计数
	unsigned int * pVertexTemp = VS_NEW unsigned int[Num];
	VSMemset((void *)pVertexTemp, 0, sizeof(unsigned int) * Num);


	unsigned int TriangleNum = GetTotleNum();

	VSDataBuffer * pIndexDate = m_pIndexBuffer->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}
	// 获取索引指针
	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndexDate->GetDate();
	if(!pIndexBuffer)
		return 0;
	
	VSDataBuffer * pDate = m_pVertexBuffer->GetPositionDate(uiVertexLevel);
	if(!pDate)
		return 0;
	if(pDate->GetChannel() != 3)
		return 0;
	//获取顶点指针
	VSVector3 * pVertexBuffer = (VSVector3 *)pDate->GetDate();
	if(!pVertexBuffer)
		return 0;
	
	//遍历所有三角形
	for (unsigned int poly = 0 ; poly < TriangleNum; poly++)
	{

		VSUSHORT_INDEX i0 = pIndexBuffer[poly*3+0];
		VSUSHORT_INDEX i1 = pIndexBuffer[poly*3+1];
		VSUSHORT_INDEX i2 = pIndexBuffer[poly*3+2];

		VSVector3 u = pVertexBuffer[i1] - pVertexBuffer[i0];
		VSVector3 v = pVertexBuffer[i2] - pVertexBuffer[i0];

		VSVector3 n;
		n.Cross(u,v);

		pVertexTemp[i0]++;
		pVertexTemp[i1]++;
		pVertexTemp[i2]++;

		pNormalBuffer[i0] += n;
		pNormalBuffer[i1] += n;
		pNormalBuffer[i2] += n;



	} // end for poly


	for (unsigned int vertex = 0; vertex < Num; vertex++)
	{


		if (pVertexTemp[vertex] > 1)
		{
			pNormalBuffer[vertex] /= (VSREAL)1.0 * pVertexTemp[vertex];
		
			
		} // end if
		pNormalBuffer[vertex].Normalize();
	} // end for
	VSMAC_DELETEA(pVertexTemp);

	m_pVertexBuffer->SetDate(pNormal,VSVertexFormat::VF_NORMAL);
	return 1;
}
/*
	(P0 - P1) = (u0 - u1) U + (v0 - v1) V
	(P0 - P2) = (u0 - u2) U + (v0 - v2) V

	(v0 - v2)(P0 - P1) = (v0 - v2)(u0 - u1) U + (v0 - v2)(v0 - v1) V
	(v0 - v1)(P0 - P2) = (v0 - v1)(u0 - u2) U + (v0 - v2)(v0 - v1) V

	U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))

	(u0 - u2)(P0 - P1) = (u0 - u2)(u0 - u1) U + (u0 - u2)(v0 - v1) V
	(u0 - u1)(P0 - P2) = (u0 - u2)(u0 - u1) U + (v0 - v2)(u0 - u1) V

	V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
*/
bool VSTriangleSet::CreateTangent(unsigned int uiTexCoordLevel)
{
	if(!m_pVertexBuffer || !m_pVertexBuffer->GetTexCoordDate(uiTexCoordLevel) || !m_pIndexBuffer)
		return 0;
	if(!m_pVertexBuffer->GetPositionDate(0))
		return 0;
	if(m_pVertexBuffer->GetTangentDate() && m_pVertexBuffer->GetBinormalDate())
		return 1;
	else if(m_pVertexBuffer->GetTangentDate() || m_pVertexBuffer->GetBinormalDate())
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 3)
		return 0;

	if(!m_pVertexBuffer->GetNormalDate(0))
	{
		if(!CreateNormal(0))
			return 0;
	}
	
	//TangentNoraml
	VSDataBuffer * pTangent = NULL;
	pTangent = new VSDataBuffer();
	if(!pTangent)
		return 0;
	if(!pTangent->CreateEmptyBuffer(Num,VSDataBuffer::DT_FLOAT32_3))
		return 0;

	VSVector3 * pTangentBuffer = (VSVector3 *)pTangent->GetDate();

	//BiNoraml
	VSDataBuffer * pBiNormal = NULL;
	pBiNormal = new VSDataBuffer();
	if(!pBiNormal)
		return 0;
	if(!pBiNormal->CreateEmptyBuffer(Num,VSDataBuffer::DT_FLOAT32_3))
		return 0;

	VSVector3 * pBiNormalBuffer = (VSVector3 *)pBiNormal->GetDate();


	unsigned int TriangleNum = GetTotleNum();


	VSDataBuffer * pIndexDate = m_pIndexBuffer->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}

	// 获取索引指针
	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndexDate->GetDate();
	if(!pIndexBuffer)
		return 0;

	//获取顶点
	VSDataBuffer * pDate = m_pVertexBuffer->GetPositionDate(0);
	if(!pDate)
		return 0;
	if(pDate->GetChannel() != 3)
		return 0;
	//获取顶点指针
	VSVector3 * pVertexBuffer = (VSVector3 *)pDate->GetDate();
	if(!pVertexBuffer)
		return 0;
	
	// get normal 
	pDate = m_pVertexBuffer->GetNormalDate(0);
	if(!pDate)
		return 0;
	VSVector3 * pNormalBuffer = (VSVector3 *)pDate->GetDate();
	if(!pNormalBuffer)
		return 0;

	// get texcoord
	pDate = m_pVertexBuffer->GetTexCoordDate(uiTexCoordLevel);
	if(!pDate)
		return 0;
	VSVector2 * pTexCoordBuffer = (VSVector2 *)pDate->GetDate();
	if(!pTexCoordBuffer)
		return 0;

	for (unsigned int poly = 0 ; poly < TriangleNum; poly++)
	{
		VSUSHORT_INDEX i0 = pIndexBuffer[poly*3+0];
		VSUSHORT_INDEX i1 = pIndexBuffer[poly*3+1];
		VSUSHORT_INDEX i2 = pIndexBuffer[poly*3+2];
		
		//U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
		VSVector3 TangentNoraml;
		TangentNoraml = 
			(	(pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) - 
				(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y))/
			(	(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) - 
				(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y));
		TangentNoraml.Normalize();
		pTangentBuffer[i0] += TangentNoraml;
		pTangentBuffer[i1] += TangentNoraml;
		pTangentBuffer[i2] += TangentNoraml;

		//V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
		VSVector3 BiNoraml;
		BiNoraml =
			(	(pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) - 
				(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x))/
			(	(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y) -
				(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y));
		BiNoraml.Normalize();
		pBiNormalBuffer[i0] += BiNoraml;
		pBiNormalBuffer[i1] += BiNoraml;
		pBiNormalBuffer[i2] += BiNoraml;

	}
	for (unsigned int i = 0 ;i < m_pVertexBuffer->GetVertexNum() ;i++)
	{
		VSVector3 N1 = pTangentBuffer[i];
		VSVector3 N2 = pBiNormalBuffer[i];
		VSVector3 Normal = pNormalBuffer[i];

		VSVector3 Tangent = N1 - Normal * (( N1 * Normal) / (Normal * Normal));

		VSVector3 Binromal = N2 - Normal * (( N2 * Normal) / (Normal * Normal)) - Tangent * (( N2 * Tangent) / (Tangent * Tangent));
		
		Tangent.Normalize();
		Binromal.Normalize();
		pTangentBuffer[i] = Tangent;
		pBiNormalBuffer[i] = Binromal;	
	}
	m_pVertexBuffer->SetDate(pBiNormal,VSVertexFormat::VF_BINORMAL);
	m_pVertexBuffer->SetDate(pTangent,VSVertexFormat::VF_TANGENT);
	return 1;
}
unsigned int VSTriangleSet::GetTotleNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 3;
	}
	else
	{
		return m_pIndexBuffer->GetNum() / 3;
	}
}
unsigned int VSTriangleSet::GetGirdNum(unsigned int uiInputNum)const
{

	return uiInputNum / 3;
}
bool VSTriangleSet::CreateCollapseRecord()
{
	if (!m_pIndexBuffer)
	{
		return false;
	}
	m_pCollapseRecord = NULL;
	m_pCollapseRecord = VSCreateClodTriFactory::CreateClodTriangle(this);
	if (m_pCollapseRecord)
	{
		m_pIndexBuffer->SetStatic(false);
		return 1;
	}
	else
		return 0;
}
void VSTriangleSet::UpDateClodMesh()
{
	if (!m_pCollapseRecord)
	{
		return;
	}
	if (!m_pIndexBuffer)
	{
		return ;
	}
	if (!m_pIndexBuffer->GetIdentifier())
	{
		return;
	}
	VSCollapseRecord * pCollapseRecord = m_pCollapseRecord;
	if (pCollapseRecord->m_uiCurRecordID == pCollapseRecord->DesireRecordID())
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
		const RecordTriInfo & Record = ((VSCollapseRecordTri *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		for (unsigned int i = 0 ; i < Record.m_MapIndex.GetNum() ; i++)
		{
			pIndex[Record.m_MapIndex[i]] = Record.m_uiKeep;
		}
		
		
	}
	while(pCollapseRecord->m_uiCurRecordID > pCollapseRecord->DesireRecordID())
	{
		
		const RecordTriInfo & Record = ((VSCollapseRecordTri *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		for (unsigned int i = 0 ; i < Record.m_MapIndex.GetNum() ; i++)
		{
			pIndex[Record.m_MapIndex[i]] = Record.m_uiThrow;
		}
		pCollapseRecord->m_uiCurRecordID--;
	}
	m_pIndexBuffer->UnLock();
}
unsigned int VSTriangleSet::GetActiveNum()const
{
	if (m_pCollapseRecord)
	{
		VSCollapseRecord * pCollapseRecord = m_pCollapseRecord;
		const RecordTriInfo & Record = ((VSCollapseRecordTri *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		return Record.m_uiLeftTraingleNum;
	}
	else
		return GetTotleNum();
}