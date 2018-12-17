#include "VSRay3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSRay3::VSRay3()
{


}
/*----------------------------------------------------------------*/
VSRay3::VSRay3(const VSVector3 & Orig,const VSVector3 &Dir)
{
	Set(Orig,Dir);
}
/*----------------------------------------------------------------*/
VSRay3::~VSRay3()
{


}
/*----------------------------------------------------------------*/
bool VSRay3::GetParameter(const VSVector3 &Point,VSREAL &fRayParameter )const
{
	if(!VSLine3::GetParameter(Point,fRayParameter))
		return 0;

	if(fRayParameter < 0)
		return 0;

	return 1;

}
/*----------------------------------------------------------------*/
void VSRay3::Transform(const VSRay3 &Ray,const VSMatrix3X3W &Mat)
{

	VSLine3::Transform(Ray,Mat);
}