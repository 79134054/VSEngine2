#include "VSTriangle3.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSTriangle3::VSTriangle3()
{

}
/*----------------------------------------------------------------*/
VSTriangle3::~VSTriangle3()
{

}
/*----------------------------------------------------------------*/
VSTriangle3::VSTriangle3(const VSVector3 & P0,const VSVector3 &P1,const VSVector3 &P2)
{
	Set(P0,P1,P2);
	
}
/*----------------------------------------------------------------*/
VSTriangle3::VSTriangle3(const VSVector3 Point[3])
{
	Set(Point);
}
/*----------------------------------------------------------------*/
bool VSTriangle3::GetParameter(const VSVector3 &Point,VSREAL fTriangleParameter[3])const
{
	VSVector3 v01, v02,v0p;
	VSVector3 vp1, vp2;
	v01 = m_V[1] - m_V[0];
	v02 = m_V[2] - m_V[0];
	v0p = Point - m_V[0];
	vp1 = m_V[1] - Point;
	vp2 = m_V[2] - Point;

	VSVector3 S[4];

	S[0].Cross(v01,v02);
	S[1].Cross(v01,v0p);
	S[2].Cross(v0p,v02);
	S[3].Cross(vp1,vp2);

	VSREAL fS[4];
	for(int i = 0 ;i < 4 ; i++)
	{
		fS[i] = S[i].GetLength();
	}
	for(int i = 0 ; i < 3 ; i++)
	{
		fTriangleParameter[i] = fS[i+1] / fS[0];
	}
	VSREAL fSum = 0.0f;
	for(int i = 0 ; i < 3 ; i++)
	{
		fSum += fTriangleParameter[i];
		if(fTriangleParameter[i] < 0.0f || fTriangleParameter[i] > 1.0f)
			return 0;

	}
	if(fSum > 1.0f)
		return 0;
	return 1;
	/*VSVector3 V[3];
	V[0] = Point - m_V[0];
	V[1] = m_V[1] - m_V[0];
	V[2] = m_V[2] - m_V[0];
	VSREAL s1,t1,s2,t2,k1,k2;
	bool flag1 = 0;
	bool flag2 = 0;
	if(ABS(V[1].x) > EPSILON_E4 && ABS(V[2].x) > EPSILON_E4)
	{
		s1 = V[1].x;
		t1 = V[2].x;
		k1 = V[0].x;
		flag1 = 1;
	}

	if(ABS(V[1].y) > EPSILON_E4 && ABS(V[2].y) > EPSILON_E4)
	{
		if(flag1 ==0)
		{
			s1 = V[1].y;
			t1 = V[2].y;
			k1 = V[0].y;
			flag1 = 1;
		}
		else
		{
			if(ABS((V[1].x * V[2].y) - (V[1].y * V[2].x))> EPSILON_E4)
			{
				flag2 = 1;
				s2 = V[1].y;
				t2 = V[2].y;
				k2 = V[0].y;
			}
		}

	}

	if(ABS(V[1].z) > EPSILON_E4 && ABS(V[2].z) > EPSILON_E4)
	{
		if(flag1 ==0)
		{
			return 0;
		}
		else
		{
			if(flag2 == 0)
			{
				s2 = V[1].z;
				t2 = V[2].z;
				k2 = V[0].z;
				flag2 = 1;
			}
		}

	}

	fTriangleParameter[1] = (t1 * k2 - t2 * k1) / (t1 * s2 - t2 * s1);
	fTriangleParameter[2] = (s1 * k2 - s2 * k1) / (s1 * t2 - s2 * t1);
	fTriangleParameter[0] = 1.0f - fTriangleParameter[1] - fTriangleParameter[2];

	if(fTriangleParameter[0] > 1.0f || fTriangleParameter[1] > 1.0f || fTriangleParameter[2] > 1.0f)
		return 0;
	return 1;
	*/
}
void VSTriangle3::Transform(const VSTriangle3 & Triangle,const VSMatrix3X3W &Mat)
{
	m_V[0] = Triangle.m_V[0] * Mat;
	m_V[1] = Triangle.m_V[1] * Mat;
	m_V[2] = Triangle.m_V[2] * Mat;
	Set(m_V);
}
