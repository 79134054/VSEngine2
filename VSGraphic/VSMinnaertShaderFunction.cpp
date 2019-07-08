#include "VSMinnaertShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMinnaertShaderFunction,VSShaderMainFunction)
BEGIN_ADD_PROPERTY(VSMinnaertShaderFunction,VSShaderMainFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMinnaertShaderFunction)
IMPLEMENT_INITIAL_END
VSMinnaertShaderFunction::VSMinnaertShaderFunction(const VSUsedName &ShowName,VSMaterial * pMaterial)
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

	InputName = _T("Power");
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
bool VSMinnaertShaderFunction::GetFuntionString(VSString &OutString)const
{
	GetAlphaTestString(OutString);
	if(!m_pInput[IN_DIFFUSE_COLOR]->GetOutputLink())
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


				VSRenderer::ms_pRenderer->MinnaertDirectionalLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_POWER]->GetNodeName().GetString(),VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),ShadowStringArray[i],OutString);


			}
			else if(i == VSLight::LT_POINT)
			{


				VSRenderer::ms_pRenderer->MinnaertPointLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_POWER]->GetNodeName().GetString(),VSShaderStringFactory::ms_WorldPos,VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),ShadowStringArray[i],OutString);

			}
			else if(i == VSLight::LT_SPOT)
			{

				VSRenderer::ms_pRenderer->MinnaertSpotLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_POWER]->GetNodeName().GetString(),VSShaderStringFactory::ms_WorldPos,VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),ShadowStringArray[i],OutString);

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
VSMinnaertShaderFunction::~VSMinnaertShaderFunction()
{

}
VSMinnaertShaderFunction::VSMinnaertShaderFunction()
{

}