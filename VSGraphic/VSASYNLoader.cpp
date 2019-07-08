#include "VSASYNLoader.h"
#include "VSGraphicInclude.h"
#include "VSImage.h"
#include "VS2DTexture.h"
#include "VSNVCompression.h"
#include "VSTimer.h"
#include "VSProfiler.h"
using namespace VSEngine2;						
DECLEAR_TIME_PROFILENODE(VSASYNLoadManagerUpdate, ApplicationUpdate)
VSResourceJob::VSResourceJob()
{
	m_uiLoadState = LS_PENDING;
	m_uiJobType = JT_TEXTURE;
	m_uiJobState = JS_NONE;
}
VSResourceJob::~VSResourceJob()
{

}
VSFileRJob::VSFileRJob()
{
	m_pBuffer = NULL;
}
VSFileRJob::~VSFileRJob()
{
	VSMAC_DELETEA(m_pBuffer);
}
bool VSFileRJob::Load()
{
	VSFile File;
	if(!File.Open(m_FileName.GetBuffer(),VSFile::OM_RB))
	{
		m_uiLoadState = VSResourceJob::LS_FAIL;
		return 0;
	}

	m_uiSize = File.GetFileSize();
	m_pBuffer = VS_NEW unsigned char[m_uiSize];
	if(!m_pBuffer)
	{
		m_uiLoadState = VSResourceJob::LS_FAIL;
		return 0;
	}
	if(!File.Read(m_pBuffer,m_uiSize,1))
	{
		m_uiLoadState = VSResourceJob::LS_FAIL;
		return 0;
	}
	m_uiLoadState = VSResourceJob::LS_LOADED;

	return true;
}
VSTextureRJob::VSTextureRJob(VSTexAllStateRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_bEngineType = true;
	m_bCompress = 0;
	m_uiCompressType = 0;
	m_bIsNormal = false;
	m_bSRGB = false;
	m_pSamplerState = NULL;
	m_uiJobType = JT_TEXTURE;
}
VSTextureRJob::~VSTextureRJob()
{
	m_pProxy = NULL;

}
VSMaterialRJob::VSMaterialRJob(VSMaterialRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_MATERIAL;
}
VSMaterialRJob::~VSMaterialRJob()
{
	m_pProxy = NULL;
}
VSAnimRJob::VSAnimRJob(VSAnimRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_ANIM;
}
VSAnimRJob::~VSAnimRJob()
{
	m_pProxy = NULL;
}

