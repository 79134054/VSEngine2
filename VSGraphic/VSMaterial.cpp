#include "VSMaterial.h"
#include "VSTexSampler.h"
#include "VS2DTexSampler.h"
#include "VSSpatial.h"
#include "VSShaderStringFactory.h"
#include "VSConstFloatValue.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSVertexColor.h"
#include "VSGraphicInclude.h"
#include "VSPhoneShaderFunction.h"
#include "VSOrenNayarShaderFunction.h"
#include "VSStream.h"
#include "VSCustomShaderFunction.h"
#include "VSLightShaderFunction.h"
#include "VSPosShaderFunction.h"
#include "VSCameraShaderFunction.h"
#include "VSNormalFunction.h"
#include "VSArithmeticShaderFunction.h"
#include "VSMinnaertShaderFunction.h"
#include "VSShirleyShaderFunction.h"
#include "VSIsotropicWardShaderFunction.h"
#include "VSAnisotropicWardShaderFunction.h"
#include "VSSchlickShaderFunction.h"
#include "VSCookTorranceShaderFunction.h"
#include "VSStraussShaderFunction.h"
#include "VSColorBuffer.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(MaterialCustomPara,MaterialCustomPara)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(MaterialCustomPara)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(MaterialCustomPara,VSObject)
REGISTER_PROPERTY(ConstValueName,ConstValueName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY

MaterialCustomPara::MaterialCustomPara()
{

}
MaterialCustomPara::~MaterialCustomPara()
{

}
IMPLEMENT_RTTI(CustomTexSampler,MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomTexSampler,MaterialCustomPara)
REGISTER_PROPERTY(m_pTexture,Texture,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomTexSampler)
IMPLEMENT_INITIAL_END
CustomTexSampler::CustomTexSampler()
{
	m_pTexture = NULL;
}
CustomTexSampler::~CustomTexSampler()
{
	m_pTexture = NULL;
}
IMPLEMENT_RTTI(CustomFloatValue,MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomFloatValue,MaterialCustomPara)
REGISTER_PROPERTY(Value,Value,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomFloatValue)
IMPLEMENT_INITIAL_END
CustomFloatValue::CustomFloatValue()
{

}
CustomFloatValue::~CustomFloatValue()
{

}

IMPLEMENT_RTTI_NoCreateFun(VSMaterialInterface,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMaterialInterface)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMaterialInterface,VSObject)
END_ADD_PROPERTY
VSMaterialInterface::VSMaterialInterface()
{
	
}
VSMaterialInterface::~VSMaterialInterface()
{
	
	
}



