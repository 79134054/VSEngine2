#include "VSCreateClodFactory.h"
#include "VSSegment3.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
VSArray<VSCreateClodTriFactory::Vertex3DAttr> VSCreateClodTriFactory::ms_V3Attr;
VSArray<VSCreateClodTriFactory::Edge3DAttr> VSCreateClodTriFactory::ms_E3Attr;
VSArray<VSCreateClodTriFactory::Triangle3DAttr> VSCreateClodTriFactory::ms_T3Attr;
VSTriangleSet* VSCreateClodTriFactory::ms_pTriangleSet = NULL;
VSCollapseRecordTri *VSCreateClodTriFactory::ms_pCRT = NULL;
VSArray<unsigned int> VSCreateClodTriFactory::ms_RecordTriangle;
unsigned int VSCreateClodTriFactory::ms_uiLeftTrangleNum = 0;
VSCollapseRecordTri * VSCreateClodTriFactory::CreateClodTriangle( VSTriangleSet * pTriangleSet)
{
	if (!pTriangleSet)
	{
		return 0;
	}
	ms_pTriangleSet = pTriangleSet;
	if (!ms_pTriangleSet->GetVertexBuffer())
	{
		return 0;

	}

	VSDataBuffer * pVertexDate = ms_pTriangleSet->GetVertexBuffer()->GetPositionDate(0);

	if (!pVertexDate)
	{
		return 0;
	}

	if (!pVertexDate->GetDate())
	{
		return 0;
	}
	unsigned int uiVerNum = pVertexDate->GetNum();

	if ( !uiVerNum)
	{
		return 0;
	}

	if (!ms_pTriangleSet->GetIndexBuffer())
	{
		return 0;
	}

	if (!ms_pTriangleSet->GetIndexBuffer()->GetIndexDate() || !ms_pTriangleSet->GetIndexBuffer()->GetNum())
	{
		return 0;
	}

	ms_V3Attr.Clear();
	ms_E3Attr.Clear();
	ms_T3Attr.Clear();

	VSMAC_DELETE(ms_pCRT);

	ms_pCRT = VS_NEW VSCollapseRecordTri();

	if (!ms_pCRT)
	{
		return 0;
	}
	ms_RecordTriangle.Clear();

	InitalDate();
	//填入原始的三角形信息，作为第 0 个索引的 record ，所以替换三角形索引的mapindex数组为0
	//keep 和 throw 都是无效的
	RecordTriInfo CollapseRecord;
	CollapseRecord.m_uiKeep = VSMAX_INTEGER;
	CollapseRecord.m_uiThrow = VSMAX_INTEGER;
	CollapseRecord.m_uiLeftTraingleNum = ms_uiLeftTrangleNum;
	ms_pCRT->m_RecordInfo.AddElement(CollapseRecord);
	unsigned int uiE3DIndex = 0 , uiKeepV3DIndex = 0, uiThrowV3DIndex = 0;

	while(GetCollapseEV(uiE3DIndex,uiKeepV3DIndex,uiThrowV3DIndex))
	{
		Collapse(uiE3DIndex,uiKeepV3DIndex,uiThrowV3DIndex);
// 		for(unsigned int i = 0 ; i < ms_E3Attr.GetNum() ; i++)
// 		{
// 			if(!ms_E3Attr[i].m_bIsDelete)
// 				ms_E3Attr[i].m_fWeight = GetCollapseWeight(ms_E3Attr[i]);
// 		}
	}

	//排序三角形，把删除的，按顺序都放在后面
	unsigned int uiExChangeIndex = 0;

	VSDataBuffer * pIndexDate = ms_pTriangleSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}

	VSUSHORT_INDEX *pIndex = (VSUSHORT_INDEX *)pIndexDate->GetDate();
	VSDataBufferPtr pCopeIndexBuffer = VS_NEW VSDataBuffer();
	VSObject::CloneObject(pIndexDate,pCopeIndexBuffer);
	VSUSHORT_INDEX *pCopyIndex = (VSUSHORT_INDEX *)pCopeIndexBuffer->GetDate();
	VSArray<unsigned int> MapTriangeIndex;
	for (unsigned int i = 0 ; i < ms_T3Attr.GetNum() ; i++)
	{
		if (!ms_T3Attr[i].m_bIsDelete)
		{
			pIndex[3 * uiExChangeIndex + 0] = pCopyIndex[3 * i + 0];

			pIndex[3 * uiExChangeIndex + 1] = pCopyIndex[3 * i + 1];

			pIndex[3 * uiExChangeIndex + 2] = pCopyIndex[3 * i + 2];

			uiExChangeIndex++;
			MapTriangeIndex.AddElement(i);
		}
	}
	for (int i = ms_RecordTriangle.GetNum() - 1 ; i >= 0 ; i--)
	{
		pIndex[3 * uiExChangeIndex + 0] = pCopyIndex[3 * ms_RecordTriangle[i] + 0];

		pIndex[3 * uiExChangeIndex + 1] = pCopyIndex[3 * ms_RecordTriangle[i] + 1];

		pIndex[3 * uiExChangeIndex + 2] = pCopyIndex[3 * ms_RecordTriangle[i] + 2];
		MapTriangeIndex.AddElement(ms_RecordTriangle[i]);
		uiExChangeIndex++;
	}

	//重新映射三角形索引
	for (int i = ms_pCRT->m_RecordInfo.GetNum() - 1 ;i >=0  ; i--)
	{
		RecordTriInfo & RecordInfoRef = ms_pCRT->m_RecordInfo[i];
		for (unsigned int j = 0 ; j < RecordInfoRef.m_MapIndex.GetNum() ; j++)
		{
			for (unsigned int k = 0 ; k < MapTriangeIndex.GetNum() ; k++)
			{
				bool IsFind = false;
				for (unsigned int s = 0 ; s < 3 ; s++)
				{
					if (RecordInfoRef.m_MapIndex[j] == (MapTriangeIndex[k] * 3 + s))
					{
						RecordInfoRef.m_MapIndex[j] = k * 3 + s;
						IsFind = true;
						break;
					}
				}
				if (IsFind)
				{
					break;
				}
				
			}
		}
	}
	VSCollapseRecordTri * PReturn = ms_pCRT;
	ms_pCRT = NULL;
	//只有一个原始信息，其他的记录信息都没有，则删除,表示这个Mesh已经无法精简
	if (PReturn->m_RecordInfo.GetNum() == 1)
	{
		VSMAC_DELETE(PReturn);
	}
