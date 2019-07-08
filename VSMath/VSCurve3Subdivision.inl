FORCEINLINE VSVector3 *VSCurve3Subdivision::GetPoints()const
{
	return m_pPoint;
}
FORCEINLINE unsigned int VSCurve3Subdivision::GetPointNum()const
{

	return m_uiPointNum;
}
FORCEINLINE VSControlCurve3 *VSCurve3Subdivision::GetCurve()const
{

	return m_pCurve;
}
FORCEINLINE VSREAL *VSCurve3Subdivision::GetT()const
{
	return m_pT;

}