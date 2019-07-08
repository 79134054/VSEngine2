#include "VSQuadTerrainGeometry.h"
#include "VSCLodTerrainNode.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSQuadTerrainGeometry,VSCLodTerrainGeometry)
BEGIN_ADD_PROPERTY(VSQuadTerrainGeometry,VSCLodTerrainGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSQuadTerrainGeometry)
IMPLEMENT_INITIAL_END
VSQuadTerrainGeometry::VSQuadTerrainGeometry()
{
	m_uiCurLevel = 0;
}
VSQuadTerrainGeometry::~VSQuadTerrainGeometry()
{

}
void VSQuadTerrainGeometry::ComputeVariance(unsigned int uiCurLevel)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	unsigned int uiVarianceLength = (1 << pTerrainNode->GetTesselationLevel()) + 1;
	if (m_Variance.GetNum() != uiVarianceLength * uiVarianceLength)
	{
		m_Variance.SetBufferNum(uiVarianceLength * uiVarianceLength);
		VSMemset(&m_Variance[0],0,m_Variance.GetNum());
	}
	
	RecursiveComputeVariance(uiCurLevel);
}
bool VSQuadTerrainGeometry::RecursiveComputeVariance(unsigned int uiLevel)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	if (uiLevel >= pTerrainNode->GetTesselationLevel())
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	VSVector3 * pVer = (VSVector3 *)m_pMeshDate->GetVertexBuffer()->GetPositionDate(0)->GetDate();
	if (!pVer)
	{
		return 0;
	}
	VSREAL fVariance = 0;
	unsigned int uiBias = pTerrainNode->GetTesselationLevel() - uiLevel;
	unsigned int uiVarianceLength = (1 << pTerrainNode->GetTesselationLevel()) + 1;
	//遍历刚层次的节点 1 << uiLevel 个
	for (unsigned int i = 0 ; i < (unsigned int)(1 << uiLevel) ; i++)
	{
		for(unsigned int j = 0 ; j < (unsigned int)(1 << uiLevel) ; j++)
		{
			//i方向上的(i << uiBias)，j方向上的(j << uiBias)，宽度(1 << pTerrainNode->GetTesselationLevel() + 1)
			unsigned int A = (j << uiBias) + (i << uiBias) * uiVarianceLength;
			unsigned int B = ((j + 1) << uiBias) + (i << uiBias) * uiVarianceLength;
			unsigned int C = ((j + 1) << uiBias) + ((i + 1) << uiBias) * uiVarianceLength;
			unsigned int D = (j << uiBias) + ((i + 1) << uiBias) * uiVarianceLength;

			if (uiLevel == pTerrainNode->GetTesselationLevel() - 1)
			{
				VSREAL fVarianceI[6] = {0};
				fVarianceI[0] = ABS(pVer[(A + B) >> 1].y - (pVer[A].y + pVer[B].y) * 0.5f);
				fVarianceI[1] = ABS(pVer[(B + C) >> 1].y - (pVer[B].y + pVer[C].y) * 0.5f);
				fVarianceI[2] = ABS(pVer[(C + D) >> 1].y - (pVer[C].y + pVer[D].y) * 0.5f);
				fVarianceI[3] = ABS(pVer[(D + A) >> 1].y - (pVer[D].y + pVer[A].y) * 0.5f);
				fVarianceI[4] = ABS(pVer[(A + C) >> 1].y - (pVer[A].y + pVer[C].y) * 0.5f);
				fVarianceI[5] = ABS(pVer[(B + D) >> 1].y - (pVer[B].y + pVer[D].y) * 0.5f);

				for( unsigned int i = 0 ; i < 6 ; i++)
				{
					if(fVariance < fVarianceI[i])
						fVariance = fVarianceI[i];
				}
				m_Variance[(A + C) >> 1] = fVariance;
			}
			else
			{
				VSREAL fVarianceI[14] = {0};
				fVarianceI[0] = ABS(pVer[(A + B) >> 1].y - (pVer[A].y + pVer[B].y) * 0.5f);
				fVarianceI[1] = ABS(pVer[(B + C) >> 1].y - (pVer[B].y + pVer[C].y) * 0.5f);
				fVarianceI[2] = ABS(pVer[(C + D) >> 1].y - (pVer[C].y + pVer[D].y) * 0.5f);
				fVarianceI[3] = ABS(pVer[(D + A) >> 1].y - (pVer[D].y + pVer[A].y) * 0.5f);
				fVarianceI[4] = ABS(pVer[(A + C) >> 1].y - (pVer[A].y + pVer[C].y) * 0.5f);
				fVarianceI[5] = ABS(pVer[(B + D) >> 1].y - (pVer[B].y + pVer[D].y) * 0.5f);
				//CL Child Level 
				unsigned int uiCLBegin = 0;
				unsigned int uiCLEnd = ((1 << (uiLevel + 1)) - 1);
				unsigned int uiCLBias = uiBias - 1;
				//left two child
				if (m_pNeighbor[NT_LEFT])
				{
					unsigned int uiLNTopA = (uiCLEnd << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					unsigned int uiLNTopC = ((uiCLEnd + 1 ) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);

					unsigned int uiLNBottomA = (uiCLEnd << uiCLBias) * uiVarianceLength + ((uiCLEnd - 1) << uiCLBias);
					unsigned int uiLNBottomC = ((uiCLEnd + 1 )<< uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					fVarianceI[6] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_LEFT])->m_Variance[(uiLNTopA + uiLNTopC) >> 1];
					fVarianceI[7] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_LEFT])->m_Variance[(uiLNBottomA + uiLNBottomC) >> 1];
				}
				//top two child
				if (m_pNeighbor[NT_TOP])
				{
					unsigned int uiTNLeftA = (uiCLBegin << uiCLBias) * uiVarianceLength + (uiCLBegin << uiCLBias);
					unsigned int uiTNLeftC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + ((uiCLBegin + 1) << uiCLBias);

					unsigned int uiTNRightA = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + (uiCLBegin << uiCLBias);
					unsigned int uiTNRightC = ((uiCLBegin + 2) << uiCLBias) * uiVarianceLength + ((uiCLBegin + 1) << uiCLBias);
					fVarianceI[8] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_TOP])->m_Variance[(uiTNLeftA + uiTNLeftC) >> 1];
					fVarianceI[9] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_TOP])->m_Variance[(uiTNRightA + uiTNRightC) >> 1];
				}
				//right two child
				if (m_pNeighbor[NT_RIGHT])
				{
					unsigned int uiLNTopA = (uiCLBegin << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					unsigned int uiLNTopC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);

					unsigned int uiLNBottomA = (uiCLBegin << uiCLBias) * uiVarianceLength + ((uiCLEnd - 1) << uiCLBias);
					unsigned int uiLNBottomC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
 					fVarianceI[10] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_RIGHT])->m_Variance[(uiLNTopA + uiLNTopC) >> 1];
					fVarianceI[11] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_RIGHT])->m_Variance[(uiLNBottomA + uiLNBottomC) >> 1];
				}
				//bottom two child
				if (m_pNeighbor[NT_BOTTOM])
				{
					unsigned int uiTNLeftA = (uiCLBegin << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					unsigned int uiTNLeftC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);

					unsigned int uiTNRightA = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					unsigned int uiTNRightC = ((uiCLBegin + 2) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);
					fVarianceI[12] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_BOTTOM])->m_Variance[(uiTNLeftA + uiTNLeftC) >> 1];
					fVarianceI[13] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_BOTTOM])->m_Variance[(uiTNRightA + uiTNRightC) >> 1];
				}
				for( unsigned int i = 0 ; i < 14 ; i++)
				{
					if(fVariance < fVarianceI[i])
						fVariance = fVarianceI[i];
				}
				m_Variance[(A + C) >> 1] = fVariance * pTerrainNode->GetCLODScale() / (pTerrainNode->GetCLODScale() - 1);
			}
		}
	}
	
	return 1;
}
void VSQuadTerrainGeometry::LinkNeighbor()
{

}
void VSQuadTerrainGeometry::ClearInfo()
{
	m_uiCurLevel = 0;
}
void VSQuadTerrainGeometry::Tessellate(const VSVector3 & CameraPos)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	unsigned int uiLevel = 1 << pTerrainNode->GetTesselationLevel();

	unsigned int A = 0;
	unsigned int B = uiLevel;
	unsigned int C = uiLevel * (uiLevel + 1) + uiLevel;

	unsigned int D = uiLevel * (uiLevel + 1);

	RecursiveTessellate(CameraPos,0,A,B,C,D);
}
void VSQuadTerrainGeometry::RecursiveTessellate(const VSVector3 & CameraPos,unsigned int uiLevel,unsigned int A,unsigned int B,
						   unsigned int C,unsigned int D)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	if (uiLevel >= pTerrainNode->GetTesselationLevel())
	{
		return ;
	}
	VSVector3 * pVer = (VSVector3 *)m_pMeshDate->GetVertexBuffer()->GetPositionDate(0)->GetDate();
	if (!pVer)
	{
		return ;
	}
	if (m_uiCurLevel < uiLevel)
	{
		m_uiCurLevel = uiLevel;
	}


	unsigned int uiMiddle = (B + D) >> 1;
	VSVector3 Dist = CameraPos - pVer[uiMiddle];
	VSVector3 Edge = pVer[A] - pVer[B];
	VSREAL fRatio = Dist.GetSqrLength() / (Edge.GetSqrLength() * Max(m_Variance[uiMiddle],1.0f) * pTerrainNode->GetCLODScale());
	if (fRatio < 1.0f)
	{
		RecursiveTessellate(CameraPos,uiLevel + 1,A,(A + B) >> 1,(B + D) >> 1,(A + D) >> 1);
		RecursiveTessellate(CameraPos,uiLevel + 1,(A + B) >> 1,B,(B + C) >> 1,(B + D) >> 1);
		RecursiveTessellate(CameraPos,uiLevel + 1,(B + D) >> 1,(B + C) >> 1,C,(C + D) >> 1);
		RecursiveTessellate(CameraPos,uiLevel + 1,(A + D) >> 1,(B + D) >> 1,(C + D) >> 1,C);
	}
	
}

