#include "VSFunction1.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSFunction1::VSFunction1()
{
	m_pNext = NULL;
	m_pHead = NULL;
	m_pRoot = NULL;
	m_uiRootNum = 0;
}
/*----------------------------------------------------------------*/
VSFunction1::~VSFunction1()
{
	if(m_pHead)
	{
	
		m_pNext = m_pHead;
		while(m_pNext)
		{
			INTERVAL *Temp = m_pNext;
			m_pNext = m_pNext->next;
			VSMAC_DELETE(Temp);
		
		}
		m_pNext = NULL;
		m_pHead = NULL;	
	}
	VSMAC_DELETEA(m_pRoot);
}
/*----------------------------------------------------------------*/
bool VSFunction1::ISHaveRoot(VSREAL t1 , VSREAL t2,unsigned int uiIterator)
{
	if(m_pHead)
	{

		m_pNext = m_pHead;
		while(m_pNext)
		{
			INTERVAL *Temp = m_pNext;
			m_pNext = m_pNext->next;
			VSMAC_DELETE(Temp);

		}
		m_pNext = NULL;
		m_pHead = NULL;	
	}
	m_uiRootNum = 0;

	VSREAL Temp = ABS(t2 - t1) / uiIterator;
	VSREAL k1 = t1;
	VSREAL k2;
	VSREAL Value1 ,Value2;
	Value1 = Value2 = GetValue(k1);
	for(unsigned int i = 0 ; i < uiIterator ; i++)
	{
		Value1 = Value2;
		k2 = k1 + Temp;
		Value2 = GetValue(k2);
		if(Value1 * Value2 < 0)
		{
			m_uiRootNum++;
			if(m_pHead == NULL)
			{
				m_pHead = VS_NEW INTERVAL;
				if(!m_pHead)
					return 0;
				m_pNext = m_pHead;
				m_pNext->t1 = k1;
				m_pNext->t2 = k2;
				m_pNext->value1 = Value1;
				m_pNext->value2 = Value2;
				m_pNext->next = NULL;
				
			}
			else
			{
				m_pNext->next = VS_NEW INTERVAL;
				if(!m_pNext->next)
					return 0;
				m_pNext = m_pNext->next;
				m_pNext->t1 = k1;
				m_pNext->t2 = k2;
				m_pNext->value1 = Value1;
				m_pNext->value2 = Value2;
				m_pNext->next = NULL;
				
			}
		}
		k1 = k2;
	
	}
	if(m_pHead)
	{
		VSMAC_DELETEA(m_pRoot);
		m_pRoot = VS_NEW VSREAL[m_uiRootNum];
		if(!m_pRoot)
			return 0;
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
void VSFunction1::Root(VSREAL t1 ,VSREAL t2,VSREAL Value1,VSREAL Value2,unsigned int uiLevel)
{
	
	VSREAL tm = (t2 + t1) / (VSREAL)2.0;

	VSREAL Valuem = GetValue(tm);
	if(uiLevel == 0)
	{
		m_pRoot[m_uiIndex] = tm;
		m_uiIndex++;
		return ;
	}
	if(Value1 * Valuem < 0.0)
	{
		Root(t1 ,tm,Value1,Valuem,uiLevel - 1);
	}
	else
	{
		Root(tm ,t2,Valuem,Value2,uiLevel - 1);
	}

}
/*----------------------------------------------------------------*/
bool VSFunction1::ComputeRoot(VSREAL t1 , VSREAL t2,unsigned int uiLevel)
{
	if(!m_pHead)
		return 0;
	m_uiIndex = 0;

	m_pNext = m_pHead;
	while(m_pNext)
	{
		Root(m_pNext->t1 ,m_pNext->t2,m_pNext->value1,m_pNext->value2,uiLevel);
		m_pNext = m_pNext->next;
		

	}
	return 1;

}
/*----------------------------------------------------------------*/
void VSFunction1::GetExtremumValue(VSREAL t1 , VSREAL t2,VSREAL & Min,VSREAL &Max,unsigned int uiLevel)
{
	VSREAL value1 = GetValue(t1);
	VSREAL value2 = GetValue(t2);
	ExtremumValue(t1 ,t2,value1,value2,Min,Max,uiLevel);

}
/*----------------------------------------------------------------*/
void VSFunction1::ExtremumValue(VSREAL t1 , VSREAL t2,VSREAL Value1,VSREAL Value2,
								VSREAL & Min,VSREAL &Max,unsigned int uiLevel)
{

	VSREAL tm = (t1 + t2) / (VSREAL)2.0;
	VSREAL valuem = GetValue(tm);
	
	
	if(uiLevel == 0)
	{
		VSREAL min,max;
		min = Value1;
		max = Value1;
		if(min > valuem)
		{
			min = valuem;
		}
		if(min > Value2)
		{
			min = Value2;
		}
		if(max < valuem)
		{
			max = valuem;
		}
		if(max < Value2)
		{
			max = Value2;
		}
		Min = min;
		Max = max;
	}
	else
	{
		VSREAL min1,max1;
		VSREAL min2,max2;
		ExtremumValue(t1,tm,Value1,valuem,min1,max1,uiLevel - 1);
		ExtremumValue(tm,t2,valuem,Value2,min2,max2,uiLevel - 1);

		if(min1 < min2)
		{
			Min = min1;
		}
		else
		{
			Min = min2;
		}
		if(max1 > max2)
		{
		
			Max = max1;		
		}
		else
		{
		
			Max = max2;
		}
	}

}
/*----------------------------------------------------------------*/
VSREAL VSFunction1::Integration(VSREAL t1 , VSREAL t2,unsigned int uiIterator)
{
	VSREAL tTemp = (t2 - t1) / uiIterator;
	VSREAL result = 0.0;
	VSREAL t1Temp = t1;
	VSREAL t2Temp = t1 + t1Temp;
	VSREAL t1TempValue = GetValue(t1Temp);
	VSREAL t2TempValue = t1TempValue;
	for(unsigned int i = 0 ; i < uiIterator ; i++)
	{
		t2TempValue = GetValue(t2Temp);
		result += (VSREAL)0.5 * tTemp * (t1TempValue + t2TempValue);
		t1Temp = t2Temp;
		t2Temp += tTemp;
		t1TempValue = t2TempValue;
	
	}
	return result;
}
/*----------------------------------------------------------------*/