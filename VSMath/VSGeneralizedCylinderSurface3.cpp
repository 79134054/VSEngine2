#include "VSGeneralizedCylinderSurface3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSGeneralizedCylinderSurface3::VSGeneralizedCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
VSGeneralizedCylinderSurface3::~VSGeneralizedCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
bool VSGeneralizedCylinderSurface3::SetCurve(VSCurve3 * pCurve1,VSCurve3 *pCurve2)
{
	if(!pCurve1 || !pCurve2)
		return 0;
	m_pCurve1 = pCurve1;
	m_pCurve2 = pCurve2;
	return 1;
}
/*----------------------------------------------------------------*/
VSVector3 VSGeneralizedCylinderSurface3::GetPoint(VSREAL U,VSREAL V)
{
	return m_pCurve1->GetPoint(U) * (1 - V) + m_pCurve2->GetPoint(U) * V;
}
/*----------------------------------------------------------------*/
VSVector3 VSGeneralizedCylinderSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 Temp = m_pCurve1->GetFirstDerivative(U) * (1 - V) + m_pCurve2->GetFirstDerivative(U) * V;
	//Temp.Normalize();
	return Temp;
}
/*----------------------------------------------------------------*/
VSVector3 VSGeneralizedCylinderSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 Temp = m_pCurve2->GetPoint(U) - m_pCurve1->GetPoint(U);
	//Temp.Normalize();
	return Temp;
}
/*----------------------------------------------------------------*/