#include "VSSurface3SubdivisionByLength.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSSurface3SubdivisionByLength::VSSurface3SubdivisionByLength(unsigned int Row,unsigned int Column,VSREAL U1,VSREAL U2,VSREAL V1 ,VSREAL V2,unsigned int uiIterations)
{
	m_uiRow = Row;
	m_uiColumn = Column;
	m_fU1 = U1;
	m_fU2 = U2;
	m_fV1 = V1;
	m_fV2 = V2;
	m_uiIterations = uiIterations;

}
/*----------------------------------------------------------------*/
VSSurface3SubdivisionByLength::~VSSurface3SubdivisionByLength()
{



}
/*----------------------------------------------------------------*/
bool VSSurface3SubdivisionByLength::Subdivision()
{
	if(!m_uiRow || !m_uiColumn || !m_pSurface)
		return 0;
	if(m_fU2 > m_fU1 || m_fV2 > m_fV1)
		return 0;
	return 1;
// 	VSMAC_DELETEA(m_pPoint);
// 
// 	m_pPoint = VS_NEW VSVector3[m_uiRow * m_uiColumn];
// 	if(!m_pPoint)
// 		return 0;
// 
// 	VSMAC_DELETEA(m_pUV);
// 	m_pUV = VS_NEW VSVector2[m_uiRow * m_uiColumn];
// 	if(!m_pUV)
// 		return 0;
// 	VSREAL fi = (VSREAL)GetULength()/ (m_uiRow - 1);
// 	VSREAL fj = (VSREAL)(m_fV2 - m_fV1) / (m_uiColumn - 1);
// 	VSREAL fiTemp = m_fU1;
// 	for(unsigned int i = 0 ; i < m_uiRow ; i++)
// 	{ 
// 		VSREAL fjTemp = m_fV1;
// 		unsigned int Temp = i * m_uiColumn;
// 		for(unsigned int j = 0 ; j < m_uiColumn ; j++)
// 		{
// 			m_pUV[Temp + j] = VSVector2(fiTemp,fjTemp);
// 			m_pPoint[ Temp + j] = m_pSurface->GetPoint(fiTemp,fjTemp);
// 			fjTemp += fj;
// 		}
// 		fiTemp += fi;
// 	}
// 	return 1;
// 
// 
// 
// 	VSMAC_DELETEA(m_pT);
// 	m_pT = VS_NEW VSREAL[m_uiPointNum]; 
// 
// 	if(!m_pT)
// 		return 0;
// 
// 	VSREAL det = m_pCurve->GetLength(m_ft1,m_ft2,m_uiIterations)/VSREAL(m_uiPointNum - 1);
// 	VSREAL temp = m_ft1;
// 	for(unsigned int i = 0 ; i < m_uiPointNum ; i++)
// 	{
// 		m_pT[i] = m_pCurve->GetTime(temp,m_uiIterations);
// 		m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
// 		temp += det;
// 
// 	}
// 	return 1;

}