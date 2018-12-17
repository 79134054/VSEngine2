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
using namespace VSEngine2;
/*----------------------------------------------------------------*/
int VSVector3::RelationWith(const VSSphere3 & Sphere)const
{
	VSREAL sqrDist = SquaredDistance(Sphere.m_Center);
	VSREAL diff = sqrDist - Sphere.m_fRadius * Sphere.m_fRadius;
	if(diff > EPSILON_E4)
		return VSOUT;
	else if(diff < -EPSILON_E4)
		return VSIN;
	else 
		return VSON;

}
/*----------------------------------------------------------------*/
int VSVector3::RelationWith(const VSPlane3 &Plane)const
{
	return Plane.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int VSVector3::RelationWith(const VSOBB3 &OBB)const
{
	VSMatrix3X3W m;
	OBB.GetTransform(m);
	VSVector3 PointTemp = *this * m;
	VSREAL fA[3];
	OBB.GetfA(fA);
	VSREAL fDiffX = ABS(PointTemp.x) - fA[0];
	VSREAL fDiffY = ABS(PointTemp.y) - fA[1];
	VSREAL fDiffZ = ABS(PointTemp.z) - fA[2];

	if(fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
		return VSOUT;
	
	if(fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
		return VSIN;

	return VSON;

}
/*----------------------------------------------------------------*/
//VSIN VSOUT VSON
int VSVector3::RelationWith(const VSAABB3 &AABB)const
{

	return AABB.RelationWith(*this);
}