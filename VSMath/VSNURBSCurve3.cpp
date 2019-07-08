#include "VSNURBSCurve3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSNURBSCurve3::VSNURBSCurve3()
{
	m_pWeight = NULL;
	m_CurveType = NURBS;
}
/*----------------------------------------------------------------*/
VSNURBSCurve3::~VSNURBSCurve3()
{
	VSMAC_DELETEA(m_pWeight);
}
/*----------------------------------------------------------------*/
bool VSNURBSCurve3::Set(const VSREAL * pWeight,unsigned int uiControlPointNum)
{
	if(!pWeight || !uiControlPointNum)
		return 0;
	m_pWeight = VS_NEW VSREAL[uiControlPointNum];
	if(!m_pWeight)
		return 0;
	VSMemcpy(m_pWeight,pWeight,sizeof(VSREAL) * uiControlPointNum);
	return 1;

}
/*----------------------------------------------------------------*/
bool VSNURBSCurve3::Get(unsigned int Order,VSREAL t ,VSVector3 *D0,VSVector3 *D1,VSVector3 *D2,VSVector3 *D3)
{
	int i;
	if(GetFuncValue(t,i,Order))
	{
		VSVector3 Point(0,0,0);
		VSREAL totalW = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			VSREAL tmp =  GetBasisValue(m_uiDegree,j) * m_pWeight[j];

			Point += m_pControlPoint[j] * tmp;
			totalW +=tmp;

		}
		VSREAL K0 = 1.0f / totalW;
		VSVector3 Point0 = Point * K0 ;
		if(D0)
			*D0 = Point0;
		if(Order == 0)
			return 1;



		VSVector3 kXDer1(0,0,0);
		VSREAL fWDer1 = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			VSREAL fTmp = GetBasisFDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer1 += m_pControlPoint[i] * fTmp;
			fWDer1 += fTmp;


		}
		VSREAL K1 = fWDer1;
		VSVector3 Point1 = (kXDer1 - Point0 * fWDer1) * K0 ;
		if(D1)
			*D1 = Point1;
		if(Order == 1)
			return 1;

		VSVector3 kXDer2(0,0,0);
		VSREAL fWDer2 = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			VSREAL fTmp = GetBasisSDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer2 += m_pControlPoint[i] * fTmp;
			fWDer2 += fTmp;

		}
		VSREAL K2 = fWDer2;
		VSVector3 Point2 = (kXDer2 - Point1 * (2.0f) * K1  - Point0  * fWDer2) * K0;
		if(D2)
			*D2 = Point2;
		if(Order == 2)
			return 1;

		VSVector3 kXDer3(0,0,0);
		VSREAL fWDer3 = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			VSREAL fTmp = GetBasisTDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer3 += m_pControlPoint[i] * fTmp;
			fWDer3 += fTmp;

		}
		VSVector3 Point3 = (kXDer3 -  Point2 * ( 3.0 )* K1  - Point1 * ( 3.0 ) * K2   -  Point0 * fWDer3) * K0;
		if(D3)
			*D3 = Point3;
		if(Order ==  3)
			return 1;

		//not control all path
		return 1;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
VSVector3	VSNURBSCurve3::GetPoint(VSREAL t)
{


	VSVector3 Point;
	if(Get(0,t,&Point,NULL,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
VSVector3	VSNURBSCurve3::GetFirstDerivative(VSREAL t)
{

	VSVector3 Point;
	if(Get(1,t,NULL,&Point,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
VSVector3	VSNURBSCurve3::GetSecondDerivative(VSREAL t)
{

	VSVector3 Point;
	if(Get(2,t,NULL,NULL,&Point,NULL))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
VSVector3	VSNURBSCurve3::GetThirdDerivative(VSREAL t)
{

	VSVector3 Point;
	if(Get(3,t,NULL,NULL,NULL,&Point))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
bool VSNURBSCurve3::Copy(const VSNURBSCurve3 &NURBSCurve3)
{
	if(!VSB_SplineCurve3::Copy(NURBSCurve3))
		return 0;
	if(!NURBSCurve3.m_pWeight)
		return 0;
	m_pWeight = VS_NEW VSREAL[m_uiControlPointNum];
	if(!m_pWeight)
		return 0;
	VSMemcpy(m_pWeight,NURBSCurve3.m_pWeight,sizeof(VSREAL) * m_uiControlPointNum);
	return 1;

}