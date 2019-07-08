#include "VSDirectionLight.h"
#include "VSGraphicInclude.h"
#include "VSSceneRender.h"
#include "VSStream.h"
#include "VSRay3.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDirectionLight,VSLocalLight)
BEGIN_ADD_PROPERTY(VSDirectionLight,VSLocalLight)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, DirectionLightShadowType,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunWidth, LightFunWidth, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunHeight, LightFunHeight, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunNear, LightFunNear, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunFar, LightFunFar, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, VSProperty::F_SAVE_LOAD_CLONE)
BEGIN_ADD_ENUM
ADD_ENUM(DirectionLightShadowType, ST_VOLUME)
ADD_ENUM(DirectionLightShadowType, ST_OSM)
ADD_ENUM(DirectionLightShadowType, ST_CSM)
ADD_ENUM(DirectionLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDirectionLight)
IMPLEMENT_INITIAL_END
#define CSM_LEVLE 3
VSDirectionLight::VSDirectionLight()
{
	m_uiRTWidth = 1024;
	SetShadowType(ST_OSM);
	m_ZBias = 0.001f;
	m_fLightFunWidth = 1000.0f;
	m_fLightFunHeight = 1000.0f; 
	m_fLightFunNear = 1.0f; 
	m_fLightFunFar = 8000.0f;
}
VSDirectionLight::~VSDirectionLight()
{

}
void VSDirectionLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	m_pPEVolumeSMSceneRender = NULL;
	m_pShadowMapSceneRender = NULL;
	m_pProjectShadowSceneRender = NULL;
	m_pVolumeShadowSceneRender = NULL;
	m_pCSMRTArray.Clear();
}
void VSDirectionLight::SetShadowType(unsigned int uiShadowType)
{
	ResetShadow();
	if (uiShadowType == ST_VOLUME)
	{
		
		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetDoubleLine());


		m_pPEVolumeSMSceneRender = VS_NEW VSPEVolumeShadowMapSceneRender();
		m_pPEVolumeSMSceneRender->SetParam(VSRenderer::CF_COLOR,VSColorRGBA(1.0f,1.0f,1.0f,1.0f),1.0f,0);



		m_pVolumeShadowSceneRender = VS_NEW VSVolumeShadowSceneRender();
		m_pVolumeShadowSceneRender->m_pLocalLight = this;
		m_pVolumeShadowSceneRender->SetParam(VSRenderer::CF_STENCIL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);

	}
	else if (uiShadowType == ST_OSM || uiShadowType == ST_PROJECT)
	{
		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetShadowMapSampler());
		m_pShadowMapSceneRender = VS_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_MAX, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
		if (uiShadowType == ST_PROJECT)
		{
			m_pProjectShadowSceneRender = VS_NEW VSProjectShadowSceneRender();
			m_pProjectShadowSceneRender->m_pLocalLight = this;
			m_pProjectShadowSceneRender->SetParam(VSRenderer::CF_STENCIL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
		}
	}
	else if (uiShadowType == ST_CSM)
	{
		for (unsigned int i = 0 ; i < CSM_LEVLE ; i++)
		{
			m_pShadowTexture.AddElement(VS_NEW VSTexAllState());
			m_pShadowTexture[i]->SetSamplerState((VSSamplerState*)VSSamplerState::GetShadowMapSampler());
		}
		m_pCSMRTArray.SetBufferNum(CSM_LEVLE);
		m_pShadowMapSceneRender = VS_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_MAX, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	m_uiShadowType = uiShadowType;

}
bool VSDirectionLight::PostClone(VSObject * pObjectSrc)
{
	VSLight::PostClone(pObjectSrc);

	SetShadowType(m_uiShadowType);
	return true;
}
bool VSDirectionLight::PostLoad(void * pDate)
{
	VSLight::PostLoad(pDate);
	SetShadowType(m_uiShadowType);
	return true;
}


