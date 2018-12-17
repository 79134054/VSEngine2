#include "VSCurveSet3.h"
#include "VSMemManager.h"
#include "VSMatrix3X3W.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSCurveSet3::VSCurveSet3()
{
	m_uiNum = 0;
	m_pHead = NULL;
	m_pNext = NULL;
}
/*----------------------------------------------------------------*/
VSCurveSet3::~VSCurveSet3()
{
	Clear();	

}
/*----------------------------------------------------------------*/
void VSCurveSet3::Clear()
{
	m_pNext = m_pHead;
	while(m_pNext)
	{
		ELEMENT * Temp = m_pNext;
		m_pNext = m_pNext->next;
		VSMAC_DELETE(Temp);
	}
	m_uiNum = 0;
}
/*----------------------------------------------------------------*/
bool VSCurveSet3::Set(VSCurve3 * pCurve)
{
	if(!pCurve)
		return 0;

	ELEMENT *Temp = NULL;
	Temp = VS_NEW ELEMENT;
	if(!Temp)
		return 0;

	Temp->Ptr = pCurve;
	Temp->next = NULL;

	if(!m_pHead)
	{
		m_pHead = Temp;
		m_pNext = m_pHead;

	
	}
	else
	{
		m_pNext->next = Temp;
		m_pNext = m_pNext->next;
	
	}
	m_uiNum++;
	return 1;
}
/*----------------------------------------------------------------*/
unsigned int VSCurveSet3::GetIndex(VSREAL &t)
{
	unsigned int i = 0;
	while(t > (VSREAL)1.0)
	{
		t = t - (VSREAL)1.0;
		i++;
	}
	return i;
}
/*----------------------------------------------------------------*/
bool VSCurveSet3::GetElement(unsigned int Index,VSCurve3 * &Element)
{
	if(Index > m_uiNum)
		return 0;
	else
	{
		m_pNext = m_pHead;
		unsigned int temp = 0;
		while(temp != Index)
		{
			m_pNext = m_pNext->next;
			temp++;
		}
	
	}
	Element = m_pNext->Ptr;
	return 1;
}
/*----------------------------------------------------------------*/
bool VSCurveSet3::GetElement(VSREAL& t,VSCurve3 * &Element)
{
	if(t > m_uiNum * 1.0)
		return 0;
	else
	{
		m_pNext = m_pHead;
	
		while(t > (VSREAL)1.0)
		{
			m_pNext = m_pNext->next;
			t = t - (VSREAL)1.0	;
		}

	}
	Element = m_pNext->Ptr;
	return 1;
}
/*----------------------------------------------------------------*/
bool	VSCurveSet3::GetFixedUpBNT(VSREAL t ,VSVector3 & B,VSVector3 & N,VSVector3 &T)
{
	VSCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return 0;
	else
	{
		if(!Element)
			return 0;
		else
		{
			
			Element->GetFixedUpBNT(t,B,N,T);
			
			
			return 1;
		}
	
	}

}
/*----------------------------------------------------------------*/
bool VSCurveSet3::GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T)
{
	VSCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return 0;
	else
	{
		if(!Element)
			return 0;
		else
		{

			Element->GetFrenetFrameBNT(t,B,N,T);


			return 1;
		}

	}
}
/*----------------------------------------------------------------*/
VSVector3	VSCurveSet3::GetFirstDerivative(VSREAL t)
{
	VSCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	else
	{
		if(!Element)
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
		else
		{

			return Element->GetFirstDerivative(t);

		
		}

	}

}
/*----------------------------------------------------------------*/
VSVector3	VSCurveSet3::GetSecondDerivative(VSREAL t)
{
	VSCurve3 *Element = NULL;

	if(!GetElement(t,Element))
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	else
	{
		if(!Element)
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
		else
		{

			return Element->GetSecondDerivative(t);

			
			
		}

	}
}
/*----------------------------------------------------------------*/
VSVector3	VSCurveSet3::GetThirdDerivative(VSREAL t)
{
	VSCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	else
	{
		if(!Element)
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
		else
		{

			return Element->GetThirdDerivative(t);

			
		}

	}

}
/*----------------------------------------------------------------*/
VSREAL		VSCurveSet3::GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations)
{
	if (t1 > t2)
		return VSMAX_REAL;
	if(t2 > m_uiNum * 1.0)
		return VSMAX_REAL;

	unsigned int index1 = GetIndex(t1);
	unsigned int index2 = GetIndex(t2);

	t1 = t1 - index1 * (VSREAL)1.0;
	t2 = t2 - index2 * (VSREAL)1.0;
	if(index1 == index2)
	{
		VSCurve3 *Element = NULL;

		GetElement(index1,Element);
	
		return Element->GetLength(t1,t2,uiIterations);
		
		
	}
	else
	{
		VSCurve3 *Element = NULL;
		VSREAL Len = 0.0;
		GetElement(index1,Element);

		Len +=Element->GetLength(t1,1.0,uiIterations);

		index1++;
		while(index1 != index2)
		{
			
			GetElement(index1,Element);
		
			Len +=Element->GetTotalLength(uiIterations);
			
			index1++;
		}
		GetElement(index1,Element);
	
		Len +=Element->GetLength(0.0,t2,uiIterations);
	
		return Len;
	}


	
	
	
}
/*----------------------------------------------------------------*/
VSVector3	VSCurveSet3::GetPoint(VSREAL t)
{
	VSCurve3 *Element = NULL;
	unsigned int Type = 0;
	if(!GetElement(t,Element))
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	else
	{
		if(!Element)
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
		else
		{
			
			return Element->GetPoint(t);

			
		}

	}

}
/*----------------------------------------------------------------*/
VSREAL		VSCurveSet3::GetTotalLength(unsigned int uiIterations)
{
	VSREAL Len = 0.0;
	m_pNext = m_pHead;
	while(m_pNext)
	{
		

		Len += m_pNext->Ptr->GetTotalLength(uiIterations);

		
		m_pNext = m_pNext->next;
		
	}
	return Len;

}
/*----------------------------------------------------------------*/