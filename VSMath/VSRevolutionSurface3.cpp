#include "VSRevolutionSurface3.h"
using namespace VSEngine2;
VSRevolutionSurface3::VSRevolutionSurface3()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
VSRevolutionSurface3::~VSRevolutionSurface3()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
bool VSRevolutionSurface3::SetCurve(VSCurve3 * pCurve,unsigned int Axis,unsigned int uiPlane)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	m_uiAxis = Axis;
	m_uiPlane = uiPlane;
	return 1;
}
/*----------------------------------------------------------------*/
VSVector3 VSRevolutionSurface3::GetPoint(VSREAL U,VSREAL V)
{
	VSVector3 Temp = m_pCurve->GetPoint(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return VSVector3(Temp.x,Temp.y * COS(VS2PI * V),Temp.y *SIN(VS2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return VSVector3(Temp.x,Temp.z * COS(VS2PI * V),Temp.z *SIN(VS2PI * V));
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return VSVector3(Temp.x * COS(VS2PI * V),Temp.y ,Temp.x *SIN(VS2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return VSVector3(Temp.z * COS(VS2PI * V),Temp.y ,Temp.z *SIN(VS2PI * V));
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return VSVector3(Temp.x * COS(VS2PI * V),Temp.x * SIN(VS2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return VSVector3(Temp.y * COS(VS2PI * V),Temp.y * SIN(VS2PI * V),Temp.z );
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
VSVector3 VSRevolutionSurface3::GetUPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 Temp = m_pCurve->GetFirstDerivative(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return VSVector3(Temp.x,Temp.y * COS(VS2PI * V),Temp.y *SIN(VS2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return VSVector3(Temp.x,Temp.z * COS(VS2PI * V),Temp.z *SIN(VS2PI * V));
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return VSVector3(Temp.x * COS(VS2PI * V),Temp.y ,Temp.x *SIN(VS2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return VSVector3(Temp.z * COS(VS2PI * V),Temp.y ,Temp.z *SIN(VS2PI * V));
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return VSVector3(Temp.x * COS(VS2PI * V),Temp.x * SIN(VS2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return VSVector3(Temp.y * COS(VS2PI * V),Temp.y * SIN(VS2PI * V),Temp.z );
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
VSVector3 VSRevolutionSurface3::GetVPartialDerivative(VSREAL U,VSREAL V)
{
	VSVector3 Temp = m_pCurve->GetPoint(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return VSVector3(Temp.x,Temp.y * -COS(VS2PI * V),Temp.y *COS(VS2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return VSVector3(Temp.x,Temp.z * -COS(VS2PI * V),Temp.z *COS(VS2PI * V));
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return VSVector3(Temp.x * -COS(VS2PI * V),Temp.y ,Temp.x *COS(VS2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return VSVector3(Temp.z * -COS(VS2PI * V),Temp.y ,Temp.z *COS(VS2PI * V));
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return VSVector3(Temp.x * -COS(VS2PI * V),Temp.x * COS(VS2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return VSVector3(Temp.y * -COS(VS2PI * V),Temp.y * COS(VS2PI * V),Temp.z );
		}
		else
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}
	else
	{
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	}

}
/*----------------------------------------------------------------*/