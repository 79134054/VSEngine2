#include "VSCaptureTexAllState.h"
#include "VSResourceManager.h"
#include "VSImage.h"
#include "VSGraphicInclude.h"
#include "VSSceneManager.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCaptureTexAllState,VSTexAllState)
BEGIN_ADD_PROPERTY(VSCaptureTexAllState,VSTexAllState)
REGISTER_PROPERTY(m_ViewCaptureName,ViewCaptureName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCaptureTexAllState)
IMPLEMENT_INITIAL_END

VSCaptureTexAllState::VSCaptureTexAllState()
{
	m_bDynamic = true;
	m_pStaticTexture = NULL;
	m_uiMipLevel = 1;
}
VSCaptureTexAllState::~VSCaptureTexAllState()
{
	m_pStaticTexture = NULL;
	if (VSSceneManager::ms_pSceneManager)
	{
		VSCaptureViewFamily * pCaptureViewFamily = DynamicCast<VSCaptureViewFamily>(VSSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));

		if (pCaptureViewFamily)
		{
			pCaptureViewFamily->m_pTexOwner = NULL;
		}
	}
	
	
	
}
bool VSCaptureTexAllState::SetViewCapture(const VSString & ViewCaptureName)
{
	
	m_ViewCaptureName = ViewCaptureName;

	VSCaptureViewFamily * pCaptureViewFamily = DynamicCast<VSCaptureViewFamily>(VSSceneManager::ms_pSceneManager->GetViewFamily(ViewCaptureName));

	if (!pCaptureViewFamily)
	{
		return false;
	}
	VSTexture * pTexture =  pCaptureViewFamily->GetTexture();

	VSMAC_ASSERT(pTexture);

	m_pTex = pTexture;
	ForceUpdate(!m_bDynamic);
	return true;
}
bool VSCaptureTexAllState::PostLoad(void * pDate)
{
	VSTexAllState::PostLoad(pDate);
	if (!m_pTex)
	{
		SetViewCapture(m_ViewCaptureName);
	}
	
	return true;
}
bool VSCaptureTexAllState::PostClone(VSObject * pObjectSrc)
{
	VSTexAllState::PostClone(pObjectSrc);
	SetViewCapture(m_ViewCaptureName);
	return true;
}
bool VSCaptureTexAllState::BeforeSave( void * pDate)
{
	VSTexAllState::BeforeSave(pDate);
	if (m_bDynamic)
	{
		m_pTex = NULL;
	}
	return true;
}
bool VSCaptureTexAllState::PostSave( void * pDate)
{
	VSTexAllState::PostSave(pDate);
	if (m_bDynamic)
	{
		SetViewCapture(m_ViewCaptureName);
	}
	return true;
}
void VSCaptureTexAllState::SetMipLevel(unsigned int uiMipLevel)
{
	m_uiMipLevel = uiMipLevel;
	ForceUpdate(!m_bDynamic);
}
VSCaptureViewFamily * VSCaptureTexAllState::GetViewFamily()
{
	VSCaptureViewFamily * pCaptureViewFamily = DynamicCast<VSCaptureViewFamily>(VSSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));


	if (!pCaptureViewFamily)
	{
		return NULL;
	}

	return pCaptureViewFamily;
}
void VSCaptureTexAllState::ForceUpdate(bool OnlyUpdateOneTime)
{
	VSCaptureViewFamily * pCaptureViewFamily = DynamicCast<VSCaptureViewFamily>(VSSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));


	if (!pCaptureViewFamily)
	{
		return ;
	}
	m_bDynamic = !OnlyUpdateOneTime;

	pCaptureViewFamily->m_OnlyUpdateOneTime = OnlyUpdateOneTime;
	pCaptureViewFamily->m_bEnable = true;
	pCaptureViewFamily->m_pTexOwner = this;

}
void VSCaptureTexAllState::CreateStaticTexture()
{
	VSCaptureViewFamily * pCaptureViewFamily = DynamicCast<VSCaptureViewFamily>(VSSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));
	VSMAC_ASSERT(pCaptureViewFamily);
	if (!m_bDynamic)
	{
		VSTexture * pTexture =  pCaptureViewFamily->GetTexture();
		unsigned int uiTextureType = pTexture->GetTexType();
		if (uiTextureType == VSTexture::TT_2D)
		{
			m_pStaticTexture = VS_NEW VS2DTexture(pTexture->GetWidth(0),pTexture->GetHeight(0),pTexture->GetFormatType(),m_uiMipLevel,1);
		}
		else if (uiTextureType == VSTexture::TT_CUBE)
		{
			m_pStaticTexture = VS_NEW VSCubeTexture(pTexture->GetWidth(0),pTexture->GetFormatType(),m_uiMipLevel,1);
		}
		m_pStaticTexture->CreateRAMDate();
		m_uiMipLevel = m_pStaticTexture->GetMipLevel();
		m_pTex = m_pStaticTexture;
	}
}
void VSCaptureTexAllState::NotifyEndDraw()
{
	CreateStaticTexture();
	VSCaptureViewFamily * pCaptureViewFamily = DynamicCast<VSCaptureViewFamily>(VSSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));
	VSMAC_ASSERT(pCaptureViewFamily);
	if (!m_bDynamic)
	{
		VSTexture * pSourceTexture =  pCaptureViewFamily->GetTexture();
		
		if (pSourceTexture->GetTexType() == VSTexture::TT_2D)
		{
			VSTexture * pDestTexture = m_pStaticTexture;
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSUpdate2DTextureMipCommand,
				VSTexture *,pSourceTexture,pSourceTexture,VSTexture *,pDestTexture,pDestTexture,
			{
				void * SourceBuffer = pSourceTexture->Lock(0);
				unsigned char * pDestBuffer = pDestTexture->GetBuffer(0);
				VSMemcpy((void *) pDestBuffer,SourceBuffer,pSourceTexture->GetByteSize(0));
				pSourceTexture->UnLock(0);

				unsigned char * pLast = pDestBuffer;
				for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
				{
					unsigned char * pNow = (unsigned char *)pDestTexture->GetBuffer(i);

					VSResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());
		
					pLast = (unsigned char *)pDestTexture->GetBuffer(i);
				}
			})
				void * SourceBuffer = pSourceTexture->Lock(0);
				unsigned char * pDestBuffer = pDestTexture->GetBuffer(0);
				VSMemcpy((void *) pDestBuffer,SourceBuffer,pSourceTexture->GetByteSize(0));
				pSourceTexture->UnLock(0);

				unsigned char * pLast = pDestBuffer;
				for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
				{
					unsigned char * pNow = (unsigned char *)pDestTexture->GetBuffer(i);

					VSResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());

					pLast = (unsigned char *)pDestTexture->GetBuffer(i);
				}
				
			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}
		else if (pSourceTexture->GetTexType() == VSTexture::TT_CUBE)
		{
			VSTexture * Temp = m_pStaticTexture;
			VSCubeTexture * pDestTexture = (VSCubeTexture *)Temp;
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSUpdateCubeTextureMipCommand,
				VSTexture *,pSourceTexture,pSourceTexture,VSCubeTexture *,pDestTexture,pDestTexture,
			{
				for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
				{
					void * SourceBuffer = pSourceTexture->Lock(0,i);
					unsigned char * pDestBuffer = pDestTexture->GetFaceBuffer(0,i);
					VSMemcpy((void *) pDestBuffer,SourceBuffer,pDestTexture->GetFaceByteSize(0));
					pSourceTexture->UnLock(0,i);
				}
				
				for (unsigned int s = 0 ; s < VSCubeTexture::F_MAX ; s++)
				{
					unsigned char * pLast = pDestTexture->GetFaceBuffer(0,s);

					for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
					{
						unsigned char * pNow = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);

						VSResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());

						pLast = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);
					}
				}
			})
				for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
				{
					void * SourceBuffer = pSourceTexture->Lock(0,i);
					unsigned char * pDestBuffer = pDestTexture->GetFaceBuffer(0,i);
					VSMemcpy((void *) pDestBuffer,SourceBuffer,pDestTexture->GetFaceByteSize(0));
					pSourceTexture->UnLock(0,i);
				}

				for (unsigned int s = 0 ; s < VSCubeTexture::F_MAX ; s++)
				{
					unsigned char * pLast = pDestTexture->GetFaceBuffer(0,s);

					for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
					{
						unsigned char * pNow = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);

						VSResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());

						pLast = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);
					}
				}

			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}

		
	}
}