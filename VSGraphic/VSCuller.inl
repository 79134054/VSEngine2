FORCEINLINE unsigned int VSCuller::GetPlaneNum()const
{
	return m_uiPlaneNum;

}
FORCEINLINE void VSCuller::SetPlaneState(unsigned int uiPlaneState)
{
	m_uiPlaneState = uiPlaneState;
}
FORCEINLINE unsigned int VSCuller::GetPlaneState()const
{

	return m_uiPlaneState;
}
FORCEINLINE unsigned int VSCuller::GetAllVisibleNum()const
{
	unsigned int uiNum = 0;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < VST_MAX ; i++)
		{
			uiNum += m_VisibleSet[j][i].GetNum();
		}
	}
	
	return uiNum;
}
FORCEINLINE unsigned int VSCuller::GetVisibleNum(unsigned int uiVisibleSetType,unsigned int uiRenderGroup)const
{
	if (uiVisibleSetType >= VST_MAX || uiRenderGroup >= RG_MAX)
	{
		return 0;
	}
	return m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum();
}
FORCEINLINE unsigned int VSCuller::GetRenderGroupVisibleNum(unsigned int uiRenderGroup)const
{
	unsigned int uiNum = 0;
	for (unsigned int i = 0 ; i < VST_MAX ; i++)
	{
		uiNum += m_VisibleSet[uiRenderGroup][i].GetNum();
	}
	return uiNum;
}
FORCEINLINE void VSCuller::ClearVisibleSet()
{
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < VST_MAX ; i++)
		{
			m_VisibleSet[j][i].Clear();
		}
	}
	
}
FORCEINLINE void VSCuller::ClearPlaneState()
{
	m_uiPlaneState =0xffffffff;
}
FORCEINLINE void VSCuller::ClearAllPlane()
{
	m_uiPlaneNum = 0;
}
FORCEINLINE void VSCuller::SetCamera(VSCamera &Camera)
{
	m_pCamera = &Camera;
}
FORCEINLINE VSRenderContext& VSCuller::GetVisibleSpatial(unsigned int i,unsigned int uiVisibleSetType,unsigned int uiRenderGroup)const
{
	if(uiVisibleSetType >= VST_MAX || uiRenderGroup > RG_MAX || i >=m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum())
	{
		VSMAC_ASSERT(0);
		return VSRenderContext::ms_RenderContextNULL;
	}
	return m_VisibleSet[uiRenderGroup][uiVisibleSetType][i];
}
FORCEINLINE unsigned int VSCuller::GetLightNum()const
{
	return m_LightSet.GetNum();
}
FORCEINLINE VSLight * VSCuller::GetLight(unsigned int i)const
{
	if (i >= m_LightSet.GetNum())
	{
		return NULL;
	}

	return m_LightSet[i];
}
FORCEINLINE void VSCuller::ClearLight()
{
	m_LightSet.Clear();
}
FORCEINLINE VSCamera * VSCuller::GetCamera()const
{
	return m_pCamera;
}