FORCEINLINE bool VSNURBSCurve3::ChangeWeight(unsigned int i,VSREAL Weight)
{
	if(!m_pWeight)
		return 0;
	else
	{
		if(i >= m_uiControlPointNum)
			return 0;
		else
			m_pWeight[i] = Weight;
		return 1;

	}

}
/*----------------------------------------------------------------*/
FORCEINLINE const VSREAL * VSNURBSCurve3::GetWeight()const
{
	return m_pWeight;

}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSNURBSCurve3::GetWeight(unsigned int i)const
{
	if(!m_pWeight)
		return VSMAX_REAL;
	else
	{
		if(i >= m_uiControlPointNum)
			return VSMAX_REAL;
		else
			return m_pWeight[i];

	}

}
/*----------------------------------------------------------------*/