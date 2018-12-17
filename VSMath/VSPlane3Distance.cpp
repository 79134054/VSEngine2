#include "VSPlane3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSVector3 &Point,VSVector3 &PlanePoint)const
{

	VSREAL Dist = ABS((m_N.Dot(Point)) - m_fD);

	VSLine3 Line(Point,m_N * (-1));
	PlanePoint = Line.GetParameterPoint(Dist);

	return Dist;
}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSSphere3 &Sphere,VSVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint);
	
}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSLine3 &Line,VSVector3 &PlanePoint,VSVector3 &LinePoint)const
{
	VSREAL fDot = Line.GetDir().Dot(m_N);
	if(ABS(fDot) < EPSILON_E4)
	{
		LinePoint = Line.GetOrig();
		return LinePoint.Distance(*this,PlanePoint);
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSRay3 & Ray,VSVector3 &PlanePoint,VSVector3 &RayPoint)const
{
	VSREAL fDot = Ray.GetDir().Dot(m_N);
	RayPoint = Ray.GetOrig();
	
	VSREAL f = RayPoint.Dot(m_N) + m_fD;
	if(fDot * f > 0)
		return RayPoint.Distance(*this,PlanePoint);
	else
		return 0;
}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSSegment3 & Segment,VSVector3 &PlanePoint,VSVector3 &SegmentPoint)const
{

	VSVector3 SegmentOrig = Segment.GetOrig();
	VSVector3 SegmentEnd = Segment.GetEnd();
	VSREAL f0 = SegmentOrig.Dot(m_N) + m_fD;
	VSREAL f1 = SegmentEnd.Dot(m_N) + m_fD;
		
	if(f0 * f1 > 0)
	{
		VSVector3 PPoint;
		VSREAL dist = SegmentOrig.Distance(*this, PPoint);
		SegmentPoint = SegmentOrig;
		PlanePoint = PPoint;
		VSREAL distTemp = SegmentEnd.Distance(*this, PPoint);
		if(dist > distTemp)
		{
			dist = distTemp;
			SegmentPoint = SegmentEnd;
			PlanePoint = PPoint;
		}
		return dist;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSPlane3 &Plane,VSVector3 &Plane1Point,VSVector3 &Plane2Point)const
{
	if(m_N.IsParallel(Plane.m_N))
	{
		Plane1Point = GetPoint();
		return Plane1Point.Distance(Plane,Plane2Point);
	}
	else
		return 0;
	
}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSTriangle3 &Triangle,VSVector3 &PlanePoint,VSVector3 &TrianglePoint)const
{
	/*int i[3];
	VSVector3 Point[3];
	Triangle.GetPoint(Point);
	i[0] = RelationWith(Point[0]);
	i[1] = RelationWith(Point[1]);
	i[2] = RelationWith(Point[2]);

	if((i[0] == i[1]) && (i[1] == i[2]))
	{
		VSREAL dist[3],distTemp;
		VSVector3 PlanePointTemp[3];
		dist[0] = Point[0].Distance(*this,PlanePointTemp[0]);
		dist[1] = Point[1].Distance(*this,PlanePointTemp[1]);
		dist[2] = Point[2].Distance(*this,PlanePointTemp[2]);
		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		TrianglePoint = Point[0];
		if(distTemp > dist[1])
		{
			distTemp = dist[1];
			PlanePoint = PlanePointTemp[1];
			TrianglePoint = Point[1];
		}
		if(distTemp > dist[2])
		{
			distTemp = dist[2];
			PlanePoint = PlanePointTemp[2];
			TrianglePoint = Point[2];
		}
		return distTemp;
	}
	return 0;*/
	VSVector3 Point[3];
	Triangle.GetPoint(Point);

	VSREAL dist[3],distTemp;
	VSVector3 PlanePointTemp[3];
	dist[0] = Point[0].Distance(*this,PlanePointTemp[0]);
	dist[1] = Point[1].Distance(*this,PlanePointTemp[1]);
	dist[2] = Point[2].Distance(*this,PlanePointTemp[2]);
	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	TrianglePoint = Point[0];
	if(distTemp > dist[1])
	{
		distTemp = dist[1];
		PlanePoint = PlanePointTemp[1];
		TrianglePoint = Point[1];
	}
	if(distTemp > dist[2])
	{
		distTemp = dist[2];
		PlanePoint = PlanePointTemp[2];
		TrianglePoint = Point[2];
	}
	return distTemp;


}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSRectangle3 &Rectangle,VSVector3 &PlanePoint,VSVector3 &RectanglePoint)const
{
	/*int f[4];
	VSVector3 Point[4];
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		f[i] = RelationWith(Point[i]);
	}

	if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]))
	{
		VSREAL dist[4],distTemp;
		VSVector3 PlanePointTemp[4];
		for(int i = 0 ; i < 4 ; i++)
			dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		RectanglePoint = Point[0];
		for(int i = 1 ; i < 4 ; i++)
		{
			if(distTemp > dist[i])
			{
				distTemp = dist[i];
				PlanePoint = PlanePointTemp[i];
				RectanglePoint = Point[i];
			}
		}
		return distTemp;
	}
	return 0;*/

	VSVector3 Point[4];
	Rectangle.GetPoint(Point);



	VSREAL dist[4],distTemp;
	VSVector3 PlanePointTemp[4];
	for(int i = 0 ; i < 4 ; i++)
		dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	RectanglePoint = Point[0];
	for(int i = 1 ; i < 4 ; i++)
	{
		if(distTemp > dist[i])
		{
			distTemp = dist[i];
			PlanePoint = PlanePointTemp[i];
			RectanglePoint = Point[i];
		}
	}
	return distTemp;



}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSOBB3& OBB,VSVector3 &PlanePoint,VSVector3 &OBBPoint)const
{
	/*int f[8];
	VSVector3 Point[8];
	OBB.GetPoint(Point);
	for(int i = 0 ; i < 8 ; i++)
	{
		f[i] = RelationWith(Point[i]);
	}

	if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]) && (f[3] == f[4]) 
			&& (f[4] == f[6]) && (f[5] == f[6]) && (f[6] == f[7]))
	{
		VSREAL dist[4],distTemp;
		VSVector3 PlanePointTemp[8];
		for(int i = 0 ; i < 8 ; i++)
			dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		OBBPoint = Point[0];
		for(int i = 1 ; i < 8 ; i++)
		{
			if(distTemp > dist[i])
			{
				distTemp = dist[i];
				PlanePoint = PlanePointTemp[i];
				OBBPoint = Point[i];
			}
		}
		return distTemp;
	}
	return 0;*/

	VSVector3 Point[8];
	OBB.GetPoint(Point);

	
	VSREAL dist[4],distTemp;
	VSVector3 PlanePointTemp[8];
	for(int i = 0 ; i < 8 ; i++)
		dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	OBBPoint = Point[0];
	for(int i = 1 ; i < 8 ; i++)
	{
		if(distTemp > dist[i])
		{
			distTemp = dist[i];
			PlanePoint = PlanePointTemp[i];
			OBBPoint = Point[i];
		}
	}
	return distTemp;


}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSAABB3 &AABB,VSVector3 &PlanePoint,VSVector3 &AABBPoint)const
{
	/*int f[8];
	VSVector3 Point[8];
	AABB.GetPoint(Point);
	for(int i = 0 ; i < 8 ; i++)
	{
		f[i] = RelationWith(Point[i]);
	}

	if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]) && (f[3] == f[4]) 
		&& (f[4] == f[6]) && (f[5] == f[6]) && (f[6] == f[7]))
	{
		VSREAL dist[4],distTemp;
		VSVector3 PlanePointTemp[8];
		for(int i = 0 ; i < 8 ; i++)
			dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		AABBPoint = Point[0];
		for(int i = 1 ; i < 8 ; i++)
		{
			if(distTemp > dist[i])
			{
				distTemp = dist[i];
				PlanePoint = PlanePointTemp[i];
				AABBPoint = Point[i];
			}
		}
		return distTemp;
	}
	return 0;*/

	VSVector3 Point[8];
	AABB.GetPoint(Point);
	


	VSREAL dist[4],distTemp;
	VSVector3 PlanePointTemp[8];
	for(int i = 0 ; i < 8 ; i++)
		dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	AABBPoint = Point[0];
	for(int i = 1 ; i < 8 ; i++)
	{
		if(distTemp > dist[i])
		{
			distTemp = dist[i];
			PlanePoint = PlanePointTemp[i];
			AABBPoint = Point[i];
		}
	}
	return distTemp;
	
}
/*----------------------------------------------------------------*/
VSREAL VSPlane3::Distance(const VSPolygon3 &Polygon,VSVector3 &PlanePoint,int& IndexTriangle,
						 VSVector3 &TrianglePoint)const
{

	return Polygon.Distance(*this,IndexTriangle,TrianglePoint,PlanePoint);
}