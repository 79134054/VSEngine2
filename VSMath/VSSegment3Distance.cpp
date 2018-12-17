#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSVector3 &Point,VSREAL &fSegmentParameter)const
{
	VSREAL SqDis = VSRay3::SquaredDistance(Point,fSegmentParameter);
	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		VSVector3 Diff;
		Diff = Point - m_End;
		return Diff.GetSqrLength();
	}
	else
	{
		return SqDis;
	}
}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSLine3 &Line,VSREAL &fSegmentParameter,VSREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter,fSegmentParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSRay3 &Ray,VSREAL &fSegmentParameter,VSREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter,fSegmentParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSSegment3 & Segment,VSREAL &fSegment1Parameter,VSREAL &fSegment2Parameter)const
{
	const VSRay3 & Ray = Segment;
	VSREAL sqrDist = SquaredDistance(Ray,fSegment1Parameter,fSegment2Parameter);
	VSREAL fLen = Segment.GetLen();
	if(fSegment2Parameter > fLen)
	{
		fSegment2Parameter = fLen;
		VSVector3 End = Segment.GetEnd();
		sqrDist = SquaredDistance(End,fSegment1Parameter);
	}
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSTriangle3& Triangle,VSREAL &fSegmentParameter,VSREAL fTriangleParameter[3])const
{
	VSREAL sqrDist = VSRay3::SquaredDistance(Triangle,fSegmentParameter,fTriangleParameter);

	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = m_End.SquaredDistance(Triangle,fTriangleParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fSegmentParameter,VSREAL fRectangleParameter[2])const
{
	VSREAL sqrDist = VSRay3::SquaredDistance(Rectangle,fSegmentParameter,fRectangleParameter);

	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = m_End.SquaredDistance(Rectangle,fRectangleParameter);

	}


	return sqrDist;


}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSOBB3 & OBB,VSREAL &fSegmentParameter,VSREAL fOBBParameter[3])const
{
	VSREAL sqrDist = VSRay3::SquaredDistance(OBB,fSegmentParameter,fOBBParameter);
	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = OBB.SquaredDistance(m_End,fOBBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::Distance(const VSSphere3 &Sphere,VSREAL &fSegmentParameter,VSVector3 & SpherePoint)const
{

	return Sphere.Distance(*this,SpherePoint,fSegmentParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::Distance(const VSPlane3 & Plane,VSVector3 &SegmentPoint,VSVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,SegmentPoint);
}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSAABB3 &AABB,VSREAL &fSegmentParameter,VSREAL fAABBParameter[3])const
{
	VSREAL sqrDist = VSRay3::SquaredDistance(AABB,fSegmentParameter,fAABBParameter);
	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = AABB.SquaredDistance(m_End,fAABBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSSegment3::SquaredDistance(const VSPolygon3 &Polygon,VSREAL &fSegmentParameter,
									int& IndexTriangle,
									VSREAL fTriangleParameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fSegmentParameter);
}