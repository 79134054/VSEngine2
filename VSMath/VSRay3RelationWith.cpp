#include "VSRay3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSTriangle3 & Triangle, bool bCull,VSREAL &fRayParameter,
				 VSREAL fTriangleParameter[3])const
{
	if(VSLine3::RelationWith(Triangle,bCull,fRayParameter,fTriangleParameter) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(fRayParameter < 0.0f)
		return VSNOINTERSECT;
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSPlane3 &Plane, bool bCull,VSREAL &fRayParameter)const
{
	int iFlag = VSLine3::RelationWith(Plane,bCull,fRayParameter);
	if( iFlag != VSINTERSECT)
		return iFlag;
	if(fRayParameter < 0.0f)
	{
		iFlag = m_Orig.RelationWith(Plane);
		return iFlag;
	}
	return VSINTERSECT;

}
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSRectangle3 &Rectangle,bool bCull,VSREAL &fRayParameter,
						VSREAL fRectangleParameter[2])const
{
	if(VSLine3::RelationWith(Rectangle,bCull,fRayParameter,fRectangleParameter) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(fRayParameter < 0.0f)
		return VSNOINTERSECT;

	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSOBB3 &OBB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	if(VSLine3::RelationWith(OBB,Quantity, tNear,tFar) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(tFar < 0.0f)
	{
		Quantity = 0;
		return VSNOINTERSECT;
	}
	if(tNear<0.0f)
	{
		Quantity = 1;
		tNear = tFar;
	}
	return VSINTERSECT;
}
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSSphere3 &sphere, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	if(VSLine3::RelationWith(sphere,Quantity, tNear,tFar) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(tFar < 0.0f)
	{
		Quantity = 0;
		return VSNOINTERSECT;
	}
	if(tNear<0.0f)
	{
		Quantity = 1;
		tNear = tFar;
	}
	return VSINTERSECT;

}
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSAABB3 &AABB, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	if(VSLine3::RelationWith(AABB,Quantity, tNear,tFar) == VSNOINTERSECT)
		return VSNOINTERSECT;
	if(tFar < 0.0f)
	{
		Quantity = 0;
		return VSNOINTERSECT;
	}
	if(tNear<0.0f)
	{
		Quantity = 1;
		tNear = tFar;
	}
	return VSINTERSECT;

}
/*----------------------------------------------------------------*/
int VSRay3::RelationWith(const VSPolygon3 &Polygon,VSREAL &fRayParameter ,
				 bool bCull,int &iIndexTriangle,VSREAL fTriangleParameter[3])const
{

	return Polygon.RelationWith(*this,bCull,iIndexTriangle,fTriangleParameter,fRayParameter);
}