IMPLEMENT_RTTI(VSCustomMaterial,VSMaterialInterface)
BEGIN_ADD_PROPERTY(VSCustomMaterial,VSMaterialInterface)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCustomMaterial)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostGray;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pGammaCorrect;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostScreenQuad;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostVolumeShadowMap;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pProjectShadowPre;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pProjectShadow;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pSSR;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pFont;
VSShaderKeyPtr VSCustomMaterial::ms_NULLShaderKey;
bool VSCustomMaterial::InitialDefaultState()
{
	ms_pPostGray = VS_NEW VSCustomMaterial();
	ms_pGammaCorrect = VS_NEW VSCustomMaterial();
	ms_pPostScreenQuad = VS_NEW VSCustomMaterial();
	ms_pPostVolumeShadowMap = VS_NEW VSCustomMaterial();

	ms_pProjectShadowPre = VS_NEW VSCustomMaterial();
	ms_pProjectShadow = VS_NEW VSCustomMaterial();
	ms_pSSR = VS_NEW VSCustomMaterial();
	ms_pFont = VS_NEW VSCustomMaterial();
	ms_NULLShaderKey = VS_NEW VSShaderKey();
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_LESSEQUAL;
		DepthStencilDesc.m_uiReference = 0x10;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pPostVolumeShadowMap->m_RenderState.SetDepthStencilState(pDepthStencilState);
	}

	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthWritable = false;
		DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_LESS;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_bTwoSideStencilMode = true;
		DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_DECREMENT;
		DepthStencilDesc.m_uiCCW_SPassZFailOP = VSDepthStencilDesc::OT_INCREMENT;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pProjectShadowPre->m_RenderState.SetDepthStencilState(pDepthStencilState);

		VSRasterizerDesc RasterizerDesc;
		RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_NONE;
		VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
		ms_pProjectShadowPre->m_RenderState.SetRasterizerState(pRasterizerState);

		VSBlendDesc BlendDesc;
		BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
		ms_pProjectShadowPre->m_RenderState.SetBlendState(pBlendState);
	}


	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_LESSEQUAL;
		DepthStencilDesc.m_uiReference = 0x10;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pProjectShadow->m_RenderState.SetDepthStencilState(pDepthStencilState);

		VSRasterizerDesc RasterizerDesc;
		RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_CW;
		VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
		ms_pProjectShadow->m_RenderState.SetRasterizerState(pRasterizerState);

		VSBlendDesc BlendDesc;
		BlendDesc.ucSrcBlend[0] = VSBlendDesc::BP_DESTCOLOR;
		BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_ZERO;

		BlendDesc.bBlendEnable[0] = true;		
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
		ms_pProjectShadow->m_RenderState.SetBlendState(pBlendState);
	}
	LoadDefault();
	return 1;
}
bool VSCustomMaterial::TerminalDefaultState()
{
	ms_pPostGray = NULL;
	ms_pGammaCorrect = NULL;
	ms_pPostScreenQuad = NULL;
	ms_pPostVolumeShadowMap = NULL;
	ms_pProjectShadowPre = NULL;
	ms_pProjectShadow = NULL;
	ms_pSSR = NULL;
	ms_NULLShaderKey = NULL;
	ms_pFont = NULL;
	return 1;
}
void VSCustomMaterial::LoadAllDeviceShader()
{
	ms_pPostGray->LoadDeviceShader();
	ms_pGammaCorrect->LoadDeviceShader();
	ms_pPostScreenQuad->LoadDeviceShader();
	ms_pPostVolumeShadowMap->LoadDeviceShader();
	ms_pProjectShadowPre->LoadDeviceShader();
	ms_pProjectShadow->LoadDeviceShader();
	ms_pSSR->LoadDeviceShader();
	ms_pFont->LoadDeviceShader();
}
void VSCustomMaterial::LoadDeviceShader()
{
	GetCurVShader(*ms_NULLShaderKey);
	GetCurPShader(*ms_NULLShaderKey);
}
bool VSCustomMaterial::LoadDefault()
{
	if (!ms_pPostGray)
	{
		return 0;
	}

	ms_pPostGray->PreLoad(VSUsedName::ms_cPostGray,_T("PostEffectVShader.txt"),_T("VSMain"),_T("GrayPShader.txt"),_T("PSMain"));

	if(!ms_pGammaCorrect)
	{
		return 0;
	}

	ms_pGammaCorrect->PreLoad(VSUsedName::ms_cGammaCorrect,_T("PostEffectVShader.txt"),_T("VSMain"),_T("GammaCorrectPShader.txt"),_T("PSMain"));

	if (!ms_pPostScreenQuad)
	{
		return 0;
	}
	ms_pPostScreenQuad->PreLoad(VSUsedName::ms_cPostScreenQuad,_T("PostEffectVShader.txt"),_T("VSMain"),_T("ScreenQuadPShader.txt"),_T("PSMain"));

	if (!ms_pPostVolumeShadowMap)
	{
		return 0;
	}
	ms_pPostVolumeShadowMap->PreLoad(VSUsedName::ms_cPostVolumeShadowMap,_T("PostEffectVShader.txt"),_T("VSMain"),_T("VolumeShadowMapPShader.txt"),_T("PSMain"));

	if (!ms_pProjectShadowPre)
	{
		return 0;
	}
	ms_pProjectShadowPre->PreLoad(_T("ProjectShadowPre"),_T("ProjectShadowPreVS.txt"),_T("VSMain"),_T("DefaultPS.txt"),_T("PSMain"));

	if (!ms_pProjectShadow)
	{
		return 0;
	}
	ms_pProjectShadow->PreLoad(_T("ProjectShadow"),_T("ProjectShadowVS.txt"),_T("VSMain"),_T("ProjectShadowPS.txt"),_T("PSMain"));

	if (!ms_pSSR)
	{
		return 0;
	}

	ms_pSSR->PreLoad(_T("ScreenSpaceReflection"),_T("PostEffectVShader.txt"),_T("VSMain"),_T("SSRPShader.txt"),_T("PSMain"));

	if (!ms_pFont)
	{
		return 0;
	}
	ms_pFont->PreLoad(_T("FontShader"), _T("FontVShader.txt"), _T("VSMain"), _T("FontPShader.txt"), _T("PSMain"));
	return 1;
}
void VSCustomMaterial::SetVShaderValue(const VSUsedName & Name,void *fValue)
{
	if (m_pVShader)
	{
		m_pVShader->SetParam(Name,fValue);
	}
}
void VSCustomMaterial::SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex)
{	
	if (m_pVShader)
	{
		m_pVShader->SetParam(TexSamplerNodeName,pTex);
	}
}
void VSCustomMaterial::SetPShaderValue(const VSUsedName & Name,void *fValue)
{
	if (m_pPShader)
	{
		m_pPShader->SetParam(Name,fValue);
	}
}
void VSCustomMaterial::SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex)
{
	if (m_pPShader)
	{
		m_pPShader->SetParam(TexSamplerNodeName,pTex);
	}
}
VSRenderState & VSCustomMaterial::GetRenderState()
{
	return m_RenderState;
}
void VSCustomMaterial::SetBlendState(VSBlendState * pBlendState)
{
	m_RenderState.SetBlendState(pBlendState);
}
void VSCustomMaterial::SetDepthStencilState(VSDepthStencilState *pDepthStencilState)
{
	m_RenderState.SetDepthStencilState(pDepthStencilState);
}
void VSCustomMaterial::SetRasterizerState(VSRasterizerState *pRasterizerState)
{
	m_RenderState.SetRasterizerState(pRasterizerState);
}
void VSCustomMaterial::AddClipPlane(const VSPlane3 & Plane)
{
	m_RenderState.AddPlane(Plane);
}
void VSCustomMaterial::AddScissorRect(const VSRect2 & Rect)
{
	m_RenderState.AddRect(Rect);
}
VSCustomMaterial::VSCustomMaterial(const VSUsedName & ResourceName,const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
				 const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName)
{
		m_pVShaderSet = NULL;
		m_pPShaderSet = NULL;
		m_pGShaderSet = NULL;
		m_pVShader = NULL;
		m_pPShader = NULL;
		m_pGShader = NULL;
		PreLoad(ResourceName,pVShaderFileName,pVMainFunName,pPShaderFileName,pPMainFunName);
	
}
VSCustomMaterial::VSCustomMaterial()
{
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
}
VSCustomMaterial::~VSCustomMaterial()
{
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
}
void VSCustomMaterial::PreLoad(const VSUsedName & ResourceName,
							const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
							  const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName)
{
	m_VShaderName = pVShaderFileName;
	m_PShaderName = pPShaderFileName;
	m_VMainFunName = pVMainFunName;
	m_PMainFunName = pPMainFunName;
	m_ResourceName = ResourceName;

}
bool VSCustomMaterial::GetVShader(VSShaderKey & VShaderKey)
{
	VSVShader * pVertexShader = NULL;

	if (!m_pVShaderSet)
	{
		m_pVShaderSet = VSResourceManager::GetVertexShaderMap().GetShaderSet(m_VShaderName);
	}
	
	
	
	if (m_pVShader == NULL)
	{
		if (m_pVShaderSet)
		{
			unsigned int uiIndex = m_pVShaderSet->Find(VShaderKey);

			if (uiIndex != m_pVShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
				pVertexShader = (VSVShader *)(pTemp);
			}

		}
		if (pVertexShader == NULL)
		{
			pVertexShader = VS_NEW VSVShader(m_VShaderName.GetBuffer(),m_VMainFunName.GetBuffer(),true);
			if (!pVertexShader)
			{
				return 0;
			}
			pVertexShader->m_ShaderKey = VShaderKey;
			VSRenderer::ms_pRenderer->LoadVShaderProgram(pVertexShader);
			if (m_pVShaderSet)
			{
				m_pVShaderSet->AddElement(VShaderKey,pVertexShader);
			}
		}
		m_pVShader = pVertexShader;
	}
	else
	{
		if (m_pVShader->m_ShaderKey == VShaderKey)
		{

		}
		else
		{

			if (m_pVShaderSet)
			{
				unsigned int uiIndex = m_pVShaderSet->Find(VShaderKey);

				if (uiIndex != m_pVShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
					pVertexShader = (VSVShader *)(pTemp);
				}
			}
			if (pVertexShader == NULL)
			{
				pVertexShader = VS_NEW VSVShader(m_VShaderName.GetBuffer(),m_VMainFunName.GetBuffer(),true);
				if (!pVertexShader)
				{
					return 0;
				}
				pVertexShader->m_ShaderKey = VShaderKey;
				VSRenderer::ms_pRenderer->LoadVShaderProgram(pVertexShader);
				if (m_pVShaderSet)
				{
					m_pVShaderSet->AddElement(VShaderKey,pVertexShader);
				}
			}
			m_pVShader = pVertexShader;
		}
	}

	if (!m_pVShaderSet)
	{
		VSResourceManager::GetVertexShaderMap().SetShader(m_VShaderName,VShaderKey,pVertexShader);
		m_pVShaderSet = VSResourceManager::GetVertexShaderMap().GetShaderSet(m_VShaderName);
	}
	return 1;
}
bool VSCustomMaterial::GetGShader(VSShaderKey & GShaderKey)
{
	unsigned int uiGShaderNum = 0;
	VSGShader * pGShader = NULL;
	if (m_GShaderName.GetBuffer() == NULL)
	{
		return false;
	}
	if (!m_pGShaderSet)
	{
		m_pGShaderSet = VSResourceManager::GetGeometryShaderMap().GetShaderSet(m_GShaderName);
	}



	if (m_pGShaderSet)
	{
		uiGShaderNum = m_pGShaderSet->GetNum();
	}


	if (m_pGShader == NULL)
	{
		if (m_pGShaderSet)
		{
			unsigned int uiIndex = m_pGShaderSet->Find(GShaderKey);

			if (uiIndex != m_pGShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
				pGShader = (VSGShader *)(pTemp);
			}
		}
		if (pGShader == NULL)
		{
			pGShader = VS_NEW VSGShader(m_GShaderName.GetBuffer(), m_GMainFunName.GetBuffer(), true);

			if (!pGShader)
			{
				return 0;
			}
			pGShader->m_ShaderKey = GShaderKey;
			VSRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
			if (m_pGShaderSet)
			{
				m_pGShaderSet->AddElement(GShaderKey, pGShader);
			}
		}
		m_pGShader = pGShader;

	}
	else
	{
		if (m_pPShader->m_ShaderKey == GShaderKey)
		{

		}
		else
		{
			if (m_pGShaderSet)
			{
				unsigned int uiIndex = m_pGShaderSet->Find(GShaderKey);

				if (uiIndex != m_pGShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
					pGShader = (VSGShader *)(pTemp);
				}
			}
			if (pGShader == NULL)
			{
				pGShader = VS_NEW VSGShader(m_GShaderName.GetBuffer(), m_GMainFunName.GetBuffer(), true);

				if (!pGShader)
				{
					return 0;
				}
				pGShader->m_ShaderKey = GShaderKey;
				VSRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
				if (m_pGShaderSet)
				{
					m_pGShaderSet->AddElement(GShaderKey, pGShader);
				}
			}
			m_pGShader = pGShader;
		}
	}

	if (!m_pGShaderSet)
	{
		VSResourceManager::GetGeometryShaderMap().SetShader(m_GShaderName, GShaderKey, pGShader);
		m_pGShaderSet = VSResourceManager::GetGeometryShaderMap().GetShaderSet(m_GShaderName);
	}
	return 1;
}
bool VSCustomMaterial::GetPShader(VSShaderKey & PShaderKey)
{
	unsigned int uiPShaderNum = 0;
	VSPShader * pPixelShader = NULL;

	if (!m_pPShaderSet)
	{
		m_pPShaderSet = VSResourceManager::GetPixelShaderMap().GetShaderSet(m_PShaderName);
	}
	
	

	if (m_pPShaderSet)
	{
		uiPShaderNum = m_pPShaderSet->GetNum();
	}


	if (m_pPShader == NULL)
	{
		if (m_pPShaderSet)
		{
			unsigned int uiIndex = m_pPShaderSet->Find(PShaderKey);

			if (uiIndex != m_pPShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
				pPixelShader = (VSPShader *)(pTemp);
			}
		}
		if (pPixelShader == NULL)
		{
			pPixelShader = VS_NEW VSPShader(m_PShaderName.GetBuffer(),m_PMainFunName.GetBuffer(),true);
			
			if (!pPixelShader)
			{
				return 0;
			}
			pPixelShader->m_ShaderKey = PShaderKey;
			VSRenderer::ms_pRenderer->LoadPShaderProgram(pPixelShader);
			if (m_pPShaderSet)
			{
				m_pPShaderSet->AddElement(PShaderKey,pPixelShader);
			}
		}
		m_pPShader = pPixelShader;

	}
	else
	{
		if (m_pPShader->m_ShaderKey == PShaderKey)
		{

		}
		else
		{
			if (m_pPShaderSet)
			{
				unsigned int uiIndex = m_pPShaderSet->Find(PShaderKey);

				if (uiIndex != m_pPShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
					pPixelShader = (VSPShader *)(pTemp);
				}
			}
			if (pPixelShader == NULL)
			{
				pPixelShader = VS_NEW VSPShader(m_PShaderName.GetBuffer(),m_PMainFunName.GetBuffer(),true);
				
				if (!pPixelShader)
				{
					return 0;
				}
				pPixelShader->m_ShaderKey = PShaderKey;
				VSRenderer::ms_pRenderer->LoadPShaderProgram(pPixelShader);
				if (m_pPShaderSet)
				{
					m_pPShaderSet->AddElement(PShaderKey,pPixelShader);
				}
			}
			m_pPShader = pPixelShader;
		}
	}

	if (!m_pPShaderSet)
	{
		VSResourceManager::GetPixelShaderMap().SetShader(m_PShaderName,PShaderKey,pPixelShader);
		m_pPShaderSet = VSResourceManager::GetPixelShaderMap().GetShaderSet(m_PShaderName);
	}
	return 1;
}
VSGShader *VSCustomMaterial::GetCurGShader(VSShaderKey & GShaderKey)
{
	if (GetGShader(GShaderKey))
	{
		return m_pGShader;
	}
	return NULL;
}
VSVShader *VSCustomMaterial::GetCurVShader(VSShaderKey & VShaderKey)
{
	if (GetVShader(VShaderKey))
	{
		return m_pVShader;
	}
	return NULL;
}
VSPShader *VSCustomMaterial::GetCurPShader(VSShaderKey & PShaderKey)
{
	if(GetPShader(PShaderKey))
	{
		return m_pPShader;
	}
	return NULL;
}
bool  VSCustomMaterial::UseMaterial()
{

	GetCurVShader(*ms_NULLShaderKey);
	GetCurPShader(*ms_NULLShaderKey);
	GetCurGShader(*ms_NULLShaderKey);
	if(!VSRenderer::ms_pRenderer->SetVShader(m_pVShader))
		return 0;
	if (!VSRenderer::ms_pRenderer->SetGShader(m_pGShader))
		return 0;
	if(!VSRenderer::ms_pRenderer->SetPShader(m_pPShader))
		return 0;
	VSRenderer::ms_pRenderer->SetRenderState(m_RenderState);
	return 1;
}