// 	VSLog RecordLod;
// 	RecordLod.FOpen("RecordLog.txt");
// 	for (unsigned int i = 0 ; i < PReturn->m_RecordInfo.GetNum() ; i++)
// 	{
// 		RecordLod.WriteInfo("%dRecord\n",i);
// 		RecordLod.WriteInfo("Keep %d,Throw %d,Left %d\n",PReturn->m_RecordInfo[i].m_uiKeep,
// 							PReturn->m_RecordInfo[i].m_uiThrow,PReturn->m_RecordInfo[i].m_uiLeftTraingleNum);
// 		RecordLod.WriteInfo("Index:");
// 		for (unsigned int j = 0 ; j < PReturn->m_RecordInfo[i].m_MapIndex.GetNum() ; j++)
// 		{
// 			RecordLod.WriteInfo("%d ",PReturn->m_RecordInfo[i].m_MapIndex[j]);
// 		}
// 		RecordLod.WriteInfo("\n");
// 	}
	
	return PReturn;
}
void VSCreateClodTriFactory::InitalDate()
{

	VSDataBuffer * pIndexDate = ms_pTriangleSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return ;
	}
	unsigned int uiVerNum = ms_pTriangleSet->GetVertexBuffer()->GetVertexNum();

	unsigned int uiTriNum = ms_pTriangleSet->GetTotleNum();
	ms_uiLeftTrangleNum = uiTriNum;
	VSDataBuffer * pVertexDate = ms_pTriangleSet->GetVertexBuffer()->GetPositionDate(0);
	ms_V3Attr.SetBufferNum(uiVerNum);
	VSVector3 * pVer = (VSVector3 *)pVertexDate->GetDate();

	for (unsigned int i = 0 ; i < uiVerNum ; i++)
	{
		ms_V3Attr[i].m_bIsDelete = false;
		ms_V3Attr[i].m_Position = *pVer;
		pVer++;
	}


	ms_T3Attr.SetBufferNum(uiTriNum);

	

	VSUSHORT_INDEX * pIndex = (VSUSHORT_INDEX *)pIndexDate->GetDate();

	for(unsigned int i = 0 ; i < uiTriNum ; i++)
	{
		ms_T3Attr[i].m_bIsDelete = false;
		ms_T3Attr[i].m_ContainVertexIndex[0] = *pIndex;
		ms_V3Attr[*pIndex].m_InTriangleIndexArray.AddElement(i);
		pIndex++;

		ms_T3Attr[i].m_ContainVertexIndex[1] = *pIndex;
		ms_V3Attr[*pIndex].m_InTriangleIndexArray.AddElement(i);
		pIndex++;

		ms_T3Attr[i].m_ContainVertexIndex[2] = *pIndex;
		ms_V3Attr[*pIndex].m_InTriangleIndexArray.AddElement(i);
		pIndex++;


	}



	for(unsigned int i = 0 ; i < uiTriNum ; i++)
	{
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			unsigned int uiVertexIndex0 = ms_T3Attr[i].m_ContainVertexIndex[j];
			unsigned int uiVertexIndex1 = ms_T3Attr[i].m_ContainVertexIndex[(j + 1) % 3];

			unsigned int uiEdge = 0;
			for ( uiEdge = 0 ; uiEdge < ms_E3Attr.GetNum() ; uiEdge++)
			{
				if ((ms_E3Attr[uiEdge].m_ContainVertexIndex[0] == uiVertexIndex0 && 
					ms_E3Attr[uiEdge].m_ContainVertexIndex[1] == uiVertexIndex1) ||
					(ms_E3Attr[uiEdge].m_ContainVertexIndex[1] == uiVertexIndex0 && 
					ms_E3Attr[uiEdge].m_ContainVertexIndex[0] == uiVertexIndex1))
				{
					break;
				}
			}

			if (uiEdge == ms_E3Attr.GetNum())
			{
				Edge3DAttr NewEdge;
				NewEdge.m_ContainVertexIndex[0] = uiVertexIndex0;
				NewEdge.m_ContainVertexIndex[1] = uiVertexIndex1;
				ms_E3Attr.AddElement(NewEdge);

				ms_V3Attr[uiVertexIndex0].m_InEdgeIndexArray.AddElement(uiEdge);
				ms_V3Attr[uiVertexIndex1].m_InEdgeIndexArray.AddElement(uiEdge);
			}
			ms_E3Attr[uiEdge].m_InTriangleIndexArray.AddElement(i);
			ms_T3Attr[i].m_ContainEdgeIndex[j] = uiEdge;
		}

	}

