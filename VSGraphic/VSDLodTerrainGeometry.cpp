#include "VSDLodTerrainGeometry.h"
#include "VSDLodTerrainNode.h"
#include "VSTriangleSet.h"
#include "VSVertexBuffer.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDLodTerrainGeometry,VSGeometry)
BEGIN_ADD_PROPERTY(VSDLodTerrainGeometry,VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainGeometry)
IMPLEMENT_INITIAL_END
VSDLodTerrainGeometry::VSDLodTerrainGeometry()
{

}
VSDLodTerrainGeometry::~VSDLodTerrainGeometry()
{

}
VSDLodTerrainNode * VSDLodTerrainGeometry::GetParentTerrainNode()const
{
	if (m_pParent)
	{
		return (VSDLodTerrainNode *)m_pParent->GetParent();
	}
	return NULL;
}
bool VSDLodTerrainGeometry::CreateMesh(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
									   unsigned int uiLevel,unsigned int uiTesselationLevel)
{
	VSDLodTerrainNode * pTerrainNode = GetParentTerrainNode();
	if (uiLevel >= uiTesselationLevel || !pTerrainNode)
	{
		return 0;
	}
	unsigned int uiVertexLenght = (1 << ( uiTesselationLevel - uiLevel) ) + 1;

	VSDataBuffer * pVertexDate = NULL ;
	pVertexDate = VS_NEW VSDataBuffer();
	if (!pVertexDate->CreateEmptyBuffer(uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 4,VSDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}


	VSVector3 * pVer = (VSVector3 *)pVertexDate->GetDate();
	VSVector3 * pVerTemp = pVer;
	unsigned int uiBiasX = uiIndexXInTerrain * (1 << uiTesselationLevel);
	unsigned int uiBiasZ = uiIndexZInTerrain * (1 << uiTesselationLevel);
	for (unsigned int i = 0 ; i < uiVertexLenght ; i++)
	{
		for(unsigned int j = 0 ; j < uiVertexLenght ; j++)
		{
			unsigned int uiIndex = i * uiVertexLenght + j;
			pVer[uiIndex].x = VSTerrainNode::WIDTH_SCALE * (uiBiasX + (i << uiLevel)) * 1.0f;
			pVer[uiIndex].y = pTerrainNode->GetHeight(uiBiasX + (i << uiLevel),uiBiasZ + (j << uiLevel));
			pVer[uiIndex].z = VSTerrainNode::WIDTH_SCALE * (uiBiasZ + (j << uiLevel)) * 1.0f;
			pVerTemp++;
		}
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * i + uiVertexLenght - 1;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(int i = uiVertexLenght - 1 ; i > 0 ; i--)
	{
		unsigned int uiIndex = uiVertexLenght * (uiVertexLenght - 1 )+ i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(int i = uiVertexLenght - 1 ; i > 0 ; i--)
	{
		unsigned int uiIndex = uiVertexLenght * i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}



	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(true);
	if (!pVertexBuffer)
	{
		return 0;
	}
	pVertexBuffer->SetDate(pVertexDate,VSVertexFormat::VF_POSITION);

	VSDataBuffer* pIndexDate = NULL;
	pIndexDate = VS_NEW VSDataBuffer();
	if (!pIndexDate)
	{
		return 0;
	}
	if (!pIndexDate->CreateEmptyBuffer(
		(uiVertexLenght - 1) * (uiVertexLenght - 1) * 2 * 3 + (uiVertexLenght - 1) * 4 * 2 * 3,VSDataBuffer::DT_USHORT))
	{
		return 0;
	}
	
	VSUSHORT_INDEX * pIndex = (VSUSHORT_INDEX *)pIndexDate->GetDate();

	for (unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		for (unsigned int j = 0 ; j < uiVertexLenght - 1; j++)
		{
			*pIndex = i * uiVertexLenght + j;
			pIndex++;

			*pIndex = i * uiVertexLenght + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j + 1;
			pIndex++;





			*pIndex = i * uiVertexLenght + j;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j;
			pIndex++;






		}
	}



	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = i;
		unsigned int uiIndex1= i + 1;
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 0 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 0 + i + 1;
		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * i + uiVertexLenght - 1;
		unsigned int uiIndex1 = uiVertexLenght * (i + 1) + uiVertexLenght - 1;
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 1 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 1 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * (uiVertexLenght - 1 ) + uiVertexLenght - 1 - i;
		unsigned int uiIndex1 = uiVertexLenght * (uiVertexLenght - 1 )+ uiVertexLenght - 1 - i - 1;
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 2 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 2 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * (uiVertexLenght - 1 - i);
		unsigned int uiIndex1 = uiVertexLenght * (uiVertexLenght - 1 - i - 1);
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 3 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 3 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		if (i == uiVertexLenght - 2)
		{
			*pIndex = uiVertexLenght * uiVertexLenght;
			pIndex++;
		}
		else
		{
			*pIndex = uiIndex3;
			pIndex++;
		}
	}

	
	VSIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = VS_NEW VSIndexBuffer();
	if (!pIndexBuffer)
	{
		return 0;
	}
// 	pIndexBuffer->SetStatic(false);
// 	pIndexBuffer->SetLockFlag(VSBind::LF_DISCARD);
	pIndexBuffer->SetDate(pIndexDate);

	VSTriangleSet * pMeshDate = NULL;
	pMeshDate = VS_NEW VSTriangleSet();

	if (!pMeshDate)
	{
		return 0;
	}
	pMeshDate->SetVertexBuffer(pVertexBuffer);
	pMeshDate->SetIndexBuffer(pIndexBuffer);
	SetMeshDate(pMeshDate);
	return 1;
}