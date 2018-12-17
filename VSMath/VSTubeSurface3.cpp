#include "VSTubeSurface3.h"
using namespace VSEngine2;
VSTubeSurface3::VSTubeSurface3()
{
	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
VSTubeSurface3::~VSTubeSurface3()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
bool VSTubeSurface3::SetCurve(VSCurve3 * pCurve,VSREAL fN,VSREAL fB)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	m_fN = fN;
	m_fB = fB;
	return 1;
}
/*----------------------------------------------------------------*/
VSVector3 VSTubeSurface3::GetPoint(VSREAL U,VSREAL V)
{
	VSVector3 T,B,N;
	m_pCurve->GetFrenetFrameBNT(U,B,N,T);
	B.Normalize();
	N.Normalize();
	return m_pCurve->GetPoint(U) + N * COS(VS2PI * V) * m_fN  + B * SIN(VS2PI * V) * m_fB;
}
/*----------------------------------------------------------------*/
VSVector3 VSTubeSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{
	return m_pCurve->GetFirstDerivative(U);
}
/*----------------------------------------------------------------*/
VSVector3 VSTubeSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 T,B,N;
	m_pCurve->GetFrenetFrameBNT(U,B,N,T);
	B.Normalize();
	N.Normalize();
	return B * COS(VS2PI * V) * m_fB - N * SIN(VS2PI * V) * m_fN ;
}
/*----------------------------------------------------------------*/