// 	VSLog LogEdge;
// 	LogEdge.FOpen("LogEdge.txt");
	for(unsigned int i = 0 ; i < ms_E3Attr.GetNum() ; i++)
	{
		ms_E3Attr[i].m_bIsDelete = false;
		ms_E3Attr[i].m_fWeight = GetCollapseWeight(ms_E3Attr[i]);
//		LogEdge.WriteInfo("%dIndex %d,%d Weight: %f\n",i,ms_E3Attr[i].m_ContainVertexIndex[0],ms_E3Attr[i].m_ContainVertexIndex[1],ms_E3Attr[i].m_fWeight);
	}


}
VSREAL VSCreateClodTriFactory::GetCollapseWeight(const Edge3DAttr & Edge3D)
{
	if (Edge3D.m_bIsDelete == true)
	{
		return VSMAX_REAL;
	}
	const VSREAL cfLengthWeight = 10.0f;
	const VSREAL cfAngleWeight = 1.0f;
	if (Edge3D.m_InTriangleIndexArray.GetNum() == 2)
	{
		unsigned int uiIndex0 = Edge3D.m_ContainVertexIndex[0];
		unsigned int uiIndex1 = Edge3D.m_ContainVertexIndex[1];
		VSMAC_ASSERT(uiIndex0 < ms_V3Attr.GetNum() && uiIndex1 < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[uiIndex0].m_bIsDelete && !ms_V3Attr[uiIndex1].m_bIsDelete );
		// 		if (uiIndex0 >= ms_V3Attr.GetNum() || uiIndex1 >= ms_V3Attr.GetNum())
		// 		{
		// 			return VSMAX_REAL;
		// 		}
		VSVector3 V0 = ms_V3Attr[uiIndex0].m_Position;
		VSVector3 V1 = ms_V3Attr[uiIndex1].m_Position;

		VSVector3 Diff = V0 - V1;

		VSREAL fWeight = cfLengthWeight * Diff.GetLength();



		unsigned int uiTri0 = Edge3D.m_InTriangleIndexArray[0];
		unsigned int uiTri1 = Edge3D.m_InTriangleIndexArray[1];

		VSMAC_ASSERT(uiTri0 < ms_T3Attr.GetNum() && uiTri1 < ms_T3Attr.GetNum());
		VSMAC_ASSERT(!ms_T3Attr[uiTri0].m_bIsDelete && !ms_T3Attr[uiTri1].m_bIsDelete);
		// 		if (uiTri0 >= ms_T3Attr.GetNum() || uiTri1 >= ms_T3Attr.GetNum() )
		// 		{
		// 			return VSMAX_REAL;
		// 		}
		VSMAC_ASSERT(ms_T3Attr[uiTri0].m_ContainVertexIndex[0] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri0].m_ContainVertexIndex[1] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri0].m_ContainVertexIndex[2] < ms_V3Attr.GetNum());

		VSMAC_ASSERT(!ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[0]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[1]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[2]].m_bIsDelete );

		// 		if (ms_T3Attr[uiTri0].m_ContainVertexIndex[0] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri0].m_ContainVertexIndex[1] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri0].m_ContainVertexIndex[2] >= ms_V3Attr.GetNum())
		// 		{
		// 			return VSMAX_REAL;
		// 		}

		VSVector3 T0V0 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[0]].m_Position;
		VSVector3 T0V1 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[1]].m_Position;
		VSVector3 T0V2 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[2]].m_Position;

		VSVector3 DiffT0_01 = T0V1 - T0V0;
		VSVector3 DiffT0_02 = T0V2 - T0V0;

		VSVector3 T0N;
		T0N.Cross(DiffT0_01,DiffT0_02);

		VSMAC_ASSERT(ms_T3Attr[uiTri1].m_ContainVertexIndex[0] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri1].m_ContainVertexIndex[1] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri1].m_ContainVertexIndex[2] < ms_V3Attr.GetNum());

		VSMAC_ASSERT(!ms_V3Attr[ms_T3Attr[uiTri1].m_ContainVertexIndex[0]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri1].m_ContainVertexIndex[1]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri1].m_ContainVertexIndex[2]].m_bIsDelete );
		// 		if (ms_T3Attr[uiTri1].m_ContainVertexIndex[0] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri1].m_ContainVertexIndex[1] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri1].m_ContainVertexIndex[2] >= ms_V3Attr.GetNum())
		// 		{
		// 			return VSMAX_REAL;
		// 		}

		VSVector3 T1V0 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[0]].m_Position;
		VSVector3 T1V1 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[1]].m_Position;
		VSVector3 T1V2 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[2]].m_Position;

		VSVector3 DiffT1_01 = T1V1 - T1V0;
		VSVector3 DiffT1_02 = T1V2 - T1V0;

		VSVector3 T1N;
		T1N.Cross(DiffT1_01,DiffT1_02);

		//2个三角行越垂直权重越大，c = |a| |b| sin; 也就是和2个三角形夹角有关，
		VSVector3 CrossAngle;
		CrossAngle.Cross(T0N,T1N);
		fWeight += CrossAngle.GetLength() * cfAngleWeight;

		return fWeight;
	}
	return VSMAX_REAL;
}
bool VSCreateClodTriFactory::GetCollapseEV(unsigned int &uiE3DIndex,unsigned int &uiKeepV3DIndex,unsigned int &uiThrowV3DIndex)
{
	while(1)
	{
		VSREAL fFindMinWeight = VSMAX_REAL;
		Edge3DAttr * pDesireEdge3D = NULL;
		for (unsigned int i = 0 ; i < ms_E3Attr.GetNum() ;i++)
		{
			if (ms_E3Attr[i].m_bIsDelete)
			{
				continue;
			}
			if (fFindMinWeight > ms_E3Attr[i].m_fWeight)
			{
				fFindMinWeight = ms_E3Attr[i].m_fWeight;
				pDesireEdge3D = &ms_E3Attr[i];
				uiE3DIndex = i;
			}
		}

		if (fFindMinWeight >= VSMAX_REAL || pDesireEdge3D == NULL)
		{
			return 0;
		}

		//检测共享这条边上顶点的所有边，如果这个顶点的所有边都有2个三角形共享，则这个顶点就是throw。如果找不到
		//则把这个边的权重改成VSMAX_REAL，继续找，直到最小权重为VSMAX_REAL，结束，则求ClodMesh结束
		unsigned int j = 0;
		for (j = 0 ; j < 2 ; j++)
		{
			unsigned int uiV3DIndex = pDesireEdge3D->m_ContainVertexIndex[j];
			VSMAC_ASSERT(uiV3DIndex < ms_V3Attr.GetNum());
			VSMAC_ASSERT(!ms_V3Attr[uiV3DIndex].m_bIsDelete);
			unsigned int k = 0;
			for ( k = 0 ; k < ms_V3Attr[uiV3DIndex].m_InEdgeIndexArray.GetNum() ; k++)
			{
				unsigned int uiE3DIndex = ms_V3Attr[uiV3DIndex].m_InEdgeIndexArray[k];

				VSMAC_ASSERT(uiE3DIndex < ms_E3Attr.GetNum());
				VSMAC_ASSERT(!ms_E3Attr[uiE3DIndex].m_bIsDelete);
				if (ms_E3Attr[uiE3DIndex].m_InTriangleIndexArray.GetNum() != 2)
				{
					break;
				}
			}
			if (k == ms_V3Attr[uiV3DIndex].m_InEdgeIndexArray.GetNum())
			{
				break;
			}
		}
		if (j < 2)
		{
			uiThrowV3DIndex = pDesireEdge3D->m_ContainVertexIndex[j];
			uiKeepV3DIndex = pDesireEdge3D->m_ContainVertexIndex[1 -j];
			return 1;
		}
		else
		{
			pDesireEdge3D->m_fWeight = VSMAX_REAL;

		}
	}
	return 1;
}
bool VSCreateClodTriFactory::Collapse(unsigned int uiE3DIndex,unsigned int uiKeepV3DIndex,unsigned int uiThrowV3DIndex)
{
	VSMAC_ASSERT(uiE3DIndex < ms_E3Attr.GetNum());
	VSMAC_ASSERT(!ms_E3Attr[uiE3DIndex].m_bIsDelete);

	VSMAC_ASSERT(uiKeepV3DIndex < ms_V3Attr.GetNum());
	VSMAC_ASSERT(!ms_V3Attr[uiKeepV3DIndex].m_bIsDelete);
	VSMAC_ASSERT(uiThrowV3DIndex < ms_V3Attr.GetNum());
	VSMAC_ASSERT(!ms_V3Attr[uiThrowV3DIndex].m_bIsDelete);

	RecordTriInfo CollapseRecord;
	CollapseRecord.m_uiKeep = uiKeepV3DIndex;
	CollapseRecord.m_uiThrow = uiThrowV3DIndex;
	CollapseRecord.m_uiLeftTraingleNum = ms_uiLeftTrangleNum;
	//1.删除共享uiE3DIndex边的三角形，要同时把共享这个三角形的顶点和边列表中删除
	for (unsigned int i = 0 ; i < ms_E3Attr[uiE3DIndex].m_InTriangleIndexArray.GetNum() ; i++)
	{
		//删除三角形
		unsigned int uiT3DIndex = ms_E3Attr[uiE3DIndex].m_InTriangleIndexArray[i];
		VSMAC_ASSERT(uiT3DIndex < ms_T3Attr.GetNum());
		VSMAC_ASSERT(!ms_T3Attr[uiT3DIndex].m_bIsDelete);
		ms_T3Attr[uiT3DIndex].m_bIsDelete = true;
		ms_uiLeftTrangleNum--;
		//删除三角形的3条边中对应的三角形信息
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			unsigned int uiEdgeIndex = ms_T3Attr[uiT3DIndex].m_ContainEdgeIndex[j];
			VSMAC_ASSERT(uiEdgeIndex < ms_E3Attr.GetNum());
			VSMAC_ASSERT(!ms_E3Attr[uiEdgeIndex].m_bIsDelete);
			if(uiE3DIndex == uiEdgeIndex)
			{
				i--;
			}
			unsigned int k = 0;
			for (k = 0 ; k < ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray.GetNum() ; k++)
			{
				if (ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray[k] == uiT3DIndex)
				{
					break;
				}	
			}
			//找到了这个三角形,这个边不再属于这个三角形，删除
			VSMAC_ASSERT(k < ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray.GetNum());
			ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray.Erase(k);
		}
		//删除三角形对应的3个顶点的三角形信息
		for (unsigned int j = 0 ; j < 3 ;j ++)
		{
			unsigned int uiVIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[j];
			VSMAC_ASSERT(uiVIndex < ms_V3Attr.GetNum());
			VSMAC_ASSERT(!ms_V3Attr[uiVIndex].m_bIsDelete);

			unsigned int k = 0;
			for (k = 0 ; k < ms_V3Attr[uiVIndex].m_InTriangleIndexArray.GetNum() ; k++)
			{
				if (ms_V3Attr[uiVIndex].m_InTriangleIndexArray[k] == uiT3DIndex)
				{
					break;
				}	
			}
			//找到了这个三角形，这个顶点不再属于这个三角形，删除
			VSMAC_ASSERT(k < ms_V3Attr[uiVIndex].m_InTriangleIndexArray.GetNum());
			ms_V3Attr[uiVIndex].m_InTriangleIndexArray.Erase(k);
		}
		ms_RecordTriangle.AddElement(uiT3DIndex);
	}

	//2.删除边信息，同时共享这个边顶点列表信息，不用删除共享这个边三角形列表信息，因为这个三角形已经被删除
	ms_E3Attr[uiE3DIndex].m_bIsDelete = true;
	for (unsigned int i = 0 ; i < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum() ; i++)
	{
		if (ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[i] == uiE3DIndex)
		{
			ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.Erase(i);
			break;
		}
	}
	for (unsigned int i = 0 ; i < ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.GetNum() ; i++)
	{
		if (ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[i] == uiE3DIndex)
		{
			ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.Erase(i);
			break;
		}
	}

	//3.处理共享要被删除顶点的三角形，替换2个顶点
	for (unsigned int i = 0 ; i < ms_V3Attr[uiThrowV3DIndex].m_InTriangleIndexArray.GetNum() ; i++)
	{
		unsigned int uiT3DIndex = ms_V3Attr[uiThrowV3DIndex].m_InTriangleIndexArray[i];
		VSMAC_ASSERT(uiT3DIndex < ms_T3Attr.GetNum());
		VSMAC_ASSERT(!ms_T3Attr[uiT3DIndex].m_bIsDelete);

		bool bFound = false;
		unsigned int uiFoundIndex;
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			if(ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[j] == uiThrowV3DIndex)
			{
				VSMAC_ASSERT(!bFound);
				uiFoundIndex = j;
				bFound = true;
			}

		}
		VSMAC_ASSERT(bFound);

		unsigned int V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[0];
		VSMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		VSVector3 V0 = ms_V3Attr[V3DIndex].m_Position;

		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[1];
		VSMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		VSVector3 V1 = ms_V3Attr[V3DIndex].m_Position;


		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[2];
		VSMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		VSVector3 V2 = ms_V3Attr[V3DIndex].m_Position;


		//替换新顶点
		ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[uiFoundIndex] = uiKeepV3DIndex;

		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[0];
		VSMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		VSVector3 NewV0 = ms_V3Attr[V3DIndex].m_Position;

		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[1];
		VSMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		VSVector3 NewV1 = ms_V3Attr[V3DIndex].m_Position;


		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[2];
		VSMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		VSMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		VSVector3 NewV2 = ms_V3Attr[V3DIndex].m_Position;

		VSVector3 Normal;
		Normal.Cross(V2 - V0 , V1 - V0);

		VSVector3 NewNormal;
		NewNormal.Cross(NewV2 - NewV0 , NewV1 - NewV0);

		
		
		ms_V3Attr[uiKeepV3DIndex].m_InTriangleIndexArray.AddElement(uiT3DIndex);
		CollapseRecord.m_MapIndex.AddElement(uiT3DIndex * 3 + uiFoundIndex);
		
	}
	
	VSMAC_ASSERT(CollapseRecord.m_uiLeftTraingleNum !=ms_uiLeftTrangleNum);

	CollapseRecord.m_uiLeftTraingleNum = ms_uiLeftTrangleNum;

	//4.边替换顶点
	for (unsigned int i = 0 ; i < ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.GetNum(); i++)
	{
		unsigned int uiE3DIndex = ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[i];
		for (unsigned int j = 0 ; j < 2 ; j++)
		{
			if(ms_E3Attr[uiE3DIndex].m_ContainVertexIndex[j] == uiThrowV3DIndex)
			{
				ms_E3Attr[uiE3DIndex].m_ContainVertexIndex[j] = uiKeepV3DIndex;
				ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.AddElement(uiE3DIndex);
				break;
			}
		}
		
	}
	

	ms_V3Attr[uiThrowV3DIndex].m_bIsDelete = true;
	//5.处理替换顶点后，冗余的边信息,合并边信息，然后处理边共享的三角形，和这些共享三角形的边信息
	for (unsigned int i = 0 ; i < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum(); i++)
	{
		unsigned int uiEIndex1 = ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[i];

		for (unsigned int j = 0 ; j < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum(); j++)
		{
			unsigned int uiEIndex2 = ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[j];

			if (uiEIndex1 != uiEIndex2)
			{

				if (ms_E3Attr[uiEIndex1] == ms_E3Attr[uiEIndex2])
				{
					ms_E3Attr[uiEIndex2].m_bIsDelete = true;
					//共享这个边的三角形，这个边要替换掉
					for (unsigned int k = 0 ; k < ms_E3Attr[uiEIndex2].m_InTriangleIndexArray.GetNum(); k++)
					{
						unsigned int uiTIndex = ms_E3Attr[uiEIndex2].m_InTriangleIndexArray[k];
						ms_E3Attr[uiEIndex1].m_InTriangleIndexArray.AddElement(uiTIndex);
						for (unsigned int m = 0 ; m < 3 ; m++)
						{
							if (ms_T3Attr[uiTIndex].m_ContainEdgeIndex[m] == uiEIndex2)
							{
								ms_T3Attr[uiTIndex].m_ContainEdgeIndex[m] = uiEIndex1;
							}
						}
						
					}
					//共享这个边的顶点，这个边要删除
					ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.Erase(j);
					
					unsigned int uiAnotherV = ms_E3Attr[uiEIndex2].m_ContainVertexIndex[0];
					if(uiAnotherV == uiKeepV3DIndex)
					{
						uiAnotherV = ms_E3Attr[uiEIndex2].m_ContainVertexIndex[1];
					}
					
					for(unsigned int s = 0 ; s < ms_V3Attr[uiAnotherV].m_InEdgeIndexArray.GetNum(); s++)
					{
						if (uiEIndex2 == ms_V3Attr[uiAnotherV].m_InEdgeIndexArray[s])
						{
							ms_V3Attr[uiAnotherV].m_InEdgeIndexArray.Erase(s);
							break;
						}
					
					}
					j--;
					
				}

				

			}
		}	
	}
	for (unsigned int i = 0 ; i < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum(); i++)
	{
		unsigned int uiEIndex1 = ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[i];
		ms_E3Attr[uiEIndex1].m_fWeight = GetCollapseWeight(ms_E3Attr[uiEIndex1]);
	}
	ms_pCRT->m_RecordInfo.AddElement(CollapseRecord);
	return 1;
}



