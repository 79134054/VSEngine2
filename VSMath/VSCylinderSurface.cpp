#include "VSCylinderSurface3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSCylinderSurface3::VSCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
VSCylinderSurface3::~VSCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
bool VSCylinderSurface3::SetCurve(VSCurve3 * pCurve1,VSSegmentCurve3 *pCurve2)
{
	if(!pCurve1 || !pCurve2)
		return 0;
	m_pCurve1 = pCurve1;
	m_pCurve2 = pCurve2;
	return 1;

}
/*----------------------------------------------------------------*/
VSVector3 VSCylinderSurface3::GetPoint(VSREAL U,VSREAL V)
{
	return m_pCurve1->GetPoint(U) + m_pCurve2->GetSegment().GetDir() * (V *  m_pCurve2->GetSegment().GetLen());

}
/*----------------------------------------------------------------*/
VSVector3 VSCylinderSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{

	return m_pCurve1->GetFirstDerivative(U);
}
/*----------------------------------------------------------------*/
VSVector3 VSCylinderSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{

	return m_pCurve2->GetSegment().GetDir();
}
/*----------------------------------------------------------------*/