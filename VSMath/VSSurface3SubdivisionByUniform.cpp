#include "VSSurface3SubdivisionByUniform.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSSurface3SubdivisionByUniform::VSSurface3SubdivisionByUniform(unsigned int uiRow,unsigned int uiColumn,VSREAL U1,VSREAL U2,VSREAL V1 ,VSREAL V2)
{

}
/*----------------------------------------------------------------*/
VSSurface3SubdivisionByUniform::~VSSurface3SubdivisionByUniform()
{

}
/*----------------------------------------------------------------*/
bool VSSurface3SubdivisionByUniform::Subdivision()
{
	if(!m_uiRow || !m_uiColumn || !m_pSurface)
		return 0;
	if(m_fU2 > m_fU1 || m_fV2 > m_fV1)
		return 0;
	VSMAC_DELETEA(m_pPoint);

	m_pPoint = VS_NEW VSVector3[m_uiRow * m_uiColumn];
	if(!m_pPoint)
		return 0;

	VSMAC_DELETEA(m_pUV);
	m_pUV = VS_NEW VSVector2[m_uiRow * m_uiColumn];
	if(!m_pUV)
		return 0;
	VSREAL fi = (VSREAL)(m_fU2 - m_fU1)/ (m_uiRow - 1);
	VSREAL fj = (VSREAL)(m_fV2 - m_fV1) / (m_uiColumn - 1);
	VSREAL fiTemp = m_fU1;
	for(unsigned int i = 0 ; i < m_uiRow ; i++)
	{
		VSREAL fjTemp = m_fV1;
		unsigned int Temp = i * m_uiColumn;
		for(unsigned int j = 0 ; j < m_uiColumn ; j++)
		{
			m_pUV[Temp + j] = VSVector2(fiTemp,fjTemp);
			m_pPoint[ Temp + j] = m_pSurface->GetPoint(fiTemp,fjTemp);
			fjTemp += fj;
		}
		fiTemp += fi;
	}
	return 1;
}
/*----------------------------------------------------------------*/