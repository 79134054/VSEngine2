#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSTriangleSet.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSGeometry,VSSpatial)
BEGIN_ADD_PROPERTY(VSGeometry,VSSpatial)
REGISTER_PROPERTY(m_LocalBV, LocalBV, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMeshDate,MeshData,VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pMorphMeshDate, MorphMeshData, VSProperty::F_CLONE)
REGISTER_PROPERTY(m_BoneName, BoneName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_GeometryName, GeometryName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMaterialInstance, MaterialInstance, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME );
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGeometry)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSGeometry> VSGeometry::ms_Quad = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCub = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCubCone = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCone = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultRenderCube = NULL;
bool VSGeometry::InitialDefaultState()
{
	ms_Quad = VS_NEW VSGeometry();
	if (!ms_Quad)
	{
		return false;
	}
	
	ms_DefaultCub = VS_NEW VSGeometry();
	if (!ms_DefaultCub)
	{
		return false;
	}

	ms_DefaultCubCone = VS_NEW VSGeometry();
	if (!ms_DefaultCubCone)
	{
		return false;
	}

	ms_DefaultCone = VS_NEW VSGeometry();
	if (!ms_DefaultCone)
	{
		return false;
	}
	ms_DefaultRenderCube = VS_NEW VSGeometry();
	if (!ms_DefaultRenderCube)
	{
		return false;
	}
	LoadDefault();
	return 1;
}
bool VSGeometry::TerminalDefaultState()
{
	ms_Quad = NULL;
	ms_DefaultCub = NULL;
	ms_DefaultCubCone = NULL;
	ms_DefaultCone = NULL;
	ms_DefaultRenderCube = NULL;
	return 1;
}
void VSGeometry::LoadDefault()
{
	//Quad
	{
		VSArray<VSVector3> VertexArray;
		VSArray<VSVector2> m_TexCoordArray;
		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 0.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 0.0f));

		m_TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		m_TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));
		m_TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		m_TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));

		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		VSDataBufferPtr  pVertexDate = VS_NEW VSDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pTexcoord = VS_NEW VSDataBuffer;
		pTexcoord->SetDate(&m_TexCoordArray[0],(unsigned int)m_TexCoordArray.GetNum(),VSDataBuffer::DT_FLOAT32_2);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);
		pVertexBuffer->SetDate(pTexcoord,VSVertexFormat::VF_TEXCOORD);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		VSTriangleSetPtr pTriangleSetDate = VS_NEW VSTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_Quad->SetMeshDate(pTriangleSetDate);
		ms_Quad->m_GeometryName = _T("DefaultQuad");
	}
	

	//Cub
	{
		VSArray<VSVector3> VertexArray;

		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 0.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 0.0f));

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(5);
		IndexArray.AddElement(4);
		IndexArray.AddElement(7);
		IndexArray.AddElement(6);

		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(7);
		IndexArray.AddElement(4);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(7);

		//right
		IndexArray.AddElement(1);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(1);
		IndexArray.AddElement(6);
		IndexArray.AddElement(2);

		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(0);
		IndexArray.AddElement(5);
		IndexArray.AddElement(1);

		//bottom
		IndexArray.AddElement(3);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(6);

		VSDataBufferPtr  pVertexDate = VS_NEW VSDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		VSTriangleSetPtr pTriangleSetDate = VS_NEW VSTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCub->SetMeshDate(pTriangleSetDate);
		ms_DefaultCub->m_GeometryName = _T("DefaultCub");
	}

	//Cub Cone
	{
		VSArray<VSVector3> VertexArray;

		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3( 0.0f,  0.0f, 0.0f));
		

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(1);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(4);
		IndexArray.AddElement(3);


		//right
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(1);


		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);


		//bottom
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(4);


		VSDataBufferPtr  pVertexDate = VS_NEW VSDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		VSTriangleSetPtr pTriangleSetDate = VS_NEW VSTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCubCone->SetMeshDate(pTriangleSetDate);
		ms_DefaultCubCone->m_GeometryName = _T("DefaultCubCone");
	}

	//Cone
	{
		unsigned int uiLevel = 1;
		unsigned int CircleLevel = 5;


		VSArray<VSVector3> VertexArray;

		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3( 0.0f,  0.0f, 0.0f));


		VSREAL RangeStep = 1.0f / uiLevel;
		for (unsigned int i = 0 ; i < uiLevel ; i++)
		{
			VSREAL Z = RangeStep * (i + 1);
			for (unsigned int j = 0 ; j < CircleLevel ; j++)
			{
				VSREAL Radio = ( j * 1.0f ) / CircleLevel * VS2PI;
				VSREAL X = Z * COS(Radio);
				VSREAL Y = Z * SIN(Radio);
				VertexArray.AddElement(VSVector3(X,Y,Z));
			}
		}
		VertexArray.AddElement(VSVector3(0.0f,0.0f, 1.0f));
		

		// first level
		for (unsigned int i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(0);

			unsigned int Index1 = i + 2;
			unsigned int Index2 = i + 1;
			if (Index1 = CircleLevel + 1)
			{
				Index1 = 1;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		//
		for (unsigned int i = 1 ; i < uiLevel ; i++)
		{
			for (unsigned int j = 0 ; j < CircleLevel ; j++)
			{
				unsigned int TopIndex1 = (i - 1) * CircleLevel + 1 + j;
				unsigned int TopIndex2 = (i - 1) * CircleLevel + 1 + j + 1;
				
				
				
				unsigned int BottomIndex1 = i * CircleLevel + 1 + j;
				unsigned int BottomIndex2 = i * CircleLevel + 1 + j + 1;

				if (j + 1 == CircleLevel)
				{
					TopIndex2 = (i - 1) * CircleLevel + 1 ;
					BottomIndex2 = i * CircleLevel + 1;
				}

				IndexArray.AddElement(TopIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(BottomIndex2);
			}
		}

		// last level
		for (unsigned int i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(VertexArray.GetNum() - 1);

			unsigned int Index1 = VertexArray.GetNum() - 3 - i;
			unsigned int Index2 = VertexArray.GetNum() - 2 - i;
			
			if (i == CircleLevel - 1)
			{
				Index1 = VertexArray.GetNum() - 2;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		

		VSDataBufferPtr  pVertexDate = VS_NEW VSDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		VSTriangleSetPtr pTriangleSetDate = VS_NEW VSTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCone->SetMeshDate(pTriangleSetDate);
		ms_DefaultCone->m_GeometryName = _T("DefaultCone");
	}

	//Quad Cub
	{
		VSArray<VSVector3> VertexArray;
		VSArray<VSVector2> TexcoordArray;
		VSArray<VSVector3> NormalArray;
		VSArray<VSUSHORT_INDEX> IndexArray;

		//pos uv
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		TexcoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, -1.0f));

		TexcoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		TexcoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, -1.0f));

		TexcoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, 1.0f));

		TexcoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, -1.0f));

		TexcoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);

		//left
		IndexArray.AddElement(8);
		IndexArray.AddElement(9);
		IndexArray.AddElement(10);
		IndexArray.AddElement(8);
		IndexArray.AddElement(10);
		IndexArray.AddElement(11);

		//right
		IndexArray.AddElement(12);
		IndexArray.AddElement(13);
		IndexArray.AddElement(14);
		IndexArray.AddElement(12);
		IndexArray.AddElement(14);
		IndexArray.AddElement(15);

		//up
		IndexArray.AddElement(16);
		IndexArray.AddElement(17);
		IndexArray.AddElement(18);
		IndexArray.AddElement(16);
		IndexArray.AddElement(18);
		IndexArray.AddElement(19);

		//bottom
		IndexArray.AddElement(20);
		IndexArray.AddElement(21);
		IndexArray.AddElement(22);
		IndexArray.AddElement(20);
		IndexArray.AddElement(22);
		IndexArray.AddElement(23);

		VSDataBufferPtr  pVertexDate = VS_NEW VSDataBuffer;
		pVertexDate->SetDate(&VertexArray[0], (unsigned int)VertexArray.GetNum(), VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pTexcoord = VS_NEW VSDataBuffer;
		pTexcoord->SetDate(&TexcoordArray[0], TexcoordArray.GetNum(), VSDataBuffer::DT_FLOAT32_2);

		VSDataBufferPtr  pNormalDate = VS_NEW VSDataBuffer;
		pNormalDate->SetDate(&NormalArray[0], (unsigned int)NormalArray.GetNum(), VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetDate(&IndexArray[0], (unsigned int)IndexArray.GetNum(), VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate, VSVertexFormat::VF_POSITION);
		pVertexBuffer->SetDate(pTexcoord, VSVertexFormat::VF_TEXCOORD);
		pVertexBuffer->SetDate(pNormalDate, VSVertexFormat::VF_NORMAL);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		VSTriangleSetPtr pTriangleSetDate = VS_NEW VSTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultRenderCube->SetMeshDate(pTriangleSetDate);
		ms_DefaultRenderCube->m_GeometryName = _T("DefaultRenderCube");
	}
}
unsigned int VSGeometry::UpdateGeometry()
{
	//更新顶点坐标，更新法向量
	if (m_pMeshDate && VSEngineFlag::EnableCLODMesh)
	{
		m_pMeshDate->SetLodDesirePercent(m_fCLodPercent);
		m_pMeshDate->UpDateClodMesh();
	}
	return UGRI_END;
	
}
void VSGeometry::CreateMorphMeshData()
{
	m_pMorphMeshDate = NULL;
	m_pMorphMeshDate = (VSMeshDate *)VSObject::CloneCreateObject(m_pMeshDate);
	m_pMorphMeshDate->GetVertexBuffer()->SetStatic(false);
}
void VSGeometry::AddMorphAABB(VSVertexBuffer * pMorphVertexBuffer)
{
	if (!pMorphVertexBuffer)
	{
		return;
	}
	LinkBoneNode();
	if (m_pMeshDate && m_pMeshDate->GetVertexBuffer())
	{
		VSAABB3 NewAABB;

		VSVertexBuffer * pVerBuffer = m_pMeshDate->GetVertexBuffer();
		if (!pMorphVertexBuffer->GetPositionDate(0))
		{
			return;
		}

		VSVector3 * pVer = (VSVector3*)pMorphVertexBuffer->GetPositionDate(0)->GetDate();
		if (!pVer)
		{
			return;
		}
		unsigned int uiVextexNum = pVerBuffer->GetPositionDate(0)->GetNum();
		VSTransform World = m_pParent->GetWorldTransform();
		if (GetAffectSkelecton())
		{
			VSDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesDate();
			VSDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightDate();

			if (!pBlendIndex || !pBoneWeight)
			{
				return;
			}
			VSArray<VSVector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVextexNum);

			if (pBlendIndex->GetDT() == VSDataBuffer::DT_UBYTE4)
			{
				DWORD * pBlendIndexData = (DWORD *)pBlendIndex->GetDate();
				DWORD * pBoneWeightData = (DWORD *)pBoneWeight->GetDate();
				for (unsigned int i = 0; i < uiVextexNum; i++)
				{
					VSVector3W BoneWeight;
					BoneWeight.CreateFormABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					VSDWCOLORGetABGR(pBlendIndexData[i], BlendIndex[0], BlendIndex[1], BlendIndex[2], BlendIndex[3]);
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (unsigned int k = 0; k < 4; k++)
					{

						VSBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if (pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}

				}
			}
			else
			{
				VSVector3W * pBlendIndexData = (VSVector3W *)pBlendIndex->GetDate();
				VSVector3W * pBoneWeightData = (VSVector3W *)pBoneWeight->GetDate();
				for (unsigned int i = 0; i < uiVextexNum; i++)
				{
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (unsigned int k = 0; k < 4; k++)
					{
						unsigned int BlendIndex = (unsigned int)pBlendIndexData[i].m[k];
						VSBoneNode * pBone = GetAffectBone(BlendIndex);
						if (pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}

			NewAABB.CreateAABB(TempBuffer.GetBuffer(), uiVextexNum);
		}
		else
		{
			NewAABB.CreateAABB(pVer, uiVextexNum);
		}
		m_LocalBV = m_LocalBV.MergAABB(NewAABB);

	}
}
void VSGeometry::CreateLocalAABB()
{
	LinkBoneNode();
	if (m_pMeshDate && m_pMeshDate->GetVertexBuffer())
	{
		VSAABB3 NewAABB;

		VSVertexBuffer * pVerBuffer = m_pMeshDate->GetVertexBuffer();
		if (!pVerBuffer->GetPositionDate(0))
		{
			return;
		}

		VSVector3 * pVer = (VSVector3*)pVerBuffer->GetPositionDate(0)->GetDate();
		if (!pVer)
		{
			return;
		}
		unsigned int uiVextexNum = pVerBuffer->GetPositionDate(0)->GetNum();
		VSTransform World  = m_pParent->GetWorldTransform();
		if (GetAffectSkelecton())
		{
			VSDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesDate();
			VSDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightDate();

			if (!pBlendIndex || !pBoneWeight)
			{
				return ;
			}
			VSArray<VSVector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVextexNum);

			if (pBlendIndex->GetDT() == VSDataBuffer::DT_UBYTE4)
			{
				DWORD * pBlendIndexData = (DWORD *)pBlendIndex->GetDate();
				DWORD * pBoneWeightData = (DWORD *)pBoneWeight->GetDate();	
				for (unsigned int i = 0; i < uiVextexNum ;i++)
				{
					VSVector3W BoneWeight;
					BoneWeight.CreateFormABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					VSDWCOLORGetABGR(pBlendIndexData[i],BlendIndex[0],BlendIndex[1],BlendIndex[2],BlendIndex[3]);
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (unsigned int k = 0 ; k < 4 ; k++)
					{

						VSBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if(pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}
					
				}
			}
			else
			{
				VSVector3W * pBlendIndexData = (VSVector3W *)pBlendIndex->GetDate();
				VSVector3W * pBoneWeightData = (VSVector3W *)pBoneWeight->GetDate();	
				for (unsigned int i = 0; i < uiVextexNum ;i++)
				{
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (unsigned int k = 0 ; k < 4 ; k++)
					{
						unsigned int BlendIndex = (unsigned int) pBlendIndexData[i].m[k];
						VSBoneNode * pBone = GetAffectBone(BlendIndex);
						if(pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}
			
			NewAABB.CreateAABB(TempBuffer.GetBuffer(),uiVextexNum);
		}
		else
		{		
			NewAABB.CreateAABB(pVer,uiVextexNum);
		}
		m_LocalBV = NewAABB;
		
	}
	
}
void VSGeometry::UpdateWorldBound(double dAppTime)
{
	m_WorldBV.Transform(m_LocalBV,m_World.GetCombine());
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;;
	}
	
}
void VSGeometry::UpdateNodeAll(double dAppTime)
{
	
	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}

	UpdateTransform(dAppTime);
	if (dAppTime > 0.0f)
	{
		UpdateOther(dAppTime);
	}
	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;

	
}
void VSGeometry::UpDateView(VSCuller & Culler, double dAppTime)
{
	VSSpatial::UpDateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		if (m_pMeshDate->IsClodMesh())
		{
			VSREAL ZFar = pCamera->GetZFar();
			VSVector3 DistVector = pCamera->GetWorldTranslate() - GetWorldTranslate();
			VSREAL Dist = DistVector.GetLength();

			VSREAL f = Dist / ZFar;
			f = 1.0f - Clamp(f, 1.0f, 0.0f);
			m_fCLodPercent = Clamp(f, 1.0f, 0.3f);
		}
		
	}
}
void VSGeometry::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	if (Culler.CullGeometry(this))
	{
		return;
	}
	UpDateView(Culler,dAppTime);
	VSMeshNode * pMeshNode = GetMeshNode();
	if (!pMeshNode)
	{
		return;
	}
	unsigned int uiRenderGroup = pMeshNode->GetRenderGroup();
	if (uiRenderGroup >= VSCuller::RG_MAX)
	{
		return ;
	}
	VSMaterialInstance * pMaterialInstance = NULL;
	if (Culler.GetUseMaterialIndex() == -1 )
	{
		pMaterialInstance = GetUseMaterialInstance();
		if (!pMaterialInstance)
		{
			AddMaterialInstance(VSResourceManager::ms_DefaultMaterialResource);
			pMaterialInstance = GetUseMaterialInstance();
		}
	}
	else
	{
		pMaterialInstance = GetMaterialInstance(Culler.GetUseMaterialIndex());
	}
	if (!pMaterialInstance)
	{
		return;
	}

	VSMaterial * pMaterial = pMaterialInstance->GetMaterial();
	for (unsigned int i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
	{
		
		VSRenderContext VisibleContext;
		VisibleContext.m_pGeometry = this;
		VisibleContext.m_pMaterialInstance = pMaterialInstance;
		VisibleContext.m_uiPassId = i;
		VisibleContext.m_pMaterial = pMaterial;
		VisibleContext.m_pMeshNode = pMeshNode;
		const VSBlendDesc & BlendDest = pMaterial->GetRenderState(i).GetBlendState()->GetBlendDesc();
		if (Culler.GetCullerType() == VSCuller::CUT_SHADOW)
		{

			if (BlendDest.IsBlendUsed())
			{
				return;
			}
			else
			{
				Culler.InsertObject(VisibleContext, VSCuller::VST_BASE, uiRenderGroup);
				return;
			}
		}
		else
		{
			if (pMaterial->GetCombine())
			{
				Culler.InsertObject(VisibleContext, VSCuller::VST_COMBINE, uiRenderGroup);
			}
			else
			{
				if (BlendDest.IsBlendUsed())
				{
					Culler.InsertObject(VisibleContext, VSCuller::VST_ALPHABLEND, uiRenderGroup);
				}
				else
					Culler.InsertObject(VisibleContext, VSCuller::VST_BASE, uiRenderGroup);
			}
		}	
	}
	
}
bool VSGeometry::IsSwapCull()
{
	VSMeshNode * pMeshNode = GetMeshNode();
	if (pMeshNode)
	{
		VSREAL fDet = pMeshNode->GetWorldTransform().GetRotate().Det();
		return (fDet < 0.0f);
	}
	else
	{
		return false;
	}
	
}
VSGeometry::VSGeometry()
{
	m_pMorphMeshDate = NULL;
	m_pMeshDate = NULL;
	m_uiActiveNum = VSMAX_INTEGER;
	m_uiCurUseMaterial = 0;
	m_fCLodPercent = 1.0f;
}
VSGeometry::~VSGeometry()
{
	m_pMeshDate = NULL;
	m_pMorphMeshDate = NULL;
}
void VSGeometry::SetAffectBoneArray(const VSArray<VSBoneNode *> & pBoneNodeArray)
{
	if(pBoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_pBoneNode = pBoneNodeArray;
		for (unsigned int i = 0 ;i  < m_pBoneNode.GetNum(); i++)
		{
			m_BoneName.AddElement(m_pBoneNode[i]->m_cName);
		}
	}

}
void VSGeometry::SetAffectBoneArray(const VSArray<VSUsedName> & BoneNodeArray)
{
	if(BoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_BoneName = BoneNodeArray;
		LinkBoneNode();
	}
}
void VSGeometry::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	return;
}
bool VSGeometry::PostLoad( void * pDate)
{
	VSSpatial::PostLoad(pDate);
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance(VSResourceManager::ms_DefaultMaterialResource);
	}
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		m_pMaterialInstance[i]->GetMaterialR()->AddLoadEventObject(m_pMaterialInstance[i]);
	}
	return 1;
}
bool VSGeometry::PostClone(VSObject * pObjectSrc)
{
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance(VSResourceManager::ms_DefaultMaterialResource);
	}
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		m_pMaterialInstance[i]->GetMaterialR()->AddLoadEventObject(m_pMaterialInstance[i]);
	}
	return true;
}
void VSGeometry::LinkBoneNode()
{
	m_pBoneNode.Clear();
	if (m_BoneName.GetNum())
	{
		VSSkelecton * pSke = GetAffectSkelecton();
		if (pSke)
		{
			m_pBoneNode.Clear();
			for (unsigned int i = 0 ; i < m_BoneName.GetNum() ; i++)
			{
				VSBoneNode * pBoneNode = pSke->GetBoneNode(m_BoneName[i]);
				if (!pBoneNode)
				{
					return ;
				}
				m_pBoneNode.AddElement(pBoneNode);
			}
		}
	}
	m_SkinWeightBuffer.SetBufferNum(GetAffectBoneNum() * 3);
}
VSMeshNode * VSGeometry::GetMeshNode()const
{
	VSMeshNode * pMeshNode = NULL;
	VSSpatial* pParent = m_pParent;
	while(!pMeshNode && pParent)
	{
		pMeshNode = DynamicCast<VSMeshNode>(pParent);
		if (!pMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	return pMeshNode;

}
VSSkelecton * VSGeometry::GetAffectSkelecton()const
{
	VSSkelectonMeshNode * pSkeMeshNode = NULL;
	VSSpatial* pParent = m_pParent;
	while(!pSkeMeshNode && pParent)
	{
		pSkeMeshNode = DynamicCast<VSSkelectonMeshNode>(pParent);
		if (!pSkeMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	if (pSkeMeshNode)
	{
		return pSkeMeshNode->GetSkelecton();
	}
	else
	{
		return NULL;
	}
}
void VSGeometry::CreateClodMesh()
{
	
	if (m_pMeshDate && !m_pMeshDate->IsClodMesh())
	{
		if(m_pMeshDate->CreateCollapseRecord())
		{
			m_bIsStatic = false;
		}
	}

	
}
void VSGeometry::RemoveClodMesh()
{
	if (m_pMeshDate)
	{
		m_pMeshDate->RemoveClodMesh();
	}
}
unsigned int VSGeometry::GetVertexNum()const
{
	if (m_pMeshDate)
	{
		if(m_pMeshDate->GetVertexBuffer())
		{
			return m_pMeshDate->GetVertexBuffer()->GetVertexNum();
		}
	}
	return 0;
}
void VSGeometry::SetMeshDate(VSMeshDate * pMeshDate)
{
	m_pMeshDate = pMeshDate;
}
VSMeshDate * VSGeometry::GetMeshDate()const
{
	if (m_pMorphMeshDate == NULL)
	{
		return m_pMeshDate;
	}
	else
	{
		return m_pMorphMeshDate;
	}
}
VSMeshDate * VSGeometry::GetOriginMeshDate()const
{
	return m_pMeshDate;
}
unsigned int VSGeometry::GetActiveNum()
{
	if (m_pMeshDate)
	{

		if (m_pMeshDate->GetIndexBuffer() && m_pMeshDate->IsClodMesh())
		{
			m_uiActiveNum = m_pMeshDate->GetActiveNum();
		}
		else
		{
			if (m_pMeshDate->GetIndexBuffer() && (m_uiActiveNum == VSMAX_INTEGER))
				m_uiActiveNum = m_pMeshDate->GetActiveNum();
			else if (m_uiActiveNum == VSMAX_INTEGER)
				return 0;


		}
		return m_uiActiveNum;

	}
	return 0;
}
bool VSGeometry::SetActiveNum(unsigned int uiActiveNum)
{
	if (uiActiveNum > GetTotleNum())
		return 0;
	m_uiActiveNum = uiActiveNum;
	return 1;

}
unsigned int VSGeometry::GetTotleNum()const
{
	if (m_pMeshDate)
	{
		return m_pMeshDate->GetTotleNum();
	}
	return 0;

}
unsigned int VSGeometry::GetMeshDateType()
{
	if (m_pMeshDate)
	{
		return m_pMeshDate->GetMeshDateType();
	}
	return  VSMeshDate::MDT_MAX;
}
unsigned int VSGeometry::AddMaterialInstance(VSMaterialR * pMaterial)
{
	if (!pMaterial)
		return 0;
	VSMaterialInstance * pMaterialInstance = VS_NEW VSMaterialInstance(pMaterial);
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
unsigned int VSGeometry::AddMaterialInstance(VSMaterialInstance * pMaterial)
{
	if (!pMaterial || !pMaterial->GetMaterial())
		return 0;
	VSMaterialInstance * pMaterialInstance = VS_NEW VSMaterialInstance(pMaterial->GetMaterialR());
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
bool VSGeometry::SetMaterialInstance(VSMaterialR * pMaterial, unsigned int uiIndex)
{
	if (uiIndex >= m_pMaterialInstance.GetNum() || !pMaterial)
	{
		return false;
	}

	VSMaterialInstance * pMaterialInstance = VS_NEW VSMaterialInstance(pMaterial);
	m_pMaterialInstance[uiIndex] = pMaterialInstance;
	return true;

}
bool VSGeometry::SetMaterialInstance(VSMaterialInstance * pMaterial, unsigned int uiIndex)
{
	if (uiIndex >= m_pMaterialInstance.GetNum() || !pMaterial)
	{
		return false;
	}

	m_pMaterialInstance[uiIndex] = (VSMaterialInstance *)VSObject::CloneCreateObject(pMaterial);

	return true;
}
bool VSGeometry::SetUseMaterialInstance(unsigned int uiIndex)
{
	if (uiIndex >= m_pMaterialInstance.GetNum())
	{
		return false;
	}
	m_uiCurUseMaterial = uiIndex;
	return true;
}
void VSGeometry::ClearAllMaterialInstance()
{
	m_pMaterialInstance.Clear();
}
void VSGeometry::DeleteMaterialInstance(unsigned int i)
{
	if (i >= m_pMaterialInstance.GetNum())
		return;
	m_pMaterialInstance[i] = NULL;
}
unsigned int VSGeometry::DeleteMaterialInstance(VSMaterialInstance * pMaterial)
{
	if (!pMaterial)
		return VSMAX_INTEGER;
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		if (pMaterial == m_pMaterialInstance[i])
		{
			m_pMaterialInstance[i] = NULL;
			return i;
		}
	}
	return VSMAX_INTEGER;
}
VSMaterialInstance * VSGeometry::GetUseMaterialInstance()const
{
	VSMaterialInstance * pMaterialInstance = NULL;
	if (m_uiCurUseMaterial < m_pMaterialInstance.GetNum())
	{
		pMaterialInstance = m_pMaterialInstance[m_uiCurUseMaterial];
	}
	return pMaterialInstance;
}

VSMaterialInstance * VSGeometry::GetMaterialInstance(unsigned int i)const
{
	if (i >= m_pMaterialInstance.GetNum())
		return NULL;
	return m_pMaterialInstance[i];
}
void VSGeometry::UpdateOther(double dAppTime)
{
	if (!m_pMeshDate)
	{
		return;
	}
	VSVertexBuffer * pVBuffer = GetMeshDate()->GetVertexBuffer();
	if(GetAffectBoneNum() && pVBuffer->GetBlendWeightDate() && pVBuffer->GetBlendIndicesDate())
	{
		VSTransform World  = m_pParent->GetWorldTransform();
		
		for (unsigned int i = 0 ; i < GetAffectBoneNum() ; i++)
		{
			VSBoneNode * pBone = GetAffectBone(i);
			if(pBone)
			{
				VSTransform BoneWorld = pBone->GetWorldTransform();
				VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
				VSVector3W ColumnVector[4];
				//TempBone.Identity();
				TempBone.GetColumnVector(ColumnVector);
				m_SkinWeightBuffer[i * 3] = ColumnVector[0];
				m_SkinWeightBuffer[i * 3 + 1] = ColumnVector[1];
				m_SkinWeightBuffer[i * 3 + 2] = ColumnVector[2];
			}
			else
			{
				m_SkinWeightBuffer[i * 3].Set(1.0f,0.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 1].Set(0.0f,1.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 2].Set(0.0f,0.0f,1.0f,0.0f);
			}

		}

	}
}
IMPLEMENT_RTTI(VSShadowVolumeGeometry,VSGeometry)
BEGIN_ADD_PROPERTY(VSShadowVolumeGeometry,VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShadowVolumeGeometry)
IMPLEMENT_INITIAL_END
VSShadowVolumeGeometry::VSShadowVolumeGeometry()
{

}
VSShadowVolumeGeometry::~VSShadowVolumeGeometry()
{

}