IMPLEMENT_RTTI_NoCreateFun(VSMaterialBase,VSMaterialInterface)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMaterialBase)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMaterialBase,VSMaterialInterface)
END_ADD_PROPERTY
VSMaterialBase::VSMaterialBase()
{
	m_VShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();
	m_PShaderCustomValue.Clear();
	m_PShaderCustomTex.Clear();
}
VSMaterialBase::~VSMaterialBase()
{
	m_VShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();

	m_PShaderCustomValue.Clear();
	m_PShaderCustomTex.Clear();
}
void VSMaterialBase::SetVShaderValue(const VSUsedName & Name,void *fValue)
{
	if(!fValue)
		return;
	static CustomFloatValue TempValue;
	TempValue.ConstValueName = Name;
	unsigned int uiIndex = m_VShaderCustomValue.FindElement(TempValue);
	if (uiIndex != m_VShaderCustomValue.GetNum())
	{
		VSMemcpy(&m_VShaderCustomValue[uiIndex].Value[0],fValue,sizeof(VSREAL) * m_VShaderCustomValue[uiIndex].Value.GetNum());
		return;
	}


	return ;
}


void VSMaterialBase::SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex)
{
	static CustomTexSampler TempValue;
	TempValue.ConstValueName = TexSamplerNodeName;
	unsigned int uiIndex = m_VShaderCustomTex.FindElement(TempValue);
	if (uiIndex != m_VShaderCustomTex.GetNum() )
	{
		m_VShaderCustomTex[uiIndex].m_pTexture = pTex;
		return ;
	}


}


void VSMaterialBase::SetPShaderValue(const VSUsedName & Name,void *fValue)
{
	if(!fValue)
		return;

	static CustomFloatValue TempValue;
	TempValue.ConstValueName = Name;
	unsigned int uiIndex = m_PShaderCustomValue.FindElement(TempValue);
	if (uiIndex != m_PShaderCustomValue.GetNum())
	{
		VSMemcpy(&m_PShaderCustomValue[uiIndex].Value[0],fValue,sizeof(VSREAL) * m_PShaderCustomValue[uiIndex].Value.GetNum());
	}

	return ;
}


