FORCEINLINE DWORD VSRenderer::ShaderVersion(UINT Major,UINT Minor)
{
	return (0xFFFE0000|((Major)<<8)|(Minor));
}
FORCEINLINE unsigned int VSRenderer::GetScreenWith()const
{	
	return m_uiScreenWidth;
}
FORCEINLINE unsigned int VSRenderer::GetScreenHeight()const
{
	return m_uiScreenHeight;
}
FORCEINLINE unsigned int VSRenderer::GetDisplayFormat()const
{
	return m_uiDisplayFormat;
}
FORCEINLINE unsigned int VSRenderer::GetCurAnisotropy()const
{
	return m_uiCurAnisotropy;
}
FORCEINLINE unsigned int VSRenderer::GetCurMultisample()const
{	
	return m_uiCurMultisample;
}
FORCEINLINE unsigned int VSRenderer::GetMaxMultisample()const
{
	return m_uiMaxMultisample;
}
FORCEINLINE unsigned int VSRenderer::GetDepthStencilFormat()const
{
	return m_uiDepthStencilFormat;
}
FORCEINLINE bool VSRenderer::IsWindowed()const
{
	return m_bWindowed;
}
FORCEINLINE unsigned int VSRenderer::WindoweNum()const
{
	if (m_bWindowed)
	{
		return m_iNumChildWindow;
	}
	return 0;
}
FORCEINLINE unsigned int VSRenderer::GetMaxTextureWidth()const
{
	return m_uiMaxTextureWidth;
}
FORCEINLINE unsigned int VSRenderer::GetMaxTextureHeight()const
{
	return m_uiMaxTextureHeight;
}
FORCEINLINE unsigned int VSRenderer::GetMaxUseClipPlane()const
{
	return m_uiMaxUseClipPlane;
}
FORCEINLINE unsigned int VSRenderer::GetMaxVTexture()const
{
	return m_uiMaxVTexture;
}
FORCEINLINE unsigned int VSRenderer::GetMaxGTexture()const
{
	return m_uiMaxGTexture;
}
FORCEINLINE unsigned int VSRenderer::GetMaxTexture()const
{
	return m_uiMaxTexture;
}
FORCEINLINE unsigned int VSRenderer::GetBufferFormat()const
{
	return m_uiBufferFormat;
}
FORCEINLINE unsigned int VSRenderer::GetMaxAnisotropy()const
{
	return m_uiMaxAnisotropy;
}
FORCEINLINE unsigned int VSRenderer::GetMaxRTNum()const
{
	return m_uiMaxRTNum;
}
FORCEINLINE VSString VSRenderer::GetAdapterName()const
{
	return m_AdapterName;
}
//----------------------------------------------------------------------------
FORCEINLINE void VSRenderer::SetClearColor (const VSColorRGBA& ClearColor)
{
	m_ClearColor = ClearColor;

}
//----------------------------------------------------------------------------
FORCEINLINE const VSColorRGBA& VSRenderer::GetClearColor () const
{
	return m_ClearColor;
}
//----------------------------------------------------------------------------
FORCEINLINE void VSRenderer::SetClearDepth (VSREAL fClearDepth)
{
	m_fClearDepth = fClearDepth;

}
//----------------------------------------------------------------------------
FORCEINLINE VSREAL VSRenderer::GetClearDepth () const
{
	return m_fClearDepth;
}
//----------------------------------------------------------------------------
FORCEINLINE void VSRenderer::SetClearStencil (unsigned int uiClearStencil)
{
	m_uiClearStencil = uiClearStencil;

}
//----------------------------------------------------------------------------
FORCEINLINE unsigned int VSRenderer::GetClearStencil () const
{
	return m_uiClearStencil;
}
FORCEINLINE unsigned int VSRenderer::GetCurRTWidth()const
{
	return m_uiCurRTWidth;
}
FORCEINLINE unsigned int VSRenderer::GetCurRTHeight()const
{
	return m_uiCurRTHeight;
}
FORCEINLINE unsigned int VSRenderer::GetCurRTMultisampler()const
{
	return m_uiCurRTMultisampler;
}
FORCEINLINE unsigned int VSRenderer::GetSuitableMultisample(unsigned int Multisample)const
{
	return Multisample > m_uiMaxMultisample ? m_uiMaxMultisample : Multisample;
}

