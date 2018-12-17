#include "VSControlSurface3.h"
using namespace VSEngine2;
#include "VSMemManager.h"
#include "VSSurfaceLengthFunction1.h"
/*----------------------------------------------------------------*/
VSControlSurface3::VSControlSurface3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;
	m_uiRow = 0;
	m_uiColumn = 0;

}
/*----------------------------------------------------------------*/
VSControlSurface3::~VSControlSurface3()
{
	VSMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;
	m_uiRow = 0;
	m_uiColumn = 0;

}

/*----------------------------------------------------------------*/
bool VSControlSurface3::Copy(const VSControlSurface3& ControlSurface3)
{
	if(!ControlSurface3.m_pControlPoint || !ControlSurface3.m_uiColumn || 
		!ControlSurface3.m_uiControlPointNum || !ControlSurface3.m_uiRow)
		return 0;
	VSMAC_DELETEA(m_pControlPoint);
	
	m_uiControlPointNum = ControlSurface3.m_uiControlPointNum;
	m_uiRow = ControlSurface3.m_uiRow;
	m_uiColumn = ControlSurface3.m_uiColumn;
	m_pControlPoint = VS_NEW VSVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;

	VSMemcpy(m_pControlPoint,ControlSurface3.m_pControlPoint,sizeof(VSVector3) * m_uiControlPointNum); 
	return 1;
}
VSREAL VSControlSurface3::GetULength(VSREAL V,VSREAL U1, VSREAL U2,unsigned int uiIterations)
{
	VSSurfaceLengthFunction1 LengthF(VSSurfaceLengthFunction1::LT_U_LEN,V);
	LengthF.SetSurface(this);
	return LengthF.Integration(U1,U2,uiIterations);
}
VSREAL VSControlSurface3::GetVLength(VSREAL U,VSREAL V1, VSREAL V2,unsigned int uiIterations)
{
	VSSurfaceLengthFunction1 LengthF(VSSurfaceLengthFunction1::LT_V_LEN,U);
	LengthF.SetSurface(this);
	return LengthF.Integration(V1,V2,uiIterations);
}
/*----------------------------------------------------------------*/
VSREAL VSControlSurface3::GetUTime (VSREAL V,VSREAL fLength, unsigned int uiIterations,VSREAL fTolerance)
{
	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	VSREAL TotalLength = GetULength(V,0.0f,1.0f,uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	VSREAL fTime = fLength / TotalLength;

	for (unsigned int i = 0; i < uiIterations; i++)
	{
		VSREAL fDifference = GetULength(V,0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetUPartialDerivative(fTime,V).GetLength());
	}

	return fTime;

}
/*----------------------------------------------------------------*/
VSREAL VSControlSurface3::GetVTime (VSREAL U,VSREAL fLength, unsigned int uiIterations,VSREAL fTolerance)
{
	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	VSREAL TotalLength = GetVLength(U,0.0f,1.0f,uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	VSREAL fTime = fLength / TotalLength;

	for (unsigned int i = 0; i < uiIterations; i++)
	{
		VSREAL fDifference = GetVLength(U,0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetVPartialDerivative(fTime,U).GetLength());
	}

	return fTime;

}