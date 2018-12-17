#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fSegmentParameter,
							VSREAL fTriangleParameter[3])const
{
	if(VSRay3::RelationWith(Triangle,bCull,fSegmentParameter,fTriangleParameter) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(fSegmentParameter > m_fLen)
		return VSNOINTERSECT;
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fSegmentParameter)const
{
	int iFlag = VSRay3::RelationWith(Plane,bCull,fSegmentParameter);
	if(iFlag != VSINTERSECT)
		return iFlag;
	if(fSegmentParameter > m_fLen)
	{
		return m_Orig.RelationWith(Plane);
	}
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fSegmentParameter,
				 VSREAL fRectangleParameter[2])const
{

	if(VSRay3::RelationWith(Rectangle,bCull,fSegmentParameter,fRectangleParameter) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(fSegmentParameter > m_fLen)
		return VSNOINTERSECT;
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	if(VSRay3::RelationWith(OBB,Quantity,tNear,tFar) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(tNear > m_fLen)
	{
		Quantity = 0;
		if(m_Orig.RelationWith(OBB) != VSOUT)
			return VSIN;
		return VSNOINTERSECT;
	}

	if(tFar > m_fLen)
	{
		Quantity = 1;
		tFar = tNear;
	}
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	if(VSRay3::RelationWith(sphere,Quantity,tNear,tFar) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(tNear > m_fLen)
	{
		Quantity = 0;
		if(m_Orig.RelationWith(sphere) != VSOUT)
			return VSIN;
		return VSNOINTERSECT;
	}

	if(tFar > m_fLen)
	{
		Quantity = 1;
		tFar = tNear;
	}
	return VSINTERSECT;

}
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{

	if(VSRay3::RelationWith(AABB,Quantity,tNear,tFar) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(tNear > m_fLen)
	{
		Quantity = 0;
		if(m_Orig.RelationWith(AABB) != VSOUT)
			return VSIN;
		return VSNOINTERSECT;
	}

	if(tFar > m_fLen)
	{
		Quantity = 1;
		tFar = tNear;
	}
	return VSINTERSECT;

}
/*----------------------------------------------------------------*/
int VSSegment3::RelationWith(const VSPolygon3 &Polygon,VSREAL &fSegmentParameter,
				 bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const
{

	return Polygon.RelationWith(*this,bCull,iIndexTriangle,fTriangleParameter,fSegmentParameter);
}