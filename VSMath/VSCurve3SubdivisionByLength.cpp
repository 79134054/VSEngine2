#include "VSCurve3SubdivisionByLength.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSCurve3SubdivisionByLength::VSCurve3SubdivisionByLength(unsigned int PointNum,VSREAL t1,VSREAL t2,unsigned int uiIterations)
{
	 m_uiPointNum = PointNum;
	 m_ft1 = t1;
	 m_ft2 = t2;
	 m_uiIterations = uiIterations;

}
/*----------------------------------------------------------------*/
VSCurve3SubdivisionByLength::~VSCurve3SubdivisionByLength()
{



}
/*----------------------------------------------------------------*/
bool VSCurve3SubdivisionByLength::Subdivision()
{
	if(!m_uiPointNum || !m_pCurve)
		return 0;
	if(m_ft1 > m_ft2)
		return 0;
	VSMAC_DELETEA(m_pPoint);
	m_pPoint = VS_NEW VSVector3[m_uiPointNum];

	if(!m_pPoint)
		return 0;

	VSMAC_DELETEA(m_pT);
	m_pT = VS_NEW VSREAL[m_uiPointNum]; 

	if(!m_pT)
		return 0;

	VSREAL det = m_pCurve->GetLength(m_ft1,m_ft2,m_uiIterations)/VSREAL(m_uiPointNum - 1);
	VSREAL temp = m_ft1;
	for(unsigned int i = 0 ; i < m_uiPointNum ; i++)
	{
		m_pT[i] = m_pCurve->GetTime(temp,m_uiIterations);
		m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		temp += det;

	}
	return 1;

}
/*----------------------------------------------------------------*/
void VSCurve3SubdivisionByLength::SetPointNum(unsigned int uiPointNum)
{
	VSMAC_ASSERT(uiPointNum);
	m_uiPointNum = uiPointNum;
}