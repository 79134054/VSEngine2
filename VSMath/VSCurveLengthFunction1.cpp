#include "VSCurveLengthFunction1.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSCurveLengthFunction1::VSCurveLengthFunction1()
{
	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
VSCurveLengthFunction1::~VSCurveLengthFunction1()
{

	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
VSREAL VSCurveLengthFunction1::GetValue(VSREAL fParameter)
{
	if(!m_pCurve)
		return VSMAX_REAL;
	VSVector3 p = m_pCurve->GetFirstDerivative(fParameter);
	return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);

}
/*----------------------------------------------------------------*/
VSREAL VSCurveLengthFunction1::GetFirstDerivative(VSREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
VSREAL VSCurveLengthFunction1::GetSecondDerivative(VSREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
VSREAL VSCurveLengthFunction1::GetThirdDerivative(VSREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/