void VSMaterialBase::SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex)
{
	static CustomTexSampler TempValue;
	TempValue.ConstValueName = TexSamplerNodeName;
	unsigned int uiIndex = m_VShaderCustomTex.FindElement(TempValue);
	if (uiIndex != m_VShaderCustomTex.GetNum() )
	{
		m_PShaderCustomTex[uiIndex].m_pTexture = pTex;
	}

}
IMPLEMENT_RTTI(VSMaterial,VSMaterialBase)
VSPointer<VSMaterial> VSMaterial::Default;
VSPointer<VSMaterial> VSMaterial::DefaultOnlyColor;
VSPointer<VSMaterial> VSMaterial::DefaultOnlyVertexColor;
VSPointer<VSMaterial> VSMaterial::DefaultOnlyColorDisableDepth;
VSPointer<VSMaterial> VSMaterial::DefaultOnlyVertexColorDisableDepth;
BEGIN_ADD_PROPERTY(VSMaterial,VSMaterialBase)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pShaderMainFunction,ShaderMainFunction,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pShaderFunctionArray,ShaderFunctionArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pLightShaderFunction, LightShaderFunction, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffectShaderFunction, PostEffectShaderFunction, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bIsCombine,IsCombine,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCustomLayer,CustomLayer,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMaterial)
ADD_PRIORITY(VSTexAllState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSMaterial::ms_bIsEnableASYNLoader = true;
bool VSMaterial::ms_bIsEnableGC = true;
bool VSMaterial::InitialDefaultState()
{
	Default = VS_NEW VSMaterial(_T("DefaultMaterial"));
	if(!Default)
		return 0;
	
	DefaultOnlyColor = VS_NEW VSMaterial(_T("DefaultOnlyColorMaterial"));

	if(!DefaultOnlyColor)
		return 0;

	DefaultOnlyVertexColor = VS_NEW VSMaterial(_T("DefaultOnlyVertexColorMaterial"));

	if (!DefaultOnlyVertexColor)
	{
		return 0;
	}

	DefaultOnlyColorDisableDepth = VS_NEW VSMaterial(_T("DefaultOnlyColorDisableDepthMaterial"));

	if(!DefaultOnlyColorDisableDepth)
		return 0;

	DefaultOnlyVertexColorDisableDepth = VS_NEW VSMaterial(_T("DefaultOnlyVertexColorDisableDepthMaterial"));

	if (!DefaultOnlyVertexColorDisableDepth)
	{
		return 0;
	}
	return 1;
}
bool VSMaterial::TerminalDefaultState()
{
	Default = NULL;
	DefaultOnlyColor = NULL;
	DefaultOnlyVertexColor = NULL;
	DefaultOnlyColorDisableDepth = NULL;
	DefaultOnlyVertexColorDisableDepth = NULL;
	return 1;
}
bool VSMaterial::LoadDefault()
{
	{
		if(!Default)
		return 0;
		VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("TexSampler"),Default);
		p2DTexSamplerNode->SetTexture(VSResourceManager::ms_DefaultTextureResource);
		VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"),Default,4,false);
		pSpecularColor->SetValue(0,0.5f);
		pSpecularColor->SetValue(1,0.5f);
		pSpecularColor->SetValue(2,0.5f);
		pSpecularColor->SetValue(3,1.0f);
		VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),Default,1,false);
		pSpecularPow ->SetValue(0,5);
		Default->m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
		Default->m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
		Default->m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
		Default->m_ResourceName = _T("_Default");
	}
	{
		if(!DefaultOnlyColor)
			return 0;

		VSConstFloatValue * pEmissiveColor = VS_NEW VSConstFloatValue(_T("EmissiveColor"),DefaultOnlyColor,4,true);
		pEmissiveColor->SetValue(0,1.0f);
		pEmissiveColor->SetValue(1,1.0f);
		pEmissiveColor->SetValue(2,1.0f);
		pEmissiveColor->SetValue(3,1.0f);

		DefaultOnlyColor->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(VSConstFloatValue::OUT_VALUE));
		DefaultOnlyColor->m_ResourceName = _T("_DefaultOnlyColor");
	}

	{
		if(!DefaultOnlyVertexColor)
			return 0;

		VSVertexColor * pVertexColor = VS_NEW VSVertexColor(_T("VertexColor"),DefaultOnlyVertexColor);

		DefaultOnlyVertexColor->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pVertexColor->GetOutputNode(VSVertexColor::OUT_COLOR));
		DefaultOnlyVertexColor->m_ResourceName = _T("_DefaultOnlyVertexColor");
	}


	{
		if(!DefaultOnlyColorDisableDepth)
			return 0;
		VSDepthStencilDesc DepthStencilDest;
		DepthStencilDest.m_bDepthEnable = false;
		VSDepthStencilState * pState = VSResourceManager::CreateDepthStencilState(DepthStencilDest);
		DefaultOnlyColorDisableDepth->SetDepthStencilState(pState);
		VSConstFloatValue * pEmissiveColor = VS_NEW VSConstFloatValue(_T("EmissiveColor"),DefaultOnlyColorDisableDepth,4,true);
		pEmissiveColor->SetValue(0,1.0f);
		pEmissiveColor->SetValue(1,1.0f);
		pEmissiveColor->SetValue(2,1.0f);
		pEmissiveColor->SetValue(3,1.0f);

		DefaultOnlyColorDisableDepth->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(VSConstFloatValue::OUT_VALUE));
		DefaultOnlyColorDisableDepth->m_ResourceName = _T("_DefaultOnlyColorDisableDepth");
	}

	{
		if(!DefaultOnlyVertexColorDisableDepth)
			return 0;

		VSDepthStencilDesc DepthStencilDest;
		DepthStencilDest.m_bDepthEnable = false;
		VSDepthStencilState * pState = VSResourceManager::CreateDepthStencilState(DepthStencilDest);
		DefaultOnlyVertexColorDisableDepth->SetDepthStencilState(pState);

		VSVertexColor * pVertexColor = VS_NEW VSVertexColor(_T("VertexColor"),DefaultOnlyVertexColorDisableDepth);

		DefaultOnlyVertexColorDisableDepth->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pVertexColor->GetOutputNode(VSVertexColor::OUT_COLOR));
		DefaultOnlyVertexColorDisableDepth->m_ResourceName = _T("_DefaultOnlyVertexColorDisableDepth");
	}
	return 1;
}
VSMaterial::VSMaterial()
{	
	m_pShaderMainFunction.Clear();
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	m_bIsCombine = false;
	m_pShaderFunctionArray.Clear();
	m_bIsAllReadyLink = false;
	m_pPass[VSPass::PT_MATERIAL] = VS_NEW VSMaterialPass();
	m_pPass[VSPass::PT_NORMALDEPTH] = VS_NEW VSNormalDepthPass();
	m_pPass[VSPass::PT_POINT_CUBE_SHADOW] = VS_NEW VSCubeShadowPass();
	m_pPass[VSPass::PT_POINT_VOLUME_SHADOW] = VS_NEW VSVolumeShadowPass();
	m_pPass[VSPass::PT_PREZ] = VSPrezPass::GetDefault();
	m_pPass[VSPass::PT_SHADOW] = VS_NEW VSShadowPass();
	m_pPass[VSPass::PT_DUAL_PARABOLOID_SHADOW] = VS_NEW VSDualParaboloidShadowPass();
	m_pPass[VSPass::PT_LIGHT_FUNCTION] = VS_NEW VSLightFunPass();
	m_pPass[VSPass::PT_POSTEFFECT] = VS_NEW VSPostEffectPass();
	m_pPass[VSPass::PT_INDIRECT] = VS_NEW VSIndirectRenderPass();
	VSMAC_ASSERT(m_pPass[VSPass::PT_MATERIAL] && m_pPass[VSPass::PT_INDIRECT]);
	m_uiCustomLayer = 10;
	
}
VSMaterial::~VSMaterial()
{
	//由于释放函数节点，会调VSMaterial::DeleteShaderFuntion从m_pShaderFunctionArray中删除,这样
	//m_pShaderFunctionArray数量就会减少，这个FOR循环中i继续加1，而m_pShaderFunctionArray.size()却减少
	//导致有些节点不能删除
	//所以用一个临时变量替代，然后把这个m_pShaderFunctionArray对应的设置为空，这样删除时找不到节点，能把正节点
	//正常释放，而且m_pShaderFunctionArray.size()保持不变。
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSShaderFunction * pShaderFunction = m_pShaderFunctionArray[i];
		m_pShaderFunctionArray[i] = NULL;
		VSMAC_DELETE(pShaderFunction);
	}
	m_pShaderFunctionArray.Clear();
	
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	m_pShaderMainFunction.Clear();
	m_bIsAllReadyLink = false;
	
	for (unsigned int i = 0 ; i < VSPass::PT_MAX ; i++)
	{	
		m_pPass[i] = NULL;
	}
	

}
VSMaterial::VSMaterial(const VSUsedName &ShowName, unsigned int uiMUT)
{

	m_ShowName = ShowName;
	//Clear必须要在shaderFunction创建前面
	m_pShaderFunctionArray.Clear();
	m_pShaderMainFunction.Clear();
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	if (uiMUT == MUT_PHONE)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSPhoneShaderFunction(_T("PSMain"),this));
		
	}
	else if (uiMUT == MUT_OREN_NAYAR)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSOrenNayarShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_MINNAERT)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSMinnaertShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_STRAUSS)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSStraussShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SHIRLEY)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSShirleyShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SCHLICK)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSSchlickShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_COOKTOORANCE)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSCookTorranceShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSIsotropicWardShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ANISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSAnisotropicWardShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_CUSTOM)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSCustomShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_LIGHT)
	{
		m_pLightShaderFunction = VS_NEW VSLightShaderFunction(_T("PSMain"),this);
	}
	else if (uiMUT == MUT_POSTEFFECT)
	{
		m_pPostEffectShaderFunction = VS_NEW VSPostEffectShaderFunction(_T("PSMain"), this);
	}
	m_bIsAllReadyLink = false;
	m_bIsCombine = false;
	m_pPass[VSPass::PT_MATERIAL] = VS_NEW VSMaterialPass();
	m_pPass[VSPass::PT_NORMALDEPTH] = VS_NEW VSNormalDepthPass();
	m_pPass[VSPass::PT_POINT_CUBE_SHADOW] = VS_NEW VSCubeShadowPass();
	m_pPass[VSPass::PT_POINT_VOLUME_SHADOW] = VS_NEW VSVolumeShadowPass();
	m_pPass[VSPass::PT_PREZ] = VSPrezPass::GetDefault();
	m_pPass[VSPass::PT_SHADOW] = VS_NEW VSShadowPass();
	m_pPass[VSPass::PT_DUAL_PARABOLOID_SHADOW] = VS_NEW VSDualParaboloidShadowPass();
	m_pPass[VSPass::PT_LIGHT_FUNCTION] = VS_NEW VSLightFunPass();
	m_pPass[VSPass::PT_POSTEFFECT] = VS_NEW VSPostEffectPass();
	m_pPass[VSPass::PT_INDIRECT] = VS_NEW VSIndirectRenderPass();
	VSMAC_ASSERT(m_pPass[VSPass::PT_MATERIAL] && m_pPass[VSPass::PT_INDIRECT]);
	m_uiCustomLayer = 10;
}
void VSMaterial::AddPass(unsigned int uiMUT)
{
	if (m_pLightShaderFunction || m_pPostEffectShaderFunction)
	{
		return ;
	}
	if (uiMUT == MUT_PHONE)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSPhoneShaderFunction(_T("PSMain"),this));

	}
	else if (uiMUT == MUT_OREN_NAYAR)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSOrenNayarShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_MINNAERT)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSMinnaertShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_STRAUSS)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSStraussShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SHIRLEY)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSShirleyShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SCHLICK)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSSchlickShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_COOKTOORANCE)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSCookTorranceShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSIsotropicWardShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ANISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSAnisotropicWardShaderFunction(_T("PSMain"),this));
	}
}
void VSMaterial::SetBlendState(VSBlendState * pBlendState,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->SetBlendState(pBlendState);
}
void VSMaterial::SetDepthStencilState(VSDepthStencilState *pDepthStencilState,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->SetDepthStencilState(pDepthStencilState);
}
void VSMaterial::SetRasterizerState(VSRasterizerState *pRasterizerState,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->SetRasterizerState(pRasterizerState);
}
void VSMaterial::AddClipPlane(const VSPlane3 & Plane,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->AddClipPlane(Plane);
}
void VSMaterial::AddScissorRect(const VSRect2 & Rect,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->AddScissorRect(Rect);
}
void VSMaterial::AddShaderFuntion(VSShaderFunction *pShaderFuntion)
{
	if(pShaderFuntion)
	{
		m_pShaderFunctionArray.AddElement(pShaderFuntion);
	}

}
void VSMaterial::DeleteShaderFuntion(VSShaderFunction *pShaderFuntion)
{
	if(pShaderFuntion)
	{
		if (m_pLightShaderFunction && m_pLightShaderFunction == pShaderFuntion)
		{
			// can't delete LightShaderFunction;
			return ;
		}

		if (m_pPostEffectShaderFunction && m_pPostEffectShaderFunction == pShaderFuntion)
		{
			// can't delete PostEffectShaderFunction;
			return;
		}
		if (m_pShaderMainFunction.GetNum() > 0 && pShaderFuntion == m_pShaderMainFunction[0])
		{
			//can't delete first 
			return ;
		}
		for (unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
		{
			if(m_pShaderFunctionArray[i] == pShaderFuntion)
			{
				for (unsigned int j = 0 ; j < pShaderFuntion->GetOutputNodeNum() ;j++)
				{
					VSOutputNode * pOutputNode = pShaderFuntion->GetOutputNode(j);
					pOutputNode->UnConnection();
				}				
				m_pShaderFunctionArray.Erase(i);
				VSMAC_DELETE(pShaderFuntion);
				return ;
			}
		}
		
	}

}


void VSMaterial::LinkAllResource()
{
	if(m_bIsAllReadyLink)
		return ;
	m_bIsAllReadyLink = true;
	m_PShaderCustomTex.Clear();
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSTexSampler *Temp = DynamicCast<VSTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp)
			continue;
		
		CustomTexSampler CTex;
		CTex.ConstValueName = Temp->GetShowName();
		CTex.m_pTexture = Temp->GetTexture();
		m_PShaderCustomTex.AddElement(CTex);
		
	}
	m_PShaderCustomValue.Clear();
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSConstValue *Temp = DynamicCast<VSConstValue>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->IsCustom())
			continue;
		if (Temp->GetType() == VSUserConstant::VT_FLOAT)
		{
			CustomFloatValue CustomValueTemp;
			CustomValueTemp.ConstValueName = Temp->GetShowName();
			CustomValueTemp.Value = ((VSConstFloatValue *)Temp)->GetValue();
			m_PShaderCustomValue.AddElement(CustomValueTemp);
		}
		
	}

}
void VSMaterial::ClearShaderFuntionVisitFlagFalse()
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		m_pShaderFunctionArray[i]->m_bIsVisited = 0;
	}
}
bool VSMaterial::GetCustomDeclareString(VSString & OutString,unsigned char uPassId)
{
	VSCustomShaderFunction * pCustomShaderFunction = DynamicCast<VSCustomShaderFunction>(m_pShaderMainFunction[uPassId]);
	if (pCustomShaderFunction)
	{
		OutString = pCustomShaderFunction->m_LightFunctionString;
		return true;
	}
	return false;
}
bool VSMaterial::GetShaderTreeString(VSString & OutString,MaterialShaderPara &MSPara,unsigned int uiOST,unsigned char uPassId)
{
	VSMap<VSShaderFunction *, VSArray<VSShaderFunction *>> NoValidMap;
	VSShaderStringFactory::ms_ShaderValueIndex = 0;
	for (unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		m_pShaderFunctionArray[i]->ResetInShaderName();
	}
	if (m_pLightShaderFunction)
	{
		bool Temp = m_pLightShaderFunction->CheckChildNodeValidAll(NoValidMap);
		if (!Temp)
		{
			return false;
		}
		m_pLightShaderFunction->ClearShaderTreeStringFlag();
		return m_pLightShaderFunction->GetShaderTreeString(OutString);
	}
	else if (m_pPostEffectShaderFunction)
	{
		bool Temp = m_pPostEffectShaderFunction->CheckChildNodeValidAll(NoValidMap);
		if (!Temp)
		{
			return false;
		}
		m_pPostEffectShaderFunction->ClearShaderTreeStringFlag();
		return m_pPostEffectShaderFunction->GetShaderTreeString(OutString);
	}
	else
	{
		bool Temp = m_pShaderMainFunction[uPassId]->CheckChildNodeValidAll(NoValidMap);
		if (!Temp)
		{
			return false;
		}
		m_pShaderMainFunction[uPassId]->SetMaterialShaderPara(MSPara);
		m_pShaderMainFunction[uPassId]->ClearShaderTreeStringFlag();

		VSCustomShaderFunction * pCustomShaderFunction = DynamicCast<VSCustomShaderFunction>(m_pShaderMainFunction[uPassId]);
		if (pCustomShaderFunction)
		{
			pCustomShaderFunction->CreatLightFunctionString(uiOST);
		}	
		return m_pShaderMainFunction[uPassId]->GetShaderTreeString(OutString,uiOST);
	}
	
}
bool VSMaterial::PostClone(VSObject * pObjectSrc)
{
	VSMaterialBase::PostClone(pObjectSrc);
	LinkAllResource();
	return true;
}

