#include "VSCurve3.h"
#include "VSMatrix3X3W.h"
using namespace VSEngine2;
VSCurve3::VSCurve3()
{

}
/*----------------------------------------------------------------*/
VSCurve3::~VSCurve3()
{


}
/*----------------------------------------------------------------*/
void VSCurve3::GetFixedUpBNT(VSREAL t ,VSVector3 & B,VSVector3 & N,VSVector3& T)
{
	VSMatrix3X3W Mat;
	VSVector3 Point(0,0,0);
	VSVector3 FD = GetFirstDerivative(t);
	FD.Normalize();
	Mat.CreateFromLookAt(Point,FD);
	B.x = Mat._00;
	B.y = Mat._10;
	B.z = Mat._20;

	N.x = Mat._01;
	N.y = Mat._11;
	N.z = Mat._21;

	T.x = Mat._02;
	T.y = Mat._12;
	T.z = Mat._22;
}
/*----------------------------------------------------------------*/