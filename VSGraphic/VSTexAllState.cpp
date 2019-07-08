#include "VSTexAllState.h"
#include "VS2DTexture.h"
#include "VSResourceManager.h"
#include "VSImage.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTexAllState,VSObject)
VSPointer<VSTexAllState> VSTexAllState::Default;
VSPointer<VSTexAllState> VSTexAllState::ms_pOrenNayarLookUpTable;
VSPointer<VSTexAllState> VSTexAllState::ms_pStraussFLookUpTable;
VSPointer<VSTexAllState> VSTexAllState::ms_pStraussSLookUpTable;
VSPointer<VSTexAllState> VSTexAllState::ms_pCookTorranceLookUpTable;
VSPointer<VSTexAllState> VSTexAllState::ms_pIsotropicWardLookUpTable;
bool VSTexAllState::ms_bIsEnableASYNLoader = true;
bool VSTexAllState::ms_bIsEnableGC = true;
BEGIN_ADD_PROPERTY(VSTexAllState,VSObject)
REGISTER_PROPERTY(m_pTex,Tex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_SamplerDesc,SamplerDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bSRGB,bSRGB,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTexAllState)
ADD_PRIORITY(VS2DTexture)
ADD_PRIORITY(VSSamplerState)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
//#define DEFAULT_16FLOAT_TEXTURE
bool VSTexAllState::InitialDefaultState()
{

	VSSamplerDesc SamplerDesc;
	SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
	VSSamplerStatePtr pSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);

	VSFileName FileName = VSResourceManager::ms_TexturePath + _T("Default.bmp");
	Default = VSResourceManager::Load2DTexture(FileName.GetBuffer(),pSamplerState);
	if(!Default)
		return 0;

	//
#ifdef DEFAULT_16FLOAT_TEXTURE
	unsigned int uiTextureFormat = VSRenderer::SFT_R16F;
#else
	unsigned int uiTextureFormat = VSRenderer::SFT_R32F;
#endif

	unsigned int uiOrenNayarTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	unsigned short * pBuffer = VS_NEW unsigned short[uiOrenNayarTexSize * uiOrenNayarTexSize];	
#else
	VSREAL * pBuffer = VS_NEW VSREAL[uiOrenNayarTexSize * uiOrenNayarTexSize];
#endif		
	for (unsigned int i = 0 ; i < uiOrenNayarTexSize ;i++)
	{
		VSREAL VdotN = (i * 1.0f / (uiOrenNayarTexSize - 1)) * 2.0f - 1.0f;
		VSREAL AngleViewNormal = ACOS(VdotN);
		for (unsigned int j = 0 ; j < uiOrenNayarTexSize ; j++)
		{			
			VSREAL LdotN = (j * 1.0f / (uiOrenNayarTexSize - 1)) * 2.0f - 1.0f;
			VSREAL AngleLightNormal = ACOS(LdotN);
			
			VSREAL Alpha = Max(AngleViewNormal,AngleLightNormal);
			VSREAL Beta = Min(AngleViewNormal,AngleLightNormal);
			VSREAL fResult = ABS(SIN(Alpha) * TAN(Beta));
#ifdef DEFAULT_16FLOAT_TEXTURE
			pBuffer[i * uiOrenNayarTexSize + j] = FloatToHalf(fResult);	
#else
			pBuffer[i * uiOrenNayarTexSize + j] = fResult;	
#endif
		}
	}
	ms_pOrenNayarLookUpTable = VSResourceManager::Create2DTexture(uiOrenNayarTexSize,uiOrenNayarTexSize,uiTextureFormat,1,pBuffer);
	VSMAC_DELETEA(pBuffer);

	
	ms_pOrenNayarLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pOrenNayarLookUpTable)
	{
		return false;
	}



	unsigned int uiStraussTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	pBuffer = VS_NEW unsigned short[uiStraussTexSize];
#else
	pBuffer = VS_NEW VSREAL[uiStraussTexSize];
#endif
	VSREAL Kf = 1.12f;
	for (unsigned int i = 0 ; i < uiStraussTexSize ;i++)
	{
		VSREAL VdotN = (i * 1.0f / (uiStraussTexSize - 1)) * 2.0f - 1.0f;
		VSREAL fShadow = ((1.0f / ((VdotN - Kf) * (VdotN - Kf))) - 1.0f/(Kf * Kf)) / ((1.0f / ((1.0f - Kf) * (1.0f - Kf))) - 1.0f/(Kf * Kf));
#ifdef DEFAULT_16FLOAT_TEXTURE
		pBuffer[i] = FloatToHalf(fShadow);	
#else
		pBuffer[i] = fShadow;	
#endif
	}

	ms_pStraussFLookUpTable = VSResourceManager::Create1DTexture(uiStraussTexSize,uiTextureFormat,1,pBuffer);


	ms_pStraussFLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pStraussFLookUpTable)
	{
		VSMAC_DELETEA(pBuffer);
		return false;
	}

	VSREAL Ks = 1.01f;
	for (unsigned int i = 0 ; i < uiStraussTexSize ;i++)
	{
		VSREAL VdotN = (i * 1.0f / (uiStraussTexSize - 1)) * 2.0f - 1.0f;
		VSREAL fShadow = (1.0f / ((1.0f - Ks) * (1.0f - Ks)) - 1.0f / ((VdotN - Ks) * (VdotN - Ks))) / (1.0f / ((1.0f - Ks) * (1.0f - Ks)) - 1.0f / (Ks * Ks));
#ifdef DEFAULT_16FLOAT_TEXTURE
		pBuffer[i] = FloatToHalf(fShadow);	
#else
		pBuffer[i] = fShadow;	
#endif
	}

	ms_pStraussSLookUpTable = VSResourceManager::Create1DTexture(uiStraussTexSize,uiTextureFormat,1,pBuffer);
	VSMAC_DELETEA(pBuffer);

	ms_pStraussSLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pStraussSLookUpTable)
	{
		return false;
	}



	unsigned int uiCookTorranceTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	pBuffer = VS_NEW unsigned short[uiCookTorranceTexSize * uiCookTorranceTexSize];
