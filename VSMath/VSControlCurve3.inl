FORCEINLINE bool VSControlCurve3::ChangeControlPoint(unsigned int i,const VSVector3 & ControlPoint)
{
	if(!m_pControlPoint)
		return 0;
	else
	{
		if(i >= m_uiControlPointNum)
			return 0;
		else
			m_pControlPoint[i] = ControlPoint;
		return 1;

	}

}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 * VSControlCurve3::GetpControlPoint()const
{

	return m_pControlPoint;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSControlCurve3::GetControlPoint(unsigned int i)const
{
	if(!m_pControlPoint)
		return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
	else
	{
		if(i >= m_uiControlPointNum)
			return VSVector3(VSMAX_REAL,VSMAX_REAL,VSMAX_REAL);
		else
			return m_pControlPoint[i];

	}
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSControlCurve3::GetControlPointNum()const
{

	return m_uiControlPointNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSControlCurve3::GetType()const
{
	return m_CurveType;
}