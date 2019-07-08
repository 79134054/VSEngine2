#include "VSB_SplineCurve3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSB_SplineCurve3::VSB_SplineCurve3()
{
	m_pControlPoint = NULL;
	m_pKnotVector = NULL;
	m_uiControlPointNum = 0;
	m_uiDegree = 0;
	m_uiKnotNum = 0;
	m_pBasis = 0;
	m_pBasisFD = 0;
	m_pBasisSD = 0;
	m_pBasisTD = 0;
	m_uiBasisNum = 0;
	m_CurveType = B_SPLINE;
}
/*----------------------------------------------------------------*/
VSB_SplineCurve3::~VSB_SplineCurve3()
{
	VSMAC_DELETEA(m_pKnotVector);
	VSMAC_DELETEA(m_pControlPoint);
	VSMAC_DELETEA(m_pBasis);
	VSMAC_DELETEA(m_pBasisFD);
	VSMAC_DELETEA(m_pBasisSD);
	VSMAC_DELETEA(m_pBasisTD);
}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::Set(const VSVector3 * pControlPoint,unsigned int uiControlPointNum)
{
	if(!pControlPoint || !uiControlPointNum)
		return 0 ;
	VSMAC_DELETEA(m_pControlPoint);

	m_pControlPoint = VS_NEW VSVector3[uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	m_uiControlPointNum = uiControlPointNum;
	VSMemcpy(m_pControlPoint,pControlPoint,uiControlPointNum * sizeof(VSVector3));
	return 1;
	
}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::ChangeToOpenClosePoints()
{
	if(!m_pControlPoint || !m_uiControlPointNum)
		return 0;
	m_uiControlPointNum = m_uiControlPointNum + 1;
	VSVector3 * m_pControlPointTemp = NULL;
	m_pControlPointTemp = VS_NEW VSVector3[m_uiControlPointNum];
	if(!m_pControlPointTemp)
		return 0;
	VSMemcpy(m_pControlPointTemp,m_pControlPoint,(m_uiControlPointNum - 1) * sizeof(VSVector3));
	m_pControlPointTemp[m_uiControlPointNum - 1] = m_pControlPointTemp[0];
	VSMAC_DELETEA(m_pControlPoint);
	m_pControlPoint = m_pControlPointTemp  ;
	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::ChangeToPeriodicClosePoints(unsigned int uiDegree)
{
	if(!uiDegree || !m_pControlPoint || !m_uiControlPointNum)
		return 0;
	m_uiDegree = uiDegree;
	unsigned int k = m_uiControlPointNum ;
	m_uiControlPointNum = m_uiControlPointNum + m_uiDegree;
	VSVector3 * m_pControlPointTemp = NULL;
	m_pControlPointTemp	= VS_NEW VSVector3[m_uiControlPointNum];
	if(!m_pControlPointTemp)
		return 0;
	VSMemcpy(m_pControlPointTemp,m_pControlPoint,k * sizeof(VSVector3));
	for(unsigned int i = 0 ; i < m_uiDegree ; i++)
	{
		m_pControlPointTemp[k + i] = m_pControlPointTemp[i];
	}
	VSMAC_DELETEA(m_pControlPoint);
	m_pControlPoint = m_pControlPointTemp  ;
	return 1;

}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::SetKnotVector(const VSREAL * pKnotVector,unsigned int uiDegree,unsigned int uiControlPointNum)
{
	if(!pKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < uiDegree + uiControlPointNum; i++)
	{
		if(pKnotVector[i] > pKnotVector[i+1])
			return 0;
	}
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	VSMAC_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;
	m_pKnotVector = VS_NEW VSREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	VSMemcpy(m_pKnotVector,pKnotVector,sizeof(VSREAL) * (m_uiKnotNum));

	VSMAC_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;


	VSMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;


	VSMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;


	VSMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;

	return 1;

}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::CreateOpenUniformKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum)
{
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	VSMAC_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;

	m_pKnotVector = VS_NEW VSREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	for(unsigned int i  = 0; i < m_uiKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiDegree)
		{
			m_pKnotVector[i] = 0;

		}
		else if( i >= m_uiDegree + 1 && i <= m_uiControlPointNum)
		{
			m_pKnotVector[i] =((VSREAL)1.0 * (i - m_uiDegree)) / (m_uiControlPointNum + 1 - m_uiDegree);
		
		}
		else
		{
			m_pKnotVector[i] = 1;
		}
	}

	VSMAC_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;


	VSMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;


	VSMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;


	VSMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;
	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::CreatePeriodicKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum)
{
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	VSMAC_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;
	m_pKnotVector = VS_NEW VSREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	for(unsigned int i  = 0; i < m_uiKnotNum ; i++)
	{
		
		m_pKnotVector[i] =((VSREAL)1.0 * (i - m_uiDegree)) / (m_uiControlPointNum + 1 - m_uiDegree);
		
	}

	VSMAC_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;
	

	VSMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;
	
	VSMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;
	

	VSMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;
	
	return 1;
}
/*----------------------------------------------------------------*/
int VSB_SplineCurve3::GetKey(VSREAL t)const
{
	if( t > 1.0f || t < 0.0f)
		return VSMAX_INTEGER;
	unsigned int i;
	for(i = 0 ; i < m_uiKnotNum ; i++)
	{
		if(t < m_pKnotVector[i])
			break;
	
	}
	return (i - 1);

}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::GetFuncValue(VSREAL t,int &i,unsigned int uiOrder)
{
	if(!m_pBasis)
		return 0;
	i = GetKey(t);
	if(i == VSMAX_INTEGER)
		return 0;
	VSMemset(m_pBasis,0,sizeof(VSREAL) * m_uiBasisNum);
	VSMemset(m_pBasisFD,0,sizeof(VSREAL) * m_uiBasisNum);
	VSMemset(m_pBasisSD,0,sizeof(VSREAL) * m_uiBasisNum);
	VSMemset(m_pBasisTD,0,sizeof(VSREAL) * m_uiBasisNum);
	SetBasisValue(0,i,1.0f);

	
	
	VSREAL fN0 = t - m_pKnotVector[i];
	VSREAL fN1 = m_pKnotVector[i + 1] - t;
	VSREAL fInvD0, fInvD1;
	unsigned int j;
	for (j = 1; j <= m_uiDegree; j++)
	{
		fInvD0 = ((VSREAL)1.0) / (m_pKnotVector[i + j] - m_pKnotVector[i]);
		fInvD1 = ((VSREAL)1.0) / (m_pKnotVector[i + 1] - m_pKnotVector[i - j + 1]);

		SetBasisValue(j,i,fN0 * GetBasisValue(j - 1,i) * fInvD0);
		SetBasisValue(j,i - j,fN1 * GetBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetBasisFDValue(j,i,(fN0 * GetBasisFDValue(j - 1,i) + GetBasisFDValue(j - 1,i)) * fInvD0);
			SetBasisFDValue(j,i - j, (fN1*GetBasisFDValue(j - 1,i - j + 1)-GetBasisFDValue(j - 1,i - j + 1)) * fInvD1);

			if (uiOrder >= 2)
			{
				SetBasisSDValue(j,i,(fN0 * GetBasisSDValue(j - 1,i) + ((VSREAL)2.0) * GetBasisSDValue(j - 1,i)) * fInvD0);
				SetBasisSDValue(j,i - j ,(fN1 * GetBasisSDValue(j - 1,i - j + 1) - ((VSREAL)2.0) * GetBasisSDValue(j - 1,i - j + 1)) * fInvD1);

				if (uiOrder >= 3)
				{
					SetBasisTDValue(j,i,(fN0 * GetBasisTDValue(j - 1,i) + ((VSREAL)3.0) * GetBasisTDValue(j - 1 , i)) * fInvD0);
					SetBasisTDValue(j,i - j,(fN1*GetBasisTDValue(j-1,i-j+1) -((VSREAL)3.0)*GetBasisTDValue(j-1,i-j+1))*fInvD1);
				}
			}
		}
	}

	for (j = 2; j <= m_uiDegree; j++)
	{
		for (int k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pKnotVector[k];
			fN1 = m_pKnotVector[k + j + 1]-t;
			fInvD0 = ((VSREAL)1.0) / (m_pKnotVector[k + j]  - m_pKnotVector[k]);
			fInvD1 = ((VSREAL)1.0) / (m_pKnotVector[k + j + 1] - m_pKnotVector[k +1 ]);

			SetBasisValue(j,k ,fN0 * GetBasisValue(j - 1,k) * fInvD0 + fN1 * GetBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetBasisFDValue(j,k,(fN0 * GetBasisFDValue(j-1,k) + GetBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetBasisFDValue(j-1,k+1) - GetBasisFDValue(j-1,k+1)) *
					fInvD1);

				if (uiOrder >= 2)
				{
					SetBasisSDValue(j,k,(fN0*GetBasisSDValue(j - 1,k) +
						((VSREAL)2.0)*GetBasisSDValue(j - 1,k))*fInvD0 +
						(fN1*GetBasisSDValue(j - 1,k + 1)- ((VSREAL)2.0)*
						GetBasisSDValue(j - 1,k + 1))*fInvD1);

					if (uiOrder >= 3)
					{
						SetBasisTDValue(j,k, (fN0*GetBasisTDValue(j-1,k) +
							((VSREAL)3.0)*GetBasisTDValue(j-1,k))*fInvD0 +
							(fN1*GetBasisTDValue(j-1,k+1) - ((VSREAL)3.0)*
							GetBasisTDValue(j-1,k+1))*fInvD1);
					}
				}
			}
		}
	}
	
	return 1;
}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineCurve3::GetPoint(VSREAL t)
{
	int i;
	if(GetFuncValue(t,i,0))
	{
		VSVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisValue(m_uiDegree,j);
		
		}
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineCurve3::GetFirstDerivative(VSREAL t)
{
	int i;
	if(GetFuncValue(t,i,1))
	{
		VSVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisFDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineCurve3::GetSecondDerivative(VSREAL t)
{
	int i;
	if(GetFuncValue(t,i,2))
	{
		VSVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisSDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineCurve3::GetThirdDerivative(VSREAL t)
{
	int i;
	if(GetFuncValue(t,i,3))
	{
		VSVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisTDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
bool VSB_SplineCurve3::Copy(const VSB_SplineCurve3 & B_SplineCurve3)
{
	if(!VSControlCurve3::Copy(B_SplineCurve3))
		return 0;

	if(!B_SplineCurve3.m_pBasis || !B_SplineCurve3.m_pBasisFD
		||!B_SplineCurve3.m_pBasisSD ||!B_SplineCurve3.m_pBasisTD||
		!B_SplineCurve3.m_pKnotVector ||!B_SplineCurve3.m_uiBasisNum
		||!B_SplineCurve3.m_uiDegree||!B_SplineCurve3.m_uiKnotNum)
		return 0;

	m_uiDegree = B_SplineCurve3.m_uiDegree;
	m_uiControlPointNum = B_SplineCurve3.m_uiControlPointNum;
	m_uiKnotNum = B_SplineCurve3.m_uiKnotNum;

	VSMAC_DELETEA(m_pKnotVector);
	VSMAC_DELETEA(m_pControlPoint);
	VSMAC_DELETEA(m_pBasis);
	VSMAC_DELETEA(m_pBasisFD);
	VSMAC_DELETEA(m_pBasisSD);
	VSMAC_DELETEA(m_pBasisTD);
	
	m_pKnotVector = VS_NEW VSREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	VSMemcpy(m_pKnotVector,B_SplineCurve3.m_pKnotVector,sizeof(VSREAL) * (m_uiKnotNum));

	VSMAC_DELETEA(m_pBasis);
	m_uiBasisNum = B_SplineCurve3.m_uiBasisNum;
	m_pBasis = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;


	VSMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;


	VSMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;


	VSMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = VS_NEW VSREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;

	return 1;

}