#else
	pBuffer = VS_NEW VSREAL[uiCookTorranceTexSize * uiCookTorranceTexSize];
#endif
	for (unsigned int i = 0 ; i < uiCookTorranceTexSize ;i++)
	{
		VSREAL HdotN = (i * 1.0f / (uiCookTorranceTexSize - 1)) * 2.0f - 1.0f;

		for (unsigned int j = 0 ; j < uiCookTorranceTexSize ; j++)
		{			
			VSREAL Roughness = (j * 1.0f / (uiCookTorranceTexSize - 1));

			VSREAL r_sq = Roughness * Roughness;

			float r_a = 1.0f / (4.0f * r_sq * POW(HdotN,4.0f));
			float r_b = HdotN * HdotN - 1.0f;
			float r_c = r_sq * HdotN * HdotN;
			VSREAL fResult = r_a * EXP(r_b / r_c);
#ifdef DEFAULT_16FLOAT_TEXTURE
			pBuffer[i * uiCookTorranceTexSize + j] = FloatToHalf(fResult);	
#else
			pBuffer[i * uiCookTorranceTexSize + j] = fResult;	
#endif
		}
	}
	ms_pCookTorranceLookUpTable = VSResourceManager::Create2DTexture(uiCookTorranceTexSize,uiCookTorranceTexSize,uiTextureFormat,1,pBuffer);
	VSMAC_DELETEA(pBuffer);


	ms_pCookTorranceLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pCookTorranceLookUpTable)
	{
		return false;
	}

	unsigned int uiIsotropicWardTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	pBuffer = VS_NEW unsigned short[uiIsotropicWardTexSize];
#else
	pBuffer = VS_NEW VSREAL[uiIsotropicWardTexSize];
#endif
	for (unsigned int i = 0 ; i < uiIsotropicWardTexSize ;i++)
	{
		VSREAL HdotN = (i * 1.0f / (uiIsotropicWardTexSize - 1)) * 2.0f - 1.0f;
		VSREAL fResult = -POW(TAN(ACOS(HdotN)),2.0f);
#ifdef DEFAULT_16FLOAT_TEXTURE
		pBuffer[i] = FloatToHalf(fResult);	
#else
		pBuffer[i] = fResult;
#endif

	}
	ms_pIsotropicWardLookUpTable = VSResourceManager::Create1DTexture(uiIsotropicWardTexSize,uiTextureFormat,1,pBuffer);
	VSMAC_DELETEA(pBuffer);


	ms_pIsotropicWardLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pIsotropicWardLookUpTable)
	{
		return false;
	}
	return 1;
}
bool VSTexAllState::TerminalDefaultState()
{
	Default = NULL;
	ms_pOrenNayarLookUpTable = NULL;
	ms_pStraussSLookUpTable = NULL;
	ms_pStraussFLookUpTable = NULL;
	ms_pCookTorranceLookUpTable = NULL;
	ms_pIsotropicWardLookUpTable = NULL;
	return 1;
}
VSTexAllState::VSTexAllState(VSTexture * pTexture)
{
	m_pTex = pTexture;
	m_pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	m_bSRGB = false;
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
}
VSTexAllState::VSTexAllState()
{
	m_pTex			= NULL;
	m_pSamplerState = NULL;
	m_bSRGB = false;
}
VSTexAllState::~VSTexAllState()
{
	m_pTex			= NULL;

	m_pSamplerState = NULL;
}
bool VSTexAllState::PostLoad(void * pDate)
{
	VSObject::PostLoad(pDate);
	m_pSamplerState = VSResourceManager::CreateSamplerState(m_SamplerDesc);
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
	return true;
}
bool VSTexAllState::PostClone(VSObject * pObjectSrc)
{
	VSObject::PostClone(pObjectSrc);
	m_pSamplerState = VSResourceManager::CreateSamplerState(m_SamplerDesc);
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
	return true;
}
void VSTexAllState::SetSRGBEable(bool bEnable)
{	
	m_bSRGB = bEnable;
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
}

