#include "VSPolygon3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
using namespace VSEngine2;
int VSPolygon3::RelationWith(const VSLine3 &Line, bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3],
							VSREAL &fLineParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		int iFlag = TriangleTemp.RelationWith(Line,bCull,fTriangleParameter,fLineParameter);
		if(iFlag == VSINTERSECT)
			return VSINTERSECT;

	}
	
	return VSNOINTERSECT;


}

int VSPolygon3::RelationWith(const VSRay3 &Ray, bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3],
								VSREAL &fRayParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		int iFlag = TriangleTemp.RelationWith(Ray,bCull,fTriangleParameter,fRayParameter);
		iIndexTriangle = i;
		if(iFlag == VSINTERSECT)
			return VSINTERSECT;

	}

	return VSNOINTERSECT;
}

int VSPolygon3::RelationWith(const VSSegment3 &Segment, bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3],
								VSREAL &fSegmentParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		int iFlag = TriangleTemp.RelationWith(Segment,bCull,fTriangleParameter,fSegmentParameter);
		iIndexTriangle = i;
		if(iFlag == VSINTERSECT)
			return VSINTERSECT;

	}

	return VSNOINTERSECT;

}
int VSPolygon3::RelationWith(const VSPlane3 & Plane)const
{
	VSSegment3 Segment;
	int iBackNum = 0 , iFrontNum = 0 ;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{

		int iFlag = m_pPoint[i].RelationWith(Plane);
		if(iFlag == VSBACK)
			iBackNum++;
		else if(iFlag == VSFRONT)
			iFrontNum++;

		if(iBackNum > 0 && iFrontNum >0)
			return VSINTERSECT;
	
	}
	if(iBackNum == 0 && iFrontNum == 0)
		return VSON;
	else if(iBackNum == 0)
		return VSFRONT;
	else if(iFrontNum == 0)
		return VSBACK;
	else
		return VSINTERSECT;

}
int VSPolygon3::RelationWith(const VSPlane3 & Plane,VSSegment3 & Segment)const
{
	VSSegment3 SegmentTemp;
	int InNum = 0;
	int OnNum = 0, BackNum = 0,FrontNum  = 0;
	VSVector3 Orig,End;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		SegmentTemp.Set(m_pPoint[i],m_pPoint[j]);
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
int VSPolygon3::RelationWith(const VSTriangle3 & Triangle)const
{
	VSSegment3 Segment;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
		VSREAL fSegmentParameter;
		VSREAL TriangleParameter[3];
		if(Segment.RelationWith(Triangle,0,fSegmentParameter,TriangleParameter) == VSINTERSECT)
			return VSINTERSECT;

	}
	VSVector3 Point[3];
	Triangle.GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		VSREAL fSegmentParameter;
		VSREAL TriangleParameter[3];
		int Index;
		if(Segment.RelationWith(*this,fSegmentParameter,0,Index,TriangleParameter) == VSINTERSECT)
			return VSINTERSECT;

	}
	return VSNOINTERSECT;

}
int VSPolygon3::RelationWith(const VSTriangle3 & Triangle,VSSegment3 & Segment)const
{
	VSSegment3 SegmentTemp;
	int InNum = 0;
	VSVector3 Orig,End;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
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
	VSVector3 Point[3];
	Triangle.GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
		VSREAL fSegmentParameter;
		VSREAL fTriangleParameter[3];
		int Index;
		int iFlag = SegmentTemp.RelationWith(*this,fSegmentParameter,0,Index,fTriangleParameter);
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
int VSPolygon3::RelationWith(const VSRectangle3 & Rectangle)const
{
	VSSegment3 Segment;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
		VSREAL fSegmentParameter;
		VSREAL TriangleParameter[2];
		if(Segment.RelationWith(Rectangle,0,fSegmentParameter,TriangleParameter) == VSINTERSECT)
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
		VSREAL TriangleParameter[3];
		int Index;
		if(Segment.RelationWith(*this,fSegmentParameter,0,Index,TriangleParameter) == VSINTERSECT)
			return VSINTERSECT;

	}
	return VSNOINTERSECT;

}
int VSPolygon3::RelationWith(const VSRectangle3 & Rectangle,VSSegment3 & Segment)const
{
	VSSegment3 SegmentTemp;
	int InNum = 0;
	VSVector3 Orig,End;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
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
		VSREAL fSegmentParameter;
		VSREAL fTriangleParameter[3];
		int Index;
		int iFlag = SegmentTemp.RelationWith(*this,fSegmentParameter,0,Index,fTriangleParameter);
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
int VSPolygon3::RelationWith(const VSAABB3 &AABB)const
{
	VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(AABB) != VSINTERSECT)
		return VSNOINTERSECT;
	int InNum = 0,OutNum = 0;

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int iFlag = m_pPoint[i].RelationWith(AABB);
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

}


int VSPolygon3::RelationWith(const VSOBB3 &OBB)const
{
	VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != VSINTERSECT)
		return VSNOINTERSECT;
	int InNum = 0,OutNum = 0;

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int iFlag = m_pPoint[i].RelationWith(OBB);
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
}


int VSPolygon3::RelationWith(const VSSphere3 &Sphere)const
{
	VSPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != VSINTERSECT)
		return VSNOINTERSECT;
	int InNum = 0,OutNum = 0;

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int iFlag = m_pPoint[i].RelationWith(Sphere);
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

}