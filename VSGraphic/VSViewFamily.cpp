#include "VSViewFamily.h"
#include "VSGraphicInclude.h"
#include "VSCubeTexture.h"
#include "VSCaptureTexAllState.h"
#include "VSResourceManager.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSSceneRenderMethod, VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSSceneRenderMethod)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSSceneRenderMethod, VSObject)
END_ADD_PROPERTY
VSSceneRenderMethod::VSSceneRenderMethod()
{
	m_pPostEffectSet = NULL;
	m_pPostEffectInstance = NULL;

	m_pDebugDrawSceneRender = VS_NEW VSDebugDrawSceneRender();
	m_pDebugDrawSceneRender->SetParam(VSRenderer::CF_NONE,VSColorRGBA(0.0f,0.0f,0.0f,1.0f),1.0f,0);

	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
	m_pColorRT = NULL;
	ColorRTIsOutSet = false;
}
void VSSceneRenderMethod::SetPostEffect(VSPostEffectSetR* pPostEffectSet)
{
	m_pPostEffectSet = pPostEffectSet;
	m_pPostEffectInstance = NULL;
	if (m_pPostEffectSet)
	{

		m_pPostEffectSet->AddLoadEventObject(this);
	}
}
VSSceneRenderMethod::~VSSceneRenderMethod()
{
	m_pPostEffectInstance = NULL;
	m_pPostEffectSet = NULL;
	m_pDebugDrawSceneRender = NULL;
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
}
VSRenderTarget * VSSceneRenderMethod::GetFinalColorRT()
{
	if (m_pPostEffectInstance)
	{
		return m_pPostEffectInstance->GetEndTarget(0);
	}
	else
	{
		return m_pColorRT;
	}
}
void VSSceneRenderMethod::DisableRT()
{
	if (m_pColorRT && !ColorRTIsOutSet)
	{
		VSResourceManager::Disable2DRenderTarget(m_pColorRT);
	}
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		if(m_pDepthStencil[i])
		{
			VSResourceManager::DisableDepthStencil(m_pDepthStencil[i]);
		}
	}
	if (m_pPostEffectInstance)
	{
		m_pPostEffectInstance->DisableRT();
	}
}
void VSSceneRenderMethod::SetColorRT(VSRenderTarget * pFinalColorRT,unsigned int uiWidth,unsigned int uiHeight)
{
	if (m_pPostEffectInstance)
	{
		if (!m_pPostEffectInstance->SetEndTarget(pFinalColorRT))
		{
			if (m_pColorRT)
			{
				VSResourceManager::Disable2DRenderTarget(m_pColorRT);
			}
			ColorRTIsOutSet = true;
			m_pColorRT = pFinalColorRT;
		}

	}
	else
	{
		if (m_pColorRT)
		{
			VSResourceManager::Disable2DRenderTarget(m_pColorRT);
		}
		ColorRTIsOutSet = true;
		m_pColorRT = pFinalColorRT;

	}
}
void VSSceneRenderMethod::GetRT(unsigned int uiWidth,unsigned int uiHeight)
{

	m_pColorRT = NULL;

	m_pColorRT = VSResourceManager::Get2DRenderTarget(uiWidth,uiHeight,VSRenderer::SFT_A8R8G8B8,0);

	ColorRTIsOutSet = false;
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
		m_pDepthStencil[i] = VSResourceManager::GetDepthStencil(uiWidth,uiHeight,VSRenderer::SFT_D24S8,0);
	}

	if (m_pDebugDrawSceneRender)
	{
		m_pDebugDrawSceneRender->ClearRTAndDepth();
		if (m_bBeforePostDebugDraw)
		{
			m_pDebugDrawSceneRender->AddRenderTarget(m_pColorRT);
		}
		else
		{
			m_pDebugDrawSceneRender->AddRenderTarget(GetFinalColorRT());
		}

		for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
		{
			m_pDebugDrawSceneRender->SetDepthStencil(m_pDepthStencil[i],i);
		}
	}	

	if (m_pPostEffectInstance)
	{
		static VSArray<VSRenderTarget *> Temp;
		Temp.Clear();
		Temp.AddElement(m_pColorRT);
		m_pPostEffectInstance->SetBeginTargetArray(&Temp);
		m_pPostEffectInstance->GetRT(uiWidth,uiHeight);
	}
}
void VSSceneRenderMethod::Draw(VSCuller & Culler,double dAppTime)
{

	if (m_bDrawDebugInfo)
	{
		m_pDebugDrawSceneRender->ClearRTAndDepth();
		if (m_bBeforePostDebugDraw)
		{
			m_pDebugDrawSceneRender->AddRenderTarget(m_pColorRT);
		}
		else
		{
			m_pDebugDrawSceneRender->AddRenderTarget(GetFinalColorRT());
		}

		for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
		{
			m_pDebugDrawSceneRender->SetDepthStencil(m_pDepthStencil[i],i);
		}
	}

	if (m_bBeforePostDebugDraw && m_bDrawDebugInfo)
	{
		m_pDebugDrawSceneRender->Draw(Culler,dAppTime);
	}

	if (m_pPostEffectInstance)
	{	
		m_pPostEffectInstance->Draw(Culler,dAppTime);

	}
	if (!m_bBeforePostDebugDraw && m_bDrawDebugInfo)
	{
		m_pDebugDrawSceneRender->Draw(Culler,dAppTime);
	}
}
void VSSceneRenderMethod::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	VSPostEffectSet *	pPostEffectInstance = m_pPostEffectSet->GetResource();
	m_pPostEffectInstance = (VSPostEffectSet *)VSObject::CloneCreateObject(pPostEffectInstance);
}

