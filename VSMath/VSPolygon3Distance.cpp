#include "VSPolygon3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
using namespace VSEngine2;
VSREAL VSPolygon3::SquaredDistance(const VSVector3 & Point,int& IndexTriangle,VSREAL fTriangleParameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;
	
	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fParaTemp[3];
		VSREAL dist = TriangleTemp.SquaredDistance(Point,fParaTemp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
				fTriangleParameter[j] = fParaTemp[j];
			IndexTriangle = i;
		}
	
	}
	return distTemp;
}
VSREAL VSPolygon3::SquaredDistance(const VSTriangle3 &Triangle,
									int& IndexTriangle,
									VSREAL fTriangle1Parameter[3],
									VSREAL fTriangle2Parameter[3])const
{

	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2Temp[3];
		VSREAL dist = TriangleTemp.SquaredDistance(Triangle,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangle1Parameter[j] = fPara1Temp[j];
				fTriangle2Parameter[j] = fPara2Temp[j];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;
}
//多边形和矩形距离
VSREAL VSPolygon3::SquaredDistance(const VSRectangle3 &Rectangle,
									int& IndexTriangle,
									VSREAL fTriangleParameter[3],
									VSREAL fRectangleParameter[2])const
{

	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2Temp[2];
		VSREAL dist = TriangleTemp.SquaredDistance(Rectangle,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
				
			}
			for( int k = 0 ; k < 2 ; k++)
			{
				fRectangleParameter[k] = fPara2Temp[k];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;

}
VSREAL VSPolygon3::SquaredDistance(const VSLine3 &Line,int& IndexTriangle,
									VSREAL fTriangleParameter[3],VSREAL &fLineParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2;
		VSREAL dist = TriangleTemp.SquaredDistance(Line,fPara1Temp,fPara2);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
			}
			fLineParameter = fPara2;
			IndexTriangle = i;
		}

	}
	return distTemp;			

}
VSREAL VSPolygon3::SquaredDistance(const VSRay3 & Ray,int& IndexTriangle,
					   VSREAL fTriangleParameter[3],VSREAL &fRayParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2;
		VSREAL dist = TriangleTemp.SquaredDistance(Ray,fPara1Temp,fPara2);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
			}
			fRayParameter = fPara2;
			IndexTriangle = i;
		}

	}
	return distTemp;	


}
VSREAL VSPolygon3::SquaredDistance(const VSSegment3& Segment,int& IndexTriangle,
					   VSREAL fTriangleParameter[3],VSREAL &fSegmentParameter)const
{

	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2;
		VSREAL dist = TriangleTemp.SquaredDistance(Segment,fPara1Temp,fPara2);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
			}
			fSegmentParameter = fPara2;
			IndexTriangle = i;
		}

	}
	return distTemp;
}
VSREAL VSPolygon3::SquaredDistance(const VSOBB3 & OBB,int& IndexTriangle,
					   VSREAL TriangleParameter[3], VSREAL OBBParameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2Temp[3];
		VSREAL dist = TriangleTemp.SquaredDistance(OBB,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				TriangleParameter[j] = fPara1Temp[j];
				OBBParameter[j] = fPara2Temp[j];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;

}

VSREAL VSPolygon3::Distance(const VSSphere3 &Sphere,int& IndexTriangle,
				VSREAL fTriangleParameter[3],VSVector3 & SpherePoint)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSVector3 Point;
		VSREAL dist = TriangleTemp.Distance(Sphere,fPara1Temp,Point);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
	
			}
			SpherePoint = Point;
			IndexTriangle = i;
		}

	}
	return distTemp;

}


VSREAL VSPolygon3::Distance(const VSPlane3 &Plane,int& IndexTriangle,
				VSVector3 &TrianglePoint,VSVector3 &PlanePoint)const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSVector3 Point1,Point2;
		VSREAL dist = TriangleTemp.Distance(Plane,Point1,Point2);
		if(distTemp < dist)
		{
			distTemp = dist;
		
			TrianglePoint = Point1;
			PlanePoint = Point2;
			IndexTriangle = i;
		}

	}
	return distTemp;

}

VSREAL VSPolygon3::SquaredDistance(const VSAABB3 &AABB ,int& IndexTriangle,
					   VSREAL TriangleParameter[3]
						, VSREAL AABBParameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2Temp[3];
		VSREAL dist = TriangleTemp.SquaredDistance(AABB,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				TriangleParameter[j] = fPara1Temp[j];
				AABBParameter[j] = fPara2Temp[j];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;					
}
VSREAL VSPolygon3::SquaredDistance(const VSPolygon3 & Polygon ,int& Index1Triangle,
									VSREAL Triangle1Parameter[3],int& Index2Triangle,
									VSREAL Triangle2Parameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	VSTriangle3 TriangleTemp;
	VSREAL distTemp = VSMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		VSREAL fPara1Temp[3];
		VSREAL fPara2Temp[3];
		int k;
		VSREAL dist = TriangleTemp.SquaredDistance(Polygon,fPara1Temp,k,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				Triangle1Parameter[j] = fPara1Temp[j];
				Triangle2Parameter[j] = fPara2Temp[j];
			}
			Index1Triangle = i;
			Index2Triangle = k;
		}

	}
	return distTemp;		
}
