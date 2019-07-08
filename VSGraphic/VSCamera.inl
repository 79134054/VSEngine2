FORCEINLINE const VSMatrix3X3W & VSCamera::GetViewMatrix()const
{
	return m_ViewMat;

}
// FORCEINLINE const VSVector3& VSCamera::GetWorldPos()const
// {
// 	return m_WorldPos;
// }
// FORCEINLINE void VSCamera::GetLocalDirection(VSVector3 &LocalDir,VSVector3 &LocalUp,VSVector3 & LocalRight)const
// {
// 	LocalDir = m_LocalDir;
// 	LocalUp = m_LocalUp;
// 	LocalRight = m_LocalRight;
// 
// }
// FORCEINLINE void VSCamera::GetWorldDirection(VSVector3 &WorldDir,VSVector3 &WorldUp,VSVector3 & WorldRight)const
// {
// 	WorldDir = m_WorldDir;
// 	WorldUp = m_WorldUp;
// 	WorldRight = m_WorldRight;
// 
// }
FORCEINLINE const VSMatrix3X3W & VSCamera::GetProjMatrix()const
{
	return m_ProjMat;
}
FORCEINLINE VSViewPort * VSCamera::GetViewPort(unsigned int i )const
{
	return &m_ViewPort[i];
}
FORCEINLINE bool VSCamera::AddViewPort(const VSViewPort &ViewPort)//йс©з		
{
	if (ViewPort.XMin < 0 || ViewPort.XMax > 1.0f || ViewPort.XMin > ViewPort.XMax ||
		ViewPort.YMin < 0 ||ViewPort.YMax > 1.0f || ViewPort.YMin > ViewPort.YMax ||
		ViewPort.ZMax > 1.0f || ViewPort.ZMin < 0.0f || ViewPort.ZMin > ViewPort.ZMax)
	{
		return false;
	}
	m_ViewPort.AddElement(ViewPort);
	return true;
}
// FORCEINLINE const VSVector3 &VSCamera::GetLocalDir()const
// {
// 	return m_LocalDir;
// }
// FORCEINLINE const VSVector3 &VSCamera::GetLocalUp()const
// {
// 	return m_LocalUp;
// }
// FORCEINLINE const VSVector3 &VSCamera::GetLocalRight()const
// {
// 	return m_LocalRight;
// }
// FORCEINLINE const VSVector3 &VSCamera::GetWorldDir()const
// {
// 	return m_WorldDir;
// }
// FORCEINLINE const VSVector3 &VSCamera::GetWorldUp()const
// {
// 	return m_WorldUp;
// }
// FORCEINLINE const VSVector3 &VSCamera::GetWorldRight()const
// {
// 	return m_WorldRight;
// }
