#include "VSRectangle3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
void VSRectangle3::GetPoint(VSVector3 Point[4])const
{

	VSVector3 temp0 = m_Center + m_A[0] * m_fA[0];
	VSVector3 temp1 = m_Center + m_A[0] * (-m_fA[0]);
	Point[0] = temp0 + m_A[1] * m_fA[1] ;
	Point[1] = temp1 + m_A[1] * m_fA[1] ;
	Point[2] = temp1 + m_A[1] * (-m_fA[1]);
	Point[3] = temp0 + m_A[1] * (-m_fA[1]);


}
/*----------------------------------------------------------------*/
VSREAL VSRectangle3::SquaredDistance(const VSRectangle3 &Rectangle,
								   VSREAL fRectangle1Parameter[2],VSREAL fRectangle2Parameter[2])const
{
	VSREAL t,temp[2];
	VSSegment3 Edge;
	VSREAL fSqrDist = VSMAX_REAL;
	VSREAL fSqrDistTmp;
	VSVector3 RectanglePoint[4];
	Rectangle.GetPoint(RectanglePoint);
	Edge.Set(RectanglePoint[1],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	VSREAL fS0,fT0,fS1,fT1;
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = t - Rectangle.m_fA[0];
		fT1 = Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}

	Edge.Set(RectanglePoint[3],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = Rectangle.m_fA[0];
		fT1 = t - Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[3]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = t -Rectangle.m_fA[0];
		fT1 = - Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[1]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = -Rectangle.m_fA[0];
		fT1 = t - Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}





	GetPoint(RectanglePoint);
	Edge.Set(RectanglePoint[1],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = t - Rectangle.m_fA[0];
		fT0 = Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}

	Edge.Set(RectanglePoint[3],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = Rectangle.m_fA[0];
		fT0 = t - Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[3]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = t - Rectangle.m_fA[0];
		fT0 = - Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[1]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = -Rectangle.m_fA[0];
		fT0 = t - Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}
	fRectangle1Parameter[0] = fS0;
	fRectangle1Parameter[1] = fT0;
	fRectangle2Parameter[0] = fS1;
	fRectangle2Parameter[1] = fT1;
	return fSqrDist;
}
/*----------------------------------------------------------------*/
//直线和矩形距离
VSREAL VSRectangle3::SquaredDistance(const VSLine3& Line,VSREAL fRectangleParameter[2],VSREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter,fRectangleParameter);


}
/*----------------------------------------------------------------*/
//矩形和OBB距离
VSREAL VSRectangle3::SquaredDistance(const VSOBB3 &OBB,VSREAL RectangleParameter[2], VSREAL OBBParameter[3])const
{

	return OBB.SquaredDistance(*this,OBBParameter,RectangleParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSRectangle3::SquaredDistance(const VSRay3 &Ray,VSREAL fRectangleParameter[2],VSREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
//线段到距离距离
VSREAL VSRectangle3::SquaredDistance(const VSSegment3 & Segment,VSREAL fRectangleParameter[2],VSREAL &fSegmentParameter)const
{
	return Segment.SquaredDistance(*this,fSegmentParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
//三角形和矩形距离
VSREAL VSRectangle3::SquaredDistance(const VSTriangle3 &Triangle,VSREAL fRectangleParameter[2],VSREAL fTriangleParameter[3])const
{

	return Triangle.SquaredDistance(*this,fTriangleParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
//点和矩形距离
VSREAL VSRectangle3::SquaredDistance(const VSVector3 &Point,VSREAL fRectangleParameter[2])const
{
	return Point.SquaredDistance(*this,fRectangleParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSRectangle3::Distance(const VSSphere3 &Sphere,VSREAL fRectangleParameter[2],VSVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fRectangleParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSRectangle3::Distance(const VSPlane3 &Plane,VSVector3 &RectanglePoint,VSVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,RectanglePoint);

}
/*----------------------------------------------------------------*/
VSREAL VSRectangle3::SquaredDistance(const VSAABB3 &AABB,VSREAL RectangleParameter[2]
						, VSREAL AABBParameter[3])const
{
	return 	AABB.SquaredDistance(*this,RectangleParameter,AABBParameter);				
						
}
/*----------------------------------------------------------------*/
VSREAL VSRectangle3::SquaredDistance(const VSPolygon3 &Polygon,
						VSREAL fRectangleParameter[2],
						int& IndexTriangle,
						VSREAL fTriangleParameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fRectangleParameter);
}