VSArray<VSCreateClodLineFactory::Vertex3DAttr> VSCreateClodLineFactory::ms_V3Attr;
VSArray<VSCreateClodLineFactory::Edge3DAttr> VSCreateClodLineFactory::ms_E3Attr;
VSLineSet* VSCreateClodLineFactory::ms_pLineSet = NULL;
VSCollapseRecordLine *VSCreateClodLineFactory::ms_pCRL = NULL;
VSArray<unsigned int> VSCreateClodLineFactory::ms_RecordLine;
unsigned int VSCreateClodLineFactory::ms_uiLeftLineNum = 0;
VSCollapseRecordLine * VSCreateClodLineFactory::CreateClodLine( VSLineSet * pLineSet)
{
	if (!pLineSet)
	{
		return 0;
	}
	ms_pLineSet = pLineSet;
	if (!ms_pLineSet->GetVertexBuffer())
	{
		return 0;

	}

	VSDataBuffer * pVertexDate = ms_pLineSet->GetVertexBuffer()->GetPositionDate(0);

	if (!pVertexDate)
	{
		return 0;
	}

	if (!pVertexDate->GetDate())
	{
		return 0;
	}
	unsigned int uiVerNum = pVertexDate->GetNum();

	if ( !uiVerNum)
	{
		return 0;
	}

	if (!ms_pLineSet->GetIndexBuffer())
	{
		return 0;
	}

	if (!ms_pLineSet->GetIndexBuffer()->GetIndexDate() || !ms_pLineSet->GetIndexBuffer()->GetNum())
	{
		return 0;
	}

	ms_V3Attr.Clear();
	ms_E3Attr.Clear();

	VSMAC_DELETE(ms_pCRL);

	ms_pCRL = VS_NEW VSCollapseRecordLine();

	if (!ms_pCRL)
	{
		return 0;
	}
	ms_RecordLine.Clear();

	InitalDate();
	//填入原始的三角形信息，作为第 0 个索引的 record ，所以替换三角形索引的mapindex数组为0
	//keep 和 throw 都是无效的
	RecordLineInfo CollapseRecord;
	CollapseRecord.m_uiKeep = VSMAX_INTEGER;
	CollapseRecord.m_uiThrow = VSMAX_INTEGER;
	CollapseRecord.m_uiLeftEdgeNum = ms_uiLeftLineNum;
	ms_pCRL->m_RecordInfo.AddElement(CollapseRecord);
	unsigned int uiThrowV3DIndex = 0;

	while(GetCollapseV(uiThrowV3DIndex))
	{
		Collapse(uiThrowV3DIndex);

	}

	//排序边，把删除的，按顺序都放在后面
	unsigned int uiExChangeIndex = 0;

	VSDataBuffer * pIndexDate = ms_pLineSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}

	VSUSHORT_INDEX *pIndex = (VSUSHORT_INDEX *)pIndexDate->GetDate();
	VSDataBufferPtr pCopeIndexBuffer = VS_NEW VSDataBuffer();
	VSObject::CloneObject(pIndexDate,pCopeIndexBuffer);
	VSUSHORT_INDEX *pCopyIndex = (VSUSHORT_INDEX *)pCopeIndexBuffer->GetDate();
	VSArray<unsigned int> MapLineIndex;
	for (unsigned int i = 0 ; i < ms_E3Attr.GetNum() ; i++)
	{
		if (!ms_E3Attr[i].m_bIsDelete)
		{
			pIndex[2 * uiExChangeIndex + 0] = pCopyIndex[2 * i + 0];

			pIndex[2 * uiExChangeIndex + 1] = pCopyIndex[2 * i + 1];

			uiExChangeIndex++;
			MapLineIndex.AddElement(i);
		}
	}
	for (int i = ms_RecordLine.GetNum() - 1 ; i >= 0 ; i--)
	{
		pIndex[2 * uiExChangeIndex + 0] = pCopyIndex[2 * ms_RecordLine[i] + 0];

		pIndex[2 * uiExChangeIndex + 1] = pCopyIndex[2 * ms_RecordLine[i] + 1];
		MapLineIndex.AddElement(ms_RecordLine[i]);
		uiExChangeIndex++;
	}
 
	//重新映射三角形索引
	for (int i = ms_pCRL->m_RecordInfo.GetNum() - 1 ;i >= 0  ; i--)
	{
		RecordLineInfo & RecordInfoRef = ms_pCRL->m_RecordInfo[i];
		
		for (unsigned int k = 0 ; k < MapLineIndex.GetNum() ; k++)
		{
			bool IsFind = false;
			for (unsigned int s = 0 ; s < 2 ; s++)
			{
				if (RecordInfoRef.m_MapIndex == (MapLineIndex[k] * 2 + s))
				{
					RecordInfoRef.m_MapIndex = k * 2 + s;
					IsFind = true;
					break;
				}
			}
			if (IsFind)
			{
				break;
			}

		}
	}
	VSCollapseRecordLine * PReturn = ms_pCRL;
	ms_pCRL = NULL;
	//只有一个原始信息，其他的记录信息都没有，则删除,表示这个Mesh已经无法精简
	if (PReturn->m_RecordInfo.GetNum() == 1)
	{
		VSMAC_DELETE(PReturn);
	}

	return PReturn;
}

