#include "VSPlane3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
#include "VSCylinder3.h"
using namespace VSEngine2;
int VSPlane3::RelationWith(const VSVector3 &Point)const 
{
	VSREAL f = (Point.Dot(m_N)) + m_fD;

	if (f >  EPSILON_E4) return VSFRONT;
	if (f < -EPSILON_E4) return VSBACK;
	return VSON;
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSLine3 &Line, bool bCull,VSREAL &fLineParameter)const
{
	
	return Line.RelationWith(*this,bCull,fLineParameter);
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSRay3 &Ray, bool bCull,VSREAL &fRayParameter)const
{
	return Ray.RelationWith(*this,bCull,fRayParameter);

}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSSegment3 &Segment, bool bCull,VSREAL &fSegmentParameter)const
{
	return Segment.RelationWith(*this,bCull,fSegmentParameter);
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSOBB3 &OBB)const
{
	return OBB.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSAABB3 &AABB)const
{
	return AABB.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSSphere3 &Sphere)const
{
	return Sphere.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSTriangle3 &Triangle)const
{
	return Triangle.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSPlane3 &Plane)const
{
	VSVector3 vcCross;
	VSREAL     fSqrLength;


	vcCross.Cross(m_N, Plane.m_N);
	fSqrLength = vcCross.GetSqrLength();
	
	
	if (fSqrLength < EPSILON_E4) 
	{
		//return Plane.m_Point.RelationWith(*this);
		return VSNOINTERSECT;	
	}
	return VSINTERSECT;
	/*VSREAL fN00 = m_N.GetSqrLength();
	VSREAL fN01 = m_N * Plane.m_N;
	VSREAL fN11 = Plane.m_N.GetSqrLength();
	VSREAL fDet = fN00*fN11 - fN01*fN01;

	if (ABS(fDet) < EPSILON_E4) 
		return Plane.m_Point.RelationWith(*this);

	VSREAL fInvDet = 1.0f/fDet;
	VSREAL fC0 = (fN11 * m_fD - fN01 * Plane.m_fD) * fInvDet;
	VSREAL fC1 = (fN00 * Plane.m_fD - fN01 * m_fD) * fInvDet;
	Line.Set(vcCross,m_N * fC0 + Plane.m_N * fC1);
	return VSINTERSECT;*/
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSRectangle3 & Rectangle)const
{
	return Rectangle.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSPlane3 &Plane,VSLine3 &Line)const
{
	VSVector3 vcCross;
	VSREAL     fSqrLength;


	vcCross.Cross(m_N, Plane.m_N);
	fSqrLength = vcCross.GetSqrLength();


	if (fSqrLength < EPSILON_E4) 
	{
		return VSNOINTERSECT;
		//return Plane.m_Point.RelationWith(*this);

	}
	VSREAL fN00 = m_N.GetSqrLength();
	VSREAL fN01 = m_N.Dot(Plane.m_N);
	VSREAL fN11 = Plane.m_N.GetSqrLength();
	VSREAL fDet = fN00*fN11 - fN01*fN01;

	if (ABS(fDet) < EPSILON_E4) 
		return VSNOINTERSECT;

	VSREAL fInvDet = 1.0f/fDet;
	VSREAL fC0 = (fN11 * m_fD - fN01 * Plane.m_fD) * fInvDet;
	VSREAL fC1 = (fN00 * Plane.m_fD - fN01 * m_fD) * fInvDet;
	Line.Set(m_N * fC0 + Plane.m_N * fC1,vcCross);
	return VSINTERSECT;/**/
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSTriangle3 &Triangle ,VSSegment3 & Segment)const
{
	return Triangle.RelationWith(*this,Segment);

}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSRectangle3 &Rectangle,VSSegment3 &Segment)const
{
	return Rectangle.RelationWith(*this,Segment);

}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSPolygon3 &Polygon)const
{
	return Polygon.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSPlane3::RelationWith(const VSPolygon3 &Polygon,VSSegment3 & Segment)const
{

	return Polygon.RelationWith(*this,Segment);
}
int VSPlane3::RelationWith(const VSCylinder3 &Cylinder3)const
{
	return VSINTERSECT;
}