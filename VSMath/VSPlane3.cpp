#include "VSMath.h"
#include "VSPlane3.h"
#include "VSLine3.h"
#include "VSTriangle3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSPlane3::VSPlane3()
{
	m_fD = 0.0f;
}
/*----------------------------------------------------------------*/
VSPlane3::VSPlane3(const VSVector3 &N , VSREAL fD)
{
	Set(N,fD);
}
/*----------------------------------------------------------------*/
VSPlane3::VSPlane3(const VSVector3 &N, const VSVector3 &P)
{
	Set(N,P);

}
/*----------------------------------------------------------------*/
VSPlane3::VSPlane3(const VSVector3 &P0,  const VSVector3 &P1, const VSVector3 &P2)
{
	Set(P0,P1,P2); 

}
/*----------------------------------------------------------------*/
VSPlane3::VSPlane3(const VSVector3 Point[3])
{
	Set(Point[0],Point[1],Point[2]);
}
/*----------------------------------------------------------------*/
void VSPlane3::Transform(const VSPlane3 &Plane,const VSMatrix3X3W &Mat)
{
	VSVector3W Temp(m_N.x,m_N.y,m_N.z,m_fD);


	VSMatrix3X3W Inv;
	Inv.InverseOf(Mat);
	VSMatrix3X3W Transpose;
	Transpose.TransposeOf(Inv);
	VSVector3W Resoult = Temp * Transpose;
	Set(VSVector3(Resoult.x, Resoult.y, Resoult.z), Resoult.w);
}
VSVector3 VSPlane3::ReflectDir(const VSVector3 & Dir)const
{
	VSVector3 TempN = m_N * (-1.0f);

	return Dir - m_N * Dir.Dot(m_N) * 2.0f;
}

