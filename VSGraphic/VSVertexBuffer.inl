FORCEINLINE VSDataBuffer * VSVertexBuffer::GetPositionDate(unsigned int uiLevel)const
{
	if(uiLevel < (unsigned int)m_pDate[VSVertexFormat::VF_POSITION].GetNum())
		return m_pDate[VSVertexFormat::VF_POSITION][uiLevel];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetNormalDate(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pDate[VSVertexFormat::VF_NORMAL].GetNum())
		return m_pDate[VSVertexFormat::VF_NORMAL][uiLevel];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetPSizeDate()const
{	
	if(m_pDate[VSVertexFormat::VF_PSIZE].GetNum())
		return m_pDate[VSVertexFormat::VF_PSIZE][0];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetColorDate(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pDate[VSVertexFormat::VF_COLOR].GetNum())
		return m_pDate[VSVertexFormat::VF_COLOR][uiLevel];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetTexCoordDate(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pDate[VSVertexFormat::VF_TEXCOORD].GetNum())
		return m_pDate[VSVertexFormat::VF_TEXCOORD][uiLevel];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetBlendWeightDate()const
{
	if(m_pDate[VSVertexFormat::VF_BLENDWEIGHT].GetNum())
		return m_pDate[VSVertexFormat::VF_BLENDWEIGHT][0];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetBlendIndicesDate()const
{
	if(m_pDate[VSVertexFormat::VF_BLENDINDICES].GetNum())
		return m_pDate[VSVertexFormat::VF_BLENDINDICES][0];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetTangentDate()const
{
	if(m_pDate[VSVertexFormat::VF_TANGENT].GetNum())
		return m_pDate[VSVertexFormat::VF_TANGENT][0];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetBinormalDate()const
{
	if(m_pDate[VSVertexFormat::VF_BINORMAL].GetNum())
		return m_pDate[VSVertexFormat::VF_BINORMAL][0];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetFogDate()const
{
	if(m_pDate[VSVertexFormat::VF_FOG].GetNum())
		return m_pDate[VSVertexFormat::VF_FOG][0];
	else
		return NULL;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetDepthDate()const
{
	if(m_pDate[VSVertexFormat::VF_DEPTH].GetNum())
		return m_pDate[VSVertexFormat::VF_DEPTH][0];
	else
		return NULL;
}
FORCEINLINE unsigned int VSVertexBuffer::GetPositionLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_POSITION);
}
FORCEINLINE unsigned int VSVertexBuffer::GetColorLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_COLOR);
}
FORCEINLINE unsigned int VSVertexBuffer::GetNormalLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_NORMAL);
}
FORCEINLINE unsigned int VSVertexBuffer::GetTexCoordLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_TEXCOORD);
}
FORCEINLINE unsigned int VSVertexBuffer::GetVertexNum()const
{
	return m_uiVertexNum;
}
FORCEINLINE VSVertexFormat * VSVertexBuffer::GetVertexFormat()const
{
	return m_pVertexFormat;
}
FORCEINLINE unsigned int VSVertexBuffer::GetOneVertexSize()const
{
	return m_uiOneVertexSize;
}
FORCEINLINE VSDataBuffer * VSVertexBuffer::GetDate(unsigned int uiVF,unsigned int uiLevel)const
{
	if(uiVF >= VSVertexFormat::VF_MAX)
		return NULL;
	if(uiLevel >= (unsigned int)m_pDate[uiVF].GetNum())
		return NULL;
	return m_pDate[uiVF][uiLevel];
}
FORCEINLINE unsigned int VSVertexBuffer::GetLevel(unsigned int uiVF)const
{
	if (uiVF >= VSVertexFormat::VF_MAX)
		return 0;
	else
		return GetSemanticsNum(uiVF);
}
FORCEINLINE bool VSVertexBuffer::GetLockDateOffSet(unsigned int uiVF,unsigned int uiLevel,unsigned int &uiOffSet)const
{
	if (!m_pVertexFormat)
	{
		VSDataBuffer * pDate = NULL;
		for(unsigned int i = 0 ; i < uiVF ; i++)
		{
			for (unsigned int j = 0 ; j < m_pDate[i].GetNum() ; j++)
			{
				pDate = GetDate(i,j);
				if(pDate)
					uiOffSet += pDate->GetStride();
			}
		}
		for (unsigned int j = 0 ; j < uiLevel ; j++)
		{
			pDate = GetDate(uiVF,j);
			if(pDate)
				uiOffSet += pDate->GetStride();
		}
		if (!pDate)
		{
			return false;
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0 ;i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if(m_pVertexFormat->m_FormatArray[i].Semantics == uiVF)
			{
				if (m_pVertexFormat->m_FormatArray[i].SemanticsIndex == uiLevel)
				{
					uiOffSet = m_pVertexFormat->m_FormatArray[i].OffSet;
					return true;
				}
			}
		}
		return false;
	}
	
}
FORCEINLINE void *VSVertexBuffer::GetLockDate(unsigned int uiVF,unsigned int uiLevel)const
{
	if(uiVF >= VSVertexFormat::VF_MAX || !m_pLockDate)
		return NULL;
	if(GetSemanticsNum(uiVF) <= uiLevel)
		return NULL;

	unsigned int uiOffSet = 0;
	if(!GetLockDateOffSet(uiVF,uiLevel,uiOffSet))
		return	NULL;	
	return (unsigned char *)m_pLockDate + uiOffSet; 
}
FORCEINLINE void *VSVertexBuffer::GetLockPositionDate(unsigned int uiLevel)const
{
	return GetLockDate(VSVertexFormat::VF_POSITION,uiLevel);
}
FORCEINLINE void *VSVertexBuffer::GetLockNormalDate(unsigned int uiLevel)const
{
	return GetLockDate(VSVertexFormat::VF_NORMAL,uiLevel);
}
FORCEINLINE void *VSVertexBuffer::GetLockPSizeDate()const
{
	return GetLockDate(VSVertexFormat::VF_PSIZE,0);
}
FORCEINLINE void *VSVertexBuffer::GetLockColorDate(unsigned int uiLevel)const
{
	return GetLockDate(VSVertexFormat::VF_COLOR,uiLevel);
}
FORCEINLINE void *VSVertexBuffer::GetLockBlendWeightDate()const
{
	return GetLockDate(VSVertexFormat::VF_BLENDWEIGHT,0);
}
FORCEINLINE void *VSVertexBuffer::GetLockBlendIndicesDate()const
{
	return GetLockDate(VSVertexFormat::VF_BLENDINDICES,0);	
}
FORCEINLINE void *VSVertexBuffer::GetLockTangentDate()const
{
	return GetLockDate(VSVertexFormat::VF_TANGENT,0);
}
FORCEINLINE void *VSVertexBuffer::GetLockBinormalDate()const
{
	return GetLockDate(VSVertexFormat::VF_BINORMAL,0);
}
FORCEINLINE void *VSVertexBuffer::GetLockFogDate()const
{
	return GetLockDate(VSVertexFormat::VF_FOG,0);
}
FORCEINLINE void *VSVertexBuffer::GetLockDepthDate()const
{
	return GetLockDate(VSVertexFormat::VF_DEPTH,0);
}
FORCEINLINE void *VSVertexBuffer::GetLockTexCoordDate(unsigned int uiLevel)const
{
	return GetLockDate(VSVertexFormat::VF_TEXCOORD,uiLevel);
}
FORCEINLINE bool VSVertexBuffer::HavePositionInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_POSITION) > uiLevel;
}
FORCEINLINE bool VSVertexBuffer::HaveNormalInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_NORMAL) > uiLevel;
}
FORCEINLINE bool VSVertexBuffer::HavePSizeInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_PSIZE) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveColorInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_COLOR) > uiLevel;
}
FORCEINLINE bool VSVertexBuffer::HaveBlendWeightInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_BLENDWEIGHT) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveBlendIndicesInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_BLENDINDICES) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveTangentInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_TANGENT) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveBinormalInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_BINORMAL) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveFogInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_FOG) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveDepthInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_DEPTH) > 0;
}
FORCEINLINE bool VSVertexBuffer::HaveTexCoordInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_TEXCOORD) > uiLevel;
}
FORCEINLINE unsigned int VSVertexBuffer::PositionChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_POSITION,uiLevel);
}
FORCEINLINE unsigned int VSVertexBuffer::NormalChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_NORMAL,uiLevel);
}
FORCEINLINE unsigned int VSVertexBuffer::PSizeChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_PSIZE,0);
}
FORCEINLINE unsigned int VSVertexBuffer::ColorChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_COLOR,uiLevel);
}
FORCEINLINE unsigned int VSVertexBuffer::BlendWeightChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_BLENDWEIGHT,0);
}
FORCEINLINE unsigned int VSVertexBuffer::BlendIndicesChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_BLENDINDICES,0);
}
FORCEINLINE unsigned int VSVertexBuffer::TangentChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_TANGENT,0);
}
FORCEINLINE unsigned int VSVertexBuffer::BinormalChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_BINORMAL,0);
}
FORCEINLINE unsigned int VSVertexBuffer::FogChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_FOG,0);
}
FORCEINLINE unsigned int VSVertexBuffer::DepthChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_DEPTH,0);
}
FORCEINLINE unsigned int VSVertexBuffer::TexCoordChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_TEXCOORD,uiLevel);
}
FORCEINLINE unsigned int VSVertexBuffer::NormalDateType(unsigned int uiLevel)const
{
	return GetSemanticsDateType(VSVertexFormat::VF_NORMAL,uiLevel);
}
FORCEINLINE unsigned int VSVertexBuffer::TangentDateType()const
{
	return GetSemanticsDateType(VSVertexFormat::VF_TANGENT,0);
}
FORCEINLINE unsigned int VSVertexBuffer::BinormalDateType()const
{
	return GetSemanticsDateType(VSVertexFormat::VF_BINORMAL,0);
}