#include "VSFont.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSFont,VSBind)
BEGIN_ADD_PROPERTY(VSFont,VSBind)
REGISTER_PROPERTY(m_Face,Face,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSize,Size,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bBold,Bold,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bItalic,Italic,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pFontTex, FontTex, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY_FIXED_DATA(m_pCharacterData, ASCII_NUM + 1, CharacterData, true)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSFont)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSFont> VSFont::Default;
bool VSFont::ms_bIsEnableASYNLoader = true;
bool VSFont::ms_bIsEnableGC = true;
bool VSFont::InitialDefaultState()
{
	VSFileName FileName = VSResourceManager::ms_FontPath + _T("Arial.FONT");
	Default = VSResourceManager::NewLoadFont(FileName.GetBuffer());
	if (!Default)
	{
		return false;
	}
	return 1;
}
bool VSFont::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
VSFont::VSFont(const TCHAR* acFace, unsigned int uiSize, bool bBold,
				bool bItalic)
{
	m_bItalic = bItalic;
	if(acFace)
	{
		m_Face = acFace;
	}
	else
	{
		m_Face = _T("Arial");
	}
	m_uiSize = uiSize;
	m_bBold = bBold;
	m_pFontTex = NULL;
	m_pCharacterData = NULL;
	m_pFontTexAllState = NULL;
}
VSFont::~VSFont()
{
	m_pFontTex = NULL;
	VSMAC_DELETEA(m_pCharacterData);
	m_pFontTexAllState = NULL;
}
VSFont::VSFont()
{
	m_bItalic = 1;
	m_Face = _T("Arial");
	m_uiSize = 20;
	m_bBold = 0;
	m_pCharacterData = NULL;
	m_pFontTexAllState = NULL;
	m_pFontTex = NULL;
}
void VSFont::SetFontData(VS2DTexture * pTexture, VSREAL * pCharacterData)
{
	if (!pTexture || !pCharacterData)
	{
		return;
	}
	m_pFontTex = pTexture;
	VSMAC_DELETEA(m_pCharacterData);
	m_pCharacterData = VS_NEW VSREAL[ASCII_NUM + 1];
	VSMemcpy(m_pCharacterData, pCharacterData, sizeof(VSREAL)* (ASCII_NUM + 1));
	return;
}
bool VSFont::PostLoad(void * pDate)
{
	VSObject::PostLoad(pDate);
	if (m_pFontTex)
	{
		VSSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
		VSSamplerStatePtr pSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);
		m_pFontTexAllState = VS_NEW VSTexAllState(m_pFontTex);
		m_pFontTexAllState->SetSamplerState(pSamplerState);
	}
	return true;
}
bool VSFont::PostClone(VSObject * pObjectSrc)
{
	VSObject::PostClone(pObjectSrc);
	if (m_pFontTex)
	{
		VSSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
		VSSamplerStatePtr pSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);
		m_pFontTexAllState = VS_NEW VSTexAllState(m_pFontTex);
		m_pFontTexAllState->SetSamplerState(pSamplerState);
	}
	return true;
}