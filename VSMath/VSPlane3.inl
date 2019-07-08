/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 & VSPlane3::GetN()const 
{
	return m_N;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSPlane3::GetPoint()const 
{
	if (ABS(m_N.z) < EPSILON_E4)
	{
		return VSVector3(0.0f,0.0f,0.0f);
	}
	return VSVector3(0.0f,0.0f,- m_fD / m_N.z);
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSPlane3::GetfD()const 
{
	return m_fD;
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSPlane3::Set(const VSVector3 &N, const VSVector3 &P) 
{
	m_N = N ;
	m_N.Normalize(); 
	m_fD = -( m_N.Dot(P));
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSPlane3::Set(const VSVector3 &P0,  const VSVector3 &P1, const VSVector3 &P2) 
{
	VSVector3 vcEdge1 = P1 - P0;
	VSVector3 vcEdge2 = P2 - P0;

	m_N.Cross(vcEdge1, vcEdge2);
	m_N.Normalize();
	m_fD = -(m_N.Dot(P0));
}
/*----------------------------------------------------------------*/
FORCEINLINE void  VSPlane3::Set(const VSVector3 Point[3])
{

	Set(Point[0],Point[1],Point[2]);
}
/*----------------------------------------------------------------*/
FORCEINLINE VSPlane3 VSPlane3::GetPlane()const
{
	return *this;

}
/*----------------------------------------------------------------*/
FORCEINLINE void  VSPlane3::Set(const VSVector3 &N , VSREAL fD)
{
	VSREAL Len = N.GetLength();
	m_N = N / Len;

	m_fD = fD / Len;
}