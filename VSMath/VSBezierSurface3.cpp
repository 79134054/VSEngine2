#include "VSBezierSurface3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSBezierSurface3::VSBezierSurface3()
{
	m_UC = NULL;
	m_VC = NULL;
	m_UPDValue = NULL;
	m_VPDValue = NULL;

}
/*----------------------------------------------------------------*/
VSBezierSurface3::~VSBezierSurface3()
{
	VSMAC_DELETEA(m_UC);
	VSMAC_DELETEA(m_VC);
	VSMAC_DELETEA(m_UPDValue);
	VSMAC_DELETEA(m_VPDValue);
}
/*----------------------------------------------------------------*/
bool VSBezierSurface3::Set(const VSVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn)
{
	if(!pControlPoint || !uiRow || !uiColumn)
		return 0;
	m_uiColumn = uiColumn;
	m_uiRow = uiRow;
	m_uiControlPointNum = m_uiColumn * m_uiRow;
	
	VSMAC_DELETEA(m_UC);
	VSMAC_DELETEA(m_VC);
	VSMAC_DELETEA(m_UPDValue);
	VSMAC_DELETEA(m_VPDValue);
	VSMAC_DELETEA(m_pControlPoint);

	m_pControlPoint = VS_NEW VSVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;

	VSMemcpy(m_pControlPoint,pControlPoint,sizeof(VSVector3) * m_uiControlPointNum);

	m_UC = VS_NEW VSREAL [uiRow * uiRow];
	if(!m_UC)
		return 0;
	VSMemset(m_UC,0,sizeof(VSREAL) * uiRow * uiRow);
	SetUC(0,0,1.0f);
	SetUC(1,0,1.0f);
	SetUC(1,1,1.0f);
	for (unsigned int i = 2; i <  uiRow ; i++)
	{
		SetUC(i,0,1.0f);
		SetUC(i,i,1.0f);

		for (unsigned int j = 1; j < i; j++)
		{

			SetUC(i,j,GetUC(i - 1,j - 1) + GetUC(i - 1,j));
		}
	}

	m_VC = VS_NEW VSREAL [m_uiColumn * m_uiColumn];
	if(!m_VC)
		return 0;
	VSMemset(m_VC,0,sizeof(VSREAL) * m_uiColumn * m_uiColumn);
	SetVC(0,0,1.0f);
	SetVC(1,0,1.0f);
	SetVC(1,1,1.0f);
	for (unsigned int i = 2; i <  m_uiColumn; i++)
	{
		SetVC(i,0,1.0f);
		SetVC(i,i,1.0f);

		for (unsigned int j = 1; j < i; j++)
		{

			SetVC(i,j,GetVC(i - 1,j - 1) + GetVC(i - 1,j));
		}
	}

	m_UPDValue = VS_NEW VSVector3[(m_uiRow - 1) * m_uiColumn];
	if(!m_UPDValue)
		return 0;
	
	for (unsigned int i = 0; i < m_uiRow - 1; i++)
	{
		for(unsigned int j = 0 ; j < m_uiColumn ; j++)
		{
			SetUPD(i,j,GetControlPoint(i + 1,j) - GetControlPoint(i,j));
		}
	}

	m_VPDValue = VS_NEW VSVector3[m_uiRow * (m_uiColumn- 1)];
	if(!m_VPDValue)
		return 0;

	for (unsigned int i = 0; i < m_uiRow; i++)
	{
		for(unsigned int j = 0 ; j < m_uiColumn - 1 ; j++)
		{
			SetVPD(i,j,GetControlPoint(i,j+1) - GetControlPoint(i,j));
		}
	}
	return 1;
}
/*----------------------------------------------------------------*/
VSVector3 VSBezierSurface3::GetPoint(VSREAL U,VSREAL V)
{
	if(!m_UPDValue)
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	VSREAL fFU = (VSREAL)1.0 - U;

	VSREAL fU =(VSREAL)1.0;
	


	unsigned int uiDegreeU = m_uiRow - 1;
	unsigned int uiDegreeV = m_uiColumn - 1;
	VSVector3 kResult(0,0,0);

	for(unsigned int i = 0 ; i <= uiDegreeU ; i++)
	{
		VSREAL fFV = (VSREAL)1.0 - V;

		VSREAL fV =(VSREAL)1.0; 
		VSREAL PU = VSREAL(uiDegreeU - i);
		for(unsigned int j = 0; j <= uiDegreeV; j++)
		{
			VSREAL PV = VSREAL(uiDegreeV - j);
			VSVector3 fCoeff = GetControlPoint(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;

		}

	}

	return kResult;

}
/*----------------------------------------------------------------*/
VSVector3 VSBezierSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{
	if(!m_UPDValue)
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	VSREAL fFU = (VSREAL)1.0 - U;
	VSREAL fU = (VSREAL)1.0;
	
	

	unsigned int uiDegreeU = m_uiRow - 2;
	unsigned int uiDegreeV = m_uiColumn - 1;

	VSVector3 kResult(0,0,0);
	
	for(unsigned int i = 0 ; i <= uiDegreeU ; i++)
	{
		VSREAL PU = VSREAL(uiDegreeU - i);
		VSREAL fFV = (VSREAL)1.0 - V;
		VSREAL fV = (VSREAL)1.0; 
		for(unsigned int j = 0; j <= uiDegreeV; j++)
		{
			VSREAL PV = VSREAL(uiDegreeV - j);
			VSVector3 fCoeff = GetUPD(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;
		}

	}
		
	kResult *= (VSREAL)uiDegreeU;	
	return kResult;

}
/*----------------------------------------------------------------*/
VSVector3 VSBezierSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{
	if(!m_UPDValue)
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	VSREAL fFU = (VSREAL)1.0 - U;
	VSREAL fU = (VSREAL)1.0;
	


	unsigned int uiDegreeU = m_uiRow - 1;
	unsigned int uiDegreeV = m_uiColumn - 2;
	VSVector3 kResult(0,0,0);

	for(unsigned int i = 0 ; i <= uiDegreeU ; i++)
	{
		VSREAL PU = VSREAL(uiDegreeU - i);
		VSREAL fFV = (VSREAL)1.0 - V;
		VSREAL fV = (VSREAL)1.0; 
		for(unsigned int j = 0; j <= uiDegreeV; j++)
		{
			VSREAL PV = VSREAL(uiDegreeV - j);
			VSVector3 fCoeff = GetVPD(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;
		}

	}

	kResult *= (VSREAL)uiDegreeV;	
	return kResult;
}
/*----------------------------------------------------------------*/
bool VSBezierSurface3::Copy(const VSBezierSurface3 & BezierSurface3)
{
	if(!VSControlSurface3::Copy(BezierSurface3))
		return 0;
	if(!BezierSurface3.m_VC || !BezierSurface3.m_UC || !BezierSurface3.m_UPDValue || !BezierSurface3.m_VPDValue)
		return 0;

	VSMAC_DELETEA(m_UC);
	VSMAC_DELETEA(m_VC);
	VSMAC_DELETEA(m_UPDValue);
	VSMAC_DELETEA(m_VPDValue);
	
	m_UC = VS_NEW VSREAL [m_uiRow * m_uiRow];
	if(!m_UC)
		return 0;
	VSMemcpy(m_UC,BezierSurface3.m_UC,sizeof(VSREAL) * m_uiRow * m_uiRow);

	m_VC = VS_NEW VSREAL [m_uiColumn * m_uiColumn];
	if(!m_VC)
		return 0;
	VSMemcpy(m_VC,BezierSurface3.m_VC,sizeof(VSREAL) * m_uiColumn * m_uiColumn);

	m_UPDValue = VS_NEW VSVector3[(m_uiRow - 1) * m_uiColumn];
	if(!m_UPDValue)
		return 0;
	VSMemcpy(m_UPDValue,BezierSurface3.m_UPDValue,sizeof(VSVector3) * (m_uiRow - 1) * m_uiColumn);

	m_VPDValue = VS_NEW VSVector3[m_uiRow * (m_uiColumn- 1)];
	if(!m_VPDValue)
		return 0;
	VSMemcpy(m_VPDValue,BezierSurface3.m_VPDValue,sizeof(VSVector3) * m_uiRow * (m_uiColumn- 1));
	return 1;
}