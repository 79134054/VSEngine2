#include "VSShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSShader,VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSShader)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSShader,VSBind)
REGISTER_PROPERTY(m_pUserConstant,UserConstant,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pUserSampler,UserSampler,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_ShaderKey,ShaderKey,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_Buffer,ShaderBuffer,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_MainFunName,MainFunName,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bCreatePara,bCreatePara,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pCacheBuffer,m_uiCacheBufferSize,CacheBuffer);
REGISTER_PROPERTY(m_uiArithmeticInstructionSlots,ArithmeticInstructionSlots,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiTextureInstructionSlots,TextureInstructionSlots,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiConstBufferSize, ConstBufferSize, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSShader::VSShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSBind(VSBind::MT_VRAM)
{
	m_pUserConstant.Clear();
	VSMAC_ASSERT(pBuffer);
	if (IsFromFile)
	{
		m_ResourceName = pBuffer;
	}
	else
	{
		m_Buffer = pBuffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	m_uiConstBufferSize = 0;
}
VSShader::VSShader()
:VSBind(VSBind::MT_VRAM)
{
	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	m_uiConstBufferSize = 0;
}
VSShader::VSShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSBind(VSBind::MT_VRAM)
{
	m_pUserConstant.Clear();
	if (IsFromFile)
	{
		m_ResourceName = Buffer;
	}
	else
	{
		m_Buffer = Buffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	
}
VSShader::~VSShader()
{
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();
}
void VSShader::SetShaderString(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
{
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();

	VSMAC_ASSERT(pBuffer);
	if (IsFromFile)
	{
		m_ResourceName = pBuffer;
	}
	else
	{
		m_Buffer = pBuffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
}
void VSShader::SetShaderString(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
{
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();

	if (IsFromFile)
	{
		m_ResourceName = Buffer;
	}
	else
	{
		m_Buffer = Buffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
}
void VSShader::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_Buffer.Clear();
		}
	}
}
bool VSShader::SetParam(const VSUsedName &Name,VSTexAllState * pTexture,unsigned int uiIndex)
{
	if (!pTexture || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0 ; i < m_pUserSampler.GetNum() ;i++)
	{
		if (Name == m_pUserSampler[i]->GetName()
			&& pTexture->m_pTex->GetTexType() == m_pUserSampler[i]->GetTexType())
		{
			m_pUserSampler[i]->SetTex(pTexture,uiIndex);
			break;
		}	
	}
	return true;
}
bool VSShader::SetParam(const VSUsedName &Name,void * pDate)
{
	if (!pDate || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0 ; i < m_pUserConstant.GetNum() ;i++)
	{
		if (Name == m_pUserConstant[i]->GetShowName())
		{
			void * pConstanDate = m_pUserConstant[i]->GetDate();
			if (!pConstanDate)
			{
				return false;
			}
			VSMemcpy(pConstanDate,pDate,m_pUserConstant[i]->GetSize());
			break;
		}	
	}
	return true;
}
bool VSShader::SetCacheBuffer(void * pBuffer,unsigned int uiSize)
{
	if (!pBuffer || !uiSize)
	{
		return false;
	}
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = uiSize;
	m_pCacheBuffer = VS_NEW UCHAR[uiSize];
	VSMemcpy(m_pCacheBuffer,pBuffer,uiSize);
	
	
	
	

	return 1;
}