VSStaticMeshRJob::VSStaticMeshRJob(VSStaticMeshNodeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_STATIC_MESH;
}
VSStaticMeshRJob::~VSStaticMeshRJob()
{
	m_pProxy = NULL;

}
VSSkelectonMeshRJob::VSSkelectonMeshRJob(VSSkelectonMeshNodeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_SKELECTON_MESH;
}
VSSkelectonMeshRJob::~VSSkelectonMeshRJob()
{
	m_pProxy = NULL;

}
VSAnimTreeRJob::VSAnimTreeRJob(VSAnimTreeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_ANIMTREE;
}
VSAnimTreeRJob::~VSAnimTreeRJob()
{
	m_pProxy = NULL;

}
VSMorphTreeRJob::VSMorphTreeRJob(VSMorphTreeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_MORPHTREE;
}
VSMorphTreeRJob::~VSMorphTreeRJob()
{
	m_pProxy = NULL;

}
VSSceneMapRJob::VSSceneMapRJob(VSSceneMapRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_MAP;
}
VSSceneMapRJob::~VSSceneMapRJob()
{
	m_pProxy = NULL;

}
VSFontRJob::VSFontRJob(VSFontRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_FONT;
}
VSFontRJob::~VSFontRJob()
{
	m_pProxy = NULL;

}
VSActorRJob::VSActorRJob(VSActorRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_ACTOR;
}
VSActorRJob::~VSActorRJob()
{
	m_pProxy = NULL;

}
void VSResourceLoaderThread::Stop()
{
	
	VSThread::Stop();
	
}
VSResourceLoaderThread::VSResourceLoaderThread()
{
	m_pResourceQueue.Clear();
}
VSResourceLoaderThread::~VSResourceLoaderThread()
{
	m_pResourceQueue.Clear();
}
void VSResourceLoaderThread::AddJob(VSResourceJob* m_pJob)
{
	bool IsEmpty = m_pResourceQueue.IsEmpty();
	m_pResourceQueue.Enqueue(m_pJob);

}
void VSResourceLoaderThread::Run()
{
	while(!IsStopTrigger())
	{
		while(!m_pResourceQueue.IsEmpty())
		{
			VSResourceJob* pJob = NULL;
			m_pResourceQueue.Dequeue(pJob);
			if(!pJob->Load())
			{
				VSMAC_ASSERT(0);
			}
		}
	}

}
bool VSMaterialRJob::Process()
{
	if (!m_pBuffer)
	{
		return false;
	}

	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSMaterial * pMaterial = (VSMaterial *)LoadStream.GetObjectByRtti(VSMaterial::ms_Type);

	if (pMaterial)
	{
		m_pProxy->SetNewResource(pMaterial);
		m_pProxy->Loaded();
		return true;
	}
	return false;
	
}
bool VSStaticMeshRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSStaticMeshNode * pStaticMesh = (VSStaticMeshNode *)LoadStream.GetObjectByRtti(VSStaticMeshNode::ms_Type);
	if (pStaticMesh)
	{
		m_pProxy->SetNewResource(pStaticMesh);
		m_pProxy->Loaded();
		return true;
	}
	return false;
	
}
bool VSSkelectonMeshRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSSkelectonMeshNode * pSkelectonMesh = (VSSkelectonMeshNode *)LoadStream.GetObjectByRtti(VSSkelectonMeshNode::ms_Type);
	if (pSkelectonMesh)
	{

		m_pProxy->SetNewResource(pSkelectonMesh);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool VSAnimTreeRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSAnimTree * pAnimTree = (VSAnimTree *)LoadStream.GetObjectByRtti(VSAnimTree::ms_Type);
	if (pAnimTree)
	{

		m_pProxy->SetNewResource(pAnimTree);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool VSMorphTreeRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSMorphTree * pMorphTree = (VSMorphTree *)LoadStream.GetObjectByRtti(VSMorphTree::ms_Type);
	if (pMorphTree)
	{

		m_pProxy->SetNewResource(pMorphTree);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool VSActorRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer, m_uiSize);
	VSActor * pActor = (VSActor *)LoadStream.GetObjectByRtti(VSActor::ms_Type);
	if (pActor)
	{

		m_pProxy->SetNewResource(pActor);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool VSSceneMapRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer, m_uiSize);
	VSSceneMap * pMap = (VSSceneMap *)LoadStream.GetObjectByRtti(VSSceneMap::ms_Type);
	if (pMap)
	{
		m_pProxy->SetNewResource(pMap);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool VSFontRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer, m_uiSize);
	VSFont * pFont = (VSFont *)LoadStream.GetObjectByRtti(VSFont::ms_Type);
	if (pFont)
	{
		m_pProxy->SetNewResource(pFont);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool VSAnimRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSAnim * pAnim = (VSAnim *)LoadStream.GetObjectByRtti(VSAnim::ms_Type);

	if (pAnim)
	{
		m_pProxy->SetNewResource(pAnim);
		m_pProxy->Loaded();
		return true;
	}
	return false;
}
VSTexAllState * VSTextureRJob::LoadTexture()
{
	if (!m_pBuffer)
	{
		return NULL;
	}
	VSStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	VSTexAllState * pTexAllState = (VSTexAllState *)LoadStream.GetObjectByRtti(VSTexAllState::ms_Type);
	if (!pTexAllState)
	{

		return NULL;
	}
	if(!pTexAllState->m_pTex)
	{
		VSMAC_DELETE(pTexAllState);

		return NULL;
	}

	return pTexAllState;
}
VSTexAllState * VSTextureRJob::Load2DTexture()
{
	if (!m_pBuffer)
	{
		return NULL;
	}
	VSString Extension; 

	if (!m_FileName.GetExtension(Extension))
	{
		return NULL;
	}
	VSTexAllState * pTexAllState = NULL;


	VSImage *pImage = NULL;

	if (Extension == VSImage::ms_ImageFormat[VSImage::IF_BMP])
	{
		pImage = VS_NEW VSBMPImage();
	}
	else if (Extension == VSImage::ms_ImageFormat[VSImage::IF_TGA])
	{
		pImage = VS_NEW VSTGAImage();
	}
	else 
	{

		return NULL;
	}

	if(!pImage->LoadFromBuffer(m_pBuffer,m_uiSize))
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = VS_NEW VS2DTexture(uiWidth, uiHeight, VSRenderer::SFT_A8R8G8B8, 0, 1);
	if(!pTexture)
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			VSMAC_DELETE(pTexture);
			VSMAC_DELETE(pImage);

			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		VSMAC_DELETE(pImage);
	}
	pTexAllState = VS_NEW VSTexAllState(pTexture);

	if (m_pSamplerState)
	{
		pTexAllState->SetSamplerState(m_pSamplerState);
	}

	pTexAllState->SetSRGBEable(m_bSRGB);

	return pTexAllState;
}
VSTexAllState * VSTextureRJob::Load2DTextureCompress()
{
	if (!m_pBuffer)
	{
		return NULL;
	}

	if (m_uiCompressType > VSNVCompression::NV_MAX)
	{

		return NULL;
	}

	VSString Extension; 

	if (!m_FileName.GetExtension(Extension))
	{

		return NULL;
	}
	VSTexAllState * pTexAllState = NULL;

	VSImage *pImage = NULL;
	if (Extension == VSImage::ms_ImageFormat[VSImage::IF_BMP])
	{
		pImage = VS_NEW VSBMPImage();
	}
	else if (Extension == VSImage::ms_ImageFormat[VSImage::IF_TGA])
	{
		pImage = VS_NEW VSTGAImage();
	}
	else 
	{

		return NULL;
	}
	if(!pImage->LoadFromBuffer(m_pBuffer,m_uiSize))
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = VS_NEW VS2DTexture(uiWidth, uiHeight, VSRenderer::SFT_A8R8G8B8, 0, 1);
	if(!pTexture)
	{
		VSMAC_DELETE(pImage);

		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!VSResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			VSMAC_DELETE(pTexture);
			VSMAC_DELETE(pImage);

			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		VSMAC_DELETE(pImage);
	}

	unsigned int uiCompressFormat = VSNVCompression::NV_ARGB;
	unsigned int uiVSTextureTpye = VSRenderer::SFT_A8R8G8B8;
	if (m_uiCompressType == 0)
	{
		uiCompressFormat = VSNVCompression::NV_ARGB;
		uiVSTextureTpye = VSRenderer::SFT_A8R8G8B8;
	}
	else if (m_uiCompressType == 1)
	{
		uiCompressFormat = VSNVCompression::NV_DXT3;
		uiVSTextureTpye = VSRenderer::SFT_DXT3;
	}
	else if (m_uiCompressType == 2)
	{
		uiCompressFormat = VSNVCompression::NV_DXT5;
		uiVSTextureTpye = VSRenderer::SFT_DXT5;
	}
	VS2DTexture * pNewTexture = VS_NEW VS2DTexture(uiWidth,uiHeight,uiVSTextureTpye,0,1);
	pNewTexture->CreateRAMDate();
	for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);
		VSNVCompression Help;
		Help.Setup(pNow,uiCompressFormat,pTexture->GetWidth(i),pTexture->GetHeight(i),false,m_bIsNormal);
		Help.Process();
		VSMemcpy(pNewTexture->GetBuffer(i),Help.OutputHandler.m_CompressDate.GetBuffer(),Help.OutputHandler.m_CompressDate.GetNum());
	}

	VSMAC_DELETE(pTexture);
	pTexAllState = VS_NEW VSTexAllState(pNewTexture);
	if (m_pSamplerState)
	{
		pTexAllState->SetSamplerState(m_pSamplerState);
	}
	pTexAllState->SetSRGBEable(m_bSRGB);
	return pTexAllState;
}
bool VSTextureRJob::Process()
{
	VSTexAllState * pTex = NULL;
	if (m_bEngineType)
	{
		pTex = LoadTexture();
	}
	else
	{
		if (m_bCompress)
		{
			pTex = Load2DTextureCompress();
		}
		else
		{
			pTex = Load2DTexture();
		}
	}

	if(pTex)
	{
		VSRenderer::ms_pRenderer->LoadTexture(pTex->m_pTex);
		m_pProxy->SetNewResource(pTex);
		m_pProxy->Loaded();
		return true;
	}
	return false;
	
}