bool VSMaterial::PostLoad(void * pDate)
{
	VSMaterialBase::PostLoad(pDate);
	LinkAllResource();
	return true;
}
void VSMaterial::SetGlobleValue(VSPass * pPass, unsigned int uiPassId,VSVShader * pVShader , VSPShader * pPShader)
{
	if (pPass == m_pPass[VSPass::PT_MATERIAL])
	{
		m_pShaderMainFunction[uiPassId]->SetGlobleValue(VSShaderMainFunction::OST_MATERIAL,pVShader,pPShader);
	}
	else if (pPass == m_pPass[VSPass::PT_NORMALDEPTH])
	{
		m_pShaderMainFunction[uiPassId]->SetGlobleValue(VSShaderMainFunction::OST_NORMAL_DEPTH,pVShader,pPShader);
	}
}
void VSMaterial::CreateCustomTexture(VSPShader * pShader)
{
	
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		VSTexSampler *Temp = DynamicCast<VSTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->m_bIsVisited)
			continue;
		unsigned int RegisterID = 0;
		if (pShader->m_pUserSampler.GetNum())
		{
			VSUserSampler * Last = pShader->m_pUserSampler[pShader->m_pUserSampler.GetNum() - 1];
			if (Last)
			{
				RegisterID = Last->GetRegisterIndex() + Last->GetRegisterNum();
			}
		}
		VSUserSampler * pSampler = VS_NEW VSUserSampler(Temp->GetShowName(),Temp->GetTexType(),RegisterID,1);
		pShader->m_pUserSampler.AddElement(pSampler);
	}
}
void VSMaterial::CreateTextureDeclare(VSString & OutString,unsigned int uiRegisterID)
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSTexSampler *Temp = DynamicCast<VSTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp|| !Temp->m_bIsVisited)
			continue;
		Temp->GetDeclareString(OutString,uiRegisterID);
		uiRegisterID++;
	}
}
void VSMaterial::CreateCustomValue(VSPShader * pShader)
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		VSConstValue *Temp = DynamicCast<VSConstValue>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->IsCustom() || !Temp->m_bIsVisited)
			continue;
		VSUserConstant * UserConstantTemp = NULL;
		unsigned int uiRegisterIndex = 0;
		if (pShader->m_pUserConstant.GetNum())
		{
			VSUserConstant * Last = pShader->m_pUserConstant[pShader->m_pUserConstant.GetNum() - 1];
			if (Last)
			{
				uiRegisterIndex = Last->GetRegisterIndex() + Last->GetRegisterNum();
			}
		}
		UserConstantTemp = VS_NEW VSUserConstant(Temp->GetShowName(), Temp->GetOutputNode(0)->GetNodeName(), NULL,Temp->GetSize(), uiRegisterIndex, 1, Temp->GetType());
		pShader->m_pUserConstant.AddElement(UserConstantTemp);
	}
}


