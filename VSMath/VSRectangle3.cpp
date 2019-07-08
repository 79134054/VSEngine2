#include "VSRectangle3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSRectangle3::VSRectangle3()
{

}
/*----------------------------------------------------------------*/
VSRectangle3::~VSRectangle3()
{

}
/*----------------------------------------------------------------*/
VSRectangle3::VSRectangle3(const VSVector3 & Center,const VSVector3 &A0,const VSVector3 & A1,VSREAL fA0, VSREAL fA1)
{
	Set(Center,A0,A1,fA0, fA1);
}
/*----------------------------------------------------------------*/
VSRectangle3::VSRectangle3(const VSVector3 & Center,const VSVector3 A[2],const VSREAL fA[2])
{
	Set(Center,A,fA);
}
/*----------------------------------------------------------------*/
bool VSRectangle3::GetParameter(const VSVector3 &Point,VSREAL fRectangleParameter[2])const
{
	VSVector3 A2;
	A2.Cross(m_A[0],m_A[1]);
	VSMatrix3X3W m;
	m.CreateInWorldObject(m_A[0],m_A[1],A2,m_Center);
	VSVector3 Temp;
	Temp = Point * m;
	fRectangleParameter[0] = Temp.x;
	fRectangleParameter[1] = Temp.y;
	if(ABS(Temp.z) > EPSILON_E4 || ABS(Temp.x) > m_fA[0] || ABS(Temp.y) > m_fA[1])
		return 0;
	return 1;
	

}
/*----------------------------------------------------------------*/
void VSRectangle3::Transform(const VSRectangle3 &Rectangle, 
							 const VSMatrix3X3W &Mat)
{
	m_A[0] = Mat.Apply3X3(Rectangle.m_A[0]);
	m_A[1] = Mat.Apply3X3(Rectangle.m_A[1]);
	m_A[0].Normalize();
	m_A[1].Normalize();
	m_Center = Rectangle.m_Center * Mat;
	m_fA[0] = Rectangle.m_fA[0];
	m_fA[1] = Rectangle.m_fA[1];

	VSVector3 N;
	N.Cross(m_A[0],m_A[1]);
	VSPlane3(N,m_Center);
}
