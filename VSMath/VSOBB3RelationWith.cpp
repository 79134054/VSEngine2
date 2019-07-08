#include "VSOBB3.h"
#include "VSMath.h"
#include "VSMatrix3X3.h"
#include "VSLine3.h"
#include "VSRay3.h"
#include "VSRectangle3.h"
#include "VSSegment3.h"
using namespace VSEngine2;
int VSOBB3::RelationWith(const VSVector3 &Point)const
{
	return Point.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSLine3 &Line, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{

	return Line.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSRay3 &Ray, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{

	return Ray.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSSegment3 &Segment, unsigned int &Quantity,VSREAL &tNear,VSREAL &tFar)const
{
	return Segment.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSPlane3 &Plane)const
{
	VSVector3 N = Plane.GetN();
	VSREAL fD = Plane.GetfD();
	VSREAL fRadius = ABS(m_fA[0] * (N.Dot(m_A[0])))
					+ ABS(m_fA[1] * (N.Dot(m_A[1])))
					+ ABS(m_fA[2] * (N.Dot(m_A[2])));


	VSREAL fTest = N.Dot(m_Center) + fD;

	
	if (fTest > fRadius)
		return VSFRONT;
	else if (fTest < -fRadius)
		return VSBACK;
	else
		return VSINTERSECT;

}
/*----------------------------------------------------------------*/
/*void VSOBB3::OBBProj(const VSOBB3 &OBB, const VSVector3 &vcV,
					VSREAL *pfMin, VSREAL *pfMax)const 
{
	VSREAL fDP = vcV * OBB.m_Center;
	VSREAL fR = OBB.m_fA[0] * ABS(vcV * OBB.m_A[0]) +
		OBB.m_fA[0] * ABS(vcV * OBB.m_A[1]) +
		OBB.m_fA[1] * ABS(vcV * OBB.m_A[2]);
	*pfMin = fDP - fR;
	*pfMax = fDP + fR;
} */
/*----------------------------------------------------------------*/
/*void VSOBB3::TriProj(const VSVector3 &v0, const VSVector3 &v1, 
					const VSVector3 &v2, const VSVector3 &vcV,
					VSREAL *pfMin, VSREAL *pfMax)const 
{
	*pfMin = vcV * v0;
	*pfMax = *pfMin;

	VSREAL fDP = vcV * v1;
	if (fDP < *pfMin) *pfMin = fDP;
	else if (fDP > *pfMax) *pfMax = fDP;

	fDP = vcV * v2;
	if (fDP < *pfMin) *pfMin = fDP;
	else if (fDP > *pfMax) *pfMax = fDP;
} */
/*----------------------------------------------------------------*/
/*
int VSOBB3::RelationWith(const VSTriangle3 & Triangle)const
{
	VSREAL     fMin0, fMax0, fMin1, fMax1;
	VSREAL     fD_C;
	VSVector3 vcV, vcTriEdge[3], vcA[3];
	VSVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);


	vcA[0] = m_A[0];
	vcA[1] = m_A[1];
	vcA[2] = m_A[2];


	vcTriEdge[0] = TrianglePoint[1] - TrianglePoint[0];
	vcTriEdge[1] = TrianglePoint[2] - TrianglePoint[0];

	vcV.Cross(vcTriEdge[0], vcTriEdge[1]);

	fMin0 = vcV * TrianglePoint[0];
	fMax0 = fMin0;

	OBBProj((*this), vcV, &fMin1, &fMax1);
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return VSNOINTERSECT;


	vcV = m_A[0];
	TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
	fD_C = vcV * m_Center;
	fMin1 = fD_C - m_fA[0];
	fMax1 = fD_C + m_fA[0];
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return VSNOINTERSECT;

	vcV = m_A[1];
	TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
	fD_C = vcV * m_Center;
	fMin1 = fD_C - m_fA[1];
	fMax1 = fD_C + m_fA[1];
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return VSNOINTERSECT;


	vcV = m_A[2];
	TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
	fD_C = vcV * m_Center;
	fMin1 = fD_C - m_fA[2];
	fMax1 = fD_C + m_fA[2];
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return VSNOINTERSECT;


	vcTriEdge[2] = vcTriEdge[1] - vcTriEdge[0];
	for (int j=0; j<3; j++) 
	{
		for (int k=0; k<3; k++) 
		{
			vcV.Cross(vcTriEdge[j], vcA[k]);

			TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
			OBBProj((*this), vcV, &fMin1, &fMax1);

			if ( (fMax1 < fMin0) || (fMax0 < fMin1) )
				return VSNOINTERSECT;
		}
	}

	return VSINTERSECT;
} // Intersects(Tri)
*/
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSTriangle3 & Triangle)const
{
	return Triangle.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSRectangle3 &Rectangle)const
{
	return Rectangle.RelationWith(*this);
}	
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSOBB3 &OBB)const
{
	VSREAL T[3];


	VSVector3 vcD = OBB.m_Center - m_Center;

	VSREAL matM[3][3];   
	VSREAL ra,           
		rb,           
		t;           


	matM[0][0] = m_A[0].Dot(OBB.m_A[0]);
	matM[0][1] = m_A[0].Dot(OBB.m_A[1]);
	matM[0][2] = m_A[0].Dot(OBB.m_A[2]);
	ra   = m_fA[0];
	rb   = OBB.m_fA[0] * ABS(matM[0][0]) + 
		OBB.m_fA[1] * ABS(matM[0][1]) + 
		OBB.m_fA[2] * ABS(matM[0][2]);

	T[0] = vcD.Dot(m_A[0]);
	t    = ABS(T[0]);
	if(t > (ra + rb) ) 
		return VSNOINTERSECT;


	matM[1][0] = m_A[1].Dot(OBB.m_A[0]);
	matM[1][1] = m_A[1].Dot(OBB.m_A[1]);
	matM[1][2] = m_A[1].Dot(OBB.m_A[2]);
	ra   = m_fA[1];
	rb   = OBB.m_fA[0] * ABS(matM[1][0]) + 
		OBB.m_fA[1] * ABS(matM[1][1]) + 
		OBB.m_fA[2] * ABS(matM[1][2]);
	T[1] = vcD.Dot(m_A[1]);
	t    = ABS(T[1]);
	if(t > (ra + rb) ) 
		return VSNOINTERSECT;


	matM[2][0] = m_A[2].Dot(OBB.m_A[0]);
	matM[2][1] = m_A[2].Dot(OBB.m_A[1]);
	matM[2][2] = m_A[2].Dot(OBB.m_A[2]);
	ra   = m_fA[2];
	rb   = OBB.m_fA[0] * ABS(matM[2][0]) + 
		OBB.m_fA[1] * ABS(matM[2][1]) + 
		OBB.m_fA[2] * ABS(matM[2][2]);
	T[2] = vcD.Dot(m_A[2]);
	t    = ABS(T[2]);
	if(t > (ra + rb) ) 
		return VSNOINTERSECT;


	ra = m_fA[0] * ABS(matM[0][0]) + 
		m_fA[1] * ABS(matM[1][0]) + 
		m_fA[2] * ABS(matM[2][0]);
	rb = OBB.m_fA[0];
	t = ABS( T[0]*matM[0][0] + T[1]*matM[1][0] + T[2]*matM[2][0] );
	if(t > (ra + rb) )
		return VSNOINTERSECT;


	ra = m_fA[0] * ABS(matM[0][1]) + 
		m_fA[1] * ABS(matM[1][1]) + 
		m_fA[2] * ABS(matM[2][1]);
	rb = OBB.m_fA[1];
	t = ABS( T[0]*matM[0][1] + T[1]*matM[1][1] + T[2]*matM[2][1] );
	if(t > (ra + rb) )
		return VSNOINTERSECT;


	ra = m_fA[0] * ABS(matM[0][2]) + 
		m_fA[1] * ABS(matM[1][2]) + 
		m_fA[2] * ABS(matM[2][2]);
	rb = OBB.m_fA[2];
	t = ABS( T[0]*matM[0][2] + T[1]*matM[1][2] + T[2]*matM[2][2] );
	if(t > (ra + rb) )
		return VSNOINTERSECT;


	ra = m_fA[1]*ABS(matM[2][0]) + m_fA[2]*ABS(matM[1][0]);
	rb = OBB.m_fA[1]*ABS(matM[0][2]) + OBB.m_fA[2]*ABS(matM[0][1]);
	t = ABS( T[2]*matM[1][0] - T[1]*matM[2][0] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[1]*ABS(matM[2][1]) + m_fA[2]*ABS(matM[1][1]);
	rb = OBB.m_fA[0]*ABS(matM[0][2]) + OBB.m_fA[2]*ABS(matM[0][0]);
	t = ABS( T[2]*matM[1][1] - T[1]*matM[2][1] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[1]*ABS(matM[2][2]) + m_fA[2]*ABS(matM[1][2]);
	rb = OBB.m_fA[0]*ABS(matM[0][1]) + OBB.m_fA[1]*ABS(matM[0][0]);
	t = ABS( T[2]*matM[1][2] - T[1]*matM[2][2] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[0]*ABS(matM[2][0]) + m_fA[2]*ABS(matM[0][0]);
	rb = OBB.m_fA[1]*ABS(matM[1][2]) + OBB.m_fA[2]*ABS(matM[1][1]);
	t = ABS( T[0]*matM[2][0] - T[2]*matM[0][0] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[0]*ABS(matM[2][1]) + m_fA[2]*ABS(matM[0][1]);
	rb = OBB.m_fA[0]*ABS(matM[1][2]) + OBB.m_fA[2]*ABS(matM[1][0]);
	t = ABS( T[0]*matM[2][1] - T[2]*matM[0][1] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[0]*ABS(matM[2][2]) + m_fA[2]*ABS(matM[0][2]);
	rb = OBB.m_fA[0]*ABS(matM[1][1]) + OBB.m_fA[1]*ABS(matM[1][0]);
	t = ABS( T[0]*matM[2][2] - T[2]*matM[0][2] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[0]*ABS(matM[1][0]) + m_fA[1]*ABS(matM[0][0]);
	rb = OBB.m_fA[1]*ABS(matM[2][2]) + OBB.m_fA[2]*ABS(matM[2][1]);
	t = ABS( T[1]*matM[0][0] - T[0]*matM[1][0] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[0]*ABS(matM[1][1]) + m_fA[1]*ABS(matM[0][1]);
	rb = OBB.m_fA[0] *ABS(matM[2][2]) + OBB.m_fA[2]*ABS(matM[2][0]);
	t = ABS( T[1]*matM[0][1] - T[0]*matM[1][1] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	ra = m_fA[0]*ABS(matM[1][2]) + m_fA[1]*ABS(matM[0][2]);
	rb = OBB.m_fA[0]*ABS(matM[2][1]) + OBB.m_fA[1]*ABS(matM[2][0]);
	t = ABS( T[1]*matM[0][2] - T[0]*matM[1][2] );
	if( t > ra + rb )
		return VSNOINTERSECT;


	return VSINTERSECT;
} 
/*----------------------------------------------------------------*/
int VSOBB3::RelationWith(const VSSphere3 &Sphere)const
{
	VSREAL fOBBParameter[3];
	VSREAL Dist = Sphere.m_Center.SquaredDistance(*this,fOBBParameter);
	Dist = SQRT(Dist);
	if(Dist - Sphere.m_fRadius < 0.0f)
		return VSINTERSECT;

	return VSNOINTERSECT;

}
