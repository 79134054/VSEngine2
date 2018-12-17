#include "VSDynamicBufferGeometry.h"
#include "VSPointSet.h"
#include "VSLineSet.h"
#include "VSTriangleSet.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDynamicBufferGeometry,VSGeometry)
VSDynamicBufferGeometry::VSDynamicBufferGeometry()
{

}
VSDynamicBufferGeometry::~VSDynamicBufferGeometry()
{

}
void VSDynamicBufferGeometry::Draw(VSCamera * pCamera)
{
	if (HaveDate())
	{
		VSMaterialInstance *pMaterialInstance = GetUseMaterialInstance();
		VSMaterial *pMaterial = pMaterialInstance->GetMaterial();
		if(!pMaterialInstance || !pMaterial)
			return;
		for (unsigned int i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
		{

			VSIndirectRenderPass * pBasePass = pMaterialInstance->GetMaterial()->GetIndirectRenderPass();
			pBasePass->SetPassId(i);
			pBasePass->SetSpatial(this);
			pBasePass->SetMaterialInstance(pMaterialInstance);
			pBasePass->SetCamera(pCamera);
			pBasePass->Draw(VSRenderer::ms_pRenderer);
		}
	}
}
IMPLEMENT_RTTI(VSDVGeometry,VSDynamicBufferGeometry)
VSDVGeometry::VSDVGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_pVertexUseBufferRender = NULL;
}
VSDVGeometry::~VSDVGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pVertexUseBufferRender = NULL;
	
}
void VSDVGeometry::Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
						   unsigned int uiMeshDateType,
						   unsigned int uiVertexNum)
{
	VSMAC_ASSERT(uiMeshDateType < VSMeshDate::MDT_MAX && ForamtArray.GetNum() && uiVertexNum);

	VSMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == VSMeshDate::MDT_POINT)
	{
		pMeshDate = VS_NEW VSPointSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_LINE)
	{
		pMeshDate = VS_NEW VSLineSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshDate);


	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(ForamtArray,uiVertexNum);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(VSBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(VSBind::LF_DISCARD);
	pMeshDate->SetVertexBuffer(pVertexBuffer);

	SetMeshDate(pMeshDate);
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;

	m_pVertexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	if (VSResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
	}
}


bool VSDVGeometry::Add(const void * pVeretexDate,unsigned int uiVertexSize)
{
	if (!pVeretexDate || !uiVertexSize)
	{
		return 0;
	}

	
	if(m_pVertexUseBuffer->Add(pVeretexDate,uiVertexSize) == false)
		return false;
	return 1;
}
void * VSDVGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
unsigned int VSDVGeometry::UpdateGeometry()
{

	VSVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
	if (!pVertexBuffer || !m_pVertexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned char * pVertexDate = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexDate)
	{
		return UGRI_FAIL;
	}

	VSMemcpy(pVertexDate,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	if (!uiActiveNum)
	{
		uiActiveNum = m_pMeshDate->GetGirdNum(uiVSize / pVertexBuffer->GetOneVertexSize());
	}
	
	SetActiveNum(uiActiveNum);

	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;
	
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSDVGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
	}
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
}
unsigned int VSDVGeometry::GetMaxRenderVertexNum()const
{
	return m_pMeshDate->GetVertexBuffer()->GetVertexNum();
}

IMPLEMENT_RTTI(VSDVDIGeometry,VSDynamicBufferGeometry)
VSDVDIGeometry::VSDVDIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;

	m_pVertexUseBufferRender = NULL;
	m_pIndexUseBufferRender = NULL;
}
VSDVDIGeometry::~VSDVDIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;

	m_pVertexUseBufferRender = NULL;
	m_pIndexUseBufferRender = NULL;
}
void VSDVDIGeometry::Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
												 unsigned int uiMeshDateType,
												 unsigned int uiVertexNum,
												 unsigned int uiIndexNum)
{
	VSMAC_ASSERT(uiMeshDateType < VSMeshDate::MDT_MAX && ForamtArray.GetNum() && uiVertexNum && uiIndexNum);

	VSMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == VSMeshDate::MDT_POINT)
	{
		pMeshDate = VS_NEW VSPointSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_LINE)
	{
		pMeshDate = VS_NEW VSLineSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshDate);


	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(ForamtArray,uiVertexNum);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(VSBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(VSBind::LF_DISCARD);
	pMeshDate->SetVertexBuffer(pVertexBuffer);

	VSIndexBuffer * pIndexBuffer = VS_NEW VSIndexBuffer(uiIndexNum);
	pIndexBuffer->SetStatic(false);
	pIndexBuffer->SetMemType(VSBind::MT_VRAM);
	pIndexBuffer->SetLockFlag(VSBind::LF_DISCARD);
	pMeshDate->SetIndexBuffer(pIndexBuffer);

	SetMeshDate(pMeshDate);
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;


	m_pVertexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());

	m_pIndexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pIndexUseBuffer->SetOneAddLimitSize(pIndexBuffer->GetByteSize());

	if (VSResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());

		m_pIndexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pIndexUseBufferRender->SetOneAddLimitSize(pIndexBuffer->GetByteSize());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
		m_pIndexUseBufferRender = m_pIndexUseBuffer;
	}
}

void * VSDVDIGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
void * VSDVDIGeometry::NewGetI(unsigned int uiIndexSize)
{
	if (!uiIndexSize)
	{
		return 0;
	}

	return m_pIndexUseBuffer->NewGet(uiIndexSize);
}
bool VSDVDIGeometry::Add(const void * pVeretexDate,unsigned int uiVertexSize,
								  const void * pIndexDate,unsigned int uiIndexSize)
{
	VSMAC_ASSERT(pVeretexDate && uiVertexSize && pIndexDate && uiIndexSize);
	if (!pVeretexDate || !uiVertexSize || !pIndexDate || !uiIndexSize)
	{
		return 0;
	}

	if(m_pVertexUseBuffer->Add(pVeretexDate,uiVertexSize) == false)
		return false;

	
		

	if(m_pIndexUseBuffer->Add(pIndexDate,uiIndexSize) == false)
		return false;

	unsigned int uiLastVertexNum = 0;
	unsigned int uiVElementNum = m_pVertexUseBuffer->GetElementNum();
	if (uiVElementNum)
	{
		VSVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
		if (pVertexBuffer)
		{
			return false;
		}
		const VSUseBuffer::VSBufferElementInfo * pElementInfo = m_pVertexUseBuffer->GetElementInfo(uiVElementNum - 1);
		uiLastVertexNum = pElementInfo->m_uiStart / pVertexBuffer->GetOneVertexSize();
	}
	unsigned char *pBuffer = m_pIndexUseBuffer->GetBuffer();
	unsigned int uiIElementNum = m_pIndexUseBuffer->GetElementNum();
	pBuffer += m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiStart;

	VSUSHORT_INDEX * pIndexDateBuffer = (VSUSHORT_INDEX *)pBuffer;
	for ( ; (unsigned int)pIndexDateBuffer < (unsigned int)m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiEnd ; pIndexDateBuffer++)
	{
		*pIndexDateBuffer += uiLastVertexNum;
	}
	if ((unsigned int)pIndexDateBuffer != (unsigned int)m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiEnd)
	{
		return false;
	}
	
	VSMAC_ASSERT(m_pVertexUseBuffer->GetElementNum() == m_pIndexUseBuffer->GetElementNum());
	return 1;
}
unsigned int VSDVDIGeometry::UpdateGeometry()
{
	VSMAC_ASSERT(m_pMeshDate && m_pVertexUseBufferRender && m_pIndexUseBufferRender);
	if (!m_pMeshDate || !m_pVertexUseBufferRender || !m_pIndexUseBufferRender)
	{
		return UGRI_FAIL;
	}


	VSVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
	VSIndexBuffer * pIndexBuffer = m_pMeshDate->GetIndexBuffer();
	if (!pVertexBuffer || !pIndexBuffer || !m_pVertexUseBufferRender->GetElementNum() || !m_pIndexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}




	unsigned int uiIElementEndIndex = m_uiCurIUseBufferElementIndex;
	unsigned int uiISize = 0;
	for (unsigned int i = m_uiCurIUseBufferElementIndex ; i < m_pIndexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pIElement = m_pIndexUseBufferRender->GetElementInfo(i);
		if (pIElement)
		{

			if (uiISize + pIElement->GetSize() > pIndexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiIElementEndIndex = i;
				uiISize += pIElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned int uiVCount = uiVElementEndIndex - m_uiCurVUseBufferElementIndex;
	unsigned int uiICount = uiIElementEndIndex - m_uiCurIUseBufferElementIndex;
	if ( uiVCount > uiICount)
	{
		uiVSize = 0;
		uiVElementEndIndex = m_uiCurVUseBufferElementIndex + uiICount;
		for (unsigned int i = m_uiCurVUseBufferElementIndex ; i <= uiVElementEndIndex ; i++)
		{
			const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
			if (pVElement)
			{
				uiVSize += pVElement->GetSize();
			}
			else
			{
				return UGRI_FAIL;
			}
		}
	}
	else if(uiVCount < uiICount)
	{
		uiISize = 0;
		uiIElementEndIndex = m_uiCurIUseBufferElementIndex + uiVCount;
		for (unsigned int i = m_uiCurIUseBufferElementIndex ; i <= uiIElementEndIndex ; i++)
		{
			const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
			if (pVElement)
			{
				uiISize += pVElement->GetSize();
			}
			else
			{
				return UGRI_FAIL;
			}
		}
	}

	unsigned char * pIndexDate = (unsigned char *)pIndexBuffer->Lock();
	if (!pIndexDate)
	{
		return UGRI_FAIL;
	}
	VSMemcpy(pIndexDate,m_pIndexUseBufferRender->GetBuffer() + m_pIndexUseBufferRender->GetElementInfo(m_uiCurIUseBufferElementIndex)->m_uiStart,uiISize);
	pIndexBuffer->UnLock();
	uiActiveNum = m_pMeshDate->GetGirdNum(uiISize / sizeof(VSUSHORT_INDEX));

	m_uiCurIUseBufferElementIndex = uiIElementEndIndex + 1;

	
	unsigned char * pVertexDate = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexDate)
	{
		return UGRI_FAIL;
	}

	VSMemcpy(pVertexDate,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	if (!uiActiveNum)
	{
		uiActiveNum = m_pMeshDate->GetGirdNum(uiVSize / pVertexBuffer->GetOneVertexSize());
	}

	SetActiveNum(uiActiveNum);

	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;

	//因为UseVBuffer 和 UseIBuffer Element个数一样,添加渲染的个数也保证一样，所以这里只判断一个。
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		m_uiCurIUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSDVDIGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
		Swap(m_pIndexUseBuffer,m_pIndexUseBufferRender);
	}
	m_pIndexUseBuffer->Clear();
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;
}


IMPLEMENT_RTTI(VSSVDIGeometry,VSDynamicBufferGeometry)
VSSVDIGeometry::VSSVDIGeometry()
{

	m_pIndexUseBuffer = NULL;

	m_uiCurIUseBufferElementIndex = 0;

	m_pIndexUseBufferRender = NULL;
}
VSSVDIGeometry::~VSSVDIGeometry()
{

	m_pIndexUseBuffer = NULL;

	m_pIndexUseBufferRender = NULL;
}
void VSSVDIGeometry::Create(unsigned int uiMeshDateType,VSVertexBuffer * pVertexBuffer,unsigned int uiIndexNum)
{
	VSMAC_ASSERT(uiMeshDateType < VSMeshDate::MDT_MAX && uiIndexNum && pVertexBuffer && pVertexBuffer->IsStatic());
	VSMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == VSMeshDate::MDT_POINT)
	{
		pMeshDate = VS_NEW VSPointSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_LINE)
	{
		pMeshDate = VS_NEW VSLineSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshDate);

	pMeshDate->SetVertexBuffer(pVertexBuffer);

	VSIndexBuffer * pIndexBuffer = VS_NEW VSIndexBuffer(uiIndexNum);
	pIndexBuffer->SetStatic(false);
	pIndexBuffer->SetMemType(VSBind::MT_VRAM);
	pIndexBuffer->SetLockFlag(VSBind::LF_DISCARD);
	pMeshDate->SetIndexBuffer(pIndexBuffer);

	SetMeshDate(pMeshDate);

	m_pIndexUseBuffer = NULL;

	m_uiCurIUseBufferElementIndex = 0;


	m_pIndexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pIndexUseBuffer->SetOneAddLimitSize(pIndexBuffer->GetByteSize());

	if (VSResourceManager::ms_bRenderThread)
	{

		m_pIndexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pIndexUseBufferRender->SetOneAddLimitSize(pIndexBuffer->GetByteSize());
	}
	else
	{
		m_pIndexUseBufferRender = m_pIndexUseBuffer;
	}
}

void * VSSVDIGeometry::NewGetI(unsigned int uiIndexSize)
{
	if (!uiIndexSize)
	{
		return 0;
	}

	return m_pIndexUseBuffer->NewGet(uiIndexSize);
}
bool VSSVDIGeometry::Add(const void * pIndexDate,unsigned int uiIndexSize)
{
	VSMAC_ASSERT(pIndexDate && uiIndexSize);
	if (!pIndexDate || !uiIndexSize)
	{
		return 0;
	}



	if(m_pIndexUseBuffer->Add(pIndexDate,uiIndexSize) == false)
		return false;

	return 1;
}
unsigned int VSSVDIGeometry::UpdateGeometry()
{
	VSMAC_ASSERT(m_pMeshDate && m_pIndexUseBufferRender);
	if (!m_pMeshDate || !m_pIndexUseBufferRender)
	{
		return UGRI_FAIL;
	}


	VSIndexBuffer * pIndexBuffer = m_pMeshDate->GetIndexBuffer();
	if (!pIndexBuffer || !m_pIndexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	




	unsigned int uiIElementEndIndex = m_uiCurIUseBufferElementIndex;
	unsigned int uiISize = 0;
	for (unsigned int i = m_uiCurIUseBufferElementIndex ; i < m_pIndexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pIElement = m_pIndexUseBufferRender->GetElementInfo(i);
		if (pIElement)
		{

			if (uiISize + pIElement->GetSize() > pIndexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiIElementEndIndex = i;
				uiISize += pIElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}


	unsigned char * pIndexDate = (unsigned char *)pIndexBuffer->Lock();
	if (!pIndexDate)
	{
		return UGRI_FAIL;
	}
	VSMemcpy(pIndexDate,m_pIndexUseBufferRender->GetBuffer() + m_pIndexUseBufferRender->GetElementInfo(m_uiCurIUseBufferElementIndex)->m_uiStart,uiISize);
	pIndexBuffer->UnLock();
	uiActiveNum = m_pMeshDate->GetGirdNum(uiISize / sizeof(VSUSHORT_INDEX));

	m_uiCurIUseBufferElementIndex = uiIElementEndIndex + 1;

	SetActiveNum(uiActiveNum);


	//因为UseVBuffer 和 UseIBuffer Element个数一样每天添加渲染的个数也保证一样，所以这里只判断一个。
	if (m_uiCurIUseBufferElementIndex >= m_pIndexUseBufferRender->GetElementNum())
	{
		m_uiCurIUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSSVDIGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{

		Swap(m_pIndexUseBuffer,m_pIndexUseBufferRender);
	}
	m_pIndexUseBuffer->Clear();

	m_uiCurIUseBufferElementIndex = 0;
}


IMPLEMENT_RTTI(VSDVSIGeometry,VSDynamicBufferGeometry)
VSDVSIGeometry::VSDVSIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_pVertexUseBufferRender = NULL;
}
VSDVSIGeometry::~VSDVSIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pVertexUseBufferRender = NULL;

}
void VSDVSIGeometry::Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
							unsigned int uiMeshDateType,
							unsigned int uiVertexNum,
							VSIndexBuffer * pIndexBuffer)
{
	VSMAC_ASSERT(uiMeshDateType < VSMeshDate::MDT_MAX && ForamtArray.GetNum() && uiVertexNum && pIndexBuffer);
	VSMAC_ASSERT(pIndexBuffer->IsStatic());
	VSMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == VSMeshDate::MDT_POINT)
	{
		pMeshDate = VS_NEW VSPointSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_LINE)
	{
		pMeshDate = VS_NEW VSLineSet();
	}
	else if ( uiMeshDateType == VSMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshDate);


	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(ForamtArray,uiVertexNum);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(VSBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(VSBind::LF_DISCARD);
	pMeshDate->SetVertexBuffer(pVertexBuffer);
	pMeshDate->SetIndexBuffer(pIndexBuffer);
	SetMeshDate(pMeshDate);
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;

	m_pVertexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	if (VSResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
	}
}
bool VSDVSIGeometry::Add(const void * pVeretexDate,unsigned int uiVertexSize)
{
	if (!pVeretexDate || !uiVertexSize)
	{
		return 0;
	}


	if(m_pVertexUseBuffer->Add(pVeretexDate,uiVertexSize) == false)
		return false;
	return 1;
}
void * VSDVSIGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
unsigned int VSDVSIGeometry::UpdateGeometry()
{

	VSVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
	if (!pVertexBuffer || !m_pVertexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned char * pVertexDate = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexDate)
	{
		return UGRI_FAIL;
	}

	VSMemcpy(pVertexDate,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;

	//因为UseVBuffer 和 UseIBuffer Element个数一样每天添加渲染的个数也保证一样，所以这里只判断一个。
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSDVSIGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
	}
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
}
