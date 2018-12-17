#include "VSSurfaceLengthFunction1.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSSurfaceLengthFunction1::VSSurfaceLengthFunction1(unsigned int uiLengthType,VSREAL fTypePara)
{
	m_pSurface = NULL;
	m_fTypePara = fTypePara;
	m_uiLengthType = uiLengthType;
}
/*----------------------------------------------------------------*/
VSSurfaceLengthFunction1::~VSSurfaceLengthFunction1()
{

	m_pSurface = NULL;
}
/*----------------------------------------------------------------*/
VSREAL VSSurfaceLengthFunction1::GetValue(VSREAL fParameter)
{
	if(!m_pSurface)
		return VSMAX_REAL;
	if (m_uiLengthType == LT_U_LEN)
	{
		VSVector3 p = m_pSurface->GetUPartialDerivative(fParameter,m_fTypePara);
		return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);
	}
	else
	{
		VSVector3 p = m_pSurface->GetVPartialDerivative(m_fTypePara,fParameter);
		return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);
	}

}
/*----------------------------------------------------------------*/
VSREAL VSSurfaceLengthFunction1::GetFirstDerivative(VSREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
VSREAL VSSurfaceLengthFunction1::GetSecondDerivative(VSREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
VSREAL VSSurfaceLengthFunction1::GetThirdDerivative(VSREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/