#include "VSTriangle3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::SquaredDistance(const VSLine3 &Line,VSREAL fTriangleParameter[3],VSREAL &fLineParameter)const
{

	return Line.SquaredDistance(*this,fLineParameter,fTriangleParameter);


}
/*----------------------------------------------------------------*/
//三角形和OBB距离
VSREAL VSTriangle3::SquaredDistance(const VSOBB3 &OBB,VSREAL TriangleParameter[3], VSREAL OBBParameter[3])const
{

	return OBB.SquaredDistance(*this, OBBParameter,TriangleParameter);

}
/*----------------------------------------------------------------*/
//射线和三角形距离
VSREAL VSTriangle3::SquaredDistance(const VSRay3 & Ray,VSREAL fTriangleParameter[3],VSREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter,fTriangleParameter);

}
/*----------------------------------------------------------------*/
//线段和三角形距离
VSREAL VSTriangle3::SquaredDistance(const VSSegment3& Segment,VSREAL fTriangleParameter[3],VSREAL &fSegmentParameter)const
{
	return Segment.SquaredDistance(*this,fSegmentParameter,fTriangleParameter);

}
/*----------------------------------------------------------------*/
//点和三角形距离
VSREAL VSTriangle3::SquaredDistance(const VSVector3 & Point,VSREAL fTriangleParameter[3])const
{
	return Point.SquaredDistance(*this,fTriangleParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::SquaredDistance(const VSTriangle3 &Triangle,
								  VSREAL fTriangle1Parameter[3],
								  VSREAL fTriangle2Parameter[3])const
{

	VSREAL fSqrDist = VSMAX_REAL;
	VSREAL fSqrDistTmp;
	VSSegment3 Edge;
	VSREAL Tempt0,Temp[3];
	VSREAL fR0,fS0,fT0,fR1,fS1,fT1;
	Edge.Set(m_V[0],m_V[1]);
	fSqrDistTmp = Edge.SquaredDistance(Triangle,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS0 = Tempt0 / Edge.GetLen();
		fR0 = 1.0f - fS0;
		fT0 = 0.0f;
		fR1 = Temp[0];
		fS1 = Temp[1];
		fT1 = Temp[2];
		fSqrDist = fSqrDistTmp;
	}


	Edge.Set(m_V[0],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Triangle,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{


		fS0 = 0.0f;
		fT0 = Tempt0 / Edge.GetLen();
		fR0 = 1.0f - fT0;

		fR1 = Temp[0];
		fS1 = Temp[1];
		fT1 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(m_V[1],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Triangle,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS0 = Tempt0 / Edge.GetLen();
		fT0 = 1.0f - fS0;
		fR0 = 0.0f;

		fR1 = Temp[0];
		fS1 = Temp[1];
		fT1 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}



	Edge.Set(Triangle.m_V[0],Triangle.m_V[1]);
	fSqrDistTmp = Edge.SquaredDistance(*this,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS1= Tempt0 / Edge.GetLen();
		fR1 = 1.0f - fS0;
		fT1 = 0.0f;
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
		fSqrDist = fSqrDistTmp;
	}


	Edge.Set(Triangle.m_V[0],Triangle.m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(*this,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{


		fS1 = 0.0f;
		fT1 = Tempt0 / Edge.GetLen();
		fR1 = 1.0f - fT0;

		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(Triangle.m_V[1],Triangle.m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(*this,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS1 = Tempt0 / Edge.GetLen();
		fT1 = 1.0f - fS0;
		fR1 = 0.0f;

		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}

	fTriangle1Parameter[0] = fR0;
	fTriangle1Parameter[1] = fS0;
	fTriangle1Parameter[2] = fT0;

	fTriangle2Parameter[0] = fR1;
	fTriangle2Parameter[1] = fS1;
	fTriangle2Parameter[2] = fT1;
	return fSqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::SquaredDistance(const VSRectangle3 &Rectangle,
								  VSREAL fTriangleParameter[3],
								  VSREAL fRectangleParameter[2])const
{

	VSVector3 RectanglePoint[4];
	Rectangle.GetPoint(RectanglePoint);
	VSSegment3 Edge;
	//VSVector3 A[2];
	//Rectangle.GetA(A);
	VSREAL fA[2];
	Rectangle.GetfA(fA);
	VSREAL t,Temp[3];
	VSREAL fSqrDist = VSMAX_REAL;
	VSREAL fSqrDistTmp;
	VSREAL fR0,fS0,fT0,fT1,fS1;
	Edge.Set(RectanglePoint[1],RectanglePoint[0]);
	fSqrDistTmp =Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;

		fS1 = t - fA[0];
		fT1 = fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];

	}
	Edge.Set(RectanglePoint[3],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = fA[0];
		fT1 = t - fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
	}
	Edge.Set(RectanglePoint[2],RectanglePoint[3]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = t - fA[0];
		fT1 =  -fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
	}
	Edge.Set(RectanglePoint[2],RectanglePoint[1]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = -fA[0];
		fT1 = t - fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
	}

	Edge.Set(m_V[0],m_V[1]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{


		fS0 = t / Edge.GetLen();
		fR0 = 1.0f - fS0;
		fT0 = 0.0f;
		fS1 = Temp[0];
		fT1 = Temp[1];
		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(m_V[0],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{


		fS0 = 0.0f;
		fT0 = t / Edge.GetLen();
		fR0 = 1.0f - fT0;

		fS1 = Temp[0];
		fT1 = Temp[1];
		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(m_V[1],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{



		fS0 = t / Edge.GetLen();
		fT0 = 1.0f - fS0;
		fR0 = 0.0f;

		fS1 = Temp[0];
		fT1 = Temp[1];
		fSqrDist = fSqrDistTmp;
	}
	fTriangleParameter[0] = fR0;
	fTriangleParameter[1] = fS0;
	fTriangleParameter[2] = fT0;	
	fRectangleParameter[0] = fS1;
	fRectangleParameter[2] = fT1;
	return fSqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::Distance(const VSSphere3 &Sphere,VSREAL fTriangleParameter[3],VSVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fTriangleParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::Distance(const VSPlane3 &Plane,VSVector3 &TrianglePoint,VSVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,TrianglePoint);

}
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::SquaredDistance(const VSAABB3 &AABB ,VSREAL TriangleParameter[3], VSREAL AABBParameter[3])const
{

	return AABB.SquaredDistance(*this,AABBParameter,TriangleParameter);


}
/*----------------------------------------------------------------*/
VSREAL VSTriangle3::SquaredDistance(const VSPolygon3& Polygon,
								   VSREAL fTriangle1Parameter[3],
								   int& IndexTriangle,
								   VSREAL fTriangle2Parameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangle2Parameter,fTriangle1Parameter);
}