void VSMaterial::CreateConstValueDeclare(VSString & OutString,unsigned int uiRegisterID)
{

	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSConstValue *Temp = DynamicCast<VSConstValue>(m_pShaderFunctionArray[i]);
		if (!Temp || !Temp->m_bIsVisited || !Temp->IsCustom())
			continue;
		Temp->GetDeclareString(OutString,uiRegisterID);
		uiRegisterID++;
	}
}
IMPLEMENT_RTTI(VSMaterialInstance,VSObject)
BEGIN_ADD_PROPERTY(VSMaterialInstance,VSObject)
REGISTER_PROPERTY(m_VShaderCustomValue,VShaderCustomValue,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_PShaderCustomValue,PShaderCustomValue,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_VShaderCustomTex,VShaderCustomTex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_PShaderCustomTex,PShaderCustomTex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMaterial,Material,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMaterialInstance)
IMPLEMENT_INITIAL_END
VSMaterialInstance::VSMaterialInstance()
{
	m_pMaterial = NULL;
	m_VShaderCustomValue.Clear();
	m_PShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();
	m_PShaderCustomTex.Clear();
	for (unsigned int i = 0 ; i < VSPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
}
VSMaterialInstance::VSMaterialInstance(VSMaterialR * pMaterial)
{
	VSMAC_ASSERT(pMaterial);
	m_pMaterial = pMaterial;
	m_pMaterial->GetResource()->LinkAllResource();	
	GetAllMaterialPara();
	for (unsigned int i = 0 ; i < VSPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
}
void VSMaterialInstance::GetAllMaterialPara()
{
	if (!m_pMaterial)
	{
		return ;
	}
	m_VShaderCustomValue = m_pMaterial->GetResource()->m_VShaderCustomValue;
	m_PShaderCustomValue = m_pMaterial->GetResource()->m_PShaderCustomValue;
	m_VShaderCustomTex = m_pMaterial->GetResource()->m_VShaderCustomTex;
	m_PShaderCustomTex = m_pMaterial->GetResource()->m_PShaderCustomTex;
}
void VSMaterialInstance::LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data)
{
	for (unsigned int i = 0; i < VSPass::PT_MAX;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
}
VSMaterialInstance::~VSMaterialInstance()
{
	m_pMaterial = NULL;
	m_VShaderCustomValue.Clear();
	m_PShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();
	m_PShaderCustomTex.Clear();
	for (unsigned int i = 0 ; i < VSPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
	
}
void VSMaterialInstance::DeleteVShaderValue(const VSUsedName &Name)
{
	for(unsigned int i = 0 ; i < m_VShaderCustomValue.GetNum() ; i++)
	{
		if(m_VShaderCustomValue[i].ConstValueName == Name)
		{
			m_VShaderCustomValue.Erase(i);
			return ;
		}
	}
}

void VSMaterialInstance::SetVShaderValue(const VSUsedName & Name,void *fValue,unsigned int uiSize)
{
	if(!uiSize || !fValue)
		return;
	for(unsigned int i = 0 ; i < m_VShaderCustomValue.GetNum() ; i++)
	{
		if(m_VShaderCustomValue[i].ConstValueName == Name)
		{
			VSMemcpy(&m_VShaderCustomValue[i].Value[0],fValue,sizeof(VSREAL) * m_VShaderCustomValue[i].Value.GetNum());
			return;
		}
	}
	//如果没有
	CustomFloatValue Temp;
	Temp.ConstValueName = Name;
	Temp.Value.SetBufferNum(uiSize);
	VSMemcpy(&Temp.Value[0],fValue,sizeof(VSREAL) * uiSize);
	m_VShaderCustomValue.AddElement(Temp);
	return ;
}
void VSMaterialInstance::DeleteVShaderTexture(const VSUsedName & TexSamplerNodeName)
{
	for(unsigned int i = 0 ; i < m_VShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_VShaderCustomTex[i].ConstValueName)
		{
			m_VShaderCustomTex.Erase(i);
			return ;
		}
	}
}

void VSMaterialInstance::SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex)
{
	for(unsigned int i = 0 ; i < m_VShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_VShaderCustomTex[i].ConstValueName)
		{
			m_VShaderCustomTex[i].m_pTexture = pTex;
			return ;
		}
	}
	CustomTexSampler CTex;
	CTex.ConstValueName = TexSamplerNodeName;
	CTex.m_pTexture = pTex;
	m_VShaderCustomTex.AddElement(CTex);
}

void VSMaterialInstance::DeletePShaderValue(const VSUsedName &Name)
{
	for(unsigned int i = 0 ; i < m_PShaderCustomValue.GetNum() ; i++)
	{
		if(m_PShaderCustomValue[i].ConstValueName == Name)
		{
			m_PShaderCustomValue.Erase(i);
			return ;
		}
	}
}

void VSMaterialInstance::SetPShaderValue(const VSUsedName & Name,void *fValue,unsigned int uiSize)
{
	if(!uiSize || !fValue)
		return;
	for(unsigned int i = 0 ; i < m_PShaderCustomValue.GetNum() ; i++)
	{
		if(m_PShaderCustomValue[i].ConstValueName == Name)
		{
			VSMemcpy(&m_PShaderCustomValue[i].Value[0],fValue,sizeof(VSREAL) * m_PShaderCustomValue[i].Value.GetNum());
			return;
		}
	}
	//如果没有
	CustomFloatValue Temp;
	Temp.ConstValueName = Name;
	Temp.Value.SetBufferNum(uiSize);
	VSMemcpy(&Temp.Value[0],fValue,sizeof(VSREAL) * uiSize);
	m_PShaderCustomValue.AddElement(Temp);
	return ;
}
void VSMaterialInstance::DeletePShaderTexture(const VSUsedName & TexSamplerNodeName)
{
	for(unsigned int i = 0 ; i < m_PShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_PShaderCustomTex[i].ConstValueName)
		{
			m_PShaderCustomTex.Erase(i);
			return ;
		}
	}
}

void VSMaterialInstance::SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex)
{
	for(unsigned int i = 0 ; i < m_PShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_PShaderCustomTex[i].ConstValueName)
		{
			m_PShaderCustomTex[i].m_pTexture = pTex;
			return ;
		}
	}
	CustomTexSampler CTex;
	CTex.ConstValueName = TexSamplerNodeName;
	CTex.m_pTexture = pTex;
	m_PShaderCustomTex.AddElement(CTex);
}

void VSMaterialInstance::SetPShaderValue(VSPShader * pPShader)
{
	if (!pPShader)
	{
		return ;	
	}
	for (unsigned int i = 0 ; i < m_PShaderCustomValue.GetNum() ; i++)
	{
		pPShader->SetParam(m_PShaderCustomValue[i].ConstValueName,m_PShaderCustomValue[i].Value.GetBuffer());
	}
	for (unsigned int i = 0 ; i < m_PShaderCustomTex.GetNum() ; i++)
	{
		if (m_PShaderCustomTex[i].m_pTexture)
		{
			pPShader->SetParam(m_PShaderCustomTex[i].ConstValueName,m_PShaderCustomTex[i].m_pTexture->GetResource());
		}
		else
		{
			pPShader->SetParam(m_PShaderCustomTex[i].ConstValueName,(VSTexAllState *)NULL);
		}
		
	}
}
void VSMaterialInstance::SetVShaderValue(VSVShader * pVShader)
{
	if (!pVShader)
	{
		return ;
	}
	for (unsigned int i = 0 ; i < m_VShaderCustomValue.GetNum() ; i++)
	{
		pVShader->SetParam(m_VShaderCustomValue[i].ConstValueName,m_VShaderCustomValue[i].Value.GetBuffer());
	}
	for (unsigned int i = 0 ; i < m_VShaderCustomTex.GetNum() ; i++)
	{
		if (m_VShaderCustomTex[i].m_pTexture)
		{
			pVShader->SetParam(m_VShaderCustomTex[i].ConstValueName,m_VShaderCustomTex[i].m_pTexture->GetResource());
		}
		else
		{
			pVShader->SetParam(m_VShaderCustomTex[i].ConstValueName,(VSTexAllState *)NULL);
		}
	}
}

VSMaterialOnlyTexture::VSMaterialOnlyTexture()
{

}
VSMaterialOnlyTexture::VSMaterialOnlyTexture(const VSUsedName &ShowName,VSTexAllStateR * pTexture)
:VSMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"),this,4,false);
	pSpecularColor->SetValue(0,1.0f);
	pSpecularColor->SetValue(1,1.0f);
	pSpecularColor->SetValue(2,1.0f);
	pSpecularColor->SetValue(3,1.0f);
	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,false);
	pSpecularPow->SetValue(0,300);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("_OnlyTexture");

}
VSMaterialOnlyTexture::~VSMaterialOnlyTexture()
{

}
VSMaterialTextureAndNormal::VSMaterialTextureAndNormal()
{

}
VSMaterialTextureAndNormal::VSMaterialTextureAndNormal(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture)
:VSMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"),this,4,false);
	pSpecularColor->SetValue(0,0.5f);
	pSpecularColor->SetValue(1,0.5f);
	pSpecularColor->SetValue(2,0.5f);
	pSpecularColor->SetValue(3,1.0f);
	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,false);
	pSpecularPow ->SetValue(0,100.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("TextureAndNormal");
}
VSMaterialTextureAndNormal::~VSMaterialTextureAndNormal()
{

}

