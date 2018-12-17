#include "VSControlCurve3.h"
#include "VSMatrix3X3W.h"
#include "VSMemManager.h"
#include "VSCurveLengthFunction1.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSControlCurve3::VSControlCurve3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;

}
/*----------------------------------------------------------------*/
VSControlCurve3::~VSControlCurve3()
{
	VSMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;

}
/*----------------------------------------------------------------*/
void	VSControlCurve3::GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T)
{
	VSVector3 FD = GetFirstDerivative(t);
	FD.Normalize();
	VSVector3 SD = GetSecondDerivative(t);
	SD.Normalize();
	VSREAL FDDot = FD.Dot(FD);
	VSREAL SDDotFD = FD.Dot(SD);
	N = SD - FD * SDDotFD;
	N.Normalize();
	T = FD;
	B.Cross(N,T);

}
/*----------------------------------------------------------------*/
VSREAL VSControlCurve3::GetTotalLength(unsigned int uiIterations)
{
	return GetLength(0.0f,1.0f,uiIterations);

}
/*----------------------------------------------------------------*/
VSREAL VSControlCurve3::GetTime (VSREAL fLength, unsigned int uiIterations,VSREAL fTolerance)
{
	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	VSREAL TotalLength = GetTotalLength(uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	VSREAL fTime = fLength / TotalLength;

	for (unsigned int i = 0; i < uiIterations; i++)
	{
		VSREAL fDifference = GetLength(0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetFirstDerivative(fTime).GetLength());
	}

	return fTime;

}
/*----------------------------------------------------------------*/
VSREAL VSControlCurve3::GetCurvature(VSREAL fTime)
{
	VSVector3 FD = GetFirstDerivative(fTime);
	VSREAL fSpeedSqr = FD.GetSqrLength();

	if (fSpeedSqr >= EPSILON_E4)
	{
		VSVector3 SD = GetSecondDerivative(fTime);
		VSVector3 Cross;
		Cross.Cross(FD,SD);
		VSREAL fNumer = Cross.GetLength();
		VSREAL fDenom = POW(fSpeedSqr,(VSREAL)1.5);

		return fNumer / fDenom;
	}
	else
	{

		return 0.0f;
	}

}
/*----------------------------------------------------------------*/
VSREAL VSControlCurve3::GetTorsion(VSREAL fTime)
{
	VSVector3 FD = GetFirstDerivative(fTime);
	VSVector3 SD = GetSecondDerivative(fTime);
	VSVector3 Cross;
	Cross.Cross(FD,SD);
	VSREAL fDenom = Cross.GetSqrLength();

	if (fDenom >= EPSILON_E4)
	{
		VSVector3 TD= GetThirdDerivative(fTime);
		VSREAL fNumer = Cross.Dot(TD);
		return fNumer / fDenom;
	}
	else
	{
		return 0.0f;
	}

}
/*----------------------------------------------------------------*/
bool VSControlCurve3::Copy(const VSControlCurve3& ControlCurve3)
{
	if(!ControlCurve3.m_pControlPoint || !ControlCurve3.m_uiControlPointNum)
		return 0;
	VSMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = ControlCurve3.m_uiControlPointNum;
	m_pControlPoint = VS_NEW VSVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	VSMemcpy(m_pControlPoint,ControlCurve3.m_pControlPoint,sizeof(VSVector3) * m_uiControlPointNum);
	return 1;
}
/*----------------------------------------------------------------*/
VSREAL	VSControlCurve3::GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations)
{
	VSCurveLengthFunction1 LengthF;
	LengthF.SetCurve(this);
	return LengthF.Integration(t1,t2,uiIterations);

}