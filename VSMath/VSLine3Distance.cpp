#include "VSLine3.h"
#include "VSRay3.h"
#include "VSSegment3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
VSREAL VSLine3::SquaredDistance(const VSVector3 &Point,VSREAL &fLineParameter)const
{
	VSVector3 Diff = Point - m_Orig;

	fLineParameter = m_Dir.Dot(Diff);

	Diff -= m_Dir * fLineParameter;

	return Diff.GetSqrLength();
}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSLine3 &Line,VSREAL &fLine1Parameter,VSREAL &fLine2Parameter)const
{
	VSREAL SquareDis;
	VSVector3 Diff = m_Orig - Line.m_Orig;
	VSREAL a01 = -(m_Dir.Dot(Line.m_Dir));
	VSREAL b0 = m_Dir.Dot(Diff);
	VSREAL c = Diff.GetSqrLength();
	VSREAL det = 1 - a01 * a01;

	if(ABS(det) >=EPSILON_E4)
	{
		VSREAL b1 = -(Line.m_Dir.Dot(Diff));
		VSREAL invDet = 1 / det;
		fLine1Parameter = (a01 * b1 - b0) * invDet;
		fLine2Parameter = (a01 * b0 - b1) * invDet;
		SquareDis = fLine1Parameter * (fLine1Parameter + a01 * fLine2Parameter + 2 * b0) + fLine2Parameter * (a01 * fLine1Parameter + fLine2Parameter + 2 * b1) + c;

	}
	else
	{
		fLine1Parameter =  - b0;
		fLine2Parameter = 0;

		SquareDis = b0 * fLine1Parameter + c;

	}
	return SquareDis;
}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSRay3 &Ray,VSREAL &fLineParameter,VSREAL &fRayParameter)const
{
	const VSLine3 Line = Ray;
	VSREAL sqrDist = SquaredDistance(Line,fLineParameter,fRayParameter);

	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = SquaredDistance(Ray.m_Orig,fLineParameter);

	}
	return sqrDist;
}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSSegment3 & Segment,VSREAL &fLineParameter,VSREAL &fSegmentParameter)const
{
	const VSRay3 Ray = Segment;
	VSREAL sqrDist = SquaredDistance(Ray,fLineParameter,fSegmentParameter);
	VSREAL fLen = Segment.GetLen();

	if(fSegmentParameter > fLen)
	{
		VSVector3 End = Segment.GetEnd(); 
		fSegmentParameter = fLen;
		sqrDist = SquaredDistance(End,fLineParameter);

	}
	return sqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSTriangle3 &Triangle,VSREAL &fLineParameter,VSREAL fTriangleParameter[3])const
{
	VSVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);

	/*VSVector3 Diff = TrianglePoint[0] - m_Orig;
	VSVector3 Edge0 = TrianglePoint[1] - TrianglePoint[0];
	VSVector3 Edge1 = TrianglePoint[2] - TrianglePoint[0];
	VSREAL fA00 = m_Dir * m_Dir;
	VSREAL fA01 = m_Dir * Edge0;
	VSREAL fA02 = m_Dir * Edge1;
	VSREAL fA11 = Edge0.GetSqrLength();
	VSREAL fA12 = Edge0 * Edge1;
	VSREAL fA22 = Edge1 * Edge1;
	VSREAL fB0  = Diff * m_Dir;
	VSREAL fB1  = Diff * Edge0;
	VSREAL fB2  = Diff * Edge1;
	VSVector3 Normal;
	Normal.Cross(Edge0,Edge1);
	VSREAL fDot = Normal * m_Dir;
	if (ABS(fDot) >= EPSILON_E4)
	{

		VSREAL fCof00 = fA11*fA22-fA12*fA12;
		VSREAL fCof01 = fA02*fA12-fA01*fA22;
		VSREAL fCof02 = fA01*fA12-fA02*fA11;
		VSREAL fCof11 = fA00*fA22-fA02*fA02;
		VSREAL fCof12 = fA02*fA01-fA00*fA12;
		VSREAL fCof22 = fA00*fA11-fA01*fA01;
		VSREAL fInvDet = (1.0f)/(fA00*fCof00+fA01*fCof01+fA02*fCof02);
		VSREAL fRhs0 = -fB0*fInvDet;
		VSREAL fRhs1 = -fB1*fInvDet;
		VSREAL fRhs2 = -fB2*fInvDet;


		fLineParameter = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;


		fTriangleParameter[1] = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
		fTriangleParameter[2] = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;
		fTriangleParameter[0] = 1.0f - fTriangleParameter[1] - fTriangleParameter[2];

		if (fTriangleParameter[0] >= 0.0f 
			&&  fTriangleParameter[1]>= 0.0f
			&&  fTriangleParameter[2] >= 0.0f)
		{

			return 0.0f;
		}
	}*/


	VSREAL fSqrDist = VSMAX_REAL;
	VSREAL fSqrDistTmp;
	VSSegment3 Edge;
	VSREAL Tempt0,Tempt1;
	Edge.Set(TrianglePoint[0],TrianglePoint[1]);
	fSqrDistTmp = SquaredDistance(Edge,Tempt0,Tempt1);

	if (fSqrDistTmp < fSqrDist)
	{


		fLineParameter = Tempt0;

		fTriangleParameter[1] = Tempt1 / Edge.GetLen();
		fTriangleParameter[0] = 1.0f - fTriangleParameter[1];
		fTriangleParameter[2] = 0.0f;

		fSqrDist = fSqrDistTmp;
	}


	Edge.Set(TrianglePoint[0],TrianglePoint[2]);
	fSqrDistTmp = SquaredDistance(Edge,Tempt0,Tempt1);

	if (fSqrDistTmp < fSqrDist)
	{


		fLineParameter = Tempt0;

		fTriangleParameter[1] = 0.0f;
		fTriangleParameter[2] = Tempt1 / Edge.GetLen();
		fTriangleParameter[0] = 1.0f - fTriangleParameter[2];


		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(TrianglePoint[1],TrianglePoint[2]);
	fSqrDistTmp = SquaredDistance(Edge,Tempt0,Tempt1);

	if (fSqrDistTmp < fSqrDist)
	{


		fLineParameter = Tempt0;

		fTriangleParameter[1] = Tempt1 / Edge.GetLen();
		fTriangleParameter[2] = 1.0f - fTriangleParameter[1];
		fTriangleParameter[0] = 0.0f;


		fSqrDist = fSqrDistTmp;
	}
	return fSqrDist;


}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSRectangle3& Rectangle,VSREAL &fLineParameter,VSREAL fRectangleParameter[2])const
{
	//没有测试相交


	VSREAL fSqrDist = VSMAX_REAL;
	VSREAL fSqrDistTmp;
	VSVector3 r[4];

	VSVector3 A[2];
	Rectangle.GetA(A);
	VSREAL fA[2];
	Rectangle.GetfA(fA);


	VSVector3 Temp0 = Rectangle.GetCenter() + A[0] * fA[0];
	VSVector3 Temp1 = Rectangle.GetCenter() + A[0] * (-fA[0]);
	r[0] = Temp0 + A[1] * fA[1] ;
	r[1] = Temp0 + A[1] * (-fA[1]);
	r[2] = Temp1 + A[1] * (-fA[1]);
	r[3] = Temp1 + A[1] * fA[1] ;

	VSSegment3 Edge;
	Edge.Set(r[0],r[1]);

	VSREAL fT0,fT1;
	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = fA[0];
		fRectangleParameter[1] = fT1 - fA[1];
	}
	Edge.Set(r[2],r[1]);

	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = fA[1];
		fRectangleParameter[1] = fT1 - fA[0];
	}
	Edge.Set(r[3],r[2]);

	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = -fA[0];
		fRectangleParameter[1] = fT1 - fA[1];
	}
	Edge.Set(r[3],r[0]);
	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = -fA[1];
		fRectangleParameter[1] = fT1 - fA[0];
	}
	return fSqrDist;

}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSOBB3 &OBB,VSREAL &fLineParameter,VSREAL fOBBParameter[3])const
{


	return OBB.SquaredDistance(*this,fOBBParameter,fLineParameter);
}
/*----------------------------------------------------------------*/
VSREAL VSLine3::Distance(const VSSphere3 &Sphere,VSREAL &fLineParameter,VSVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fLineParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSLine3::Distance(const VSPlane3 &Plane,VSVector3 &LinePoint,VSVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,LinePoint);

}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSAABB3 &AABB,VSREAL &fLineParameter, VSREAL fAABBParameter[3])const
{
	return AABB.SquaredDistance(*this,fAABBParameter,fLineParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSLine3::SquaredDistance(const VSPolygon3 & Polygon,VSREAL &fLineParameter,
					   int& IndexTriangle,
					   VSREAL fTriangleParameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fLineParameter);
}