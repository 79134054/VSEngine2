#include "VSCustomShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSNormalFunction.h"
#include "VSLightShaderFunction.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCustomShaderFunction,VSShaderMainFunction)
BEGIN_ADD_PROPERTY(VSCustomShaderFunction,VSShaderMainFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCustomShaderFunction)
IMPLEMENT_INITIAL_END
VSCustomShaderFunction::VSCustomShaderFunction(const VSUsedName &ShowName,VSMaterial * pMaterial)
:VSShaderMainFunction(ShowName,pMaterial)
{

	VSString InputName = _T("EmissiveColor");
	VSInputNode * pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
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

	InputName = _T("Custom");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Diffuse");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
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
bool VSCustomShaderFunction::GetFuntionString(VSString &OutString)const
{

	GetAlphaTestString(OutString);
	if(!m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") + VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));

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


				VSRenderer::ms_pRenderer->CustomDirectionalLight(iLightNum[i],m_CustomDefine,ShadowStringArray[i],OutString);


			}
			else if(i == VSLight::LT_POINT)
			{


				VSRenderer::ms_pRenderer->CustomPointLight(iLightNum[i],m_CustomDefine,VSShaderStringFactory::ms_WorldPos,ShadowStringArray[i],OutString);

			}
			else if(i == VSLight::LT_SPOT)
			{

				VSRenderer::ms_pRenderer->CustomSpotLight(iLightNum[i],m_CustomDefine,VSShaderStringFactory::ms_WorldPos,ShadowStringArray[i],OutString);

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
VSCustomShaderFunction::~VSCustomShaderFunction()
{

}
VSCustomShaderFunction::VSCustomShaderFunction()
{
	
}
bool VSCustomShaderFunction::IsValidNodeToThis(VSShaderFunction * pShaderFunction)
{
	if(pShaderFunction->GetType().IsSameType(VSWorldNormal::ms_Type) || pShaderFunction->GetType().IsSameType(VSViewNormal::ms_Type))
	{
		if(GetNormalNode()->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)GetNormalNode()->GetOutputLink()->GetOwner();

			if (pOwner->HaveThisChild(pShaderFunction) == true)
			{
				return false;
			}
		}
	}

	if (pShaderFunction->GetType().IsSameType(VSLightDir::ms_Type) || pShaderFunction->GetType().IsSameType(VSLightColor::ms_Type) || pShaderFunction->GetType().IsSameType(VSLightSpecular::ms_Type))
	{
		if(m_pInput[IN_CUSTOM]->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner();

			if (pOwner->HaveThisChild(pShaderFunction) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}
void VSCustomShaderFunction::CreatLightFunctionString(unsigned int uiOutPutStringType)
{
	if (uiOutPutStringType != OST_MATERIAL)
	{
		return ;
	}
	VSString CustomFuntionString;
	VSArray<VSShaderFunction *>  NoLightFunctionParentNodeArray;
	m_LightFunctionString.Clear();
	m_CustomContentString.Clear();
	m_CustomDefine.Clear();
	if (m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		
		((VSShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner())->GetNoLightFunctionParentNode(NoLightFunctionParentNodeArray);

		for (unsigned int i = 0 ; i < NoLightFunctionParentNodeArray.GetNum() ; i++)
		{
			NoLightFunctionParentNodeArray[i]->GetShaderTreeString(m_CustomContentString);
		}
		

		((VSShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner())->GetShaderTreeString(CustomFuntionString);
	}
	
	unsigned int uiCustomValueType = m_pInput[IN_CUSTOM]->GetValueType();
	VSString Temp;
	if(uiCustomValueType == VSPutNode::VT_4)
	{
		CustomFuntionString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
	}
	if(!m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		CustomFuntionString += m_pInput[IN_CUSTOM]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		CustomFuntionString += GetValueEqualString(m_pInput[IN_CUSTOM]->GetOutputLink(),m_pInput[IN_CUSTOM]);
	}

	VSString CustomDeclareString;
	for (unsigned int i = 0 ; i < NoLightFunctionParentNodeArray.GetNum() ; i++)
	{
		VSString NodeName = NoLightFunctionParentNodeArray[i]->GetOutputNode(0)->GetNodeName().GetString();
		unsigned int VTType = NoLightFunctionParentNodeArray[i]->GetOutputNode(0)->GetValueType();

		VSString TypeString;
		if(VTType == VSPutNode::VT_1)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		}
		else if(VTType == VSPutNode::VT_2)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;

		}
		else if(VTType == VSPutNode::VT_3)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/

		}
		else if(VTType == VSPutNode::VT_4)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/

		}
		CustomDeclareString += TypeString + NodeName + _T(","); 
		m_CustomDefine += NodeName + _T(","); 

	}
	VSString DirectionLightString;
	DirectionLightString = VSRenderer::ms_pRenderer->Float4() + _T(" CustomDirectionLightFun(") + CustomDeclareString + 
							VSRenderer::ms_pRenderer->Float4() + VSShaderStringFactory::ms_LightColor + _T(",") +
							VSRenderer::ms_pRenderer->Float4() + VSShaderStringFactory::ms_LightSpecular + _T(",") +
							VSRenderer::ms_pRenderer->Float3() + VSShaderStringFactory::ms_LightDir + _T(")");
	DirectionLightString += _T("\n{\n") + CustomFuntionString + VSRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + _T(";}\n");

	VSString PointLightString;
	PointLightString = VSRenderer::ms_pRenderer->Float4() + _T(" CustomPointLightFun(") + CustomDeclareString + 
							VSRenderer::ms_pRenderer->Float3() + VSShaderStringFactory::ms_WorldPos + _T(",") +
							VSRenderer::ms_pRenderer->Float4() + VSShaderStringFactory::ms_LightColor + _T(",") +
							VSRenderer::ms_pRenderer->Float4() + VSShaderStringFactory::ms_LightSpecular + _T(",") +
							VSRenderer::ms_pRenderer->Float() +_T("Range") + _T(",") +
							VSRenderer::ms_pRenderer->Float3() + _T("LightWorldPos") + _T(")");

	VSString  PointLightUseString ;
	VSString  PointAttenuationDivString ;
	VSRenderer::ms_pRenderer->CustomPointLightUseString(PointLightUseString,PointAttenuationDivString);
	PointLightString += _T("\n{\n") + PointLightUseString + CustomFuntionString + VSRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + PointAttenuationDivString + _T(";}\n");



	VSString SpotLightString;
	SpotLightString = VSRenderer::ms_pRenderer->Float4() + _T(" CustomSpotLightFun(") + CustomDeclareString + 
						VSRenderer::ms_pRenderer->Float3() + VSShaderStringFactory::ms_WorldPos + _T(",") +
						VSRenderer::ms_pRenderer->Float4() + VSShaderStringFactory::ms_LightColor + _T(",") +
						VSRenderer::ms_pRenderer->Float4() + VSShaderStringFactory::ms_LightSpecular + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Range") + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Phi") + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Theta") + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Falloff") + _T(",") +
						VSRenderer::ms_pRenderer->Float3() +_T("LightWorldPos") + _T(",") +
						VSRenderer::ms_pRenderer->Float3() + _T("LightWorldDirection") + _T(")");
	VSString  SpotLightUseString ;
	VSString  SpotAttenuationDivString ;
	VSRenderer::ms_pRenderer->CustomSpotLightUseString(SpotLightUseString,SpotAttenuationDivString);
	SpotLightString += _T("\n{\n") + SpotLightUseString + CustomFuntionString + VSRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + SpotAttenuationDivString + _T(";}\n");

	m_LightFunctionString = DirectionLightString + PointLightString + SpotLightString;
	
}
bool VSCustomShaderFunction::GetInputValueString(VSString &OutString,unsigned int uiOutPutStringType)const
{
	if(!VSRenderer::ms_pRenderer)
		return 0;
	VSString Temp;
	if (uiOutPutStringType == OST_MATERIAL)
	{
		for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
		{

			if (m_pInput[i] == GetNormalNode() || i == IN_CUSTOM)
			{
				continue;
			}
			if(m_pInput[i]->GetValueType() == VSPutNode::VT_1)
			{
				OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
				if (m_pInput[i] == GetAlphaNode())
				{
					Temp = VSRenderer::ms_pRenderer->FloatConst(_T("1"));
				}
				else
				{
					Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
				}

			}
			else if(m_pInput[i]->GetValueType() == VSPutNode::VT_2)
			{
				OutString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
				Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
			}
			else if(m_pInput[i]->GetValueType() == VSPutNode::VT_3)
			{
				OutString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
				Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
			}
			else if(m_pInput[i]->GetValueType() == VSPutNode::VT_4)
			{
				OutString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
				Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
			}
			else
				return 0;
			if(!m_pInput[i]->GetOutputLink())
			{
				OutString += m_pInput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
				continue;
			}
			// 		if(m_pInput[i]->GetValueType() != m_pInput[i]->GetOutputLink()->GetValueType())
			// 			return 0;
			OutString += GetValueEqualString(m_pInput[i]->GetOutputLink(),m_pInput[i]);/*m_pInput[i]->GetNodeName() + _T(" = ") + m_pInput[i]->GetOutputLink()->GetNodeName() + _T(";\n");*/
		}
	}
	else if (uiOutPutStringType == OST_INDIRECT)
	{

		GetEmissiveInputValueString(OutString);
		GetAlphaInputValueString(OutString);
		GetDiffuseInputValueString(OutString);
	}
	else if (uiOutPutStringType == OST_NORMAL_DEPTH)
	{
		GetAlphaInputValueString(OutString);
		GetReflectMipInputValueString(OutString);
		GetReflectPowInputValueString(OutString);
	}
	else if (uiOutPutStringType == OST_CUB_SHADOW || uiOutPutStringType == OST_SHADOW || uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
	{
		GetAlphaInputValueString(OutString);
	}
	return 1;
}
bool VSCustomShaderFunction::GetShaderTreeString(VSString &OutString,unsigned int uiOutPutStringType)
{
	if(m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;

		
		if (uiOutPutStringType == OST_MATERIAL)
		{
			GetValueUseDeclareString(OutString, VUS_ALL);
			// first get normal string
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);

			GetAlphaString(OutString);
			
			GetEmissiveString(OutString);
			if (m_pInput[IN_CUSTOM]->GetOutputLink())
			{
				OutString += m_CustomContentString;
			}
		}
		else if (uiOutPutStringType == OST_INDIRECT)
		{
			GetValueUseDeclareString(OutString, VUS_ALL);
			// first get normal string
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);

			GetAlphaString(OutString);

			GetEmissiveString(OutString);
			GetDiffuseString(OutString);
		}
		else if (uiOutPutStringType == OST_NORMAL_DEPTH)
		{

			GetValueUseDeclareString(OutString, VUS_ALL);
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);
			GetAlphaString(OutString);
			GetReflectMipString(OutString);
			GetReflectPowString(OutString);
		}
		else if (uiOutPutStringType == OST_CUB_SHADOW || uiOutPutStringType == OST_SHADOW || uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL);
			GetAlphaString(OutString);
		}

		if(!GetInputValueString(OutString,uiOutPutStringType))
			return 0;
		if(!GetOutPutValueString(OutString))
			return 0;
		if (uiOutPutStringType == OST_MATERIAL)
		{
			if(!GetFuntionString(OutString))
				return 0;
		}
		else if (uiOutPutStringType == OST_INDIRECT)
		{
			if (!GetIndirectRenderString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_NORMAL_DEPTH)
		{
			if (!GetNormalDepthString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_CUB_SHADOW)
		{
			if (!GetCubShadowString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_SHADOW)
		{
			if (!GetShadowString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
		{
			if (!GetSDualParaboloidhadowString(OutString))
			{
				return 0;
			}
		}
		return 1;
	}
}