void VSCreateClodLineFactory::InitalDate()
{

	VSDataBuffer * pIndexDate = ms_pLineSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return ;
	}
	unsigned int uiVerNum = ms_pLineSet->GetVertexBuffer()->GetVertexNum();

	unsigned int uiLineNum = ms_pLineSet->GetTotleNum();

	ms_uiLeftLineNum = uiLineNum;
	VSDataBuffer * pVertexDate = ms_pLineSet->GetVertexBuffer()->GetPositionDate(0);
	ms_V3Attr.SetBufferNum(uiVerNum);
	VSVector3 * pVer = (VSVector3 *)pVertexDate->GetDate();

	for (unsigned int i = 0 ; i < uiVerNum ; i++)
	{
		ms_V3Attr[i].m_bIsDelete = false;
		ms_V3Attr[i].m_Position = *pVer;
		ms_V3Attr[i].m_uiIndexInVertexArray = i;
		pVer++;
	}


	ms_E3Attr.SetBufferNum(uiLineNum);

	

	VSUSHORT_INDEX * pIndex = (VSUSHORT_INDEX *)pIndexDate->GetDate();

	for(unsigned int i = 0 ; i < uiLineNum ; i++)
	{
		ms_E3Attr[i].m_bIsDelete = false;
		ms_E3Attr[i].m_ContainVertexIndex[0] = *pIndex;
		ms_V3Attr[*pIndex].m_InEdgeIndexArray.AddElement(i);
		pIndex++;

		ms_E3Attr[i].m_ContainVertexIndex[1] = *pIndex;
		ms_V3Attr[*pIndex].m_InEdgeIndexArray.AddElement(i);
		pIndex++;

	}

	// 	VSLog LogEdge;
	// 	LogEdge.FOpen("LogEdge.txt");
	for(unsigned int i = 0 ; i < ms_V3Attr.GetNum() ; i++)
	{
		ms_V3Attr[i].m_bIsDelete = false;
		ms_V3Attr[i].m_fWeight = GetCollapseWeight(ms_V3Attr[i]);
		//		LogEdge.WriteInfo("%dIndex %d,%d Weight: %f\n",i,ms_E3Attr[i].m_ContainVertexIndex[0],ms_E3Attr[i].m_ContainVertexIndex[1],ms_E3Attr[i].m_fWeight);
	}


}
VSREAL VSCreateClodLineFactory::GetCollapseWeight(const Vertex3DAttr & Vertex3D)
{
	if (Vertex3D.m_bIsDelete == true)
	{
		return VSMAX_REAL;
	}
	if (Vertex3D.m_InEdgeIndexArray.GetNum() == 2)
	{
		unsigned int uiIndex0 = Vertex3D.m_InEdgeIndexArray[0];
		unsigned int uiIndex1 = Vertex3D.m_InEdgeIndexArray[1];

		
		VSMAC_ASSERT(uiIndex0 < ms_E3Attr.GetNum() && uiIndex1 < ms_E3Attr.GetNum());
		VSMAC_ASSERT(!ms_E3Attr[uiIndex0].m_bIsDelete && !ms_E3Attr[uiIndex1].m_bIsDelete );
		
		VSVector3 V0;
		VSVector3 V1;
		if (ms_E3Attr[uiIndex0].m_ContainVertexIndex[0] == Vertex3D.m_uiIndexInVertexArray)
		{
			V0 = ms_V3Attr[ms_E3Attr[uiIndex0].m_ContainVertexIndex[1]].m_Position;
		}
		else if(ms_E3Attr[uiIndex0].m_ContainVertexIndex[1] == Vertex3D.m_uiIndexInVertexArray)
		{
			V0 = ms_V3Attr[ms_E3Attr[uiIndex0].m_ContainVertexIndex[0]].m_Position;
		}
		else
		{
			VSMAC_ASSERT(0);
		}
		
		if (ms_E3Attr[uiIndex1].m_ContainVertexIndex[0] == Vertex3D.m_uiIndexInVertexArray)
		{
			V1 = ms_V3Attr[ms_E3Attr[uiIndex1].m_ContainVertexIndex[1]].m_Position;
		}
		else if(ms_E3Attr[uiIndex1].m_ContainVertexIndex[1] == Vertex3D.m_uiIndexInVertexArray)
		{
			V1 = ms_V3Attr[ms_E3Attr[uiIndex1].m_ContainVertexIndex[0]].m_Position;
		}
		else
		{
			VSMAC_ASSERT(0);
		}

		VSVector3 Diff = V0 - V1;

		VSREAL fLength = Diff.GetLength();
		VSSegment3 Segment3(V0,V1);
		VSREAL fParam;
		VSREAL fDistan = Vertex3D.m_Position.SquaredDistance(Segment3,fParam);
		
		return fDistan / fLength;
	}
	return VSMAX_REAL;
}
bool VSCreateClodLineFactory::GetCollapseV(unsigned int &uiThrowV3DIndex)
{

	VSREAL fFindMinWeight = VSMAX_REAL;
	Vertex3DAttr * pDesireVertex3D = NULL;
	for (unsigned int i = 0 ; i < ms_V3Attr.GetNum() ;i++)
	{
		if (ms_V3Attr[i].m_bIsDelete)
		{
			continue;
		}
		if (fFindMinWeight > ms_V3Attr[i].m_fWeight)
		{
			fFindMinWeight = ms_V3Attr[i].m_fWeight;
			pDesireVertex3D = &ms_V3Attr[i];
			uiThrowV3DIndex = i;
		}
	}

	if (fFindMinWeight >= VSMAX_REAL || pDesireVertex3D == NULL)
	{
		return 0;
	}

	return 1;
}
bool VSCreateClodLineFactory::Collapse(unsigned int uiThrowV3DIndex)
{

	VSMAC_ASSERT(uiThrowV3DIndex < ms_V3Attr.GetNum());
	VSMAC_ASSERT(!ms_V3Attr[uiThrowV3DIndex].m_bIsDelete);

	VSMAC_ASSERT(ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.GetNum() == 2);
	unsigned int uiIndex0 = ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[0];
	unsigned int uiIndex1 = ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[1];

	VSMAC_ASSERT(uiIndex0 < ms_E3Attr.GetNum() && uiIndex1 < ms_E3Attr.GetNum());
	VSMAC_ASSERT(!ms_E3Attr[uiIndex0].m_bIsDelete && !ms_E3Attr[uiIndex1].m_bIsDelete );

	//删除顶点和边
	ms_V3Attr[uiThrowV3DIndex].m_bIsDelete = true;
	ms_uiLeftLineNum--;
	ms_E3Attr[uiIndex1].m_bIsDelete = true;

	//找到被删除边的和throw不相同的另一个顶点
	unsigned int uiEdgeV = ms_E3Attr[uiIndex1].m_ContainVertexIndex[0];

	if (uiEdgeV == uiThrowV3DIndex)
	{
		uiEdgeV = ms_E3Attr[uiIndex1].m_ContainVertexIndex[1];
	}

	ms_RecordLine.AddElement(uiIndex1);



	VSMAC_ASSERT(uiEdgeV < ms_V3Attr.GetNum());

	VSMAC_ASSERT(!ms_V3Attr[uiEdgeV].m_bIsDelete);

	//边替换顶点
	unsigned int uiEdgeV1 = ms_E3Attr[uiIndex0].m_ContainVertexIndex[0];
	unsigned int uiMapInde = 0;
	if (uiEdgeV1 == uiThrowV3DIndex)
	{
		uiEdgeV1 = ms_E3Attr[uiIndex0].m_ContainVertexIndex[1];

		uiMapInde = 0;
		ms_E3Attr[uiIndex0].m_ContainVertexIndex[0] = uiEdgeV;
	}
	else
	{
	
		uiMapInde = 1;
		ms_E3Attr[uiIndex0].m_ContainVertexIndex[1] = uiEdgeV;
	}

	//替换这个顶点的边
	unsigned int i ;
	for ( i = 0 ; i < ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.GetNum() ;i++)
	{
		if (ms_V3Attr[uiEdgeV].m_InEdgeIndexArray[i] == uiIndex1)
		{
			
			break;
		}
	}
	VSMAC_ASSERT(i <ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.GetNum());
	ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.Erase(i);
	ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.AddElement(uiIndex0);

	VSMAC_ASSERT(uiMapInde < 2);
	RecordLineInfo CollapseRecord;
	CollapseRecord.m_uiThrow = uiThrowV3DIndex;
	CollapseRecord.m_MapIndex = uiIndex0 * 2 + uiMapInde;
	CollapseRecord.m_uiKeep = uiEdgeV;
	CollapseRecord.m_uiLeftEdgeNum = ms_uiLeftLineNum;
	ms_pCRL->m_RecordInfo.AddElement(CollapseRecord);

	ms_V3Attr[uiEdgeV].m_fWeight = GetCollapseWeight(ms_V3Attr[uiEdgeV]);
	ms_V3Attr[uiEdgeV1].m_fWeight = GetCollapseWeight(ms_V3Attr[uiEdgeV1]);
	return 1;
}