VSDebugDraw * VSSceneRenderMethod::GetDebugDraw(unsigned int uiRenderGroup)
{
	if (m_pDebugDrawSceneRender)
	{
		return m_pDebugDrawSceneRender->GetDebugDraw(uiRenderGroup);
	}
	return NULL;
}
IMPLEMENT_RTTI_NoCreateFun(VSViewFamily,VSObject)
VSViewFamily::VSViewFamily(const VSString &ViewFamilyName,VSCamera * pCamera)
{
	m_pCamera = pCamera;
	m_pCamera->AddViewFamily(this);

	m_uiWidth = 0;
	m_uiHeight = 0;




	m_ViewFamilyName = ViewFamilyName;
	m_bEnable = true;
}
VSViewFamily::VSViewFamily(const VSString &ViewFamilyName, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
{
	m_pCamera = pCamera;
	m_pCamera->AddViewFamily(this);

	m_uiWidth = 0;
	m_uiHeight = 0;

	


	m_ViewFamilyName = ViewFamilyName;
	m_bEnable = true;

	VSString RMName = RenderMethodRTTIName;
	m_pSceneRenderMethod = DynamicCast<VSSceneRenderMethod>(VSObject::GetNoGCInstance(RMName));
	if (m_pSceneRenderMethod)
	{
		m_pSceneRenderMethod->SetPostEffect(pPostEffectSet);
	}
	
}

VSViewFamily::~VSViewFamily()
{
	m_pCamera = NULL;

	m_pScene.Clear();

}
bool VSViewFamily::IsReCreate(unsigned int uiWidth,unsigned int uiHeight)
{
	if (m_uiWidth == uiWidth && m_uiHeight == uiHeight)
	{
		return false;
	}
	return true;
}



void VSViewFamily::Update(double dAppTime)
{
	
	VSArray<VSScene *> Temp ;
	if (!m_pCamera)
	{
		return ;
	}
	if (!m_pCamera->m_bEnable)
	{
		return ;
	}
	Temp.Clear();	
	Temp.AddElement(m_pScene,0,m_pScene.GetNum() - 1);
	if (Temp.GetNum() > 0)
	{
		m_Culler.ClearPlaneState();
		m_Culler.ClearAllPlane();
		m_Culler.ClearVisibleSet();
		m_Culler.ClearLight();
		m_Culler.PushCameraPlane(*m_pCamera);
		for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
		{
			VSScene * pScene = Temp[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(m_Culler,false,dAppTime);	
	
		}

		if(m_Culler.GetAllVisibleNum() == 0)
			return ;
		m_Culler.Sort();
		for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
		{
			VSScene * pScene = Temp[i];
			if (!pScene)
			{
				continue;
			}
			for (unsigned int i = 0 ; i < pScene->GetAllLightNum() ; i++)
			{
				VSLight * pLight = pScene->GetAllLight(i);
				if (pLight)
				{
					pLight->Cullby(m_Culler);
				}
			}
		}
		m_Culler.GetAndSortLight();
	}
}
void VSViewFamily::OnDraw(double dAppTime)
{
	if (m_pSceneRenderMethod)
	{
		m_pSceneRenderMethod->Draw(m_Culler,dAppTime);
	}
}
void VSViewFamily::Draw(double dAppTime)
{
	if (m_pCamera)
	{
		if (!m_pCamera->m_bEnable)
		{
			return ;
		}

		if (m_pSceneRenderMethod)
		{
			m_pSceneRenderMethod->GetRT(m_uiWidth,m_uiHeight);
		}
		
		OnDraw(dAppTime);

		if (m_pSceneRenderMethod)
		{
			m_pSceneRenderMethod->DisableRT();
		}
		
	}
}
void VSViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	m_uiHeight = uiHeight;
	m_uiWidth = uiWidth;
	
}
VSRenderTarget * VSViewFamily::GetFinalColorRT()
{	

	if (m_pSceneRenderMethod)
	{
		return m_pSceneRenderMethod->GetFinalColorRT();
	}
	return NULL;

}
IMPLEMENT_RTTI_NoCreateFun(VSWindowViewFamily,VSViewFamily);
VSWindowViewFamily::VSWindowViewFamily(const VSString &ViewFamilyName, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName, int iWindowID)
					:VSViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName)
{

	
	m_iWindowID = iWindowID;
	m_pScreenQuadRenderer = VS_NEW VSPEScreenQuadSceneRender();

}
VSWindowViewFamily::VSWindowViewFamily(const VSString &ViewFamilyName,VSCamera * pCamera,int iWindowID)
:VSViewFamily(ViewFamilyName,pCamera)
{
	m_iWindowID = iWindowID;
	m_pScreenQuadRenderer = VS_NEW VSPEScreenQuadSceneRender();
}
VSWindowViewFamily::~VSWindowViewFamily()
{
	m_pScreenQuadRenderer = NULL;
	
}
void VSWindowViewFamily::OnDraw(double dAppTime)
{
	VSViewFamily::OnDraw(dAppTime);
	VSRenderTarget *pRenderTarget = GetFinalColorRT();
	VSMAC_ASSERT(pRenderTarget);

	m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
	m_pScreenQuadRenderer->Draw(m_Culler, dAppTime);


}
void VSWindowViewFamily::Draw(double dAppTime)
{
	unsigned int uiHeight = VSRenderer::ms_pRenderer->GetScreenHeight();
	unsigned int uiWidth = VSRenderer::ms_pRenderer->GetScreenWith();
	VSRenderer::ms_pRenderer->UseWindow(m_iWindowID);
	if (VSRenderer::ms_pRenderer->IsWindowed())
	{

		VSRenderer::ChildWindowInfo * pChildInfo = VSRenderer::ms_pRenderer->GetChildWindowInfo(m_iWindowID);
		if (pChildInfo)
		{
			uiHeight = pChildInfo->m_uiHeight;
			uiWidth = pChildInfo->m_uiWidth;
		}

	}

	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}

	VSViewFamily::Draw(dAppTime);
	
}
void VSWindowViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	VSViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);

	m_pScreenQuadRenderer->SetNoUseRTRenderSize(uiWidth,uiHeight);

}
IMPLEMENT_RTTI_NoCreateFun(VSCaptureViewFamily,VSViewFamily)
VSCaptureViewFamily::VSCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera)
:VSViewFamily(ViewFamilyName,pCamera)
{
	VSMAC_ASSERT(uiWidth && uiHeight);
	m_pScreenQuadRenderer = VS_NEW VSPEScreenQuadSceneRender();
	
	m_OnlyUpdateOneTime = false;
	m_pTexOwner = NULL;
}
VSCaptureViewFamily::VSCaptureViewFamily(const VSString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
:VSViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName)
{
	VSMAC_ASSERT(uiWidth && uiHeight);
	m_pScreenQuadRenderer = VS_NEW VSPEScreenQuadSceneRender();

	m_OnlyUpdateOneTime = false;
	m_pTexOwner = NULL;
}
void VSCaptureViewFamily::Draw(double dAppTime)
{
	
	VSViewFamily::Draw(dAppTime);
	if (m_pTexOwner)
	{
		m_pTexOwner->NotifyEndDraw();
		m_pTexOwner = NULL;
	}
	if (m_OnlyUpdateOneTime)
	{
		m_bEnable = false;
	}
}
void VSCaptureViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	VSViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);
	m_pScreenQuadRenderer->SetNoUseRTRenderSize(uiWidth,uiHeight);

}
VSCaptureViewFamily::~VSCaptureViewFamily()
{
	m_pScreenQuadRenderer = NULL;
}
void VSCaptureViewFamily::OnDraw(double dAppTime)
{
	VSViewFamily::OnDraw(dAppTime);
	VSRenderTarget *pRenderTarget = GetFinalColorRT();
	VSMAC_ASSERT(pRenderTarget);

	m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
	m_pScreenQuadRenderer->Draw(m_Culler,dAppTime);
}
void VSCaptureViewFamily::SetSize(unsigned int uiWidth,unsigned int uiHeight)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
IMPLEMENT_RTTI_NoCreateFun(VS2DCaptureViewFamily,VSCaptureViewFamily)
VS2DCaptureViewFamily::VS2DCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera)
:VSCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
VS2DCaptureViewFamily::VS2DCaptureViewFamily(const VSString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
:VSCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
VS2DCaptureViewFamily::~VS2DCaptureViewFamily()
{
	m_pRenderTarget = NULL;
}
void VS2DCaptureViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	VSCaptureViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);
	VS2DTexture * pTexture = VS_NEW VS2DTexture(m_uiWidth,m_uiHeight,VSRenderer::SFT_A8R8G8B8);

	m_pRenderTarget = VSResourceManager::CreateRenderTarget(pTexture,VSRenderer::ms_pRenderer->GetCurMultisample());

}
VSTexture * VS2DCaptureViewFamily::GetTexture()const
{
	return m_pRenderTarget->GetCreateBy();
}
void VS2DCaptureViewFamily::OnDraw(double dAppTime)
{

	VSViewFamily::OnDraw(dAppTime);
	VSRenderTarget *pRenderTarget = GetFinalColorRT();
	VSMAC_ASSERT(pRenderTarget);

 	m_pScreenQuadRenderer->ClearRTAndDepth();
 	m_pScreenQuadRenderer->AddRenderTarget(m_pRenderTarget);
	m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
	m_pScreenQuadRenderer->Draw(m_Culler,dAppTime);

}
IMPLEMENT_RTTI_NoCreateFun(VSCubCaptureViewFamily,VSCaptureViewFamily)
VSCubCaptureViewFamily::VSCubCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera)
:VSCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
VSCubCaptureViewFamily::VSCubCaptureViewFamily(const VSString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
:VSCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
VSCubCaptureViewFamily::~VSCubCaptureViewFamily()
{
	for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ;i++)
	{
		m_pCubRenderTarget[i] = NULL;
	}
}