VSASYNLoadManager *VSASYNLoadManager::ms_pASYNLoadManager = NULL;
VSASYNLoadManager::VSASYNLoadManager()
{
	
	m_ResourceJobArray.Clear();
	VSMAC_ASSERT(!ms_pASYNLoadManager);
	ms_pASYNLoadManager = this;
}
VSASYNLoadManager::~VSASYNLoadManager()
{

	m_ResourceLoadThread.Start();

	
	m_ResourceLoadThread.Stop();
	
	for (unsigned int i = 0 ; i < m_ResourceJobArray.GetNum() ;i++)
	{
		VSMAC_DELETE(m_ResourceJobArray[i]);
	}
	m_ResourceJobArray.Clear();
}
void VSASYNLoadManager::Update(double AppTime)
{
	ADD_TIME_PROFILE(VSASYNLoadManagerUpdate)
	unsigned int i = 0 ;
	VSREAL t1 = (VSREAL)VSTimer::ms_pTimer->GetGamePlayTime();
	while(i < m_ResourceJobArray.GetNum())
	{
		if (m_ResourceJobArray[i]->m_uiLoadState == VSResourceJob::LS_PENDING)
		{
			i++;
			continue;
		}
		else if (m_ResourceJobArray[i]->m_uiLoadState == VSResourceJob::LS_LOADED)
		{	
			if (m_ResourceJobArray[i]->m_uiJobState == VSResourceJob::JS_DELETE)
			{
				
			}
			else
			{
				VSMAC_ASSERT(VSRenderer::ms_pRenderer);
				if (!m_ResourceJobArray[i]->Process())
				{
					VSMAC_ASSERT(0);
				}
			}
			
			VSMAC_DELETE(m_ResourceJobArray[i]);
			m_ResourceJobArray.Erase(i);				
		}
		
		else
		{
			VSMAC_DELETE(m_ResourceJobArray[i]);
			m_ResourceJobArray.Erase(i);
			VSMAC_ASSERT(0);
		}
		VSREAL t2 = (VSREAL)VSTimer::ms_pTimer->GetGamePlayTime() - t1;
		if (t2 > 10.0f)
		{
			break;
		}
	}
	if (m_ResourceJobArray.GetNum() == 0)
	{
		m_ResourceLoadThread.Suspend();
	}
}
void VSASYNLoadManager::AddTextureLoad(VSTexAllStateRPtr& pTexture,
									   VSFileName & FileName,
									   bool bEngineType,
									   VSSamplerStatePtr pSamplerState,					
									   bool bCompress,
									   unsigned int uiCompressType,
									   bool bIsNormal,bool bSRGB)
{
	VSTextureRJob* pResourceProxyJob = VS_NEW VSTextureRJob(pTexture);
	pResourceProxyJob->m_bIsNormal = bIsNormal;
	pResourceProxyJob->m_bEngineType = bEngineType;
	pResourceProxyJob->m_pSamplerState = pSamplerState;
	pResourceProxyJob->m_bCompress = bCompress;
	pResourceProxyJob->m_uiCompressType = uiCompressType;
	pResourceProxyJob->m_FileName = FileName;
	pResourceProxyJob->m_bSRGB = bSRGB;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddMaterial(VSMaterialRPtr& pMaterial,VSFileName & FileName)
{
	VSMaterialRJob* pResourceProxyJob = VS_NEW VSMaterialRJob(pMaterial);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddAnim(VSAnimRPtr& pAnim,VSFileName & FileName)
{
	VSAnimRJob* pResourceProxyJob = VS_NEW VSAnimRJob(pAnim);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}

void VSASYNLoadManager::AddStaticMesh(VSStaticMeshNodeRPtr& pStaticMesh,VSFileName & FileName)
{
	VSStaticMeshRJob * pResourceProxyJob = VS_NEW VSStaticMeshRJob(pStaticMesh);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddSkelectonMesh(VSSkelectonMeshNodeRPtr& pSkelectonMesh,VSFileName & FileName)
{
	VSSkelectonMeshRJob* pResourceProxyJob = VS_NEW VSSkelectonMeshRJob(pSkelectonMesh);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddAnimTree(VSAnimTreeRPtr& pAnimTree,VSFileName & FileName)
{
	VSAnimTreeRJob* pResourceProxyJob = VS_NEW VSAnimTreeRJob(pAnimTree);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddActor(VSActorRPtr& pActor, VSFileName & FileName)
{
	VSActorRJob* pResourceProxyJob = VS_NEW VSActorRJob(pActor);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddFont(VSFontRPtr& pFont, VSFileName & FileName)
{
	VSFontRJob* pResourceProxyJob = VS_NEW VSFontRJob(pFont);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddMap(VSSceneMapRPtr& pMap, VSFileName & FileName)
{
	VSSceneMapRJob* pResourceProxyJob = VS_NEW VSSceneMapRJob(pMap);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddMorphTree(VSMorphTreeRPtr& pMorphTree,VSFileName & FileName)
{
	VSMorphTreeRJob* pResourceProxyJob = VS_NEW VSMorphTreeRJob(pMorphTree);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::AddJob(VSResourceJob* pResourceProxyJob)
{

	m_ResourceJobArray.AddElement(pResourceProxyJob);
	//m_ResourceLoadThread.AddJob(pResourceProxyJob); //放在这里m_ResourceLoadThread 线程会死锁
	if (m_ResourceJobArray.GetNum() == 1)
	{
		m_ResourceLoadThread.Start();
	}
	m_ResourceLoadThread.AddJob(pResourceProxyJob);
}
void VSASYNLoadManager::DeleteLoadResource(VSFileName & FileName)
{
	for (unsigned int i = 0; i < m_ResourceJobArray.GetNum(); i++)
	{
		if (m_ResourceJobArray[i]->m_uiJobType == VSResourceJob::JT_MAP)
		{
			if (((VSSceneMapRJob *)m_ResourceJobArray[i])->m_FileName == FileName)
			{
				((VSSceneMapRJob *)m_ResourceJobArray[i])->m_uiJobState = VSResourceJob::JS_DELETE;
				return;
			}
		}
	}
}