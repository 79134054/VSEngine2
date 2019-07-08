/*----------------------------------------------------------------*/
FORCEINLINE bool VSB_SplineCurve3::SetKnotVector(unsigned int i,VSREAL Value)
{
	if(!m_pKnotVector)
		return 0;
	if(i >= m_uiKnotNum)
		return 0;
	m_pKnotVector[i] = Value;
	return 1;

}
/*----------------------------------------------------------------*/
FORCEINLINE const VSREAL * VSB_SplineCurve3::GetpKnotVector()const
{

	return m_pKnotVector;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineCurve3:: GetKnotVectorValue(unsigned int i)const
{

	if(!m_pKnotVector)
		return VSMAX_REAL;
	if(i >= m_uiKnotNum)
		return VSMAX_REAL;
	return m_pKnotVector[i];
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineCurve3::SetBasisValue(int i, int j, VSREAL Value)
{
	m_pBasis[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineCurve3::GetBasisValue(int i ,int j)const
{
	return m_pBasis[ i * m_uiKnotNum + j];

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineCurve3::SetBasisFDValue(int i, int j, VSREAL Value)
{
	m_pBasisFD[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineCurve3::GetBasisFDValue(int i ,int j)const
{
	return m_pBasisFD[ i * m_uiKnotNum + j];
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineCurve3::SetBasisSDValue(int i, int j, VSREAL Value)
{
	m_pBasisSD[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineCurve3::GetBasisSDValue(int i ,int j)const
{
	return m_pBasisSD[ i * m_uiKnotNum + j];
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineCurve3::SetBasisTDValue(int i, int j, VSREAL Value)
{
	m_pBasisTD[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineCurve3::GetBasisTDValue(int i ,int j)const
{
	return m_pBasisTD[ i * m_uiKnotNum + j];

}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSB_SplineCurve3::GetDegree()const
{
	return m_uiDegree;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSB_SplineCurve3::GetKnotNum()const
{
	return m_uiKnotNum;
}
/*----------------------------------------------------------------*/