VSTexture * VSCubCaptureViewFamily::GetTexture()const
{
	return m_pCubRenderTarget[0]->GetCreateBy();
}
void VSCubCaptureViewFamily::OnDraw(double dAppTime)
{

	for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
	{
		m_pSceneRenderMethod->Draw(m_CubCuller[Index],dAppTime);

		VSRenderTarget *pRenderTarget = GetFinalColorRT();
		VSMAC_ASSERT(pRenderTarget);
		m_pScreenQuadRenderer->ClearRTAndDepth();
		m_pScreenQuadRenderer->AddRenderTarget(m_pCubRenderTarget[Index]);
		m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
		m_pScreenQuadRenderer->Draw(m_Culler,dAppTime);

	}
		
}
void VSCubCaptureViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	
	VSCaptureViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);
	VSCubeTexture * pTexture = VS_NEW VSCubeTexture(m_uiWidth,VSRenderer::SFT_A8R8G8B8);
	for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ;i++)
	{
		VSRenderTarget * pRenderTarget = VSResourceManager::CreateRenderTarget(pTexture,VSRenderer::ms_pRenderer->GetCurMultisample(),0,i);
		m_pCubRenderTarget[i] = pRenderTarget;
	}

}
void VSCubCaptureViewFamily::Update(double dAppTime)
{
	VSArray<VSScene *> Temp ;
	if (!m_pCamera)
	{
		return ;
	}
	if (!m_pCamera->m_bEnable)
	{
		return ;
	}
	Temp.Clear();	
	Temp.AddElement(m_pScene,0,m_pScene.GetNum() - 1);
	VSMatrix3X3 MatTemp[VSCubeTexture::F_MAX] = {VSMatrix3X3::ms_CameraViewRight,VSMatrix3X3::ms_CameraViewLeft,VSMatrix3X3::ms_CameraViewUp,VSMatrix3X3::ms_CameraViewDown,VSMatrix3X3::ms_CameraViewFront,VSMatrix3X3::ms_CameraViewBack};
	
	if (Temp.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
		{
			m_CubCuller[Index].ClearPlaneState();
			m_CubCuller[Index].ClearAllPlane();
			m_CubCuller[Index].ClearVisibleSet();
			m_CubCuller[Index].ClearLight();

			CubCameraPtr[Index] = VS_NEW VSCamera();
			CubCameraPtr[Index]->CreateFromEuler(m_pCamera->GetWorldTranslate(),0.0f,0.0f,0.0f);
			CubCameraPtr[Index]->SetLocalRotate(MatTemp[Index]);
			CubCameraPtr[Index]->SetPerspectiveFov(AngleToRadian(90.0f),1.0f,1.0f,m_pCamera->GetZFar());
			CubCameraPtr[Index]->UpdateAll(0);

			m_CubCuller[Index].PushCameraPlane(*CubCameraPtr[Index]);
			for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
			{
				VSScene * pScene = Temp[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(m_CubCuller[Index],false,dAppTime);	

			}

			if(m_CubCuller[Index].GetAllVisibleNum() == 0)
				continue ;
			m_CubCuller[Index].Sort();
			for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
			{
				VSScene * pScene = Temp[i];
				if (!pScene)
				{
					continue;
				}
				for (unsigned int i = 0 ; i < pScene->GetAllLightNum() ; i++)
				{
					VSLight * pLight = pScene->GetAllLight(i);
					if (pLight)
					{
						pLight->Cullby(m_CubCuller[Index]);
					}
				}
			}
		}
		
	}
}

IMPLEMENT_RTTI(VSForwordHighEffectSceneRenderMethod, VSSceneRenderMethod)
BEGIN_ADD_PROPERTY(VSForwordHighEffectSceneRenderMethod, VSSceneRenderMethod)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSForwordHighEffectSceneRenderMethod)
IMPLEMENT_INITIAL_END
VSForwordHighEffectSceneRenderMethod::~VSForwordHighEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = NULL;
	m_pNormalDepthSceneRender = NULL;
	m_pGammaCorrectSceneRender = NULL;
	m_pSSRSceneRender = NULL;

	m_pMaterialRT = NULL;
	m_pNormalDepthRT = NULL;
}
VSForwordHighEffectSceneRenderMethod::VSForwordHighEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = VS_NEW VSMaterialSceneRender();
	m_pMaterialSceneRenderder->SetParam(VSRenderer::CF_COLOR,VSColorRGBA(0.0f,0.0f,0.0f,1.0f),1.0f,0);
	m_pNormalDepthSceneRender = VS_NEW VSNormalDepthSceneRender();
	m_pNormalDepthSceneRender->SetParam(VSRenderer::CF_USE_MAX,VSColorRGBA(0.0f,0.0f,1.0f,0.0f),1.0f,0);
	m_pGammaCorrectSceneRender = VS_NEW VSGammaCorrectSceneRender();
	m_pSSRSceneRender = VS_NEW VSPESSRSceneRender();

	m_pMaterialRT = NULL;
	m_pNormalDepthRT = NULL;

}
void VSForwordHighEffectSceneRenderMethod::Draw(VSCuller & Culler, double dAppTime)
{
	m_pNormalDepthSceneRender->Draw(Culler,dAppTime);
	m_pMaterialSceneRenderder->Draw(Culler,dAppTime);

	//m_pSSRSceneRender->SetUseTexture(m_pNormalDepthRT->GetCreateBy(),m_pMaterialRT->GetCreateBy());
	//m_pSSRSceneRender->Draw(Culler,dAppTime);

	m_pGammaCorrectSceneRender->SetSourceTarget(m_pMaterialRT->GetCreateBy());
	m_pGammaCorrectSceneRender->Draw(Culler,dAppTime);
	VSSceneRenderMethod::Draw(Culler,dAppTime);
}
void VSForwordHighEffectSceneRenderMethod::DisableRT()
{
	VSSceneRenderMethod::DisableRT();
	if (m_pMaterialRT)
	{
		VSResourceManager::Disable2DRenderTarget(m_pMaterialRT);
	}
	
	if (m_pNormalDepthRT)
	{
		VSResourceManager::Disable2DRenderTarget(m_pNormalDepthRT);
	}
	
	if (m_pSSRRT)
	{
		VSResourceManager::Disable2DRenderTarget(m_pSSRRT);
	}
}
void VSForwordHighEffectSceneRenderMethod::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	VSSceneRenderMethod::GetRT(uiWidth,uiHeight);

	m_pMaterialRT = NULL;
	m_pMaterialRT = VSResourceManager::Get2DRenderTarget(uiWidth,uiHeight,VSRenderer::SFT_A16B16G16R16F,0);

	m_pNormalDepthRT = NULL;
	m_pNormalDepthRT = VSResourceManager::Get2DRenderTarget(uiWidth,uiHeight,VSRenderer::SFT_A16B16G16R16F,0);

	m_pSSRRT = NULL;
	//SFT_ARGB8 ?
	m_pSSRRT = VSResourceManager::Get2DRenderTarget(uiWidth,uiHeight,VSRenderer::SFT_A16B16G16R16F,0);


	m_pNormalDepthSceneRender->ClearRTAndDepth();
	m_pNormalDepthSceneRender->AddRenderTarget(m_pNormalDepthRT);
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pNormalDepthSceneRender->SetDepthStencil(m_pDepthStencil[i],i);
	}

	m_pMaterialSceneRenderder->ClearRTAndDepth();
	m_pMaterialSceneRenderder->AddRenderTarget(m_pMaterialRT);
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pMaterialSceneRenderder->SetDepthStencil(m_pDepthStencil[i],i);
	}
	m_pMaterialSceneRenderder->SetNormalDepthTexture((VS2DTexture *)m_pNormalDepthRT->GetCreateBy());

	m_pSSRSceneRender->ClearRTAndDepth();
	m_pSSRSceneRender->AddRenderTarget(m_pSSRRT);

	m_pGammaCorrectSceneRender->ClearRTAndDepth();
	m_pGammaCorrectSceneRender->AddRenderTarget(m_pColorRT);

}
void VSForwordHighEffectSceneRenderMethod::SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_pMaterialSceneRenderder->SetUseState(RenderState,uiRenderStateInheritFlag);
}
void VSForwordHighEffectSceneRenderMethod::ClearUseState()
{
	m_pMaterialSceneRenderder->ClearUseState();
}


