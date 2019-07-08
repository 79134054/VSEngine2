#include "VSSphere3.h"
#include "VSAABB3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSSphere3::VSSphere3()
{


}
/*----------------------------------------------------------------*/
VSSphere3::~VSSphere3()
{
	m_fRadius = 0.0f;

}
/*----------------------------------------------------------------*/
VSSphere3::VSSphere3(const VSVector3 & Center, VSREAL fRadius)
{
	Set(Center,fRadius);

}
/*----------------------------------------------------------------*/
void VSSphere3::CreateSphere(const VSVector3 *pPointArray,unsigned int uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return ;
	VSVector3 Sum;
	for(unsigned int i = 0 ; i < uiPointNum ; i++)
		Sum += pPointArray[i];
	m_Center = Sum / (uiPointNum * 1.0f);
	VSREAL radiusSqr = 0.0f;
	for(unsigned int i = 0 ; i < uiPointNum ; i++)
	{
		VSVector3 diff = pPointArray[i] - m_Center;
		VSREAL Temp = diff.GetSqrLength();
		if(Temp > radiusSqr)
			radiusSqr = Temp;
	
	}
	m_fRadius = SQRT(radiusSqr);
}
/*----------------------------------------------------------------*/
VSSphere3 VSSphere3::MergSpheres(const VSSphere3 &Sphere)const
{
	VSSphere3 Temp;
	VSVector3 CenterDiff = m_Center - Sphere.m_Center;
	VSREAL fRadiusDiff = m_fRadius - Sphere.m_fRadius;
	VSREAL fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
	VSREAL Lsqr = CenterDiff.GetSqrLength();
	if(fRadiusDiffSqr >= Lsqr)
	{
		if(fRadiusDiff >= 0.0f)
		{
			 Temp.m_Center = m_Center;
			Temp.m_fRadius = m_fRadius;
		}
		else
		{
			Temp.m_Center = Sphere.m_Center;
			Temp.m_fRadius = Sphere.m_fRadius;
		}
	
	
	}
	else
	{
		VSREAL L = SQRT(Lsqr);
		VSREAL t = (L + m_fRadius - Sphere.m_fRadius)/(2.0f * L);
		Temp.m_Center = Sphere.m_Center + CenterDiff * t;
		Temp.m_fRadius = (L + m_fRadius + Sphere.m_fRadius)/2.0f;
	}

	return Temp;

}
/*----------------------------------------------------------------*/
void VSSphere3::Transform(const VSSphere3 & Sphere,const VSMatrix3X3W &Mat)
{
	m_Center = Sphere.m_Center * Mat;
	/*VSMatrix3X3 M;
	Mat.Get3X3(M);
	VSVector3 V[3];
	VSREAL E[3];
	M.GetEigenSystem(E,V);
	VSREAL Max = E[0];
	for(unsigned int i = 1 ; i < 3 ; i++)
	{
	if(Max < E[i])
	Max = E[i];
	}
	m_fRadius = Sphere.m_fRadius * Max;*/
	VSVector3 V0(1.0f,0.0f,0.0f),V1(0.0f,1.0f,0.0f),V2(0.0f,0.0f,1.0f);
	VSVector3 Temp0 = Sphere.m_Center +  V0 * Sphere.m_fRadius;
	VSVector3 Temp1 = Sphere.m_Center +  V1 * Sphere.m_fRadius;
	VSVector3 Temp2 = Sphere.m_Center +  V2 * Sphere.m_fRadius;

	Temp0 = Temp0 * Mat;
	Temp1 = Temp1 * Mat;
	Temp2 = Temp2 * Mat;

	VSREAL fRadiusTemp;

	m_fRadius = (Temp0 - m_Center).GetLength();

	fRadiusTemp = (Temp1 - m_Center).GetLength();

	if(m_fRadius < fRadiusTemp)
		m_fRadius = fRadiusTemp;

	fRadiusTemp = (Temp2 - m_Center).GetLength();

	if(m_fRadius < fRadiusTemp)
		m_fRadius = fRadiusTemp;
}
VSAABB3 VSSphere3::GetAABB()const
{
	VSVector3 Max = m_Center + VSVector3(m_fRadius,m_fRadius,m_fRadius);
	VSVector3 Min = m_Center + VSVector3(-m_fRadius,-m_fRadius,-m_fRadius);
	return VSAABB3(Max,Min);
}