VSMaterialOnlyEmissive::VSMaterialOnlyEmissive(const VSUsedName &ShowName)
:VSMaterial(ShowName)
{

	VSConstFloatValue * pEmissiveColor = VS_NEW VSConstFloatValue(_T("EmissiveColor"),this,4,true);
	pEmissiveColor->SetValue(0,1.0f);
	pEmissiveColor->SetValue(1,1.0f);
	pEmissiveColor->SetValue(2,1.0f);
	pEmissiveColor->SetValue(3,1.0f);

	m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(VSConstFloatValue::OUT_VALUE));
	m_ResourceName = _T("OnlyEmissive");
}
VSMaterialOnlyEmissive::~VSMaterialOnlyEmissive()
{

}
VSMaterialNoLight::VSMaterialNoLight(const VSUsedName &ShowName)
:VSMaterial(ShowName)
{
	// float One = 1.0f;
	VSConstFloatValue * pOne = VS_NEW VSConstFloatValue(_T("One"), this, 1, false);
	pOne->SetValue(0, 1.0f);

	// WorldPos
	VSWorldPos * pWorldPos = VS_NEW VSWorldPos(_T("WorldPos"), this);

	//WorldPos_One = float4(WorldPos,1.0f);
	VSMakeValue * pWorldPos_One = VS_NEW VSMakeValue(_T("WorldPos_One"), this, 2);
	pWorldPos_One->GetInputNode(VSMakeValue::IN_A)->Connection(pWorldPos->GetOutputNode(VSAdd::OUT_VALUE));
	pWorldPos_One->GetInputNode(VSMakeValue::IN_B)->Connection(pOne->GetOutputNode(VSAdd::OUT_VALUE));

	//float4X4 ReflectViewProject;
	VSConstFloatValue * pReflectViewProject_1 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_1 "), this, 4, true);
	VSConstFloatValue * pReflectViewProject_2 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_2 "), this, 4, true);
	VSConstFloatValue * pReflectViewProject_3 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_3 "), this, 4, true);
	VSConstFloatValue * pReflectViewProject_4 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_4 "), this, 4, true);

	//float Dot_1 = dot(WorldPos_One,ReflectViewProject_1);
	VSDot * pDot_1 = VS_NEW VSDot(_T("Dot_1"), this);
	pDot_1->GetInputNode(VSAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(VSWorldPos::OUT_VALUE));
	pDot_1->GetInputNode(VSAdd::IN_B)->Connection(pReflectViewProject_1->GetOutputNode(VSWorldPos::OUT_VALUE));

	//float Dot_2 = dot(WorldPos_One,ReflectViewProject_2);
	VSDot * pDot_2 = VS_NEW VSDot(_T("Dot_2"), this);
	pDot_2->GetInputNode(VSAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(VSWorldPos::OUT_VALUE));
	pDot_2->GetInputNode(VSAdd::IN_B)->Connection(pReflectViewProject_2->GetOutputNode(VSWorldPos::OUT_VALUE));

	//float Dot_3 = dot(WorldPos_One,ReflectViewProject_3);
	VSDot * pDot_3 = VS_NEW VSDot(_T("Dot_3"), this);
	pDot_3->GetInputNode(VSAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(VSWorldPos::OUT_VALUE));
	pDot_3->GetInputNode(VSAdd::IN_B)->Connection(pReflectViewProject_3->GetOutputNode(VSWorldPos::OUT_VALUE));

	//float Dot_4 = dot(WorldPos_One,ReflectViewProject_4);
	VSDot * pDot_4 = VS_NEW VSDot(_T("Dot_4"), this);
	pDot_4->GetInputNode(VSAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(VSWorldPos::OUT_VALUE));
	pDot_4->GetInputNode(VSAdd::IN_B)->Connection(pReflectViewProject_4->GetOutputNode(VSWorldPos::OUT_VALUE));

	//float2 Dot_12 = float2(Dot_1,Dot_2);
	VSMakeValue * pDot_12 = VS_NEW VSMakeValue(_T("Dot_12"), this, 2);
	pDot_12->GetInputNode(VSMakeValue::IN_A)->Connection(pDot_1->GetOutputNode(VSDot::OUT_VALUE));
	pDot_12->GetInputNode(VSMakeValue::IN_B)->Connection(pDot_2->GetOutputNode(VSDot::OUT_VALUE));

	//float2 Dot_44 = float2(Dot_4,Dot_4);
	VSMakeValue * pDot_44 = VS_NEW VSMakeValue(_T("Dot_44"), this, 2);
	pDot_44->GetInputNode(VSMakeValue::IN_A)->Connection(pDot_4->GetOutputNode(VSDot::OUT_VALUE));
	pDot_44->GetInputNode(VSMakeValue::IN_B)->Connection(pDot_4->GetOutputNode(VSDot::OUT_VALUE));

	// float2 Div = Dot_12 / Dot_44;
	VSDiv *pDiv = VS_NEW VSDiv(_T("Div"), this);
	pDiv->GetInputNode(VSDiv::IN_A)->Connection(pDot_12->GetOutputNode(VSMakeValue::OUT_VALUE));
	pDiv->GetInputNode(VSDiv::IN_B)->Connection(pDot_44->GetOutputNode(VSMakeValue::OUT_VALUE));


	VSConstFloatValue * pPointFive = VS_NEW VSConstFloatValue(_T("PointFive"), this, 1, false);
	pPointFive->SetValue(0, 0.5f);

	VSConstFloatValue * pNegPointFive = VS_NEW VSConstFloatValue(_T("NegPointFive"), this, 1, false);
	pNegPointFive->SetValue(0, -0.5f);

	//float2 FiveNegFive = float2(0.5f,-0.5f);
	VSMakeValue * pFiveNegFive = VS_NEW VSMakeValue(_T("FiveNegFive"), this, 2);
	pFiveNegFive->GetInputNode(VSMakeValue::IN_A)->Connection(pPointFive->GetOutputNode(VSConstFloatValue::OUT_VALUE));
	pFiveNegFive->GetInputNode(VSMakeValue::IN_B)->Connection(pNegPointFive->GetOutputNode(VSConstFloatValue::OUT_VALUE));

	// Div * FiveFive
	VSMul * pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(VSMul::IN_A)->Connection(pDiv->GetOutputNode(VSDiv::OUT_VALUE));
	pMul->GetInputNode(VSMul::IN_B)->Connection(pFiveNegFive->GetOutputNode(VSMakeValue::OUT_VALUE));


	//float2 FiveFive = float2(0.5f,0.5f);
	VSMakeValue * pFiveFive = VS_NEW VSMakeValue(_T("FiveFive"), this, 2);
	pFiveFive->GetInputNode(VSMakeValue::IN_A)->Connection(pPointFive->GetOutputNode(VSConstFloatValue::OUT_VALUE));
	pFiveFive->GetInputNode(VSMakeValue::IN_B)->Connection(pPointFive->GetOutputNode(VSConstFloatValue::OUT_VALUE));

	VSAdd *pAdd = VS_NEW VSAdd(_T("Add"), this);
	pAdd->GetInputNode(VSAdd::IN_A)->Connection(pMul->GetOutputNode(VSMul::OUT_VALUE));
	pAdd->GetInputNode(VSAdd::IN_B)->Connection(pFiveFive->GetOutputNode(VSMakeValue::OUT_VALUE));

	

	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);

	p2DTexSamplerNode->GetInputNode(VS2DTexSampler::IN_TEXCOORD)->Connection(pAdd->GetOutputNode(VSAdd::OUT_VALUE));

	m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(p2DTexSamplerNode->GetOutputNode(VS2DTexSampler::OUT_COLOR));
	m_ResourceName = _T("NoLight");
}
VSMaterialNoLight::~VSMaterialNoLight()
{

}
VSMaterialOrenNayar::VSMaterialOrenNayar()
{

}
VSMaterialOrenNayar::VSMaterialOrenNayar(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
							   VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_OREN_NAYAR)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);


	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pRoughnessSquared = VS_NEW VSConstFloatValue(_T("RoughnessSquared"),this,1,true);
	pRoughnessSquared ->SetValue(0,0.5f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
 	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessSquared"))->Connection(pRoughnessSquared->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
 	pFinalAlpah->SetValue(0,0.6f);
 
 	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	m_ResourceName = _T("_MaterialOrenNayar");
}
VSMaterialOrenNayar::~VSMaterialOrenNayar()
{

}
VSMaterialPhone::VSMaterialPhone()
{

}
VSMaterialPhone::VSMaterialPhone(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
										 VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture,bool bPhoneSpecular):VSMaterial(ShowName,MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,true);
	pSpecularPow ->SetValue(0,50.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));


	if(bPhoneSpecular)
	{
		((VSPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(VSPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhone");

}
VSMaterialPhone::~VSMaterialPhone()
{

}
VSMaterialPhoneTwoPass::VSMaterialPhoneTwoPass()
{

}
VSMaterialPhoneTwoPass::VSMaterialPhoneTwoPass(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,
	VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture, bool bPhoneSpecular) :VSMaterial(ShowName, MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"), this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"), this, 1, true);
	pSpecularPow->SetValue(0, 50.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);


	if (bPhoneSpecular)
	{
		((VSPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(VSPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhoneTwoPass");


	AddPass(MUT_PHONE);
	VSConstFloatValue * pFinalColor = VS_NEW VSConstFloatValue(_T("FinalColor"), this, 3, false);
	pFinalColor->SetValue(0, 1.0f);
	pFinalColor->SetValue(1, 0.0f);
	pFinalColor->SetValue(2, 0.0f);
	m_pShaderMainFunction[1]->GetInputNode(_T("EmissiveColor"))->Connection(pFinalColor->GetOutputNode(0));

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.5f);

	m_pShaderMainFunction[1]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	VSBlendDesc BlendDesc;
	BlendDesc.bBlendEnable[0] = true;
	BlendDesc.ucBlendOp[0] = VSBlendDesc::BO_ADD;
	BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_INVSRCALPHA;
	BlendDesc.ucSrcBlend[0] = VSBlendDesc::BP_SRCALPHA;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	SetBlendState(pBlendState, 1);
}
VSMaterialPhoneTwoPass::~VSMaterialPhoneTwoPass()
{

}
VSMaterialMinnaert::VSMaterialMinnaert()
{

}
VSMaterialMinnaert::VSMaterialMinnaert(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
								VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_MINNAERT)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pPower= VS_NEW VSConstFloatValue(_T("Power"),this,1,true);
	pPower ->SetValue(0,0.5f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Power"))->Connection(pPower->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialMinnaert");

}
VSMaterialMinnaert::~VSMaterialMinnaert()
{

}
VSMaterialStrauss::VSMaterialStrauss()
{

}
VSMaterialStrauss::VSMaterialStrauss(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
									   VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_STRAUSS)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSmoothness= VS_NEW VSConstFloatValue(_T("Smoothness"),this,1,true);
	pSmoothness ->SetValue(0,0.5f);

	VSConstFloatValue * pMetalness= VS_NEW VSConstFloatValue(_T("Metalness"),this,1,true);
	pMetalness ->SetValue(0,0.5f);

	VSConstFloatValue * pTransparency= VS_NEW VSConstFloatValue(_T("Transparency"),this,1,true);
	pTransparency ->SetValue(0,0.5f);

	VSConstFloatValue * pK= VS_NEW VSConstFloatValue(_T("K"),this,1,true);
	pK ->SetValue(0,0.5f);

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Smoothness"))->Connection(pSmoothness->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Metalness"))->Connection(pMetalness->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Transparency"))->Connection(pTransparency->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("K"))->Connection(pK->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialStrauss");

}
VSMaterialStrauss::~VSMaterialStrauss()
{

}
VSMaterialShirley::VSMaterialShirley()
{

}
VSMaterialShirley::VSMaterialShirley(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
									 VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_SHIRLEY)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pNU= VS_NEW VSConstFloatValue(_T("NU"),this,1,true);
	pNU ->SetValue(0,0.5f);

	VSConstFloatValue * pNV= VS_NEW VSConstFloatValue(_T("NV"),this,1,true);
	pNV ->SetValue(0,0.5f);

	VSConstFloatValue * pRs= VS_NEW VSConstFloatValue(_T("Rs"),this,1,true);
	pRs ->SetValue(0,0.5f);

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("NU"))->Connection(pNU->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("NV"))->Connection(pNV->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Rs"))->Connection(pRs->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialShirley");

}
VSMaterialShirley::~VSMaterialShirley()
{

}
VSMaterialSchlick::VSMaterialSchlick()
{

}
VSMaterialSchlick::VSMaterialSchlick(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
									 VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_SCHLICK)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSpecularPow= VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,true);
	pSpecularPow ->SetValue(0,0.5f);



	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));



	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialSchlick");

}
VSMaterialSchlick::~VSMaterialSchlick()
{

}
VSMaterialCookTorrance::VSMaterialCookTorrance()
{

}
VSMaterialCookTorrance::VSMaterialCookTorrance(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
									 VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture,bool bSample):VSMaterial(ShowName,MUT_COOKTOORANCE)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pRoughness= VS_NEW VSConstFloatValue(_T("Roughness"),this,1,true);
	pRoughness ->SetValue(0,0.5f);
	VSConstFloatValue * pC= VS_NEW VSConstFloatValue(_T("C"),this,1,true);
	pC ->SetValue(0,0.5f);
	VSConstFloatValue * pR= VS_NEW VSConstFloatValue(_T("R"),this,1,true);
	pR ->SetValue(0,0.5f);


	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Roughness"))->Connection(pRoughness->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("C"))->Connection(pC->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("R"))->Connection(pR->GetOutputNode(0));



	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));


	if (bSample)
	{
		((VSCookTorranceShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(VSCookTorranceShaderFunction::ST_ALTERNATE_BLINN);
	}

	m_ResourceName = _T("_MaterialCookTorrance");

}
VSMaterialCookTorrance::~VSMaterialCookTorrance()
{

}
VSMaterialIsotropicWard::VSMaterialIsotropicWard()
{

}
VSMaterialIsotropicWard::VSMaterialIsotropicWard(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
											   VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_ISOTROPICWARD)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pRoughness= VS_NEW VSConstFloatValue(_T("Roughness"),this,1,true);
	pRoughness ->SetValue(0,0.5f);



	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Roughness"))->Connection(pRoughness->GetOutputNode(0));




	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialIsotropicWard");

}
VSMaterialIsotropicWard::~VSMaterialIsotropicWard()
{

}
VSMaterialAnisotropicWard::VSMaterialAnisotropicWard()
{

}
VSMaterialAnisotropicWard::VSMaterialAnisotropicWard(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
												 VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_ANISOTROPICWARD)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pRoughnessX= VS_NEW VSConstFloatValue(_T("RoughnessX"),this,1,true);
	pRoughnessX ->SetValue(0,0.5f);

	VSConstFloatValue * pRoughnessY= VS_NEW VSConstFloatValue(_T("RoughnessY"),this,1,true);
	pRoughnessY ->SetValue(0,0.5f);

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessX"))->Connection(pRoughnessX->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessY"))->Connection(pRoughnessY->GetOutputNode(0));



	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialAnIsotropicWard");

}
VSMaterialAnisotropicWard::~VSMaterialAnisotropicWard()
{

}
VSMaterialCustom::VSMaterialCustom()
{

}
VSMaterialCustom::VSMaterialCustom(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture, VSTexAllStateR * pEmissiveTexture)
:VSMaterial(ShowName, MUT_CUSTOM)
{
	VSLightDir * pLightDir = VS_NEW VSLightDir(_T("LightDir"), this);

	VSConstFloatValue * pValue = VS_NEW VSConstFloatValue(_T("ValueTest"), this, 1, false);
	pValue->SetValue(0, -1.0f);

	VSMul * pMul1 = VS_NEW VSMul(_T("Mul1"), this);

	pMul1->GetInputNode(0)->Connection(pLightDir->GetOutputNode(0));
	pMul1->GetInputNode(1)->Connection(pValue->GetOutputNode(0));

	VSWorldNormal * pWorldNormal = VS_NEW VSWorldNormal(_T("WorldNormal"), this);

	VSDot * pDot = VS_NEW VSDot(_T("Dot"), this);

	pDot->GetInputNode(0)->Connection(pWorldNormal->GetOutputNode(0));
	pDot->GetInputNode(1)->Connection(pMul1->GetOutputNode(0));

	VSSaturate *pSaturate = VS_NEW VSSaturate(_T("Saturate"), this);
	pSaturate->GetInputNode(0)->Connection(pDot->GetOutputNode(0));

	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);


	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSMul* pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(0)->Connection(pSaturate->GetOutputNode(0));
	pMul->GetInputNode(1)->Connection(p2DTexSamplerNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Custom"))->Connection(pMul->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Diffuse"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	m_ResourceName = _T("_MaterialCustom");

	//m_pShaderMainFunction[0]->SetAlphaTestValue(0.9f);

}
VSMaterialCustom::~VSMaterialCustom()
{

}

