#include "VSB_SplineSurface3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSB_SplineSurface3::VSB_SplineSurface3()
{
	m_pUKnotVector = NULL;
	m_uiUDegree = 0;
	m_uiUKnotNum = 0;
	m_pUBasis = NULL;
	m_pUBasisFD = NULL;
	m_uiUBasisNum = 0;


	m_pVKnotVector = NULL;
	m_uiVDegree = 0;
	m_uiVKnotNum = 0;
	m_pVBasis = NULL;
	m_pVBasisFD = NULL;
	m_uiVBasisNum = 0;
	
}
/*----------------------------------------------------------------*/
VSB_SplineSurface3::~VSB_SplineSurface3()
{
	VSMAC_DELETEA(m_pUKnotVector);
	m_uiUDegree = 0;
	m_uiUKnotNum = 0;
	VSMAC_DELETEA(m_pUBasis);
	VSMAC_DELETEA(m_pUBasisFD);
	m_uiUBasisNum = 0;


	VSMAC_DELETEA(m_pVKnotVector);
	m_uiVDegree = 0;
	m_uiVKnotNum = 0;
	VSMAC_DELETEA(m_pVBasis);
	VSMAC_DELETEA(m_pVBasisFD);
	m_uiVBasisNum = 0;

}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::Set(const VSVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn)
{
	if(!pControlPoint || !uiRow || !uiColumn)
		return 0 ;
	VSMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = uiRow * uiColumn;
	m_uiRow = uiRow;
	m_uiColumn = uiColumn;
	m_pControlPoint = VS_NEW VSVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	VSMemcpy(m_pControlPoint,pControlPoint,m_uiControlPointNum * sizeof(VSVector3));
	return 1;

}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::SetUKnotVector(const VSREAL * pUKnotVector,unsigned int uiUDegree,unsigned int uiRow)
{
	if(!pUKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < uiUDegree + uiRow; i++)
	{
		if(pUKnotVector[i] > pUKnotVector[i+1])
			return 0;
	}
	m_uiUDegree = uiUDegree;
	m_uiRow = uiRow;
	VSMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiUDegree;
	m_pUKnotVector = VS_NEW VSREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
	VSMemcpy(m_pUKnotVector,pUKnotVector,sizeof(VSREAL) * m_uiUKnotNum);

	VSMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiUDegree + 1) * m_uiUKnotNum;
	m_pUBasis = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	VSMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;



	return 1;

}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::SetVKnotVector(const VSREAL * pVKnotVector,unsigned int uiVDegree,unsigned int uiColumn)
{
	if(!pVKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < uiVDegree + uiColumn; i++)
	{
		if(pVKnotVector[i] > pVKnotVector[i+1])
			return 0;
	}
	m_uiVDegree = uiVDegree;
	m_uiColumn= uiColumn;
	VSMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiVDegree;
	m_pVKnotVector = VS_NEW VSREAL[m_uiVKnotNum];
	VSMemcpy(m_pVKnotVector,pVKnotVector,sizeof(VSREAL) * (m_uiVKnotNum));

	VSMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiVDegree + 1) * m_uiVKnotNum;
	m_pVBasis = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	VSMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;



	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::CreateOpenUniformUKnotVector(unsigned int uiDegree,unsigned int uiRow)
{
	m_uiUDegree = uiDegree;
	m_uiRow = uiRow;
	VSMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiDegree;

	m_pUKnotVector = VS_NEW VSREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
	for(unsigned int i = 0; i < m_uiUKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiUDegree)
		{
			m_pUKnotVector[i] = 0;

		}
		else if( i >= m_uiUDegree + 1 && i <= m_uiRow)
		{
			m_pUKnotVector[i] = ((VSREAL)1.0 * (i - m_uiUDegree)) / (m_uiRow + 1 - m_uiUDegree);

		}
		else
		{
			m_pUKnotVector[i] = 1;
		}
	}

	VSMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiDegree + 1) * m_uiUKnotNum;
	m_pUBasis = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	VSMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;


	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::CreatePeriodicUKnotVector(unsigned int uiDegree,unsigned int uiRow)
{
	m_uiUDegree = uiDegree;
	m_uiRow = uiRow;
	VSMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiDegree;
	m_pUKnotVector = VS_NEW VSREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < m_uiUKnotNum ; i++)
	{

		m_pUKnotVector[i] = ((VSREAL)1.0 * (i - m_uiUDegree)) / (m_uiRow + 1 - m_uiUDegree);

	}

	VSMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiDegree + 1) * m_uiUKnotNum;
	m_pUBasis = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	VSMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;

	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::CreateOpenUniformVKnotVector(unsigned int uiDegree,unsigned int uiColumn)
{
	m_uiVDegree = uiDegree;
	m_uiColumn = uiColumn;
	VSMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiDegree;

	m_pVKnotVector = VS_NEW VSREAL[m_uiVKnotNum];
	if(!m_pVKnotVector)
		return 0;
	for(unsigned int i  = 0 ; i < m_uiVKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiVDegree)
		{
			m_pVKnotVector[i] = 0;

		}
		else if( i >= m_uiVDegree + 1 && i <= m_uiColumn)
		{
			m_pVKnotVector[i] = ((VSREAL)1.0 * (i - m_uiVDegree)) / (m_uiColumn + 1 - m_uiVDegree);

		}
		else
		{
			m_pVKnotVector[i] = 1;
		}
	}

	VSMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiDegree + 1) * m_uiVKnotNum;
	m_pVBasis = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	VSMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;

	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::CreatePeriodicVKnotVector(unsigned int uiDegree,unsigned int uiColumn)
{
	m_uiVDegree = uiDegree;
	m_uiColumn = uiColumn;
	VSMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiDegree;
	m_pVKnotVector = VS_NEW VSREAL[m_uiVKnotNum];
	if(!m_pVKnotVector)
		return 0;
	for(unsigned int i = 0; i < m_uiVKnotNum ; i++)
	{

		m_pVKnotVector[i] = ((VSREAL)1.0 * (i - m_uiVDegree)) / (m_uiColumn + 1 - m_uiVDegree);

	}

	VSMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiDegree + 1) * m_uiVKnotNum;
	m_pVBasis = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	VSMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;


	return 1;

}
/*----------------------------------------------------------------*/
int VSB_SplineSurface3::GetUKey(VSREAL t)const
{
	if( t > 1.0f || t < 0.0f)
		return VSMAX_INTEGER;
	unsigned int i;
	for(i = 0 ; i < m_uiUKnotNum ; i++)
	{
		if(t < m_pUKnotVector[i])
			break;

	}
	return (i - 1);
}
/*----------------------------------------------------------------*/
int VSB_SplineSurface3::GetVKey(VSREAL t)const
{
	if( t > 1.0f || t < 0.0f)
		return VSMAX_INTEGER;
	unsigned int i;
	for(i = 0 ; i < m_uiVKnotNum ; i++)
	{
		if(t < m_pVKnotVector[i])
			break;

	}
	return (i - 1);
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::GetUFuncValue(VSREAL t,int &i,unsigned int uiOrder)
{
	if(!m_pUBasis)
		return 0;
	i = GetUKey(t);
	if(i == VSMAX_INTEGER)
		return 0;
	VSMemset(m_pUBasis,0,sizeof(VSREAL) * m_uiUBasisNum);
	VSMemset(m_pUBasisFD,0,sizeof(VSREAL) * m_uiUBasisNum);

	SetUBasisValue(0,i,1.0f);



	VSREAL fN0 = t - m_pUKnotVector[i];
	VSREAL fN1 = m_pUKnotVector[i + 1] - t;
	VSREAL fInvD0, fInvD1;
	unsigned int j;
	for (j = 1; j <= m_uiUDegree; j++)
	{
		fInvD0 = ((VSREAL)1.0) / (m_pUKnotVector[i + j] - m_pUKnotVector[i]);
		fInvD1 = ((VSREAL)1.0) / (m_pUKnotVector[i + 1] - m_pUKnotVector[i - j + 1]);

		SetUBasisValue(j,i,fN0 * GetUBasisValue(j - 1,i) * fInvD0);
		SetUBasisValue(j,i - j,fN1 * GetUBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetUBasisFDValue(j,i,(fN0 * GetUBasisFDValue(j - 1,i) + GetUBasisFDValue(j - 1,i)) * fInvD0);
			SetUBasisFDValue(j,i - j, (fN1*GetUBasisFDValue(j - 1,i - j + 1)-GetUBasisFDValue(j - 1,i - j + 1)) * fInvD1);

			
		}
	}

	for (j = 2; j <= m_uiUDegree; j++)
	{
		for (int k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pUKnotVector[k];
			fN1 = m_pUKnotVector[k + j + 1]-t;
			fInvD0 = ((VSREAL)1.0) / (m_pUKnotVector[k + j]  - m_pUKnotVector[k]);
			fInvD1 = ((VSREAL)1.0) / (m_pUKnotVector[k + j + 1] - m_pUKnotVector[k +1 ]);

			SetUBasisValue(j,k ,fN0 * GetUBasisValue(j - 1,k) * fInvD0 + fN1 * GetUBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetUBasisFDValue(j,k,(fN0 * GetUBasisFDValue(j-1,k) + GetUBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetUBasisFDValue(j-1,k+1) - GetUBasisFDValue(j-1,k+1)) *
					fInvD1);

				
			}
		}
	}

	return 1;
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::GetVFuncValue(VSREAL t,int &i,unsigned int uiOrder)
{

	if(!m_pVBasis)
		return 0;
	i = GetVKey(t);
	if(i == VSMAX_INTEGER)
		return 0;
	VSMemset(m_pVBasis,0,sizeof(VSREAL) * m_uiVBasisNum);
	VSMemset(m_pVBasisFD,0,sizeof(VSREAL) * m_uiVBasisNum);

	SetVBasisValue(0,i,1.0f);



	VSREAL fN0 = t - m_pVKnotVector[i];
	VSREAL fN1 = m_pVKnotVector[i + 1] - t;
	VSREAL fInvD0, fInvD1;
	unsigned int j;
	for (j = 1; j <= m_uiVDegree; j++)
	{
		fInvD0 = ((VSREAL)1.0) / (m_pVKnotVector[i + j] - m_pVKnotVector[i]);
		fInvD1 = ((VSREAL)1.0) / (m_pVKnotVector[i + 1] - m_pVKnotVector[i - j + 1]);

		SetVBasisValue(j,i,fN0 * GetVBasisValue(j - 1,i) * fInvD0);
		SetVBasisValue(j,i - j,fN1 * GetVBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetVBasisFDValue(j,i,(fN0 * GetVBasisFDValue(j - 1,i) + GetVBasisFDValue(j - 1,i)) * fInvD0);
			SetVBasisFDValue(j,i - j, (fN1*GetVBasisFDValue(j - 1,i - j + 1)-GetVBasisFDValue(j - 1,i - j + 1)) * fInvD1);


		}
	}

	for (j = 2; j <= m_uiVDegree; j++)
	{
		for (int k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pVKnotVector[k];
			fN1 = m_pVKnotVector[k + j + 1]-t;
			fInvD0 = ((VSREAL)1.0) / (m_pVKnotVector[k + j]  - m_pVKnotVector[k]);
			fInvD1 = ((VSREAL)1.0) / (m_pVKnotVector[k + j + 1] - m_pVKnotVector[k +1 ]);

			SetVBasisValue(j,k ,fN0 * GetVBasisValue(j - 1,k) * fInvD0 + fN1 * GetVBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetVBasisFDValue(j,k,(fN0 * GetVBasisFDValue(j-1,k) + GetVBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetVBasisFDValue(j-1,k+1) - GetVBasisFDValue(j-1,k+1)) *
					fInvD1);


			}
		}
	}

	return 1;
}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineSurface3::GetPoint(VSREAL U,VSREAL V)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,0) && GetUFuncValue(V,i1,0))
	{
		VSVector3 Point(0,0,0);
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			Point += GetControlPoint(j0,j1) * GetUBasisValue(m_uiUDegree,j0) * GetUBasisValue(m_uiVDegree,j1);

		}
		return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,1) && GetUFuncValue(V,i1,0))
	{
		VSVector3 Point(0,0,0);
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
			for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
			{
				Point += GetControlPoint(j0,j1) * GetUBasisFDValue(m_uiUDegree,j0) * GetUBasisValue(m_uiVDegree,j1);

			}
			return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
VSVector3 VSB_SplineSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,0) && GetUFuncValue(V,i1,1))
	{
		VSVector3 Point(0,0,0);
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
			for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
			{
				Point += GetControlPoint(j0,j1) * GetUBasisValue(m_uiUDegree,j0) * GetUBasisFDValue(m_uiVDegree,j1);

			}
			return Point;
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
bool VSB_SplineSurface3::Copy(const VSB_SplineSurface3 & B_SplineSurface3)
{
	if(!VSControlSurface3::Copy(B_SplineSurface3))
		return 0;

	if(!B_SplineSurface3.m_pUBasis || !B_SplineSurface3.m_pUKnotVector || !B_SplineSurface3.m_pUBasisFD ||
		!B_SplineSurface3.m_uiUDegree || !B_SplineSurface3.m_uiUBasisNum || !B_SplineSurface3.m_uiUKnotNum)
		return 0;
	if(!B_SplineSurface3.m_pVBasis || !B_SplineSurface3.m_pVKnotVector || !B_SplineSurface3.m_pVBasisFD ||
		!B_SplineSurface3.m_uiVDegree || !B_SplineSurface3.m_uiVBasisNum || !B_SplineSurface3.m_uiVKnotNum)
		return 0;

	m_uiUDegree = B_SplineSurface3.m_uiUDegree;
	VSMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = B_SplineSurface3.m_uiUKnotNum;
	m_pUKnotVector = VS_NEW VSREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
		VSMemcpy(m_pUKnotVector,B_SplineSurface3.m_pUKnotVector,sizeof(VSREAL) * (m_uiUKnotNum));

	VSMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = B_SplineSurface3.m_uiUBasisNum;
	m_pUBasis = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	VSMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = VS_NEW VSREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;
	


	m_uiVDegree = B_SplineSurface3.m_uiVDegree;
	VSMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = B_SplineSurface3.m_uiVKnotNum;
	m_pVKnotVector = VS_NEW VSREAL[m_uiVKnotNum];
	if(!m_pVKnotVector)
		return 0;
		VSMemcpy(m_pVKnotVector,B_SplineSurface3.m_pVKnotVector,sizeof(VSREAL) * (m_uiVKnotNum));

	VSMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = B_SplineSurface3.m_uiVBasisNum;
	m_pVBasis = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	VSMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = VS_NEW VSREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;
	return 1;
}