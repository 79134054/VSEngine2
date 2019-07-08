#include "VSPostEffectSet.h"
#include "VSPEBeginFunction.h"
#include "VSPEEndFunction.h"
#include "VSGraphicInclude.h"
#include "VSPEGray.h"
#include "VSPEMaterial.h"
#include "VSRenderTarget.h"
#include "VSStream.h"
using namespace VSEngine2;
VSPointer<VSPostEffectSet> VSPostEffectSet::Default = NULL;
bool VSPostEffectSet::ms_bIsEnableASYNLoader = false;
bool VSPostEffectSet::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(VSPostEffectSet,VSObject)
BEGIN_ADD_PROPERTY(VSPostEffectSet,VSObject)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPEBeginFunc,BeginFuntion,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPEEndFunc,EndFunction,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffectFunctionArray,PostEffectFunctionArray,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPostEffectSet)
IMPLEMENT_INITIAL_END
VSPostEffectSet::VSPostEffectSet()
{
	m_pPostEffectFunctionArray.Clear();
}
VSPostEffectSet::VSPostEffectSet(const VSUsedName &ShowName)
{
	m_pPostEffectFunctionArray.Clear();
	m_ShowName = ShowName;
	m_pPEBeginFunc = VS_NEW VSPEBeginFunction(_T("Begin"),this);
	m_pPEEndFunc = VS_NEW VSPEEndFunction(_T("End"),this);
}
VSPostEffectSet::~VSPostEffectSet()
{
	for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ; i++)
	{
		VSPostEffectFunction * pShaderFunction = m_pPostEffectFunctionArray[i];
		m_pPostEffectFunctionArray[i] = NULL;
		VSMAC_DELETE(pShaderFunction);
	}
	m_pPostEffectFunctionArray.Clear();

	m_pPEBeginFunc = NULL;
	m_pPEEndFunc = NULL;
}
void VSPostEffectSet::AddPostEffectFunction(VSPostEffectFunction * pPostEffectFunction)
{
	if(pPostEffectFunction)
	{
		m_pPostEffectFunctionArray.AddElement(pPostEffectFunction);
	}

}
void VSPostEffectSet::DeletePostEffectFunction(VSPostEffectFunction * pPostEffectFunction)
{
	if(pPostEffectFunction)
	{
		for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ; i++)
		{
			if(m_pPostEffectFunctionArray[i] == pPostEffectFunction)
			{
				m_pPostEffectFunctionArray.Erase(i);
				return ;
			}
		}

	}

}

void VSPostEffectSet::Draw(VSCuller & Culler,double dAppTime)
{
	m_pPEEndFunc->ClearFlag();
	m_pPEEndFunc->Draw(Culler,dAppTime);
}
void VSPostEffectSet::SetBeginTargetArray(VSArray<VSRenderTarget *> * pBeginTargetArray)
{
	m_pPEBeginFunc->SetPara(pBeginTargetArray);
}
void VSPostEffectSet::GetRT(unsigned int uiWidth,unsigned int uiHeight)
{
	m_pPEEndFunc->GetRT(uiWidth,uiHeight);
}
void VSPostEffectSet::DisableRT()
{
	m_pPEEndFunc->DisableRT();
}
VSRenderTarget * VSPostEffectSet::GetEndTarget(unsigned int i)
{
	if (i < m_pPEEndFunc->m_pInput.GetNum())
	{
		VSRenderTarget * pTexture = m_pPEEndFunc->GetRenderTarget(m_pPEEndFunc->m_pInput[i]);
		return pTexture;
	}
	return NULL;
}
bool VSPostEffectSet::SetEndTarget(VSRenderTarget *pEndTarget)
{
	for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ;i++)
	{
		m_pPostEffectFunctionArray[i]->m_bLastOne = false;
	}
	return m_pPEEndFunc->SetEndTarget(pEndTarget);

}
VSPostEffectFunction * VSPostEffectSet::GetPEFunctionFromShowName(const VSUsedName & ShowName)
{
	for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ;i++)
	{
		if (m_pPostEffectFunctionArray[i]->GetShowName() == ShowName)
		{
			return m_pPostEffectFunctionArray[i];
		}
	}
	return NULL;
}
VSPESetGray::VSPESetGray(const VSUsedName &ShowName) : VSPostEffectSet(ShowName)
{
	VSPEGray * pPEGray = VS_NEW VSPEGray(_T("Gray"),this);
	m_pPEEndFunc->GetInputNode(VSPEEndFunction::INPUT_COLOR)->Connection(pPEGray->GetOutputNode(VSPEGray::OUT_COLOR));
	pPEGray->GetInputNode(VSPEGray::INPUT_COLOR)->Connection(m_pPEBeginFunc->GetOutputNode(VSPEBeginFunction::OUT_COLOR));
}
VSPESetGray::~VSPESetGray()
{
}

VSPESetMaterialAndGray::VSPESetMaterialAndGray(const VSUsedName &ShowName, VSMaterialR * pMaterial) : VSPostEffectSet(ShowName)
{
	VSPEGray * pPEGray = VS_NEW VSPEGray(_T("Gray"), this);
	VSPEMaterial * pPEMaterial = VS_NEW VSPEMaterial(_T("Material"),this);
	pPEMaterial->SetMaterial(pMaterial);
	m_pPEEndFunc->GetInputNode(VSPEEndFunction::INPUT_COLOR)->Connection(pPEMaterial->GetOutputNode(VSPEMaterial::OUT_COLOR));
	pPEMaterial->GetInputNode(VSPEMaterial::INPUT_COLOR)->Connection(pPEGray->GetOutputNode(VSPEGray::OUT_COLOR));
	pPEGray->GetInputNode(VSPEGray::INPUT_COLOR)->Connection(m_pPEBeginFunc->GetOutputNode(VSPEBeginFunction::OUT_COLOR));
}
VSPESetMaterialAndGray::~VSPESetMaterialAndGray()
{
}
