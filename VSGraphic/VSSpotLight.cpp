#include "VSSpotLight.h"
#include "VSGraphicInclude.h"
#include "VSSceneRender.h"
#include "VSStream.h"
#include "VSOBB3.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSpotLight,VSLocalLight)
BEGIN_ADD_PROPERTY(VSSpotLight,VSLocalLight)
REGISTER_PROPERTY(m_Range, m_Range, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Falloff, Falloff, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Theta, Theta, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Phi, Phi, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, SpotLightShadowType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(SpotLightShadowType, ST_NORMAL)
ADD_ENUM(SpotLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSpotLight)
IMPLEMENT_INITIAL_END
VSSpotLight::VSSpotLight()
{
	m_Range = 1500.0f;

	m_Falloff		= 1.0;
	m_Theta			= VSPI2 / 2.0;
	m_Phi			= VSPI2;
	m_uiRTWidth = 1024;
	m_ZBias = 0.005f;
	SetShadowType(ST_NORMAL);
	m_ProjectShadowColor = VSColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
}
VSSpotLight::~VSSpotLight()
{
	m_pShadowTexture.Clear();

	m_pShadowMapSceneRender = NULL;

}
void VSSpotLight::GetLightRange()
{	
	VSVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	VSVector3 Point3 = GetWorldTranslate();
	VSREAL R = TAN(m_Phi * 0.5f) * m_Range;
	VSOBB3 Obb(Dir, Up, Right, m_Range * 0.5f, R, R, Point3 + Dir * m_Range * 0.5f);
	m_WorldRenderBV = Obb.GetAABB();
	if (HaveLightFun())
	{
		VSCamera LightCamera;
		LightCamera.CreateFromLookDir(Point3, Dir);
		LightCamera.UpdateAll(0);

		LightCamera.SetPerspectiveFov(m_Phi, 1.0f, 1.0f, m_Range);
		m_MVP = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();
	}	
}
bool VSSpotLight::PostClone(VSObject * pObjectSrc)
{
	VSLight::PostClone(pObjectSrc);
	SetShadowType(m_uiShadowType);
	return true;
}
bool VSSpotLight::PostLoad(void * pDate)
{
	VSLight::PostLoad(pDate);
	SetShadowType(m_uiShadowType);
	return true;
}
bool VSSpotLight::Cullby(VSCuller & Culler)
{
	unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV,true);
	if (uiVSF == VSCuller::VSF_ALL || uiVSF == VSCuller::VSF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);
	}
	return true;
}
bool VSSpotLight::IsRelative(VSGeometry * pGeometry)
{
	if (!VSLocalLight::IsRelative(pGeometry))
	{
		return false;
	}
	VSAABB3 GeometryAABB = pGeometry->GetWorldAABB();

	if (GeometryAABB.RelationWith(m_WorldRenderBV) == VSNOINTERSECT)
	{
		return false;
	}
	return true;
}
void VSSpotLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	m_pShadowMapSceneRender = NULL;
	m_pProjectShadowSceneRender = NULL;
}
void VSSpotLight::SetShadowType(unsigned int uiShadowType)
{
	ResetShadow();
	if (uiShadowType == ST_NORMAL || uiShadowType == ST_PROJECT)
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
	m_bShadowMapDrawEnd = false;
	m_uiShadowType = uiShadowType;
}
void VSSpotLight::DisableDepandenceShadowMap(VSCuller & CurCuller,double dAppTime)
{
	if (m_uiShadowType == ST_PROJECT)
	{
		if (m_pShadowRenderTarget)
		{
			VSResourceManager::Disable2DRenderTarget(m_pShadowRenderTarget);
		}
		for (unsigned int i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->m_pTex = NULL;
		}
	}
	
}
void VSSpotLight::DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_PROJECT)
		{
			VSVector3 WorldPos = GetWorldTranslate();

			VSVector3 Dir,Up,Right;
			GetWorldDir(Dir,Up,Right);

			VSShadowCuller TempCuller;


			VSMatrix3X3 Rot = GetWorldRotate();
			VSCamera TempCamera;
			TempCamera.CreateFromLookDir(GetWorldTranslate(), Dir);
			TempCamera.SetPerspectiveFov(m_Phi, 1.0f, 1.0f, m_Range);

			TempCuller.PushCameraPlane(TempCamera);
			TempCuller.m_pLocalLight = this;


			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				VSScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(TempCuller,false,dAppTime);	
			}
			TempCuller.Sort();

			m_LightShadowMatrix = TempCamera.GetViewMatrix() * TempCamera.GetProjMatrix();

			m_pShadowRenderTarget = VSResourceManager::Get2DRenderTarget(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_R32F,0);
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_D24S8,0);
			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,VSCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pShadowRenderTarget);

			m_pShadowMapSceneRender->Draw(TempCuller,dAppTime);

			m_pShadowTexture[0]->m_pTex = m_pShadowRenderTarget->GetCreateBy();

			VSResourceManager::DisableDepthStencil(pDepthStencil);

			m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
			m_pProjectShadowSceneRender->m_fLightRange = m_Range;
			m_pProjectShadowSceneRender->m_LightWorldDirection = Dir;
			m_pProjectShadowSceneRender->m_Falloff = m_Falloff;
			m_pProjectShadowSceneRender->m_Theta = m_Theta;
			m_pProjectShadowSceneRender->m_Phi = m_Phi;
			m_pProjectShadowSceneRender->Draw(CurCuller, dAppTime);
			DisableDepandenceShadowMap(TempCuller, dAppTime);
		}
	}
}
void VSSpotLight::DrawDepandenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	
}
void VSSpotLight::DrawNormalShadowMap(VSCuller & CurCuller,double dAppTime)
{
	if (CurCuller.HasLight(this) == false)
	{
		return;
	}
	VSVector3 Dir,Up,Right;
	GetWorldDir(Dir,Up,Right);

	VSShadowCuller	TempCuller;

	VSCamera LightCamera;
	LightCamera.CreateFromLookDir(GetWorldTranslate(),Dir);
	LightCamera.SetPerspectiveFov(m_Phi,1.0f,1.0f,m_Range);
	if (m_pScene.GetNum() > 0)
	{
		TempCuller.m_pLocalLight = this;
		TempCuller.PushCameraPlane(LightCamera);
		for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
		{
			VSScene * pScene = m_pScene[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(TempCuller,false,dAppTime);	

		}
		TempCuller.Sort();		
	}

	m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();

	m_pShadowRenderTarget = VSResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R32F, 0);
	VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);
	m_pShadowMapSceneRender->ClearRTAndDepth();
	m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);
	m_pShadowMapSceneRender->AddRenderTarget(m_pShadowRenderTarget);

	m_pShadowMapSceneRender->Draw(TempCuller, dAppTime);

	m_pShadowTexture[0]->m_pTex = m_pShadowRenderTarget->GetCreateBy();

	VSResourceManager::DisableDepthStencil(pDepthStencil);
}
void VSSpotLight::DrawNoDepandenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_bShadowMapDrawEnd == false)
		{
			m_bShadowMapDrawEnd = true;
		}
		else
		{
			return;
		}
		if (m_uiShadowType == ST_NORMAL)
		{
			
			DrawNormalShadowMap(CurCuller, dAppTime);
		}
	}
}
void VSSpotLight::DisableNoDepandenceShadowMap(double dAppTime)
{
	VSLocalLight::DisableNoDepandenceShadowMap(dAppTime);
	if (m_uiShadowType == ST_NORMAL)
	{
		if (m_pShadowRenderTarget)
		{
			VSResourceManager::Disable2DRenderTarget(m_pShadowRenderTarget);
		}
		for (unsigned int i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->m_pTex = NULL;
		}
	}
	
}