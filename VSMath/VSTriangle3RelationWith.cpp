#include "VSTriangle3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSLine3 &Line, bool bCull,VSREAL fTriangleParameter[3],
				 VSREAL &fLineParameter)const
{

	return Line.RelationWith(*this,bCull,fLineParameter,fTriangleParameter);
}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSRay3 & Ray, bool bCull,VSREAL fTriangleParameter[3],
							 VSREAL &fRayParameter)const
{

	return Ray.RelationWith(*this,bCull,fRayParameter,fTriangleParameter);
}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSSegment3 & Segment, bool bCull,VSREAL fTriangleParameter[3],
							 VSREAL &fSegmentParameter)const
{

	return Segment.RelationWith(*this,bCull,fSegmentParameter,fTriangleParameter);
}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSPlane3 & Plane)const
{
	int iFrontNum = 0;
	int iBackNum = 0;

	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = m_V[i].RelationWith(Plane);
		if( iFlag == VSFRONT)
		{
			iFrontNum++;
		}
		else if(iFlag == VSBACK)
		{
			iBackNum++;
		}
		if(iFrontNum > 0 && iBackNum >0)
			return VSINTERSECT;
			
			
	}
	if(iFrontNum == 0 && iBackNum == 0)
	{
		return VSON;
	}
	else if(iFrontNum == 0)
		return VSBACK;
	else if(iBackNum  == 0)
		return VSFRONT;
	else
		return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSTriangle3 & Triangle)const
{
	VSSegment3 Segment;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		VSREAL fSegmentParameter;
		VSREAL TriangleParameter[3];
		if(Segment.RelationWith(Triangle,0,fSegmentParameter,TriangleParameter) == VSINTERSECT)
			return VSINTERSECT;
	
	}
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(Triangle.m_V[i],Triangle.m_V[j]);
		VSREAL fSegmentParameter;
		VSREAL TriangleParameter[3];
		if(Segment.RelationWith(*this,0,fSegmentParameter,TriangleParameter) == VSINTERSECT)
			return VSINTERSECT;

	}
	return VSNOINTERSECT;

}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSRectangle3 & Rectangle)const
{
	VSSegment3 Segment;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		VSREAL fSegmentParameter;
		VSREAL RectangleParameter[2];
		if(Segment.RelationWith(Rectangle,0,fSegmentParameter,RectangleParameter) == VSINTERSECT)
			return VSINTERSECT;

	}
	VSVector3 Point[4];
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		VSREAL fSegmentParameter;
		VSREAL RectangleParameter[2];
		if(Segment.RelationWith(*this,0,fSegmentParameter,RectangleParameter) == VSINTERSECT)
			return VSINTERSECT;

	}
	return VSNOINTERSECT;

}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSAABB3 &AABB)const
{
	VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(AABB) != VSINTERSECT)
		return VSNOINTERSECT;
	int InNum = 0,OutNum = 0;
	VSVector3 Point[3];
	GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = Point[i].RelationWith(AABB);
		if(iFlag == VSIN || iFlag == VSON)
		{
			InNum++;

		}
		else if(iFlag == VSOUT)
		{
		
			OutNum++;
		}
		if(InNum > 0 && OutNum > 0)
			return VSINTERSECT;
	}
	if(InNum == 0)
		return VSOUT;
	if(OutNum == 0)
		return VSIN;
	else 
		return VSINTERSECT;
	/*VSSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		VSREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(AABB,uiQuantity,fNear,fFar);
		if( iFlag == VSINTERSECT)
			return VSINTERSECT;
		else if(iFlag == VSIN)
			InNum ++;


	}
	if(InNum == 3)
		return VSIN;

	return VSNOINTERSECT;*/

}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSOBB3 &OBB)const
{
	VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != VSINTERSECT)
		return VSNOINTERSECT;
	int InNum = 0,OutNum = 0;
	VSVector3 Point[3];
	GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = Point[i].RelationWith(OBB);
		if(iFlag == VSIN || iFlag == VSON)
		{
			InNum++;

		}
		else if(iFlag == VSOUT)
		{

			OutNum++;
		}
		if(InNum > 0 && OutNum > 0)
			return VSINTERSECT;
	}
	if(InNum == 0)
		return VSOUT;
	if(OutNum == 0)
		return VSIN;
	else 
		return VSINTERSECT;
	/*VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != VSINTERSECT)
		return VSNOINTERSECT;
	VSSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		VSREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(OBB,uiQuantity,fNear,fFar);
		if( iFlag == VSINTERSECT)
			return VSINTERSECT;
		else if(iFlag == VSIN)
			InNum ++;


	}
	if(InNum == 3)
		return VSIN;
	return VSNOINTERSECT;*/

}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSSphere3 &Sphere)const
{

	VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != VSINTERSECT)
		return VSNOINTERSECT;
	int InNum = 0,OutNum = 0;
	VSVector3 Point[3];
	GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = Point[i].RelationWith(Sphere);
		if(iFlag == VSIN || iFlag == VSON)
		{
			InNum++;

		}
		else if(iFlag == VSOUT)
		{

			OutNum++;
		}
		if(InNum > 0 && OutNum > 0)
			return VSINTERSECT;
	}
	if(InNum == 0)
		return VSOUT;
	if(OutNum == 0)
		return VSIN;
	else 
		return VSINTERSECT;
	/*VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != VSINTERSECT)
		return VSNOINTERSECT;
	VSSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		VSREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(Sphere,uiQuantity,fNear,fFar);
		if( iFlag == VSINTERSECT)
			return VSINTERSECT;
		else if(iFlag == VSIN)
			InNum ++;


	}
	if(InNum == 3)
		return VSIN;
	return VSNOINTERSECT;*/
	
}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSPlane3 & Plane,VSSegment3 & Segment)const
{
	VSSegment3 SegmentTemp;
	int InNum = 0;
	int OnNum = 0, BackNum = 0,FrontNum  = 0;
	VSVector3 Orig,End;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(m_V[i],m_V[j]);
		VSREAL t;
		int iFlag = SegmentTemp.RelationWith(Plane,0,t);
		if( iFlag == VSINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(t);
				
			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(t);
				Segment.Set(Orig,End);	
				return VSINTERSECT;

			}
		}
		else if(iFlag == VSON)
		{
			OnNum++;

		}
		else if(iFlag == VSBACK)
		{
			BackNum++;
		
		}
		else if(iFlag == VSFRONT)
		{
			FrontNum++;
		}

	}
	if(OnNum == 3)
		return VSON;
	if(BackNum == 3)
		return VSBACK;
	if(FrontNum == 3)
		return VSFRONT;
	//not control all path
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSTriangle3 & Triangle,VSSegment3 & Segment)const
{
	VSSegment3 SegmentTemp;
	int InNum = 0;
	VSVector3 Orig,End;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(m_V[i],m_V[j]);
		VSREAL fTriangleParameter[3];
		VSREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(Triangle,0,fSegmentParameter,fTriangleParameter);
		if( iFlag == VSINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return VSINTERSECT;
			}
		}
		

	}

	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(Triangle.m_V[i],Triangle.m_V[j]);
		VSREAL fTriangleParameter[3];
		VSREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(*this,0,fSegmentParameter,fTriangleParameter);
		if( iFlag == VSINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return VSINTERSECT;
			}
		}


	}
	return VSNOINTERSECT;

}
/*----------------------------------------------------------------*/
int VSTriangle3::RelationWith(const VSRectangle3 & Rectangle,VSSegment3 & Segment)const
{
	VSSegment3 SegmentTemp;
	int InNum = 0;
	VSVector3 Orig,End;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(m_V[i],m_V[j]);
		VSREAL fTriangleParameter[2];
		VSREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(Rectangle,0,fSegmentParameter,fTriangleParameter);
		if( iFlag == VSINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return VSINTERSECT;
			}
		}


	}
	
	VSVector3 Point[4];
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
		VSREAL fTriangleParameter[3];
		VSREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(*this,0,fSegmentParameter,fTriangleParameter);
		if( iFlag == VSINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return VSINTERSECT;
			}
		}


	}
	return VSNOINTERSECT;
}