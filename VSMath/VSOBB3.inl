/*----------------------------------------------------------------*/
FORCEINLINE void VSOBB3::Set(const VSVector3 A[3],VSREAL fA[3],const VSVector3 & Center)
{
	m_A[0] = A[0];
	m_A[1] = A[1];
	m_A[2] = A[2];
	m_A[0].Normalize();
	m_A[1].Normalize();
	m_A[2].Normalize();
	m_fA[0] = ABS(fA[0]);
	m_fA[1] = ABS(fA[1]);
	m_fA[2] = ABS(fA[2]);
	m_Center = Center;

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSOBB3::Set(const VSVector3 &A0,const VSVector3 &A1,const VSVector3 &A2,
						VSREAL fA0,VSREAL fA1,VSREAL fA2,
						const VSVector3 & Center)
{

	m_A[0] = A0;
	m_A[1] = A1;
	m_A[2] = A2;

	m_A[0].Normalize();
	m_A[1].Normalize();
	m_A[2].Normalize();

	m_fA[0] = ABS(fA0);
	m_fA[1] = ABS(fA1);
	m_fA[2] = ABS(fA2);
	m_Center = Center;
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSOBB3::GetA(VSVector3 A[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		A[i] = m_A[i];
	}
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSOBB3::GetfA(VSREAL fA[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		fA[i] = m_fA[i];
	}
}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 & VSOBB3::GetCenter()const
{

	return m_Center;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSOBB3::GetParameterPoint(VSREAL fOBBParameter[3])const
{
	return (m_Center + m_A[0] * fOBBParameter[0] + m_A[1] * fOBBParameter[1] + m_A[2] * fOBBParameter[2]);

}
FORCEINLINE VSVector3 VSOBB3::GetParameterPoint(VSREAL fOBBParameter0,VSREAL fOBBParameter1,VSREAL fOBBParameter2)const
{

	return (m_Center + m_A[0] * fOBBParameter0 + m_A[1] * fOBBParameter1 + m_A[2] * fOBBParameter2);

}