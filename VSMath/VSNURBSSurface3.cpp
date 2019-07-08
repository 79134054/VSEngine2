#include "VSNURBSSurface3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSNURBSSurface3::VSNURBSSurface3()
{
	m_pWeight = NULL;
}
/*----------------------------------------------------------------*/
VSNURBSSurface3::~VSNURBSSurface3()
{
	VSMAC_DELETEA(m_pWeight);
}
/*----------------------------------------------------------------*/
bool VSNURBSSurface3::Set(const VSREAL * pWeight,unsigned int uiRow,unsigned int uiColumn)
{
	if(!pWeight || !uiRow || !uiColumn)
		return 0;
	unsigned int uiControlPointNum = uiRow * uiColumn;
	m_pWeight = VS_NEW VSREAL[uiControlPointNum];
	if(!m_pWeight)
		return 0;
	VSMemcpy(m_pWeight,pWeight,sizeof(VSREAL) * uiControlPointNum);
	return 1;

}
/*----------------------------------------------------------------*/
bool VSNURBSSurface3::Get(unsigned int Order,VSREAL U,VSREAL V ,VSVector3 *D0,VSVector3 *DU,VSVector3 *DV)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,Order) && GetVFuncValue(V,i1,Order))
	{
		VSVector3 Point(0,0,0);
		VSREAL totalW = 0.0f;
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			VSREAL tmp =  GetUBasisValue(m_uiUDegree,j0) * GetVBasisValue(m_uiVDegree,j1) *GetWeight(j0,j1);

			Point += GetControlPoint(j0,j1) * tmp;
			totalW +=tmp;

		}
		VSREAL K0 = 1.0f / totalW;
		VSVector3 Point0 = Point * K0 ;
		if(D0)
			*D0 = Point0;
		if(Order == 0)
			return 1;



		VSVector3 kXDer1(0,0,0);
		VSVector3 kXDer2(0,0,0);
		VSREAL fWDer1 = 0.0f;
		VSREAL fWDer2 = 0.0f;
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			VSREAL fTmp1 = GetUBasisFDValue(m_uiUDegree,j0) * GetVBasisValue(m_uiVDegree,j1) *GetWeight(j0,j1);
			VSREAL fTmp2 = GetUBasisValue(m_uiUDegree,j0) * GetVBasisFDValue(m_uiVDegree,j1) *GetWeight(j0,j1); 
			kXDer1 += GetControlPoint(j0,j1) * fTmp1;
			kXDer2 += GetControlPoint(j0,j1) * fTmp2;
			fWDer1 += fTmp1;
			fWDer2 += fTmp2;

		}

		VSVector3 Point1 = (kXDer1 - Point0 * fWDer1) * K0 ;
		VSVector3 Point2 = (kXDer2 - Point0 * fWDer2) * K0 ;
		if(DU)
			*DU = Point1;
		if(DV)
			*DV = Point2;
		if(Order == 1)
			return 1;
		// not control all path
		return 1;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
VSVector3 VSNURBSSurface3::GetPoint(VSREAL U,VSREAL V)
{
	VSVector3 Point;
	if(Get(0,U,V,&Point,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
VSVector3 VSNURBSSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 Point;
	if(Get(1,U,V,NULL,&Point,NULL))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
VSVector3 VSNURBSSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 Point;
	if(Get(1,U,V,NULL,NULL,&Point))
	{
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}


}
/*----------------------------------------------------------------*/
bool VSNURBSSurface3::Copy(const VSNURBSSurface3 & NURBSSurface3)
{
	if(!VSB_SplineSurface3::Copy(NURBSSurface3))
		return 0;
	if(!NURBSSurface3.m_pWeight)
		return 0;

	m_pWeight = VS_NEW VSREAL[m_uiControlPointNum];
	if(!m_pWeight)
		return 0;
	VSMemcpy(m_pWeight,NURBSSurface3.m_pWeight,sizeof(VSREAL) * m_uiControlPointNum);
	return 1;


}