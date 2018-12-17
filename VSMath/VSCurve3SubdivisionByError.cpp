#include "VSCurve3SubdivisionByError.h"
#include "VSSegment3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSCurve3SubdivisionByError::VSCurve3SubdivisionByError(VSREAL t1,VSREAL t2,unsigned int Level, VSREAL error)
{

	m_pPointHead = NULL;
	m_uiPointNum = 0;
	m_uiLevel = Level;
	m_fError = error;
	m_ft1 = t1;
	m_ft2 = t2;
}
/*----------------------------------------------------------------*/
VSCurve3SubdivisionByError::~VSCurve3SubdivisionByError()
{
	m_pNext = m_pPointHead;
	while(m_pNext)
	{
		POINTLIST * Temp = m_pNext;
		m_pNext = m_pNext->next;
		VSMAC_DELETE(Temp);
	}
}
/*----------------------------------------------------------------*/
void VSCurve3SubdivisionByError::Clear()
{

	m_pNext = m_pPointHead;
	while(m_pNext)
	{
		POINTLIST * Temp = m_pNext;
		m_pNext = m_pNext->next;
		VSMAC_DELETE(Temp);
	}
	m_pPointHead = NULL;
	m_pNext = NULL;
}
/*----------------------------------------------------------------*/
bool VSCurve3SubdivisionByError::Subdivision()
{
	if(!m_uiLevel || !m_pCurve)
		return 0;
	if(m_ft2 < m_ft1)
		return 0;

	Clear();


	m_uiPointNum = 0;
	VSVector3 Point1 = m_pCurve->GetPoint(m_ft1);
	VSVector3 Point2 = m_pCurve->GetPoint(m_ft2);

	m_pPointHead = VS_NEW POINTLIST;
	if(!m_pPointHead)
		return 0;
	m_pPointHead->Point = Point1;
	m_pPointHead->next = NULL;
	m_uiPointNum++;
	m_pNext = m_pPointHead;

	if(!SubdivisionBisect(m_uiLevel,m_fError,m_ft1,m_ft2,Point1,Point2))
	{

		Clear();
		return 0;
	}
	if(!m_uiPointNum)
		return 0;
	VSMAC_DELETEA(m_pPoint);
	m_pPoint = VS_NEW VSVector3[m_uiPointNum];
	if(!m_pPoint)
		return 0;
	

	VSMAC_DELETEA(m_pT);
	m_pT = VS_NEW VSREAL[m_uiPointNum]; 

	if(!m_pT)
		return 0;

	unsigned int i = 0;
	m_pNext = m_pPointHead;
	while(m_pNext)
	{
		m_pPoint[i] = m_pNext->Point;
		m_pT[i] = m_pNext->t;
		POINTLIST * Temp = m_pNext;
		m_pNext = m_pNext->next;
		i++;
		//VSMAC_DELETE(Temp);
	}
	Clear();
	return 1;
}
/*----------------------------------------------------------------*/
bool VSCurve3SubdivisionByError::SubdivisionBisect(unsigned int Level, VSREAL error,VSREAL t1,VSREAL t2,
												const VSVector3 &Point1,const VSVector3 &Point2)
{
	VSREAL tm = (t1 + t2) / (VSREAL)2.0;
	if(Level > 0)
	{
		
		VSVector3 Point = m_pCurve->GetPoint(tm);
		VSSegment3 s;
		s.Set(Point1,Point2);
		VSREAL Length = s.GetLen();
		VSREAL t;
		VSREAL Dis = s.SquaredDistance(Point,t);
		VSREAL TempError = Dis / Length;
		if(TempError > error)
		{
			if(!SubdivisionBisect(Level - 1,error,t1,tm,Point1,Point))
				return 0;
			if(!SubdivisionBisect(Level - 1,error,tm,t2,Point,Point2))
				return 0;
			return 1;
		}
	
	}
	
	POINTLIST * Temp = NULL;
	Temp = VS_NEW POINTLIST;
	if(!Temp)
		return 0;
	Temp->Point = Point2;
	Temp->t = tm;
	Temp->next = NULL;
	m_pNext->next = Temp;
	m_pNext = Temp;
	m_uiPointNum++;
	return 1;


}
/*----------------------------------------------------------------*/