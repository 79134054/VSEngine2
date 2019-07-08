FORCEINLINE VSREAL VSBezierCurve3::GetC(int i,int j)const
{
	return m_C[m_uiControlPointNum * i + j];
}
FORCEINLINE VSREAL VSBezierCurve3::SetC(int i,int j,VSREAL Value)
{
	return m_C[m_uiControlPointNum * i + j] = Value;
}