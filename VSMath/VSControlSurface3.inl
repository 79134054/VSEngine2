FORCEINLINE VSVector3 VSControlSurface3::GetControlPoint(unsigned int U,unsigned int V)const
{
	if(U > m_uiRow || V > m_uiColumn)
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	else
		return m_pControlPoint[U * m_uiColumn + V];

}
/*----------------------------------------------------------------*/
FORCEINLINE bool VSControlSurface3::ChangeControlPoint(unsigned int U,unsigned int V,const VSVector3 &Value)
{
	if(U > m_uiRow || V > m_uiColumn)
		return 0;
	else
	{
		m_pControlPoint[U * m_uiColumn + V] = Value;
		return 1;
	}


}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 *VSControlSurface3::GetControlPoint()const
{
	return m_pControlPoint;

}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSControlSurface3::GetControlPointNum()const
{
	return m_uiControlPointNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSControlSurface3::GetRow()const
{
	return m_uiRow;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSControlSurface3::GetColumn()const
{
	return m_uiColumn;
}
/*----------------------------------------------------------------*/