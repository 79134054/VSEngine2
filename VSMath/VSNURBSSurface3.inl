/*----------------------------------------------------------------*/
FORCEINLINE bool VSNURBSSurface3::ChangeWeight(unsigned int U,unsigned int V,VSREAL Weight)
{
	if(U > m_uiRow || V > m_uiColumn)
		return 0;
	else
	{
		m_pWeight[U * m_uiColumn + V] = Weight;
		return 1;
	}
}
/*----------------------------------------------------------------*/
FORCEINLINE const VSREAL * VSNURBSSurface3::GetWeight()const
{
	return m_pWeight;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSNURBSSurface3::GetWeight(unsigned int U,unsigned int V)const
{
	if(U > m_uiRow || V > m_uiColumn)
		return VSMAX_REAL;
	else
		return m_pWeight[U * m_uiColumn + V];

}
/*----------------------------------------------------------------*/