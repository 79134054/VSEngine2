#include "VSIndexBuffer.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSIndexBuffer,VSBind)
BEGIN_ADD_PROPERTY(VSIndexBuffer,VSBind)
REGISTER_PROPERTY(m_pDate,IndexData,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNum,Num,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT,DataType,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSIndexBuffer)
IMPLEMENT_INITIAL_END
VSIndexBuffer::VSIndexBuffer()
{
	m_pDate = NULL;
	m_uiNum = 0;
	m_uiDT = VSDataBuffer::DT_USHORT;
	m_pLockDate = NULL;
}
VSIndexBuffer::~VSIndexBuffer()
{
	ReleaseResource();
	m_pDate = NULL;
	m_pLockDate = NULL;
}
void VSIndexBuffer::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_pDate = NULL;
		}
	}
}
bool VSIndexBuffer::LoadResource(VSRenderer * pRender)
{
	if(!VSBind::LoadResource(pRender))
	{
		return 0;
	}
	
	return 1;
}
bool VSIndexBuffer::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadIBuffer(this,pID))
		return 0;
	return 1;
}
bool VSIndexBuffer::OnReleaseResource(VSResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseIBuffer(pID))
		return 0;
	return 1;
}
void *VSIndexBuffer::Lock()
{
	if (m_pLockDate || m_bIsStatic)
	{
		return NULL;
	}
	if (m_pUser)
	{
		m_pLockDate = m_pUser->Lock(this);
	}

	return m_pLockDate;
}
void VSIndexBuffer::UnLock()
{
	if (!m_pLockDate || m_bIsStatic)
	{
		return;
	}
	if (m_pUser)
	{
		m_pUser->UnLock(this);
	}
	m_pLockDate = NULL;
}
bool VSIndexBuffer::SetDate(VSDataBuffer * pDate)
{
	if ((pDate->GetDT() != VSDataBuffer::DT_USHORT && pDate->GetDT() != VSDataBuffer::DT_UINT)
		|| !pDate->GetNum() || !pDate->GetDate())
	{
		return 0;
	}
	m_pDate = pDate;
	m_uiNum = pDate->GetNum();
	m_uiDT = pDate->GetDT();
	return 1;
}

unsigned int VSIndexBuffer::GetNum()const
{
	return m_uiNum;
}
unsigned int VSIndexBuffer::GetByteSize()const
{
	return VSDataBuffer::ms_uiDateTypeByte[m_uiDT] * m_uiNum;
}
VSIndexBuffer::VSIndexBuffer(unsigned int uiNum,unsigned int uiDT)
{
	VSMAC_ASSERT(uiNum);
	VSMAC_ASSERT(uiDT == VSDataBuffer::DT_USHORT || uiDT == VSDataBuffer::DT_UINT)
	m_pDate = NULL;
	m_uiNum = uiNum;
	m_uiDT = uiDT;
	m_pLockDate = NULL;
}