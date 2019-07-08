FORCEINLINE unsigned int VSTexture::GetFormatType()const
{
	return m_uiFormatType;
}
FORCEINLINE unsigned int VSTexture::GetBytePerPixel()const
{
	return VSRenderer::GetBytesPerPixel(m_uiFormatType);
}
FORCEINLINE unsigned int VSTexture::GetChannelPerPixel()const
{
	return VSRenderer::GetChannelPerPixel(m_uiFormatType);
}
FORCEINLINE unsigned int VSTexture::GetMipLevel()const
{
	return m_uiMipLevel;
}
FORCEINLINE unsigned char * VSTexture::GetBuffer(unsigned int uiLevel)const
{

	return m_pBufferArray[uiLevel];
}
