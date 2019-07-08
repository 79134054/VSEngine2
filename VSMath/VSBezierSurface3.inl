FORCEINLINE VSREAL VSBezierSurface3::GetUC(int i,int j)const
{
	return m_UC[i * m_uiRow + j];
	
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSBezierSurface3::SetUC(int i,int j,VSREAL Value)
{
	return m_UC[i * m_uiRow + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSBezierSurface3::GetVC(int i,int j)const
{
	return m_VC[i * m_uiColumn + j];

}
/*----------------------------------------------------------------*/
FORCEINLINE VSREAL VSBezierSurface3::SetVC(int i,int j,VSREAL Value)
{
	return m_VC[i * m_uiColumn + j] = Value;
}
FORCEINLINE void VSBezierSurface3::SetUPD(int i, int j,const VSVector3 & Value)
{
	m_UPDValue[i * m_uiColumn + j] = Value;

}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSBezierSurface3::GetUPD(int i, int j)
{

	return m_UPDValue[i * m_uiColumn + j];
}
/*----------------------------------------------------------------*/
FORCEINLINE void VSBezierSurface3::SetVPD(int i, int j,const VSVector3 & Value)
{

	m_VPDValue[i * (m_uiColumn- 1) + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSBezierSurface3::GetVPD(int i, int j)
{

	return m_VPDValue[i * (m_uiColumn- 1) + j];
}
/*----------------------------------------------------------------*/