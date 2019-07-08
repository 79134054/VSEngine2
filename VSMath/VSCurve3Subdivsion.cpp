#include "VSCurve3Subdivision.h"
#include "VSMemManager.h"
#include "VSB_SplineCurve3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSCurve3Subdivision::VSCurve3Subdivision()
{
	m_pPoint = NULL;
	m_uiPointNum = 0;
	m_pCurve = NULL;
	m_pT = NULL;
}
/*----------------------------------------------------------------*/
VSCurve3Subdivision::~VSCurve3Subdivision()
{
	VSMAC_DELETEA(m_pPoint);
	VSMAC_DELETEA(m_pT);
	m_uiPointNum = 0;


}
/*----------------------------------------------------------------*/
bool VSCurve3Subdivision::SetSubCurve(VSControlCurve3 * pCurve)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	return 1;
}
/*----------------------------------------------------------------*/
bool VSCurve3Subdivision::ControlPointChange(unsigned int i)
{
	if(!m_pCurve || !m_pPoint || !m_pT || !m_uiPointNum)
		return 0;

	if(i >= m_pCurve->GetControlPointNum())
		return 0;
	if(m_pCurve->GetType() == VSControlCurve3::BEZIER)
	{
		for(unsigned int i = 0; i < m_uiPointNum ; i++)
			m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		return 1;
	}
	else if(m_pCurve->GetType() == VSControlCurve3::B_SPLINE || m_pCurve->GetType() == VSControlCurve3::NURBS)
	{
		VSB_SplineCurve3 *pCurve =(VSB_SplineCurve3 *)m_pCurve;
		VSREAL t1 =pCurve->GetKnotVectorValue(i);
		VSREAL t2 =pCurve->GetKnotVectorValue(pCurve->GetDegree() + i); 
		for(unsigned int i = 0; i < m_uiPointNum ; i++)
			if(m_pT[i] >= t1 && m_pT[i] <= t2)
				m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/