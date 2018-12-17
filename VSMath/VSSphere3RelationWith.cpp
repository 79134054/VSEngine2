#include "VSSphere3.h"
#include "VSMath.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
using namespace VSEngine2;
int VSSphere3::RelationWith(const VSVector3 &Point)const
{

	return Point.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSLine3 &Line, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	return Line.RelationWith(*this,Quantity,tNear,tFar);

}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSRay3 &Ray, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	return Ray.RelationWith(*this,Quantity,tNear,tFar);

}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSSegment3 &Segment, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	return Segment.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSPlane3 &Plane)const
{
	VSVector3 N = Plane.GetN();
	VSREAL fD = Plane.GetfD();

	VSREAL test = m_Center.Dot(N) + fD;
	if(test > m_fRadius)
		return VSFRONT;
	else if(test < -m_fRadius)
		return VSBACK;
	else
		return VSINTERSECT;

}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSTriangle3 Triangle)const
{
	return Triangle.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSRectangle3 &Rectangle)const
{
	return Rectangle.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSOBB3 &OBB)const
{
	return OBB.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSSphere3::RelationWith(const VSSphere3 &Sphere)const
{
	VSREAL Sum = m_fRadius + Sphere.m_fRadius;

	Sum *=Sum;

	VSVector3 Sub = m_Center - Sphere.m_Center;

	if(Sub.GetSqrLength() > Sum)
		return VSNOINTERSECT;

	return VSINTERSECT;

}