#include "VSBind.h"
#include "VSRenderer.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
VSArray<VSBind *> VSBind::ms_DynamicTwoBindArray;
VSArray<VSBind *> VSBind::ms_BindArray;
IMPLEMENT_RTTI_NoCreateFun(VSBind,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSBind,VSObject)
REGISTER_PROPERTY(m_bIsStatic,IsStatic,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiSwapChainNum,SwapChainNum,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiLockFlag,LockFlag,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiMemType,MemType,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSResourceIdentifier* VSBind::GetIdentifier ()
{
	if (!m_InfoArray.GetNum())
	{
		return NULL;
	}
	VSResourceIdentifier * pID = NULL;
	pID = m_InfoArray[m_uiCurID].ID;
	return pID;
}
void VSBind::ExChange()
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		m_uiCurID = (m_uiCurID + 1) % m_uiSwapChainNum;
	}	
}
unsigned int VSBind::GetInfoQuantity () const
{
	return m_InfoArray.GetNum();
}
VSBind::VSBind(unsigned int uiMemType)
{
	m_InfoArray.Clear();
	m_bIsStatic = 1;
	m_uiSwapChainNum = 1;
	m_uiCurID = 0;
	m_uiLockFlag = LF_NOOVERWRITE;
	m_pUser = NULL;
	m_uiMemType = uiMemType;
	m_uiClearState = MCS_NONE;
	ms_BindArray.AddElement(this);
}
VSBind::~VSBind()
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		for (unsigned int i = 0 ;i  < ms_DynamicTwoBindArray.GetNum() ; i++)
		{
			if (ms_DynamicTwoBindArray[i] == this)
			{
				ms_DynamicTwoBindArray.Erase(i);
				break;
			}
		}
	}
	for (unsigned int i = 0 ;i  < ms_BindArray.GetNum() ; i++)
	{
		if (ms_BindArray[i] == this)
		{
			ms_BindArray.Erase(i);
			break;
		}
	}
	ReleaseResource();
}
void VSBind::Bind(VSResourceIdentifier* pID)
{
	if(!pID)
		return ;
	INFO_TYPE Info;
	Info.ID = pID;
	m_InfoArray.AddElement(Info);
}

bool VSBind::LoadResource(VSRenderer * pRender)
{
	if(!pRender)
		return 0;
	if (m_uiMemType == MT_RAM)
	{
		return 1;
	}
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
		return 1;
	else
	{
		m_pUser = pRender;
		for (unsigned int i = 0 ; i < m_uiSwapChainNum ; i++)
		{
			VSResourceIdentifier *pID = NULL;
			if(!OnLoadResource(pID))
				return 0;
			if(!pID)
				return 0;
			Bind(pID);
		}
		if (!VSResourceManager::ms_bRenderThread)
		{
			ClearInfo();
		}
		return 1;	
	
	}
	
}
bool VSBind::ReleaseResource()
{
	for (unsigned int i = 0 ; i < m_InfoArray.GetNum() ; i++)
	{
		INFO_TYPE &rInfo = m_InfoArray[i];
		if(!OnReleaseResource(rInfo.ID))
			return 0;
	}
	m_InfoArray.Clear();
	return 1;
}
void VSBind::ClearInfo()
{

}
void VSBind::ASYNClearInfo()
{
	if (m_uiMemType == MT_VRAM)
	{
		if(m_uiSwapChainNum == m_InfoArray.GetNum())
		{
			if (m_uiClearState == MCS_NONE)
			{
				m_uiClearState = MCS_READY;
				return ;
			}
			else if (m_uiClearState == MCS_READY)
			{
				m_uiClearState = MCS_DONE;
				ClearInfo();
				return ;
			}
		}
	}
}
void VSBind::SetStatic(bool bIsStatic,bool bMulThreadUse)
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		for (unsigned int i = 0; i < ms_DynamicTwoBindArray.GetNum(); i++)
		{
			if (ms_DynamicTwoBindArray[i] == this)
			{
				ms_DynamicTwoBindArray.Erase(i);
				break;
			}
		}
	}
	m_bIsStatic = bIsStatic;
	
	if(m_bIsStatic)
	{
		m_uiSwapChainNum = 1;	
	}
	else
	{
		if (VSResourceManager::ms_bRenderThread && bMulThreadUse && VSRenderer::ms_pRenderer->IsSupportMulBufferSwtich())
		{

			m_uiSwapChainNum = 2;
			ms_DynamicTwoBindArray.AddElement(this);

		}
		else
		{
			m_uiSwapChainNum = 1;
		}

	}

	m_uiCurID = 0;

}
unsigned int VSBind::GetByteSize()const
{
	return 0;
}
