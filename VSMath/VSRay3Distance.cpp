#include "VSRay3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSVector3 &Point,VSREAL &fLineParameter)const
{
	VSREAL fSqDis = VSLine3::SquaredDistance(Point,fLineParameter);
	if(fLineParameter >= 0)
		return fSqDis;
	else
	{
		fLineParameter = 0;
		VSVector3 Diff;
		Diff = Point - m_Orig;
		return Diff.GetSqrLength();
	}
}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSLine3 &Line,VSREAL &fRayParameter,VSREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter,fRayParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSRay3 &Ray,VSREAL &fRay1Parameter,VSREAL &fRay2Parameter)const
{
	const VSLine3 Line = Ray;
	VSREAL sqrDist = SquaredDistance(Line,fRay1Parameter,fRay2Parameter);
	if(fRay2Parameter < 0)
	{
		fRay2Parameter = 0;
		sqrDist = SquaredDistance(Ray.m_Orig,fRay1Parameter);
	}

	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSSegment3 & Segment,VSREAL &fRayParameter,VSREAL &fSegmentParameter)const
{

	const VSRay3 Ray = Segment;
	VSREAL sqrDist = SquaredDistance(Ray,fRayParameter,fSegmentParameter);
	VSREAL fLen = Segment.GetLen();
	if(fSegmentParameter > fLen)
	{
		fSegmentParameter = fLen;
		VSVector3 End = Segment.GetEnd();
		sqrDist = SquaredDistance(End,fRayParameter);

	}
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSTriangle3& Triangle,VSREAL &fRayParameter,VSREAL fTriangleParameter[3])const
{
	VSREAL sqrDist = VSLine3::SquaredDistance(Triangle,fRayParameter,fTriangleParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = m_Orig.SquaredDistance(Triangle,fTriangleParameter);

	}


	return sqrDist;


}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fRayParameter,VSREAL fRectangleParameter[2])const
{
	VSREAL sqrDist = VSLine3::SquaredDistance(Rectangle,fRayParameter,fRectangleParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = m_Orig.SquaredDistance(Rectangle,fRectangleParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSOBB3 & OBB,VSREAL &fRayParameter,VSREAL fOBBParameter[3])const
{
	VSREAL sqrDist = VSLine3::SquaredDistance(OBB,fRayParameter,fOBBParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = OBB.SquaredDistance(m_Orig,fOBBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSRay3::Distance(const VSSphere3 &Sphere,VSREAL &fRayParameter,VSVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fRayParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSRay3::Distance(const VSPlane3 & Plane,VSVector3 &RayPoint,VSVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,RayPoint);
}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSAABB3 &AABB,VSREAL &fRayParameter, VSREAL fAABBParameter[3])const
{
	VSREAL sqrDist = VSLine3::SquaredDistance(AABB,fRayParameter,fAABBParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = AABB.SquaredDistance(m_Orig,fAABBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSRay3::SquaredDistance(const VSPolygon3 &Polygon,VSREAL &fRayParameter,
					   int& IndexTriangle,
					   VSREAL fTriangleParameter[3])const
{


	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fRayParameter);
}