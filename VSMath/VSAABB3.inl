FORCEINLINE void VSAABB3::Set(const VSVector3 & Max , const VSVector3 & Min)
{
	m_Max = Max;
	m_Min = Min;
	m_Center = (Max + Min) / 2.0f;
	VSVector3 Temp = (Max - Min) / 2.0f;
	for(int i = 0 ; i < 3; i++)
		m_fA[i] = Temp.m[i];

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSAABB3::Set(const VSVector3 & Center,VSREAL fA0,VSREAL fA1,VSREAL fA2)
{
	m_fA[0] = abs(fA0);
	m_fA[1] = abs(fA1);
	m_fA[2] = abs(fA2);
	
	m_Center = Center;
	
	m_Max.Set(m_Center.x + m_fA[0],m_Center.y + m_fA[1],m_Center.z + m_fA[2]);
	m_Min.Set(m_Center.x - m_fA[0],m_Center.y - m_fA[1],m_Center.z - m_fA[2]);
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSAABB3::Set(const VSVector3 & Center,VSREAL fA[3])
{
	m_fA[0] = abs(fA[0]);
	m_fA[1] = abs(fA[1]);
	m_fA[2] = abs(fA[2]);

	m_Center = Center;

	m_Max.Set(m_Center.x + m_fA[0],m_Center.y + m_fA[1],m_Center.z + m_fA[2]);
	m_Min.Set(m_Center.x - m_fA[0],m_Center.y - m_fA[1],m_Center.z - m_fA[2]);

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSAABB3::GetfA(VSREAL fA[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		fA[i] = m_fA[i];
	}

}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 & VSAABB3::GetCenter()const
{
	
	return m_Center;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSAABB3::GetParameterPoint(VSREAL fAABBParameter[3])const
{

	return VSVector3(m_Center.x + fAABBParameter[0],m_Center.y + fAABBParameter[1] ,m_Center.z + fAABBParameter[2]);
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSAABB3::GetParameterPoint(VSREAL fAABBParameter0,VSREAL fAABBParameter1,VSREAL fAABBParameter2)const
{

	return VSVector3(m_Center.x + fAABBParameter0,m_Center.y + fAABBParameter1 ,m_Center.z + fAABBParameter2);
}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 & VSAABB3::GetMaxPoint()const
{
	return m_Max;
}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 & VSAABB3::GetMinPoint()const
{	
	return m_Min;
}