VSLightMaterial::VSLightMaterial()
{

}
VSLightMaterial::VSLightMaterial(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture)
:VSMaterial(ShowName, MUT_LIGHT)
{




	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);



	m_pLightShaderFunction->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));


	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pLightShaderFunction->GetInputNode(_T("Alpha"))->Connection(p2DTexSamplerNode->GetOutputNode(4));

	m_ResourceName = _T("_LightMaterial");

	//m_pShaderMainFunction[0]->SetAlphaTestValue(0.9f);

}
VSLightMaterial::~VSLightMaterial()
{

}

VSPostEffectMaterial::VSPostEffectMaterial()
{

}
VSPostEffectMaterial::VSPostEffectMaterial(const VSUsedName &ShowName)
:VSMaterial(ShowName, MUT_POSTEFFECT)
{




	VSColorBuffer* pColorBuffer = VS_NEW VSColorBuffer(_T("ColorBuffer"), this);
	VSConstFloatValue * pMulColor = VS_NEW VSConstFloatValue(_T("Color"), this, 4, false);
	pMulColor->SetValue(0, 1.0f);
	pMulColor->SetValue(1, 0.0f);
	pMulColor->SetValue(2, 0.0f);
	pMulColor->SetValue(3, 1.0f);

	VSMul* pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(0)->Connection(pColorBuffer->GetOutputNode(0));
	pMul->GetInputNode(1)->Connection(pMulColor->GetOutputNode(0));

	m_pPostEffectShaderFunction->GetInputNode(VSPostEffectShaderFunction::IN_COLOR)->Connection(pMul->GetOutputNode(0));
	m_ResourceName = _T("_PostEffectMaterial");

}
VSPostEffectMaterial::~VSPostEffectMaterial()
{

}