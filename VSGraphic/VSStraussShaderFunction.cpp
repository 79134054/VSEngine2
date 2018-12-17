#include "VSStraussShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSStraussShaderFunction,VSShaderMainFunction)
BEGIN_ADD_PROPERTY(VSStraussShaderFunction,VSShaderMainFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStraussShaderFunction)
IMPLEMENT_INITIAL_END
VSStraussShaderFunction::VSStraussShaderFunction(const VSUsedName &ShowName,VSMaterial * pMaterial)
:VSShaderMainFunction(ShowName,pMaterial)
{

	VSString InputName = _T("DiffuseColor");
	VSInputNode * pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("EmissiveColor");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("SpecularColor");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Smoothness");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Metalness");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Transparency");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("K");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Normal");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	InputName = _T("Alpha");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("ReflectMip");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("ReflectPow");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	VSString OutputName = _T("OutputColor");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

}
bool VSStraussShaderFunction::GetFuntionString(VSString &OutString)const
{
	GetAlphaTestString(OutString);
	if(!m_pInput[IN_DIFFUSE_COLOR]->GetOutputLink() && !m_pInput[IN_SPECULAR_COLOR]->GetOutputLink())
	{
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") + VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));
		OutString += _T(";\n");
	}
	else
	{

		int iLightNum[VSLight::LT_MAX] = { 0 };	
		for (unsigned int i = 0 ; i < m_MSPara.LightArray.GetNum() ; i++)
		{
			if (m_MSPara.LightArray[i])
			{
				for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
				{
					if (m_MSPara.LightArray[i]->GetLightType() == j)
					{

						iLightNum[j]++;
					}
				}
			}
		}


		OutString += VSRenderer::ms_pRenderer->Float3() + _T("WorldCameraDir = ");
		VSRenderer::ms_pRenderer->ComputeDir(VSShaderStringFactory::ms_CameraWorldPos,VSShaderStringFactory::ms_WorldPos,OutString);
		OutString += _T(";\n");

		VSRenderer::ms_pRenderer->TranLightToTemp(m_MSPara.LightArray,OutString);
		VSRenderer::ms_pRenderer->GetLightFunction(m_MSPara.LightArray,VSShaderStringFactory::ms_WorldPos,OutString);
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") +
			VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("0")); 

		
		VSArray<VSString> ShadowStringArray[VSLight::LT_MAX];
		GetLightShadow(m_MSPara,ShadowStringArray);
		for (unsigned int i = 0 ; i < VSLight::LT_MAX ; i++)
		{
			if (!iLightNum[i])
			{
				continue;
			}
			if(i == VSLight::LT_DIRECTION)
			{			


				VSRenderer::ms_pRenderer->StraussDirectionalLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString(),
					m_pInput[IN_SPECULAR_COLOR]->GetNodeName().GetString(),
					m_pInput[IN_SMOOTHNESS]->GetNodeName().GetString(),m_pInput[IN_METALNESS]->GetNodeName().GetString(),
					m_pInput[IN_TRANSPARENCY]->GetNodeName().GetString(),m_pInput[IN_K]->GetNodeName().GetString(),
					VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),UseLookUpTable(),ShadowStringArray[i],OutString);


			}
			else if(i == VSLight::LT_POINT)
			{


				VSRenderer::ms_pRenderer->StraussPointLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString(),
					m_pInput[IN_SPECULAR_COLOR]->GetNodeName().GetString(),
					m_pInput[IN_SMOOTHNESS]->GetNodeName().GetString(),m_pInput[IN_METALNESS]->GetNodeName().GetString(),
					m_pInput[IN_TRANSPARENCY]->GetNodeName().GetString(),m_pInput[IN_K]->GetNodeName().GetString(),
					VSShaderStringFactory::ms_WorldPos,VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),UseLookUpTable(),ShadowStringArray[i],OutString);

			}
			else if(i == VSLight::LT_SPOT)
			{

				VSRenderer::ms_pRenderer->StraussSpotLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString(),
					m_pInput[IN_SPECULAR_COLOR]->GetNodeName().GetString(),
					m_pInput[IN_SMOOTHNESS]->GetNodeName().GetString(),m_pInput[IN_METALNESS]->GetNodeName().GetString(),
					m_pInput[IN_TRANSPARENCY]->GetNodeName().GetString(),m_pInput[IN_K]->GetNodeName().GetString(),
					VSShaderStringFactory::ms_WorldPos,VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),UseLookUpTable(),ShadowStringArray[i],OutString);

			}

		}
		OutString += _T(";\n");
	}

	GetSRGBWriteString(OutString);

	VSString NodeStringA = VSRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_A);
	OutString += NodeStringA + _T(" = ") + m_pInput[IN_ALPHA]->GetNodeName().GetString();
	OutString += _T(";\n");
	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return 1;

}
void VSStraussShaderFunction::SetGlobleValue(unsigned int uiOutPutStringType , VSVShader * pVShader , VSPShader * pPShader)
{
	if (uiOutPutStringType == OST_MATERIAL)
	{
		if (pPShader && UseLookUpTable())
		{
			VSTexAllState * pTex = (VSTexAllState *)VSTexAllState::GetStraussFLookUpTable();
			static VSUsedName PSStraussFLookUpTableSampler = VSShaderStringFactory::ms_PSStraussFLookUpTableSampler;
			pPShader->SetParam(PSStraussFLookUpTableSampler,pTex);
			
			pTex = (VSTexAllState *)VSTexAllState::GetStraussSLookUpTable();
			static VSUsedName PSStraussSLookUpTableSampler = VSShaderStringFactory::ms_PSStraussSLookUpTableSampler;
			pPShader->SetParam(PSStraussSLookUpTableSampler,pTex);
		}
	}
}
VSStraussShaderFunction::~VSStraussShaderFunction()
{

}
VSStraussShaderFunction::VSStraussShaderFunction()
{

}