/*----------------------------------------------------------------*/
FORCEINLINE bool VSB_SplineSurface3::SetVKnotVector(unsigned int i,VSREAL Value)
{
	if(!m_pVKnotVector)
		return 0;
	if(i >= m_uiVKnotNum)
		return 0;
	m_pVKnotVector[i] = Value;
	return 1;

}
/*----------------------------------------------------------------*/
FORCEINLINE const VSREAL * VSB_SplineSurface3::GetpVKnotVector()const
{

	return m_pVKnotVector;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineSurface3:: GetVKnotVectorValue(unsigned int i)const
{

	if(!m_pVKnotVector)
		return VSMAX_REAL;
	if(i >= m_uiVKnotNum)
		return VSMAX_REAL;
	return m_pVKnotVector[i];
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineSurface3::SetVBasisValue(int i, int j, VSREAL Value)
{
	m_pVBasis[ i * m_uiVKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineSurface3::GetVBasisValue(int i ,int j)const
{
	return m_pVBasis[ i * m_uiVKnotNum + j];

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineSurface3::SetVBasisFDValue(int i, int j, VSREAL Value)
{
	m_pVBasisFD[ i * m_uiUKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineSurface3::GetVBasisFDValue(int i ,int j)const
{
	return m_pVBasisFD[ i * m_uiVKnotNum + j];
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSB_SplineSurface3::GetVDegree()const
{
	return m_uiVDegree;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSB_SplineSurface3::GetVKnotNum()const
{
	return m_uiVKnotNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE bool VSB_SplineSurface3::SetUKnotVector(unsigned int i,VSREAL Value)
{
	if(!m_pUKnotVector)
		return 0;
	if(i >= m_uiUKnotNum)
		return 0;
	m_pUKnotVector[i] = Value;
	return 1;

}
/*----------------------------------------------------------------*/
FORCEINLINE const VSREAL * VSB_SplineSurface3::GetpUKnotVector()const
{

	return m_pUKnotVector;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineSurface3:: GetUKnotVectorValue(unsigned int i)const
{

	if(!m_pUKnotVector)
		return VSMAX_REAL;
	if(i >= m_uiUKnotNum)
		return VSMAX_REAL;
	return m_pUKnotVector[i];
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineSurface3::SetUBasisValue(int i, int j, VSREAL Value)
{
	m_pUBasis[ i * m_uiUKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineSurface3::GetUBasisValue(int i ,int j)const
{
	return m_pUBasis[ i * m_uiUKnotNum + j];

}
/*----------------------------------------------------------------*/
FORCEINLINE void VSB_SplineSurface3::SetUBasisFDValue(int i, int j, VSREAL Value)
{
	m_pUBasisFD[ i * m_uiUKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSB_SplineSurface3::GetUBasisFDValue(int i ,int j)const
{
	return m_pUBasisFD[ i * m_uiUKnotNum + j];
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSB_SplineSurface3::GetUDegree()const
{
	return m_uiUDegree;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSB_SplineSurface3::GetUKnotNum()const
{
	return m_uiUKnotNum;
}
/*----------------------------------------------------------------*/