IMPLEMENT_RTTI(VSForwordEffectSceneRenderMethod, VSSceneRenderMethod)
BEGIN_ADD_PROPERTY(VSForwordEffectSceneRenderMethod, VSSceneRenderMethod)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSForwordEffectSceneRenderMethod)
IMPLEMENT_INITIAL_END
VSForwordEffectSceneRenderMethod::~VSForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = NULL;

	m_pGammaCorrectSceneRender = NULL;

	m_pMaterialRT = NULL;
}
VSForwordEffectSceneRenderMethod::VSForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = VS_NEW VSMaterialSceneRender();
	m_pMaterialSceneRenderder->SetParam(VSRenderer::CF_USE_MAX, VSColorRGBA(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	m_pGammaCorrectSceneRender = VS_NEW VSGammaCorrectSceneRender();


	m_pMaterialRT = NULL;


}
void VSForwordEffectSceneRenderMethod::Draw(VSCuller & Culler, double dAppTime)
{

	m_pMaterialSceneRenderder->Draw(Culler, dAppTime);
	m_pGammaCorrectSceneRender->SetSourceTarget(m_pMaterialRT->GetCreateBy());
	m_pGammaCorrectSceneRender->Draw(Culler, dAppTime);	
	VSSceneRenderMethod::Draw(Culler, dAppTime);
}
void VSForwordEffectSceneRenderMethod::DisableRT()
{
	VSSceneRenderMethod::DisableRT();
	if (m_pMaterialRT)
	{
		VSResourceManager::Disable2DRenderTarget(m_pMaterialRT);
	}
}
void VSForwordEffectSceneRenderMethod::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	VSSceneRenderMethod::GetRT(uiWidth, uiHeight);

	m_pMaterialRT = NULL;
	m_pMaterialRT = VSResourceManager::Get2DRenderTarget(uiWidth, uiHeight, VSRenderer::SFT_A16B16G16R16F, 0);


	m_pMaterialSceneRenderder->ClearRTAndDepth();
	m_pMaterialSceneRenderder->AddRenderTarget(m_pMaterialRT);
	for (unsigned int i = 0; i < VSCuller::RG_MAX; i++)
	{
		m_pMaterialSceneRenderder->SetDepthStencil(m_pDepthStencil[i], i);
	}

	m_pGammaCorrectSceneRender->ClearRTAndDepth();
	m_pGammaCorrectSceneRender->AddRenderTarget(m_pColorRT);

}
void VSForwordEffectSceneRenderMethod::SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_pMaterialSceneRenderder->SetUseState(RenderState, uiRenderStateInheritFlag);
}
void VSForwordEffectSceneRenderMethod::ClearUseState()
{
	m_pMaterialSceneRenderder->ClearUseState();
}

