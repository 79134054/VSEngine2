#include "VSCLodTerrainGeometry.h"
#include "VSTerrainNode.h"
#include "VSTriangleSet.h"
#include "VSVertexBuffer.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSCLodTerrainGeometry,VSGeometry)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSCLodTerrainGeometry)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSCLodTerrainGeometry,VSGeometry)
REGISTER_PROPERTY(m_pNeighbor[NT_LEFT],NeighborLeft,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_RIGHT],NeighborRight,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_TOP],NeighborTFop,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_BOTTOM],NeighborBottom,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiIndexXInTerrain,IndexXInTerrain,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiIndexZInTerrain,IndexZInTerrain,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSCLodTerrainGeometry::VSCLodTerrainGeometry()
{
	for (unsigned int i = 0 ; i < NT_MAX ; i++)
	{
		m_pNeighbor[i] = NULL;
	}
	m_uiIndexXInTerrain = 0;
	m_uiIndexZInTerrain = 0;
	m_bIsStatic = false;
	
}
VSCLodTerrainGeometry::~VSCLodTerrainGeometry()
{

}
bool VSCLodTerrainGeometry::CreateMeshDate(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
									   unsigned int uiTesselationLevel)
{

	if (uiTesselationLevel < 0)
	{
		return 0 ;
	}
	VSTerrainNode * pTerrainNode = DynamicCast<VSTerrainNode>(m_pParent);
	if (!pTerrainNode)
	{
		return 0 ;
	}

	VSDataBuffer * pVertexDate = NULL ;
	pVertexDate = VS_NEW VSDataBuffer();
	unsigned int uiVertexLenght= (1 << uiTesselationLevel) + 1;
	if (!pVertexDate->CreateEmptyBuffer(uiVertexLenght * uiVertexLenght,VSDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}
	

	VSVector3 * pVer = (VSVector3 *)pVertexDate->GetDate();
	unsigned int uiBiasX = uiIndexXInTerrain * (1 << uiTesselationLevel);
	unsigned int uiBiasZ = uiIndexZInTerrain * (1 << uiTesselationLevel);
	for (unsigned int i = 0 ; i < uiVertexLenght ; i++)
	{
		for(unsigned int j = 0 ; j < uiVertexLenght ; j++)
		{
			unsigned int uiIndex = i * uiVertexLenght + j;
			pVer[uiIndex].x = 1.0f * (uiBiasX + i) * VSTerrainNode::WIDTH_SCALE;
			pVer[uiIndex].y = pTerrainNode->GetHeight(uiBiasX + i,uiBiasZ + j);
			pVer[uiIndex].z = 1.0f * (uiBiasZ + j) * VSTerrainNode::WIDTH_SCALE;
		}
	}
	
	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(true);
	if (!pVertexBuffer)
	{
		return 0;
	}
	pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);


 	unsigned int uiCurRenderTriNum = (uiVertexLenght - 1) * (uiVertexLenght - 1) * 2 * 3;

	VSIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = VS_NEW VSIndexBuffer(uiCurRenderTriNum);
	if (!pIndexBuffer)
	{
		return 0;
	}
	//test mul thread render
	if (GetTerrainGeometryType() == TGT_ROAM)
	{
		pIndexBuffer->SetStatic(false,true);
	}
	else
	{
		pIndexBuffer->SetStatic(false);
	}
	
	pIndexBuffer->SetLockFlag(VSBind::LF_DISCARD);

	VSTriangleSet * pMeshDate = NULL;
	pMeshDate = VS_NEW VSTriangleSet();

	if (!pMeshDate)
	{
		return 0;
	}

	pMeshDate->SetVertexBuffer(pVertexBuffer);
	pMeshDate->SetIndexBuffer(pIndexBuffer);
	SetMeshDate(pMeshDate);
	m_uiIndexXInTerrain = uiIndexXInTerrain;
	m_uiIndexZInTerrain = uiIndexZInTerrain;
	return 1;
}
bool VSCLodTerrainGeometry::AddNeighbor(VSCLodTerrainGeometry * pTerrainGemetry,unsigned int uiNeighbor)
{
	if ( !pTerrainGemetry || uiNeighbor >= NT_MAX)
	{
		return 0;
	}
	VSTerrainNode * pTerrainNode = DynamicCast<VSTerrainNode>(m_pParent);
	if (!pTerrainNode)
	{
		return 0 ;
	}
	
	pTerrainNode->DeleteChild(m_pNeighbor[uiNeighbor]);
	m_pNeighbor[uiNeighbor] = pTerrainGemetry;

	return 1;
}
void VSCLodTerrainGeometry::UpDateView(VSCuller & Culler,double dAppTime)
{
	VSGeometry::UpDateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		VSTerrainNode * pTerrainNode = DynamicCast<VSTerrainNode>(m_pParent);
		if (!pTerrainNode)
		{
			return ;
		}
		VSTransform Tran = pTerrainNode->GetWorldTransform();
		VSVector3 Loc = pCamera->GetWorldTranslate() * Tran.GetCombineInverse();


		Tessellate(Loc);
	}
}

