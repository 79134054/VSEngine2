#include "VSSphere3.h"
#include "VSMath.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSVector3 & Point,VSVector3 & SpherePoint)const
{
	VSREAL sqrDist = Point.SquaredDistance(m_Center);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 Line(m_Center,Point);

	SpherePoint = Line.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSLine3 & Line,VSVector3 & SpherePoint,VSREAL &fLineParameter)const
{

	VSREAL sqrDist = Line.SquaredDistance(m_Center,fLineParameter);
	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,Line.GetParameterPoint(fLineParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
	
}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSRay3 & Ray,VSVector3 & SpherePoint,VSREAL &fRayParameter)const
{
	VSREAL sqrDist = Ray.SquaredDistance(m_Center,fRayParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,Ray.GetParameterPoint(fRayParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSSegment3 & Segment,VSVector3 & SpherePoint,VSREAL &fSegmentParameter)const
{
	VSREAL sqrDist = Segment.SquaredDistance(m_Center,fSegmentParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,Segment.GetParameterPoint(fSegmentParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSOBB3 &OBB,VSVector3 & SpherePoint,VSREAL fOBBParameter[3])const
{
	VSREAL sqrDist = OBB.SquaredDistance(m_Center,fOBBParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,OBB.GetParameterPoint(fOBBParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSPlane3 & Plane,VSVector3 & SpherePoint)const
{
	VSVector3 PlanePoint;
	VSREAL sqrDist = Plane.Distance(m_Center,PlanePoint);


	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,PlanePoint);

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSRectangle3 & Rectangle,VSVector3 & SpherePoint,VSREAL fRectangleParameter[2])const
{
	VSREAL sqrDist = Rectangle.SquaredDistance(m_Center,fRectangleParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,Rectangle.GetParameterPoint(fRectangleParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSTriangle3 Triangle,VSVector3 & SpherePoint,VSREAL fTriangleParameter[3])const
{
	VSREAL sqrDist = Triangle.SquaredDistance(m_Center,fTriangleParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,Triangle.GetParameterPoint(fTriangleParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSAABB3 &AABB,VSVector3 & SpherePoint,VSREAL fAABBParameter[3])const
{
	VSREAL sqrDist = AABB.SquaredDistance(m_Center,fAABBParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	VSLine3 LineTemp(m_Center,AABB.GetParameterPoint(fAABBParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSSphere3::Distance(const VSPolygon3 &Polygon,VSVector3 & SpherePoint,int& IndexTriangle,
							VSREAL fTriangleParameter[3])const
{

	return Polygon.Distance(*this,IndexTriangle,fTriangleParameter,SpherePoint);
}