void VSDirectionLight::DisableDepandenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType == ST_VOLUME || m_uiShadowType == ST_OSM || m_uiShadowType == ST_PROJECT)
	{
		if (m_pVolumeShadowRenderTarget)
		{
			VSResourceManager::Disable2DRenderTarget(m_pVolumeShadowRenderTarget);
		}
	}
	else if (m_uiShadowType == ST_CSM)
	{
		for (unsigned int i = 0 ; i < CSM_LEVLE ; i++)
		{
			if (m_pCSMRTArray[i])
			{
				VSResourceManager::Disable2DRenderTarget(m_pCSMRTArray[i]);
			}
		}
	}
	for (unsigned int i = 0 ; i < m_pShadowTexture.GetNum() ; i++)
	{
		m_pShadowTexture[i]->m_pTex = NULL;
	}
}
void VSDirectionLight::DrawVolumeShadow(VSCuller & CurCuller,double dAppTime)
{
	if (m_uiShadowType != ST_VOLUME)
	{
		return;
	}
	VSVolumeShadowMapCuller SMCuller;
	SMCuller.m_pLocalLight = this;
	if (m_pScene.GetNum() > 0)
	{

		SMCuller.PushCameraPlane(*CurCuller.GetCamera());

		for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
		{
			VSScene * pScene = m_pScene[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(SMCuller, false, dAppTime);

		}
		SMCuller.Sort();
	}


	m_pVolumeShadowSceneRender->Draw(SMCuller, dAppTime);
	
	m_pVolumeShadowRenderTarget = VSResourceManager::Get2DRenderTarget(VSRenderer::ms_pRenderer->GetCurRTWidth(), VSRenderer::ms_pRenderer->GetCurRTHeight(), VSRenderer::SFT_A8R8G8B8, 0);

	m_pPEVolumeSMSceneRender->ClearRTAndDepth();
	m_pPEVolumeSMSceneRender->AddRenderTarget(m_pVolumeShadowRenderTarget);
	m_pPEVolumeSMSceneRender->Draw(CurCuller,dAppTime);

	m_pShadowTexture[0]->m_pTex = m_pVolumeShadowRenderTarget->GetCreateBy();

}
VSAABB3 VSDirectionLight::GetMaxAABB(VSArray<VSAABB3> &AABBArray)
{
	VSAABB3 Temp;
	for (unsigned int i = 0 ;i < AABBArray.GetNum() ; i++)
	{
		
		if (i == 0)
		{
			Temp = AABBArray[i];
		}
		else
		{
			Temp = Temp.MergAABB(AABBArray[i]);
		}
	}
	return Temp;
}
void VSDirectionLight::DrawOSM(VSCuller & CurCuller,double dAppTime)
{
#define DRAW_DEPTH \
	{\
		m_pOSMShadowRenderTarget = VSResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);\
		VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);\
		m_pShadowMapSceneRender->ClearRTAndDepth();\
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);\
		m_pShadowMapSceneRender->AddRenderTarget(m_pOSMShadowRenderTarget);\
		m_pShadowMapSceneRender->Draw(DSMCuller, dAppTime);\
		m_pShadowTexture[0]->m_pTex = m_pOSMShadowRenderTarget->GetCreateBy();\
		VSResourceManager::DisableDepthStencil(pDepthStencil);\
	}
	VSDirShadowMapCuller	DSMCuller;
	DSMCuller.m_pLocalLight = this;
	if (m_pScene.GetNum() > 0)
	{
		DSMCuller.PushCameraPlane(*CurCuller.GetCamera());
		for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
		{
			VSScene * pScene = m_pScene[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(DSMCuller, false, dAppTime);

		}
		DSMCuller.Sort();
	}

	VSArray<VSAABB3> CasterAABBArray;
	GetCullerAABBArray(DSMCuller, CasterAABBArray);
	if (CasterAABBArray.GetNum() == 0)
	{
		DRAW_DEPTH;
		return;
	}

	VSAABB3 CasterAABB = GetMaxAABB(CasterAABBArray);

	VSVector3 Center = CasterAABB.GetCenter();

	VSVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	VSRay3 Ray(Center,Dir * (-1.0f));

	unsigned int Q;
	VSREAL tN,tF;
	if(CasterAABB.RelationWith(Ray,Q,tN,tF) != VSINTERSECT)
	{
		DRAW_DEPTH;
		return ;
	}

	VSVector3 LigthPT = Center - Dir * tN * 2.0f;

	VSCamera LightCamera;
	LightCamera.CreateFromLookAt(LigthPT,Center);

	VSMatrix3X3W LightView = LightCamera.GetViewMatrix();

	VSAABB3 NewCasterAABB;
	NewCasterAABB.Transform(CasterAABB,LightView);

	VSREAL NewNear = NewCasterAABB.GetMinPoint().z;
	VSREAL NewFar = NewCasterAABB.GetMaxPoint().z;
	LightCamera.SetOrthogonal(NewCasterAABB.GetMaxPoint().x - NewCasterAABB.GetMinPoint().x,
		NewCasterAABB.GetMaxPoint().y - NewCasterAABB.GetMinPoint().y,
		NewNear,NewFar);

	m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();


	DRAW_DEPTH;
}
void VSDirectionLight::DrawDepandenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{	
		if (m_uiShadowType == ST_OSM)
		{
			DrawOSM(CurCuller,dAppTime);	
		}
		else if (m_uiShadowType == ST_CSM)
		{
			DrawCSM(CurCuller,dAppTime);
		}

	}
}
void VSDirectionLight::DrawCSM(VSCuller & CurCuller, double dAppTime)
{
	if (HaveLightFun())
	{
		return;
	}
#define DRAW_CSM_DEPTH \
	{\
		m_pCSMRTArray[i] = VSResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);\
		VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);\
		m_pShadowMapSceneRender->ClearRTAndDepth();\
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);\
		m_pShadowMapSceneRender->AddRenderTarget(m_pCSMRTArray[i]);\
		m_pShadowMapSceneRender->Draw(Culler, dAppTime);\
		m_pShadowTexture[i]->m_pTex = m_pCSMRTArray[i]->GetCreateBy();\
		VSResourceManager::DisableDepthStencil(pDepthStencil);\
	}

	VSCamera * pCamera = CurCuller.GetCamera();
	VSREAL fNear = pCamera->GetZNear();
	VSREAL fFar = pCamera->GetZFar();

	VSVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	VSREAL Range[CSM_LEVLE + 1] = {fNear , 1000.0f , 3500.0f , fFar};
	for (unsigned int i = 0; i < CSM_LEVLE; i++)
	{

		VSDirShadowMapCuller Culler;
		Culler.m_pLocalLight = this;

		if (m_pScene.GetNum() > 0)
		{
			pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), Range[i], Range[i + 1]);
			Culler.PushCameraPlane(*pCamera);
			for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
			{
				VSScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(Culler, false, dAppTime);

			}
			Culler.Sort();
		}

		VSArray<VSAABB3> CasterAABBArray;
		GetCullerAABBArray(Culler, CasterAABBArray);
		if (CasterAABBArray.GetNum() == 0)
		{
			DRAW_CSM_DEPTH;
			continue;
		}
		VSAABB3 CasterAABB = GetMaxAABB(CasterAABBArray);
		VSAABB3 ReceiverAABB = Culler.GetCamera()->GetFrustumAABB();

		VSMatrix3X3W LightRot;
		LightRot.CreateFromLookDir(VSVector3::ms_Zero,Dir);

		VSAABB3 NewCasterAABB, NewReceiverAABB;
		NewCasterAABB.Transform(CasterAABB, LightRot);
		NewReceiverAABB.Transform(ReceiverAABB, LightRot);

		VSAABB3 MinAABB = NewReceiverAABB.GetMin(NewCasterAABB);
		VSVector3 MinP(MinAABB.GetMinPoint().x, MinAABB.GetMinPoint().y, NewCasterAABB.GetMinPoint().z);
		VSVector3 MaxP(MinAABB.GetMaxPoint().x, MinAABB.GetMaxPoint().y, NewCasterAABB.GetMaxPoint().z);
		MinAABB.Set(MaxP,MinP);
		VSVector3 Center = MinAABB.GetCenter();

		VSRay3 Ray(Center, VSVector3::ms_Front * (-1.0f));

		unsigned int Q;
		VSREAL tN, tF;
		if (MinAABB.RelationWith(Ray, Q, tN, tF) != VSINTERSECT)
		{
			DRAW_CSM_DEPTH;
			continue;
		}

		VSVector3 LigthPT = Center - VSVector3::ms_Front * tN * 10.0f;

		VSVector3 NewLightPT = LigthPT * LightRot.GetInverse();

		VSCamera LightCamera;
		LightCamera.CreateFromLookDir(NewLightPT, Dir);

		MinAABB.Set(MinAABB.GetMaxPoint() - LigthPT,MinAABB.GetMinPoint() - LigthPT);

		
		LightCamera.SetOrthogonal(MinAABB.GetMaxPoint().x - MinAABB.GetMinPoint().x,
			MinAABB.GetMaxPoint().y - MinAABB.GetMinPoint().y,
			MinAABB.GetMinPoint().z, MinAABB.GetMaxPoint().z);

		m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();

		m_CSMLightShadowMatrix[i] = m_LightShadowMatrix;

		DRAW_CSM_DEPTH;
		
	}
	pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), fNear, fFar);
}
void VSDirectionLight::GetLightRange()
{
	
	VSVector3 Point3 = GetWorldTranslate();
	VSVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	VSVector3 Middle = Point3 + Dir * (m_fLightFunFar + m_fLightFunNear) * 0.5;
	VSOBB3 OBB(Dir, Up, Right, (m_fLightFunFar - m_fLightFunNear) * 0.5f, m_fLightFunHeight * 0.5f, m_fLightFunWidth * 0.5f, Middle);
	m_WorldRenderBV = OBB.GetAABB();
	if (HaveLightFun())
	{
		VSCamera LightCamera;
		LightCamera.CreateFromLookDir(Point3, Dir);
		LightCamera.SetOrthogonal(m_fLightFunWidth, m_fLightFunHeight, m_fLightFunNear, m_fLightFunFar);
		LightCamera.UpdateAll(0);
		m_MVP = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();
	}
}
bool VSDirectionLight::IsRelative(VSGeometry * pGeometry)
{
	if (!VSLight::IsRelative(pGeometry))
	{
		return false;
	}
	if (HaveLightFun())
	{
		VSAABB3 GeometryAABB = pGeometry->GetWorldAABB();
		if (GeometryAABB.RelationWith(m_WorldRenderBV) == VSNOINTERSECT)
		{
			return false;
		}
	}	
	return true;
}
void VSDirectionLight::DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow && m_uiShadowType == ST_PROJECT)
	{
		VSVector3 Dir,Up,Right;
		GetWorldDir(Dir,Up,Right);
		VSDirShadowMapCuller	SMCuller;
		SMCuller.m_pLocalLight = this;
		if (m_pScene.GetNum() > 0)
		{

			SMCuller.PushCameraPlane(*CurCuller.GetCamera());

			for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
			{
				VSScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(SMCuller, false, dAppTime);

			}
			SMCuller.Sort();
		}

		VSArray<VSDirShadowMapCuller> Temp;
		VSMeshNode * pCurMeshNode = NULL;
		for (unsigned int t = 0; t <= VSCuller::VST_MAX; t++)
		{
			for (unsigned int j = 0; j < SMCuller.GetVisibleNum(t); j++)
			{
				VSRenderContext& RenderContext = SMCuller.GetVisibleSpatial(j, t);
				if (pCurMeshNode != RenderContext.m_pMeshNode)
				{
					Temp.AddElement(VSDirShadowMapCuller());
					Temp[Temp.GetNum() - 1].PushCameraPlane(*SMCuller.GetCamera());
					pCurMeshNode = RenderContext.m_pMeshNode;
				}
				Temp[Temp.GetNum() - 1].InsertObject(RenderContext, t);
			}
		}
		m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
		for (unsigned int i = 0; i < Temp.GetNum(); i++)
		{
			VSArray<VSAABB3> CasterAABBArray;
			GetCullerAABBArray(Temp[i], CasterAABBArray);
	
			VSAABB3 CasterAABB = GetMaxAABB(CasterAABBArray);
			VSVector3 Center = CasterAABB.GetCenter();

			VSRay3 Ray(Center, Dir * (-1.0f));

			unsigned int Q;
			VSREAL tN, tF;
			if (CasterAABB.RelationWith(Ray, Q, tN, tF) != VSINTERSECT)
			{
				continue;
			}

			VSVector3 LigthPT = Center - Dir * tN * 2.0f;

			VSCamera LightCamera;
			LightCamera.CreateFromLookAt(LigthPT, Center);

			LightCamera.UpdateAll(0);
			VSMatrix3X3W LightView = LightCamera.GetViewMatrix();

			VSAABB3 NewCasterAABB;
			NewCasterAABB.Transform(CasterAABB, LightView);


			LightCamera.SetOrthogonal(NewCasterAABB.GetMaxPoint().x - NewCasterAABB.GetMinPoint().x,
				NewCasterAABB.GetMaxPoint().y - NewCasterAABB.GetMinPoint().y,
				NewCasterAABB.GetMinPoint().z, NewCasterAABB.GetMaxPoint().z + 10000.0f);

			m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();


			m_pProjectShadowRenderTarget = VSResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);

			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(Temp[i], dAppTime);
			m_pShadowTexture[0]->m_pTex = m_pProjectShadowRenderTarget->GetCreateBy();
			VSResourceManager::DisableDepthStencil(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(Temp[i], dAppTime);
			DisableDepandenceShadowMap(Temp[i], dAppTime);
		}	
	}
}
bool VSDirectionLight::SetLightMaterial(VSMaterialR * pMaterial)
{
	if (VSLocalLight::SetLightMaterial(pMaterial))
	{
		return true;
	}
	return false;
}
bool VSDirectionLight::Cullby(VSCuller & Culler)
{
	if (HaveLightFun())
	{
		unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV, true);
		if (uiVSF == VSCuller::VSF_ALL || uiVSF == VSCuller::VSF_PARTIAL)
		{
			m_bEnable = true;
			Culler.InsertLight(this);

		}
	}
	else
	{
		m_bEnable = true;
		Culler.InsertLight(this);
	}
	return true;
}