/*
_________
|\  |  /|
|6\5|4/3|
|__\|/__|
|  /|\ 2|
|7/0|1\ | 
|/__|__\|



*/
unsigned int VSQuadTerrainGeometry::UpdateGeometry()
{
	
	return UGRI_END;
}
void VSQuadTerrainGeometry::UpdateOther(double dAppTime)
{
	if (!m_pMeshDate)
	{
		return;
	}
	struct VSQuadTerrainUpdateGeometryPara
	{
		unsigned int uiTesselationLevel;
		VSQuadTerrainGeometry * pTG;
		unsigned int m_uiCurLevel;
		unsigned int m_NeighborLevel[VSCLodTerrainGeometry::NT_MAX];
		VSQuadTerrainGeometry * pNeightbor[VSCLodTerrainGeometry::NT_MAX];
	};
	VSQuadTerrainUpdateGeometryPara QuadTerrainUpdateGeometryPara;
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	QuadTerrainUpdateGeometryPara.uiTesselationLevel = pTerrainNode->GetTesselationLevel();
	QuadTerrainUpdateGeometryPara.pTG = this;
	QuadTerrainUpdateGeometryPara.m_uiCurLevel = m_uiCurLevel;
	for (unsigned int i = 0 ; i < VSCLodTerrainGeometry::NT_MAX ;i++)
	{
		QuadTerrainUpdateGeometryPara.pNeightbor[i] = ((VSQuadTerrainGeometry *)m_pNeighbor[i]);
		if (QuadTerrainUpdateGeometryPara.pNeightbor[i])
		{
			QuadTerrainUpdateGeometryPara.m_NeighborLevel[i] = ((VSQuadTerrainGeometry *)m_pNeighbor[i])->m_uiCurLevel;
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(VSQuadTerrainUpdateGeometryCommand,
		VSQuadTerrainUpdateGeometryPara,QuadTerrainUpdateGeometryPara,QuadTerrainUpdateGeometryPara,
	{
		int iLevelDelt = 0; 

		VSUSHORT_INDEX * pIndexData = (VSUSHORT_INDEX *)QuadTerrainUpdateGeometryPara.pTG->GetMeshDate()->GetIndexBuffer()->Lock();

		if (!pIndexData)
		{
			return UGRI_FAIL;
		}
		unsigned int uiCurRenderTriNum = 0;
		unsigned int uiLeftTopFanFlag = 0;
		unsigned int uiLeftBottomFanFlag = 0;
		unsigned int uiRightTopFanFlag = 0;
		unsigned int uiRightBottomFanFlag = 0;

		unsigned int uiBias = QuadTerrainUpdateGeometryPara.uiTesselationLevel - QuadTerrainUpdateGeometryPara.m_uiCurLevel;
		unsigned int uiVarianceLength = (1 << QuadTerrainUpdateGeometryPara.uiTesselationLevel) + 1;
		unsigned int uiLength = (1 << QuadTerrainUpdateGeometryPara.m_uiCurLevel);
		for (unsigned int i = 1 ; i < uiLength - 1; i++)
		{
			for(unsigned int j = 1 ; j < uiLength - 1; j++)
			{

				unsigned int A = (i << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + ((j + 1) << uiBias);

				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_LEFT])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_LEFT];
			if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
			{

				for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
				{
					unsigned int A = (j << uiBias);
					unsigned int B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
					unsigned int C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					unsigned int D = ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
				}
				uiLeftTopFanFlag |= 0x01;
				uiLeftBottomFanFlag |= 0x01;


			}
			else
			{

				for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
				{
					unsigned int A = (j << uiBias);
					unsigned int B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
					unsigned int C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					unsigned int D = ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
			{
				unsigned int A = (j << uiBias);
				unsigned int B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_RIGHT])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_RIGHT];
			if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
			{

				for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
				{
					unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
					unsigned int B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
					unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
				}
				uiRightTopFanFlag |= 0x01;
				uiRightBottomFanFlag |= 0x01;


			}
			else
			{

				for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
				{
					unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
					unsigned int B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
					unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
			{
				unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_TOP])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_TOP];
			if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
			{

				for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
				{
					unsigned int A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
					unsigned int D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
				}
				uiLeftTopFanFlag |= 0x02;
				uiRightTopFanFlag |= 0x02;


			}
			else
			{
				for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
				{
					unsigned int A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
					unsigned int D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
			{
				unsigned int A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_BOTTOM])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_BOTTOM];
			if (iLevelDelt >= 0 && iLevelDelt == 1) //写成 == 1 其实就可以
			{

				for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
				{
					unsigned int A = (i << uiBias) * uiVarianceLength;
					unsigned int B = ((i + 1) << uiBias) * uiVarianceLength;
					unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
					unsigned int D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
				}
				uiLeftBottomFanFlag |= 0x02;
				uiRightBottomFanFlag |= 0x02;					

			}
			else
			{
				for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
				{
					unsigned int A = (i << uiBias) * uiVarianceLength;
					unsigned int B = ((i + 1) << uiBias) * uiVarianceLength;
					unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
					unsigned int D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
			{
				unsigned int A = (i << uiBias) * uiVarianceLength;
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength;
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
		//处理4个角
		//left top
		{
			unsigned int A = ((uiLength - 1) << uiBias);
			unsigned int B = ((1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			unsigned int C = ((1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
			unsigned int D = (uiLength << uiBias);
			if (uiLeftTopFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP | AT_NLEFT,uiCurRenderTriNum);
			}
			else if (uiLeftTopFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
			}
			else if (uiLeftTopFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}

		//right top
		{
			unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			unsigned int B = (uiLength << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			unsigned int C = (uiLength << uiBias) * uiVarianceLength + (uiLength << uiBias);
			unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
			if (uiRightTopFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP | AT_NRIGHT,uiCurRenderTriNum);
			}
			else if (uiRightTopFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
			}
			else if (uiRightTopFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}

		//left bottom
		{
			unsigned int A = 0;
			unsigned int B = ((1) << uiBias) * uiVarianceLength;
			unsigned int C = ((1) << uiBias) * uiVarianceLength + ((1) << uiBias);
			unsigned int D = ((1) << uiBias);
			if (uiLeftBottomFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM | AT_NLEFT,uiCurRenderTriNum);
			}
			else if (uiLeftBottomFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
			}
			else if (uiLeftBottomFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}


		//rigth bottom
		{
			unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength;
			unsigned int B = (uiLength << uiBias) * uiVarianceLength;
			unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((1) << uiBias);
			unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
			if (uiRightBottomFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM | AT_NRIGHT,uiCurRenderTriNum);
			}
			else if (uiRightBottomFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
			}
			else if (uiRightBottomFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}

		QuadTerrainUpdateGeometryPara.pTG->GetMeshDate()->GetIndexBuffer()->UnLock();
		QuadTerrainUpdateGeometryPara.pTG->SetActiveNum(uiCurRenderTriNum);
	})
		int iLevelDelt = 0; 

	VSUSHORT_INDEX * pIndexData = (VSUSHORT_INDEX *)QuadTerrainUpdateGeometryPara.pTG->GetMeshDate()->GetIndexBuffer()->Lock();

	if (!pIndexData)
	{
		return ;
	}
	unsigned int uiCurRenderTriNum = 0;
	unsigned int uiLeftTopFanFlag = 0;
	unsigned int uiLeftBottomFanFlag = 0;
	unsigned int uiRightTopFanFlag = 0;
	unsigned int uiRightBottomFanFlag = 0;

	unsigned int uiBias = QuadTerrainUpdateGeometryPara.uiTesselationLevel - QuadTerrainUpdateGeometryPara.m_uiCurLevel;
	unsigned int uiVarianceLength = (1 << QuadTerrainUpdateGeometryPara.uiTesselationLevel) + 1;
	unsigned int uiLength = (1 << QuadTerrainUpdateGeometryPara.m_uiCurLevel);
	for (unsigned int i = 1 ; i < uiLength - 1; i++)
	{
		for(unsigned int j = 1 ; j < uiLength - 1; j++)
		{

			unsigned int A = (i << uiBias) * uiVarianceLength + (j << uiBias);
			unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + (j << uiBias);
			unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
			unsigned int D = (i << uiBias) * uiVarianceLength + ((j + 1) << uiBias);

			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}
	}
	if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_LEFT])
	{
		iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_LEFT];
		if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
		{

			for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
			{
				unsigned int A = (j << uiBias);
				unsigned int B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
			}
			uiLeftTopFanFlag |= 0x01;
			uiLeftBottomFanFlag |= 0x01;


		}
		else
		{

			for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
			{
				unsigned int A = (j << uiBias);
				unsigned int B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
	}
	else
	{
		for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
		{
			unsigned int A = (j << uiBias);
			unsigned int B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
			unsigned int C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
			unsigned int D = ((j + 1) << uiBias);
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}

	}
	if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_RIGHT])
	{
		iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_RIGHT];
		if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
		{

			for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
			{
				unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
			}
			uiRightTopFanFlag |= 0x01;
			uiRightBottomFanFlag |= 0x01;


		}
		else
		{

			for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
			{
				unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
				unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
	}
	else
	{
		for (unsigned int j = 1 ; j < uiLength - 1 ; j ++)
		{
			unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
			unsigned int B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
			unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
			unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}

	}
	if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_TOP])
	{
		iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_TOP];
		if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
		{

			for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
			{
				unsigned int A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
			}
			uiLeftTopFanFlag |= 0x02;
			uiRightTopFanFlag |= 0x02;


		}
		else
		{
			for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
			{
				unsigned int A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
	}
	else
	{
		for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
		{
			unsigned int A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			unsigned int B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
			unsigned int D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}
	}
	if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_BOTTOM])
	{
		iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_BOTTOM];
		if (iLevelDelt >= 0 && iLevelDelt == 1) //写成 == 1 其实就可以
		{

			for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
			{
				unsigned int A = (i << uiBias) * uiVarianceLength;
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength;
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
			}
			uiLeftBottomFanFlag |= 0x02;
			uiRightBottomFanFlag |= 0x02;					

		}
		else
		{
			for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
			{
				unsigned int A = (i << uiBias) * uiVarianceLength;
				unsigned int B = ((i + 1) << uiBias) * uiVarianceLength;
				unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
				unsigned int D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
	}
	else
	{
		for (unsigned int i = 1 ; i < uiLength - 1 ; i ++)
		{
			unsigned int A = (i << uiBias) * uiVarianceLength;
			unsigned int B = ((i + 1) << uiBias) * uiVarianceLength;
			unsigned int C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
			unsigned int D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}

	}
	//处理4个角
	//left top
	{
		unsigned int A = ((uiLength - 1) << uiBias);
		unsigned int B = ((1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
		unsigned int C = ((1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
		unsigned int D = (uiLength << uiBias);
		if (uiLeftTopFanFlag == 3)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP | AT_NLEFT,uiCurRenderTriNum);
		}
		else if (uiLeftTopFanFlag == 2)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
		}
		else if (uiLeftTopFanFlag == 1)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
		}
		else
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}
	}

	//right top
	{
		unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
		unsigned int B = (uiLength << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
		unsigned int C = (uiLength << uiBias) * uiVarianceLength + (uiLength << uiBias);
		unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
		if (uiRightTopFanFlag == 3)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP | AT_NRIGHT,uiCurRenderTriNum);
		}
		else if (uiRightTopFanFlag == 2)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
		}
		else if (uiRightTopFanFlag == 1)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
		}
		else
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}
	}

	//left bottom
	{
		unsigned int A = 0;
		unsigned int B = ((1) << uiBias) * uiVarianceLength;
		unsigned int C = ((1) << uiBias) * uiVarianceLength + ((1) << uiBias);
		unsigned int D = ((1) << uiBias);
		if (uiLeftBottomFanFlag == 3)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM | AT_NLEFT,uiCurRenderTriNum);
		}
		else if (uiLeftBottomFanFlag == 2)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
		}
		else if (uiLeftBottomFanFlag == 1)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
		}
		else
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}
	}


	//rigth bottom
	{
		unsigned int A = ((uiLength - 1) << uiBias) * uiVarianceLength;
		unsigned int B = (uiLength << uiBias) * uiVarianceLength;
		unsigned int C = (uiLength << uiBias) * uiVarianceLength + ((1) << uiBias);
		unsigned int D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
		if (uiRightBottomFanFlag == 3)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM | AT_NRIGHT,uiCurRenderTriNum);
		}
		else if (uiRightBottomFanFlag == 2)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
		}
		else if (uiRightBottomFanFlag == 1)
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
		}
		else
		{
			VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
		}
	}

	QuadTerrainUpdateGeometryPara.pTG->GetMeshDate()->GetIndexBuffer()->UnLock();
	SetActiveNum(uiCurRenderTriNum);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void VSQuadTerrainGeometry::AddTriangle(VSUSHORT_INDEX * &pIndexData,unsigned int A,unsigned int B,
										unsigned int C,unsigned int D,unsigned int uiATFlag,unsigned int &uiCurRenderTriNum)
{
	unsigned int M = (A + C) >> 1;
	unsigned int AB = (A + B) >> 1;
	unsigned int BC = (C + B) >> 1;
	unsigned int CD = (C + D) >> 1;
	unsigned int DA = (D + A) >> 1;
	if (uiATFlag & AT_NBOTTOM)
	{
		*pIndexData = A;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = B;
		pIndexData++;
		uiCurRenderTriNum += 1;	
	}
	else
	{
		//0
		*pIndexData = A;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = AB;
		pIndexData++;

		//1
		*pIndexData = AB;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = B;
		pIndexData++;

		uiCurRenderTriNum += 2;
		
	}
	
	if (uiATFlag & AT_NRIGHT)
	{
		*pIndexData = B;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = C;
		pIndexData++;
		uiCurRenderTriNum += 1;
	}
	else
	{
		//2
		*pIndexData = B;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = BC;
		pIndexData++;

		//3
		*pIndexData = BC;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = C;
		pIndexData++;
		uiCurRenderTriNum += 2;
		
	}
	if (uiATFlag & AT_NTOP)
	{
		*pIndexData = C;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = D;
		pIndexData++;
		uiCurRenderTriNum += 1;
	}
	else
	{
		//4
		*pIndexData = C;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = CD;
		pIndexData++;

		//5
		*pIndexData = CD;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = D;
		pIndexData++;
		uiCurRenderTriNum += 2;
		
	}
	if (uiATFlag & AT_NLEFT)
	{
		*pIndexData = D;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = A;
		pIndexData++;
		uiCurRenderTriNum += 1;
	}
	else
	{

		//6
		*pIndexData = D;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = DA;
		pIndexData++;

		//7
		*pIndexData = DA;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = A;
		pIndexData++;
		uiCurRenderTriNum += 2;
		

	}
	

}