IMPLEMENT_RTTI(VSSimpleForwordEffectSceneRenderMethod, VSSceneRenderMethod)
BEGIN_ADD_PROPERTY(VSSimpleForwordEffectSceneRenderMethod, VSSceneRenderMethod)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSimpleForwordEffectSceneRenderMethod)
IMPLEMENT_INITIAL_END
VSSimpleForwordEffectSceneRenderMethod::~VSSimpleForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = NULL;
}
VSSimpleForwordEffectSceneRenderMethod::VSSimpleForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = VS_NEW VSMaterialSceneRender();
	m_pMaterialSceneRenderder->SetParam(VSRenderer::CF_USE_MAX, VSColorRGBA(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);



}
void VSSimpleForwordEffectSceneRenderMethod::Draw(VSCuller & Culler, double dAppTime)
{


	m_pMaterialSceneRenderder->Draw(Culler, dAppTime);

	VSSceneRenderMethod::Draw(Culler, dAppTime);
}
void VSSimpleForwordEffectSceneRenderMethod::DisableRT()
{
	VSSceneRenderMethod::DisableRT();


}
void VSSimpleForwordEffectSceneRenderMethod::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	VSSceneRenderMethod::GetRT(uiWidth, uiHeight);


	m_pMaterialSceneRenderder->ClearRTAndDepth();
	m_pMaterialSceneRenderder->AddRenderTarget(m_pColorRT);
	for (unsigned int i = 0; i < VSCuller::RG_MAX; i++)
	{
		m_pMaterialSceneRenderder->SetDepthStencil(m_pDepthStencil[i], i);
	}



}
void VSSimpleForwordEffectSceneRenderMethod::SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_pMaterialSceneRenderder->SetUseState(RenderState, uiRenderStateInheritFlag);
}
void VSSimpleForwordEffectSceneRenderMethod::ClearUseState()
{
	m_pMaterialSceneRenderder->ClearUseState();
}