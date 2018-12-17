#include "VSVector3.h"
#include "VSMath.h"
#include "VSMatrix3X3.h"
#include "VSQuat.h"
#include "VSLine3.h"
#include "VSRay3.h"
#include "VSSegment3.h"
#include "VSTriangle3.h"
#include "VSRectangle3.h"
#include "VSOBB3.h"
#include "VSSphere3.h"
#include "VSAABB3.h"
#include "VSPolygon3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSVector3 &Point)const
{
	return (x * Point.x + y * Point.y + z * Point.z);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSLine3 & Line,VSREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSRay3 & Ray,VSREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSSegment3 & Segment,VSREAL &fSegmentParameter)const
{
	return  Segment.SquaredDistance(*this,fSegmentParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSTriangle3 &Triangle,VSREAL fTriangleParameter[3])const
{
	VSVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);

	VSVector3 Diff = TrianglePoint[0] - *this;
	VSVector3 Edge0 = TrianglePoint[1]  - TrianglePoint[0] ;
	VSVector3 Edge1 = TrianglePoint[2]  - TrianglePoint[0] ;

	VSREAL fA00 = Edge0.GetSqrLength();
	VSREAL fA01 = Edge0.Dot(Edge1);
	VSREAL fA11 = Edge1.GetSqrLength();
	VSREAL fB0 = Diff.Dot(Edge0);
	VSREAL fB1 = Diff.Dot(Edge1);
	VSREAL fC = Diff.GetSqrLength();
	VSREAL fDet = ABS(fA00*fA11-fA01*fA01);
	VSREAL fS = fA01*fB1-fA11*fB0;
	VSREAL fT = fA01*fB0-fA00*fB1;
	VSREAL fSqrDistance;

	if (fS + fT <= fDet)
	{
		if (fS < 0.0f)
		{
			if (fT < 0.0f)  // region 4
			{
				if (fB0 < 0.0f)
				{
					fT = 0.0f;
					if (-fB0 >= fA00)
					{
						fS = 1.0f;
						fSqrDistance = fA00+(2.0f) * fB0 + fC;
					}
					else
					{
						fS = -fB0 / fA00;
						fSqrDistance = fB0 * fS + fC;
					}
				}
				else
				{
					fS = 0.0f;
					if (fB1 >= 0.0f)
					{
						fT = 0.0f;
						fSqrDistance = fC;
					}
					else if (-fB1 >= fA11)
					{
						fT = 1.0f;
						fSqrDistance = fA11 + 2.0f * fB1 + fC;
					}
					else
					{
						fT = -fB1 / fA11;
						fSqrDistance = fB1 * fT + fC;
					}
				}
			}
			else  // region 3
			{
				fS = 0.0f;
				if (fB1 >= 0.0f)
				{
					fT = 0.0f;
					fSqrDistance = fC;
				}
				else if (-fB1 >= fA11)
				{
					fT = 1.0f;
					fSqrDistance = fA11 + 2.0f * fB1 + fC;
				}
				else
				{
					fT = -fB1 / fA11;
					fSqrDistance = fB1 * fT + fC;
				}
			}
		}
		else if (fT < 0.0f)  // region 5
		{
			fT = 0.0f;
			if (fB0 >= 0.0f)
			{
				fS = 0.0f;
				fSqrDistance = fC;
			}
			else if (-fB0 >= fA00)
			{
				fS = 1.0f;
				fSqrDistance = fA00 + 2.0f * fB0 + fC;
			}
			else
			{
				fS = -fB0 / fA00;
				fSqrDistance = fB0 * fS + fC;
			}
		}
		else  // region 0
		{

			VSREAL fInvDet = 1.0f / fDet;
			fS *= fInvDet;
			fT *= fInvDet;
			fSqrDistance = fS * ( fA00 * fS + fA01 * fT + 2.0f * fB0) +
				fT * ( fA01 * fS + fA11 * fT + 2.0f * fB1 ) + fC;
		}
	}
	else
	{
		VSREAL fTmp0, fTmp1, fNumer, fDenom;

		if (fS < 0.0f)  // region 2
		{
			fTmp0 = fA01 + fB0;
			fTmp1 = fA11 + fB1;
			if (fTmp1 > fTmp0)
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00 - 2.0f * fA01 + fA11;
				if (fNumer >= fDenom)
				{
					fS = 1.0f;
					fT = 0.0f;
					fSqrDistance = fA00 + 2.0f * fB0 + fC;
				}
				else
				{
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDistance = fS * ( fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT * ( fA01 * fS + fA11 * fT + 2.0f * fB1 )+fC;
				}
			}
			else
			{
				fS = 0.0f;
				if (fTmp1 <= 0.0f)
				{
					fT = 1.0f;
					fSqrDistance = fA11 + 2.0f * fB1 + fC;
				}
				else if (fB1 >= 0.0f)
				{
					fT = 0.0f;
					fSqrDistance = fC;
				}
				else
				{
					fT = -fB1 / fA11;
					fSqrDistance = fB1 * fT + fC;
				}
			}
		}
		else if (fT < 0.0f)  // region 6
		{
			fTmp0 = fA01 + fB1;
			fTmp1 = fA00 + fB0;
			if (fTmp1 > fTmp0)
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00 - 2.0f * fA01 + fA11;
				if (fNumer >= fDenom)
				{
					fT = 1.0f;
					fS = 0.0f;
					fSqrDistance = fA11 + 2.0f * fB1 + fC;
				}
				else
				{
					fT = fNumer / fDenom;
					fS = 1.0f - fT;
					fSqrDistance = fS * ( fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT*( fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
				}
			}
			else
			{
				fT = 0.0f;
				if (fTmp1 <= 0.0f)
				{
					fS = 1.0f;
					fSqrDistance = fA00 + 2.0f * fB0 + fC;
				}
				else if (fB0 >= 0.0f)
				{
					fS = 0.0f;
					fSqrDistance = fC;
				}
				else
				{
					fS = -fB0/fA00;
					fSqrDistance = fB0*fS+fC;
				}
			}
		}
		else  // region 1
		{
			fNumer = fA11 + fB1 - fA01 - fB0;
			if (fNumer <= 0.0f)
			{
				fS = 0.0f;
				fT = 1.0f;
				fSqrDistance = fA11 + 2.0f * fB1 + fC;
			}
			else
			{
				fDenom = fA00 - 2.0f * fA01 + fA11;
				if (fNumer >= fDenom)
				{
					fS = 1.0f;
					fT = 0.0f;
					fSqrDistance = fA00 + 2.0f * fB0 + fC;
				}
				else
				{
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
				}
			}
		}
	}


	if (fSqrDistance < 0.0f)
	{
		fSqrDistance = 0.0f;
	}
	fTriangleParameter[1] = fS;
	fTriangleParameter[2] = fT;
	fTriangleParameter[0] = 1 - fTriangleParameter[1] -fTriangleParameter[2];

	return fSqrDistance;
}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSRectangle3 &Rectangle,VSREAL fRectangleParameter[2])const
{
	VSVector3 Diff = Rectangle.GetCenter() - *this;

	VSVector3 A[2];
	Rectangle.GetA(A);
	VSREAL fA[2];
	Rectangle.GetfA(fA);

	VSREAL fB0 = Diff.Dot(A[0]);
	VSREAL fB1 = Diff.Dot(A[1]);
	VSREAL fS = -fB0;
	VSREAL fT = -fB1;
	VSREAL fSqrDistance = Diff.GetSqrLength();

	if (fS < -fA[0])
	{
		fS = -fA[0];
	}
	else if (fS > fA[0])
	{
		fS = fA[0];
	}
	fSqrDistance += fS * (fS + 2.0f * fB0);

	if (fT < -fA[1])
	{
		fT = -fA[1];
	}
	else if (fT > fA[1])
	{
		fT = fA[1];
	}
	fSqrDistance += fT * ( fT + 2.0f * fB1 );


	if (fSqrDistance <0.0f)
	{
		fSqrDistance = 0.0f;
	}
	fRectangleParameter[0] = fS;
	fRectangleParameter[1] = fT;
	return fSqrDistance;

}
/*----------------------------------------------------------------*/
//µãºÍOBB¾àÀë
VSREAL VSVector3::SquaredDistance(const VSOBB3 &OBB,VSREAL fOBBParameter[3])const
{
	return OBB.SquaredDistance(*this,fOBBParameter);



}
/*----------------------------------------------------------------*/
VSREAL VSVector3::Distance(const VSSphere3& Sphere,VSVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::Distance(const VSPlane3 & Plane,VSVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSAABB3 &AABB,VSREAL fAABBParameter[3])const
{
	return AABB.SquaredDistance(*this,fAABBParameter);

}
/*----------------------------------------------------------------*/
VSREAL VSVector3::SquaredDistance(const VSPolygon3 & Polygon , int& IndexTriangle,VSREAL fTriangleParameter[3])const
{
	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter);

}