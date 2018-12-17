#include "VSDx11Renderer.h"
#include "VS2DTexSampler.h"
#include "VS1DTexSampler.h"
#include "VSGeometry.h"
#include "VSString.h"
#include "VSResourceManager.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSOrenNayarShaderFunction.h"
#include "VSStraussShaderFunction.h"
#include "VSCookTorranceShaderFunction.h"
#include "VSIsotropicWardShaderFunction.h"
#include "VSPointLight.h"
#include "VSDirectionLight.h"
#include "VSArithmeticShaderFunction.h"
#include "VSSpotLight.h"
#include "VSColorBuffer.h"
using namespace VSEngine2;
VSString VSDX11Renderer::TexColorBuffer(const VSColorBuffer * pColorBuffer) const
{
	if (!pColorBuffer)
		return VSString();
	return pColorBuffer->GetOutputNode(VSColorBuffer::OUT_COLOR)->GetNodeName().GetString()
		+ _T(" = ") + VSShaderStringFactory::ms_PSColorBufferSampler + _T("Texture") + _T(".Sample(") + VSShaderStringFactory::ms_PSColorBufferSampler + _T("Sampler") + _T(", ") +
		pColorBuffer->GetInputNode(VSColorBuffer::IN_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Tex2D(const VS2DTexSampler * p2DTexSampler) const
{
	VSMAC_ASSERT(p2DTexSampler);
	if (!p2DTexSampler)
		return VSString();
	return p2DTexSampler->GetOutputNode(VS2DTexSampler::OUT_COLOR)->GetNodeName().GetString()
		+ _T(" = ") + p2DTexSampler->GetShowName().GetString() + _T("Texture") + 
		_T(".Sample(") + p2DTexSampler->GetShowName().GetString() + _T("Sampler") + _T(", ") +
			p2DTexSampler->GetInputNode(VS2DTexSampler::IN_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Tex1D(const VS1DTexSampler * p1DTexSampler) const
{
	VSMAC_ASSERT(p1DTexSampler);
	if (!p1DTexSampler)
		return VSString();
	return p1DTexSampler->GetOutputNode(VS2DTexSampler::OUT_COLOR)->GetNodeName().GetString()
		+ _T(" = ") + p1DTexSampler->GetShowName().GetString() + _T("Texture") +
		_T(".Sample(") + p1DTexSampler->GetShowName().GetString() + _T("Sampler") + _T(", ") +
		p1DTexSampler->GetInputNode(VS2DTexSampler::IN_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Mul(const VSMul * pMul) const
{
	return pMul->GetOutputNode(VSMul::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pMul->GetInputNode(VSMul::IN_A)->GetNodeName().GetString() + _T(" * ") + 
		pMul->GetInputNode(VSMul::IN_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Saturate(const VSSaturate * pSaturate) const
{
	return pSaturate->GetOutputNode(VSDiv::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = saturate(") + pSaturate->GetInputNode(VSSaturate::IN_VALUE)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Div(const VSDiv * pDiv) const
{
	return pDiv->GetOutputNode(VSDiv::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pDiv->GetInputNode(VSDiv::IN_A)->GetNodeName().GetString() + _T(" / ") +
		pDiv->GetInputNode(VSDiv::IN_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Sub(const VSSub * pSub) const
{
	return pSub->GetOutputNode(VSSub::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pSub->GetInputNode(VSSub::IN_A)->GetNodeName().GetString() + _T(" - ") + 
		pSub->GetInputNode(VSSub::IN_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Add(const VSAdd * pAdd) const
{
	return pAdd->GetOutputNode(VSSub::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pAdd->GetInputNode(VSSub::IN_A)->GetNodeName().GetString() + _T(" + ") +
		pAdd->GetInputNode(VSSub::IN_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Dot(const VSDot * pDot) const
{
	return pDot->GetOutputNode(VSDot::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = dot(") + pDot->GetInputNode(VSDot::IN_A)->GetNodeName().GetString() + _T(",") + 
		pDot->GetInputNode(VSDot::IN_B)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::MakeValue(const VSMakeValue * pMakeValue)const
{
	VSString Temp;
	if (pMakeValue->GetOutputNode(VSMakeValue::OUT_VALUE)->GetValueType() == VSPutNode::VT_1)
	{
		Temp = _T("float(");
	}
	else if (pMakeValue->GetOutputNode(VSMakeValue::OUT_VALUE)->GetValueType() == VSPutNode::VT_2)
	{
		Temp = _T("float2(");		
	}
	else if (pMakeValue->GetOutputNode(VSMakeValue::OUT_VALUE)->GetValueType() == VSPutNode::VT_3)
	{
		Temp = _T("float3(");
	}
	else if (pMakeValue->GetOutputNode(VSMakeValue::OUT_VALUE)->GetValueType() == VSPutNode::VT_4)
	{
		Temp = _T("float4(");
	}
	for (unsigned int i = 0; i < pMakeValue->GetInputNodeNum(); i++)
	{
		Temp += pMakeValue->GetInputNode(i)->GetNodeName().GetString();
		if (i == pMakeValue->GetInputNodeNum() - 1)
		{
			Temp += _T(")");
		}
		else
		{
			Temp += _T(",");
		}
	}


	return pMakeValue->GetOutputNode(VSDot::OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + Temp +  _T(";\n");
}
VSString VSDX11Renderer::Texture(unsigned int uiType)const
{
	if (uiType == VSTexture::TT_1D)
	{
		return VSString(_T("Texture1D "));
	}
	else if (uiType == VSTexture::TT_2D)
	{
		return VSString(_T("Texture2D "));
	}
	else if (uiType == VSTexture::TT_3D)
	{
		return VSString(_T("Texture3D "));
	}
	else if (uiType == VSTexture::TT_CUBE)
	{
		return VSString(_T("TextureCube "));
	}
	return VSString(_T("Texture2D "));
}
VSString VSDX11Renderer::Sampler(unsigned int uiType)const
{


	return VSString(_T("SamplerState "));

}
VSString VSDX11Renderer::Float()const
{
	return VSString(_T("float "));
}
VSString VSDX11Renderer::Float2()const
{
	return VSString(_T("float2 "));
}
VSString VSDX11Renderer::Float3()const
{
	return VSString(_T("float3 "));
}
VSString VSDX11Renderer::Float4()const
{
	return VSString(_T("float4 "));
}
VSString VSDX11Renderer::Return()const
{
	return VSString(_T("return "));
}
VSString VSDX11Renderer::FloatConst(const VSString & Value1)const
{
	return Value1;
}
VSString VSDX11Renderer::Float2Const(const VSString &Value1,const VSString &Value2)const
{
	return VSString(_T("float2")) + _T("(") + Value1 + _T(",") + Value2 + _T(")");
}
VSString VSDX11Renderer::Float3Const(const VSString & Value1,
							 const VSString& Value2,const VSString &Value3)const
{
	return VSString(_T("float3")) + _T("(") + Value1 + _T(",") + Value2 + _T(",") + Value3 + _T(")");
}
VSString VSDX11Renderer::Float4Const(const VSString &Value1,const VSString &Value2,
							 const VSString &Value3,const VSString &Value4)const
{
	return VSString(_T("float4")) + _T("(") + Value1 + _T(",") + Value2 + _T(",") + Value3 + _T(",") + Value4 + _T(")");
}
void VSDX11Renderer::CustomPointLightUseString(VSString & PointLightUseString ,VSString & PointAttenuationDivString)
{
	PointLightUseString = _T("float3 ") + VSShaderStringFactory::ms_LightDir + _T(" = WorldPos - LightWorldPos;\n")
		+ _T("float  fDistance = length(") + VSShaderStringFactory::ms_LightDir + _T(");\n")
		+ VSShaderStringFactory::ms_LightDir + _T(" = ") + VSShaderStringFactory::ms_LightDir + _T(" / fDistance;\n") 
		+ _T("float fLightAttenuationDiv = saturate(1.0f - fDistance / Range);\n");
	PointAttenuationDivString = _T("* fLightAttenuationDiv");
}
void VSDX11Renderer::CustomSpotLightUseString(VSString & SpotLightUseString ,VSString & SpotAttenuationDivString)
{
	SpotLightUseString = _T("float3 ") + VSShaderStringFactory::ms_LightDir + _T(" = WorldPos - LightWorldPos;\n")
		+ _T("float  fDistance = length(") + VSShaderStringFactory::ms_LightDir + _T(");\n")
		+ VSShaderStringFactory::ms_LightDir + _T(" = ") + VSShaderStringFactory::ms_LightDir + _T(" / fDistance;\n")  
		+ _T("float fLightAttenuationDiv = saturate(1.0f - fDistance / Range);\n")
		+ _T("float fSpotLightCos = dot(") + VSShaderStringFactory::ms_LightDir + _T(",LightWorldDirection);\n")
		+ _T("float fLightIf = saturate((fSpotLightCos - cos(Phi/ 2)) / (cos(Theta/ 2) - cos(Phi / 2)));\n")
		+ _T("float fLightEffect = pow(fLightIf,Falloff);\n");

	SpotAttenuationDivString = _T(" * fLightAttenuationDiv  * fLightEffect");
}
VSString VSDX11Renderer::GetValueElement(const VSString & InputString,
	unsigned char uiVE)const
{
	VSString Temp = InputString;
	if (uiVE > 0)
	{
		VSString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		unsigned int Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp += _T(".");

		for (unsigned int i = 0; i < 4; i++)
		{
			if (uiVE & Mask[i])
			{
				Temp += Value[i];
			}
		}

	}


	return Temp;
}
VSString VSDX11Renderer::GetValueElement(const VSPutNode * pPutNode,
								 unsigned char uiVE)const
{
	VSMAC_ASSERT(pPutNode);
	if (!pPutNode)
		return VSString::ms_StringNULL;
	VSString Temp = pPutNode->GetNodeName().GetString();
	if (uiVE > 0)
	{
		if (pPutNode->GetValueType() == VSPutNode::VT_1 && (uiVE & VE_R))
		{
			return Temp;
		}
		else if(pPutNode->GetValueType() == VSPutNode::VT_1)
		{
			VSMAC_ASSERT(0);
			return VSString::ms_StringNULL;
		}
		VSString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		unsigned int Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp +=  _T(".");
		
		for (unsigned int i = 0 ; i < 4 ; i++)
		{
			if ( i <= pPutNode->GetValueType())
			{
				if (uiVE & Mask[i])
				{
					Temp += Value[i];
				}
			}
			
		}

	}

	
	return Temp;
}
VSString VSDX11Renderer::GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const
{
	VSString OutString;
	if (uiValueType == VSPutNode::VT_1)
	{
		OutString += _T("float ");
	}
	else if (uiValueType == VSPutNode::VT_2)
	{
		OutString += _T("float2 ");
	}
	else if (uiValueType == VSPutNode::VT_3)
	{
		OutString += _T("float3 ");
	}
	else if (uiValueType == VSPutNode::VT_4)
	{
		OutString += _T("float4 ");
	}
	OutString += ShowName + _T(";\n");
	return OutString;
}
VSString VSDX11Renderer::GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const
{
	VSString OutString = Sampler(SamplerType) + _T(" ");
	OutString += ShowName + _T("Sampler")
		+ SetRegister(RT_S, uiRegisterIndex) + _T(";\n");
	OutString += Texture(SamplerType) + _T(" ") + ShowName + _T("Texture")
		+ SetRegister(RT_T, uiRegisterIndex) + _T(";\n");
	return OutString;
}
VSString VSDX11Renderer::SetRegister(unsigned int uiRegisterType,unsigned int uiRegisterIndex)const
{

	VSString RegisterID = IntToString(uiRegisterIndex);
	if(uiRegisterType == RT_S)
		return _T(" : register(s") + RegisterID + _T(")");
	else if (uiRegisterType == RT_T)
		return _T(" : register(t") + RegisterID + _T(")");
	else
		VSMAC_ASSERT(1);
	return VSString();
}
void VSDX11Renderer::CreateVInputDeclarePosition(VSVertexBuffer * pVBuffer,
								 VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetPositionLevel(); i++)
	{	
		if(pVBuffer->HavePositionInfo(i))
		{
			VSString VertexID = IntToString(i);		
			OutString += _T("float3 Position") + VertexID + _T(":POSITION") + VertexID + _T(";\n");
		}

	}
}
void VSDX11Renderer::CreateVInputDeclareTexCoord(VSVertexBuffer * pVBuffer,
								 VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureID = IntToString(i);
			OutString += _T("float2 TexCoord") + TextureID+ _T(":TEXCOORD") + TextureID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareNormal(VSVertexBuffer * pVBuffer,
							   VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetNormalLevel(); i++)
	{
		if(pVBuffer->HaveNormalInfo(i))
		{
			VSString NomalID = IntToString(i);
			OutString += _T("float3 Normal") + NomalID + _T(":NORMAL") + NomalID + _T(";\n");
		}
	}

	if(pVBuffer->HaveTangentInfo())
	{
		OutString += _T("float4 Tangent : TANGENT;\n");
		if (pVBuffer->HaveBinormalInfo())
		{
			OutString += _T("float3 Binormal : BINORMAL;\n");
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareColor(VSVertexBuffer * pVBuffer,
							  VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(0))
		{
			VSString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareSkin(VSVertexBuffer * pVBuffer,
							 VSString & OutString)
{
	if(pVBuffer->HaveBlendWeightInfo())
	{
		OutString += _T("float4 BlendWeight :BLENDWEIGHT;\n");
	}

	if(pVBuffer->HaveBlendIndicesInfo())
	{
		OutString += _T("uint4 BlendIndices :BLENDINDICES;\n");
	}
}
void VSDX11Renderer::CreateVInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString)
{

	VSString TempDeclare;
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	CreateVInputDeclarePosition(pVBuffer,TempDeclare);
	CreateVInputDeclareTexCoord(pVBuffer,TempDeclare);
	CreateVInputDeclareNormal(pVBuffer,TempDeclare);
	CreateVInputDeclareColor(pVBuffer,TempDeclare);
	CreateVInputDeclareSkin(pVBuffer,TempDeclare);
	
	OutString += _T("struct VS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	
}
void VSDX11Renderer::CreateVOutputDeclarePosition(
								  VSString & OutString)
{
	OutString += _T("float4 Position:SV_POSITION;\n");
}
void VSDX11Renderer::CreateVOutputDeclareTexCoord(VSVertexBuffer * pVBuffer,unsigned int& ID,
								  VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{

		if(pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureID = IntToString(ID);
			OutString += _T("float2 TexCoord") + TextureID+ _T(":TEXCOORD") + TextureID + _T(";\n");
			ID++;
		}
	}
}
void VSDX11Renderer::CreateVOutputDeclareNormal(VSVertexBuffer * pVBuffer,unsigned int& ID,
								VSString & OutString)
{
	if(pVBuffer->HaveNormalInfo(0))
	{		
		VSString TextureID = IntToString(ID);

		OutString += _T("float3 Normal:TEXCOORD") + TextureID + _T(";\n");
		ID++;
	}




	if(pVBuffer->HaveTangentInfo())
	{
		{
			VSString TextureID = IntToString(ID);

			OutString += _T("float3 Tangent:TEXCOORD") + TextureID + _T(";\n");
			ID++;
		}
		{
			VSString TextureID = IntToString(ID);

			OutString += _T("float3 Binormal:TEXCOORD") + TextureID + _T(";\n");
			ID++;
		}
	}
}
void VSDX11Renderer::CreateVOutputDeclareColor(VSVertexBuffer * pVBuffer,
							   VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{
		if(pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVOutputDeclareLocalPos(unsigned int& ID,
								  VSString & OutString)
{
	VSString TextureID  = IntToString(ID);
	OutString += _T("float3 Pos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void VSDX11Renderer::CreateVOutputDeclareWorldPos(VSString & OutString)
{
	OutString += _T("float3 WorldPos:WPOS;\n");
}
void VSDX11Renderer::CreateVOutputDeclareProjectPos(unsigned int& ID,
												 VSString & OutString)
{
	VSString TextureID  = IntToString(ID);
	OutString += _T("float4 ProjectPos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void VSDX11Renderer::CreateVOutputDeclareProjectZ(unsigned int& ID,
								  VSString & OutString)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float ProjectZ:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void VSDX11Renderer::CreateVOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										 VSString & OutString)
{
	VSString TempDeclare;
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	unsigned int ID = 0;
	if (uiPassType != VSPass::PT_DIRECT_VOLUME_SHADOW && uiPassType != VSPass::PT_POINT_VOLUME_SHADOW)
	{
		CreateVOutputDeclarePosition(TempDeclare);
		CreateVOutputDeclareTexCoord(pVBuffer, ID, TempDeclare);
		CreateVOutputDeclareNormal(pVBuffer, ID, TempDeclare);
		CreateVOutputDeclareColor(pVBuffer, TempDeclare);
	}
	else
	{
		CreateVOutputDeclareWorldPos(TempDeclare);
	}
	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_NORMALDEPTH)
	{		
		CreateVOutputDeclareLocalPos(ID,TempDeclare);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVOutputDeclareLocalPos(ID, TempDeclare);
		CreateVOutputDeclareProjectZ(ID,TempDeclare);
	}
	OutString += _T("struct VS_OUTPUT \n{\n") + TempDeclare + _T("};\n");
}
void VSDX11Renderer::CreateUserConstantWorldViewProjectMatrix(VSShader * pShader,unsigned int& ID,
											  VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_WorldViewProjectMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_WorldViewProjectMatrix,NULL,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantLightShadowMatrix(VSShader * pShader,unsigned int& ID,
															 VSString & OutString)
{
	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_LightShadowMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_LightShadowMatrix, NULL, sizeof(VSREAL)* 16, 0, 4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}

void VSDX11Renderer::CreateUserConstantLightWorldPos(VSShader * pShader,unsigned int& ID,
									 VSString & OutString)
{
	OutString += _T("float3 ") + VSShaderStringFactory::ms_LightWorldPos + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_LightWorldPos,NULL,sizeof(VSREAL) * 3,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantLightDirection(VSShader * pShader,unsigned int& ID,
									  VSString & OutString)
{
	OutString += _T("float3 ") + VSShaderStringFactory::ms_LightWorldDirection + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_LightWorldDirection,NULL,sizeof(VSREAL) * 3,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantViewMatrix(VSShader * pShader,unsigned int& ID,
												 VSString & OutString)
{
	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_ViewMatrix +_T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_ViewMatrix,NULL,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
}
void VSDX11Renderer::CreateUserConstantProjectMatrix(VSShader * pShader, unsigned int& ID,
	VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_ProjectMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_ProjectMatrix, NULL, sizeof(VSREAL)* 16, 0, 4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantViewProjectMatrix(VSShader * pShader,unsigned int& ID,
															 VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_ViewProjectMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_ViewProjectMatrix,NULL,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantWorldViewMatrix(VSShader * pShader,unsigned int& ID,
									   VSString & OutString)
{
	VSString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_WorldViewMatrix +_T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_WorldViewMatrix,NULL,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
}

void VSDX11Renderer::CreateUserConstantSkin(VSGeometry * pGeometry,VSShader * pShader,unsigned int& ID,
							VSString & OutString)
{
	VSVertexBuffer * pVBuffer = pGeometry->GetMeshDate()->GetVertexBuffer();
	if(pGeometry->GetAffectBoneNum() && pVBuffer->GetBlendWeightDate() && pVBuffer->GetBlendIndicesDate() )
	{

		unsigned int uiBoneNum = VSResourceManager::GetGpuSkinBoneNum();
		VSString BoneNum = IntToString(uiBoneNum * 3);
		OutString += _T("float4 ") + VSShaderStringFactory::ms_BoneMatrix + _T("[") + BoneNum + _T("]")+ _T(";\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_BoneMatrix,NULL,sizeof(VSREAL) * 4 * uiBoneNum * 3,0,uiBoneNum * 3);
		pShader->m_pUserConstant.AddElement(pUserConstant);
	}
}
void VSDX11Renderer::CreateVUserConstant(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString)
{
	OutString += _T("cbuffer CB : register( b0 ) \n{\n");
	unsigned int uiRegisterID = 0;
	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_PREZ 
		|| uiPassType == VSPass::PT_NORMALDEPTH 
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		CreateUserConstantWorldViewProjectMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantViewProjectMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantLightDirection(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		CreateUserConstantLightShadowMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateUserConstantLightShadowMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantPointLightRange(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		//CreateUserConstantInvRTWidth(pVShader,uiRegisterID,OutString);
	}
	if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		CreateUserConstantWorldViewMatrix(pVShader,uiRegisterID,OutString);
	}
	OutString += _T("} \n");
	

}
void VSDX11Renderer::CreateVFuntionDualParaboloidPositionAndNormal(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	CreateVFuntionPositionAndNormal(MSPara,FuntionBody);
	FuntionBody += _T("float fLength = length(Out.Position.xyz);\n");
	FuntionBody += _T("Out.Position.xyz /= fLength;\n");
 	FuntionBody += _T("Out.Position.x /= Out.Position.z + 1.01f;\n");
 	FuntionBody += _T("Out.Position.y /= Out.Position.z + 1.01f;\n");
	FuntionBody += _T("Out.Position.z = saturate(fLength / ") + VSShaderStringFactory::ms_PointLightRange + _T(");\n");
	

}
void VSDX11Renderer::CreateVFuntionPositionAndNormal(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	FuntionBody +=_T("float3 Position;\n");


	if(pVBuffer->HaveNormalInfo(0))
	{
		FuntionBody +=_T("float3 Normal;\n");
	}

	if(pVBuffer->HaveTangentInfo())
	{
		FuntionBody +=_T("float4 Tangent;\n");
		FuntionBody +=_T("float3 Binormal;\n");
	}



	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo())
	{
		FuntionBody +=_T("float4 U = 0;\n");
		FuntionBody +=_T("float4 V = 0;\n");
		FuntionBody +=_T("float4 N = 0;\n");

		VSRenderer::ms_pRenderer->ComputeBoneVector("Input.BlendIndices","Input.BlendWeight",VSShaderStringFactory::ms_BoneMatrix
			,"U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

		FuntionBody += "Position = ";
		VSRenderer::ms_pRenderer->BoneTranPos("float4(Input.Position0,1)","U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");



		if(pVBuffer->HaveNormalInfo(0))
		{
			FuntionBody += "Normal = Input.Normal0;\n";
			if (pVBuffer->NormalDateType(0) == VSDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Normal = ";
				VSRenderer::ms_pRenderer->DecodeNormal1("Normal",FuntionBody);
				FuntionBody +=_T(";\n");
			}


		}	

		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody += "Tangent = Input.Tangent;\n";
			if (pVBuffer->TangentDateType() == VSDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Tangent = ";
				VSRenderer::ms_pRenderer->DecodeNormal1("Tangent",FuntionBody);
				FuntionBody +=_T(";\n");
			}
			if(pVBuffer->HaveBinormalInfo())
			{
				FuntionBody += "Binormal = Input.Binormal;\n";
				if (pVBuffer->BinormalDateType() == VSDataBuffer::DT_UBYTE4N)
				{
					FuntionBody += "Binormal = ";
					VSRenderer::ms_pRenderer->DecodeNormal1("Binormal",FuntionBody);
					FuntionBody +=_T(";\n");
				}

			}
			else
			{
				FuntionBody += "Binormal = ";
				VSRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"),_T("Tangent"),FuntionBody);
				FuntionBody +=_T(";\n");
			}

		}



		if(pVBuffer->HaveNormalInfo(0))
		{
			FuntionBody += "Normal = ";
			VSRenderer::ms_pRenderer->BoneTranNormal("Normal","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");
		}

		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody += "Tangent.xyz = ";
			VSRenderer::ms_pRenderer->BoneTranNormal("Tangent.xyz","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");

			FuntionBody += "Binormal = ";
			VSRenderer::ms_pRenderer->BoneTranNormal("Binormal","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");
		}

	}
	else
	{	
		FuntionBody +=_T("Position = Input.Position0;\n");


		if(pVBuffer->HaveNormalInfo(0))
		{	
			FuntionBody +=_T("Normal = Input.Normal0;\n");
			if (pVBuffer->NormalDateType(0) == VSDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Normal = ";
				VSRenderer::ms_pRenderer->DecodeNormal1("Normal",FuntionBody);
				FuntionBody +=_T(";\n");
			}
		}



		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody +=_T("Tangent = Input.Tangent;\n");
			if (pVBuffer->TangentDateType() == VSDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Tangent = ";
				VSRenderer::ms_pRenderer->DecodeNormal1("Tangent",FuntionBody);
				FuntionBody +=_T(";\n");
			}
			if(pVBuffer->HaveBinormalInfo())
			{
				FuntionBody +=_T("Binormal = Input.Binormal;\n");
			}
			else
			{
				FuntionBody += "Binormal = ";
				VSRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"),_T("Tangent"),FuntionBody);
				FuntionBody +=_T(";\n");
			}
		}



	}
	FuntionBody += _T("Out.Position = mul(float4(Position,1), ") +
		VSShaderStringFactory::ms_WorldViewProjectMatrix + _T(");\n");


	if(pVBuffer->HaveNormalInfo(0))
	{	
		FuntionBody +=_T("Out.Normal = Normal;\n");

	}


	if(pVBuffer->HaveTangentInfo())
	{
		FuntionBody +=_T("Out.Tangent = Tangent.xyz;\n");
		FuntionBody +=_T("Out.Binormal = Binormal;\n");
	}



	
}
void VSDX11Renderer::CreateVFuntionWorldPosition(MaterialShaderPara &MSPara,VSString & FuntionBody,unsigned int uiPass)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	FuntionBody +=_T("float3 Position;\n");






	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo())
	{
		FuntionBody +=_T("float4 U = 0;\n");
		FuntionBody +=_T("float4 V = 0;\n");
		FuntionBody +=_T("float4 N = 0;\n");

		VSRenderer::ms_pRenderer->ComputeBoneVector("Input.BlendIndices","Input.BlendWeight",VSShaderStringFactory::ms_BoneMatrix
			,"U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

		FuntionBody += "Position = ";
		VSRenderer::ms_pRenderer->BoneTranPos("float4(Input.Position0,1)","U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

	}
	else
	{	
		FuntionBody +=_T("Position = Input.Position0;\n");
	}
	
	FuntionBody += _T("Position = mul(float4(Position,1), ") +
		VSShaderStringFactory::ms_WorldMatrix + _T(").xyz;\n");

	FuntionBody += _T("Out.WorldPos = Position; \n");
}
void VSDX11Renderer::CreateVFuntionLocalPosition(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	FuntionBody +=_T("Out.Pos = Position;\n");	
}
void VSDX11Renderer::CreateVFuntionColor(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			FuntionBody += _T("Out.Color") + ColorID + _T(" = Input.Color") + ColorID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVFuntionTexCoord(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(0))
		{
			VSString TextureID = IntToString(i);
			FuntionBody += _T("Out.TexCoord") + TextureID + _T(" = Input.TexCoord") + TextureID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVFuntionPosition(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	FuntionBody +=_T("float3 Position;\n");

	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo())
	{
		FuntionBody +=_T("float4 U = 0;\n");
		FuntionBody +=_T("float4 V = 0;\n");
		FuntionBody +=_T("float4 N = 0;\n");

		VSRenderer::ms_pRenderer->ComputeBoneVector("Input.BlendIndices","Input.BlendWeight",VSShaderStringFactory::ms_BoneMatrix
			,"U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

		FuntionBody += "Position = ";
		VSRenderer::ms_pRenderer->BoneTranPos("float4(Input.Position0,1)","U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");
	}
	else
	{	
		FuntionBody +=_T("Position = Input.Position0;\n");
	}
	FuntionBody += _T("Out.Position = mul(float4(Position,1), ") +
		VSShaderStringFactory::ms_WorldViewProjectMatrix + _T(");\n");
}
void VSDX11Renderer::CreateVFuntionProjectZ(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	FuntionBody += _T("Out.ProjectZ = Out.Position.z / Out.Position.w;\n");
}
void VSDX11Renderer::CreateVFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
								   VSString & OutString)
{
	VSString FuntionBody;
	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_NORMALDEPTH)
	{
		CreateVFuntionPositionAndNormal(MSPara,FuntionBody);
		CreateVFuntionColor(MSPara,FuntionBody);
		CreateVFuntionTexCoord(MSPara,FuntionBody);
		CreateVFuntionLocalPosition(MSPara,FuntionBody);		
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW || uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateVFuntionWorldPosition(MSPara, FuntionBody, uiPassType);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVFuntionDualParaboloidPositionAndNormal(MSPara,FuntionBody);
		CreateVFuntionColor(MSPara,FuntionBody);
		CreateVFuntionTexCoord(MSPara,FuntionBody);
		CreateVFuntionLocalPosition(MSPara, FuntionBody);
		CreateVFuntionProjectZ(MSPara,FuntionBody);
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{	
		CreateVFuntionPosition(MSPara,FuntionBody);
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		CreateVFuntionPost(MSPara,FuntionBody);
	}
	OutString += _T("VS_OUTPUT ") + ms_VShaderProgramMain + _T("( VS_INPUT Input)\n{\nVS_OUTPUT Out = (VS_OUTPUT) 0; \n") + FuntionBody + _T("return Out;\n};\n");	
}
void VSDX11Renderer::CreateVFuntionPost(MaterialShaderPara &MSPara,VSString & FuntionBody)
{
	FuntionBody += _T("Out.Position = float4(Input.Position0.xy, 0, 1);\n");
	FuntionBody += _T("Out.TexCoord0.xy = 0.5 * (1 + Input.Position0.xy * float2(1,-1));\n");
}
void VSDX11Renderer::CreatePInputDeclareTexCoord(VSVertexBuffer * pVBuffer,unsigned int& ID,
								 VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureID = IntToString(ID);
			OutString += _T("float2 TexCoord") + TextureID+ _T(":TEXCOORD") + TextureID + _T(";\n");
			VSShaderStringFactory::ms_PSTextureInputCoordValue[i] = _T("ps_Input.TexCoord") + TextureID;
			ID++;
		}
	}
}
void VSDX11Renderer::CreatePInputDeclareNormal(VSVertexBuffer * pVBuffer,unsigned int& ID,
							   VSString & OutString)
{
	if(pVBuffer->HaveNormalInfo(0))
	{
		VSString TextureID = IntToString(ID);
		OutString += _T("float3 Normal:TEXCOORD") + TextureID + _T(";\n");
		VSShaderStringFactory::ms_PSInputLocalNormal = _T("ps_Input.Normal");
		ID++;
	}



	if(pVBuffer->HaveTangentInfo())
	{
		{
			VSString TextureID = IntToString(ID);
			OutString += _T("float3 Tangent:TEXCOORD") + TextureID + _T(";\n");
			VSShaderStringFactory::ms_PSInputLocalTangent = _T("ps_Input.Tangent");
			ID++;
		}

		{
			VSString TextureID = IntToString(ID);
			OutString += _T("float3 Binormal:TEXCOORD") + TextureID + _T(";\n");
			VSShaderStringFactory::ms_PSInputLocalBinormal = _T("ps_Input.Binormal");
			ID++;
		}
	}
}
void VSDX11Renderer::CreatePInputDeclareColor(VSVertexBuffer * pVBuffer,
							  VSString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
			VSShaderStringFactory::ms_PSColor[i] = _T("ps_Input.Color") + ColorID;
		}
	}
}

void VSDX11Renderer::CreatePInputDeclareLocalPos(unsigned int& ID,
								 VSString & OutString)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float3 Pos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
	VSShaderStringFactory::ms_PSInputLocalPos = _T("ps_Input.Pos");
}

void VSDX11Renderer::CreatePInputDeclareWorldPos(unsigned int& ID,
												VSString & OutString)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float3 WorldPos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
	VSShaderStringFactory::ms_PSInputWorldPos = _T("ps_Input.WorldPos");
}

void VSDX11Renderer::CreatePInputDeclareProjectPos(unsigned int& ID,
							  VSString & OutString)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float4 ProjectPos:TEXCOORD") + TextureID + _T(";\n");
	VSShaderStringFactory::ms_PSInputProjectPos = _T("ps_Input.ProjectPos");
	ID++;
}
void VSDX11Renderer::CreatePInputDeclareProjectZ(unsigned int& ID,
								 VSString & OutString)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float ProjectZ:TEXCOORD") + TextureID + _T(";\n");
	VSShaderStringFactory::ms_PSInputProjectZ = _T("ps_Input.ProjectZ");
	ID++;
}
void VSDX11Renderer::CreatePInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	VSString & OutString)
{
	VSString TempDeclare;
	
	unsigned int j = 0;
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	if (uiPassType == VSPass::PT_MATERIAL
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_NORMALDEPTH)
	{
		CreateVOutputDeclarePosition(TempDeclare);
		CreatePInputDeclareTexCoord(pVBuffer, j, TempDeclare);
		CreatePInputDeclareNormal(pVBuffer, j, TempDeclare);
		CreatePInputDeclareColor(pVBuffer, TempDeclare);
		CreatePInputDeclareLocalPos(j, TempDeclare);

		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW || uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateVOutputDeclarePosition(TempDeclare);
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVOutputDeclarePosition(TempDeclare);
		CreatePInputDeclareTexCoord(pVBuffer,j,TempDeclare);
		CreatePInputDeclareNormal(pVBuffer,j,TempDeclare);	
		CreatePInputDeclareColor(pVBuffer,TempDeclare);
		CreatePInputDeclareLocalPos(j, TempDeclare);
		CreatePInputDeclareProjectZ(j,TempDeclare);	
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{
		CreateVOutputDeclarePosition(TempDeclare);
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION || uiPassType == VSPass::PT_POSTEFFECT)
	{
		CreateVOutputDeclarePosition(TempDeclare);
		CreatePInputDeclareTexCoord(pVBuffer,j,TempDeclare);
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}

}
void VSDX11Renderer::CreatePOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										 VSString & OutString)
{

	VSString TempDeclare;
	TempDeclare += _T("float4 Color0:SV_Target0;");
	VSShaderStringFactory::ms_PSOutputColorValue = _T("Out.Color0");
	OutString += _T("struct PS_OUTPUT \n{ \n") + TempDeclare + _T("\n};\n");

}
void VSDX11Renderer::CreateUserConstantWorldMatrix(VSShader * pShader,unsigned int& ID,
												  VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + VSShaderStringFactory::ms_WorldMatrix +  _T(";\n");
	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_WorldMatrix,NULL,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

	
}
void VSDX11Renderer::CreateUserConstantCameraWorldPos(VSShader * pShader,unsigned int& ID,
													 VSString & OutString)
{
	OutString += _T("float3 ") + VSShaderStringFactory::ms_CameraWorldPos + _T(";\n");		 
	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_CameraWorldPos,NULL,sizeof(VSREAL) * 3,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantDirectionLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
													 VSString & OutString)
{
	if (uiLightNum > 0)
	{
		VSString TypeString;
		GetLightType(VSLight::LT_DIRECTION,TypeString);

		OutString += TypeString + _T(" ") + VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + IntToString(uiLightNum) + _T("];\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION],NULL,sizeof(VSREAL) * 84 * uiLightNum,0,21 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);

	}
	

}
void VSDX11Renderer::CreateUserConstantPointLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
												 VSString & OutString)
{
	if (uiLightNum > 0)
	{
		VSString TypeString;
		GetLightType(VSLight::LT_POINT,TypeString);

		OutString += TypeString + _T(" ") + VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + IntToString(uiLightNum) + _T("];\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_LightName[VSLight::LT_POINT],NULL,sizeof(VSREAL) * 52 * uiLightNum,0,13 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);

	}
}
void VSDX11Renderer::CreateUserConstantSpotLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
												VSString & OutString)
{
	if (uiLightNum > 0)
	{
		VSString TypeString;
		GetLightType(VSLight::LT_SPOT,TypeString);
		VSString RegisterID = IntToString(ID);
		OutString += TypeString + _T(" ") + VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + IntToString(uiLightNum) + _T("];\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT],NULL,sizeof(VSREAL) * 56 * uiLightNum,0,14 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);
		ID += pUserConstant->GetRegisterNum();
	}
}
void VSDX11Renderer::CreateUserConstantLightFuntion(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString)
{
	unsigned int uiLightFuntion = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->HaveLightFun())
		{
			OutString += Sampler(VSTexture::TT_2D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFuntion) +_T("Sampler")
				+ SetRegister(RT_S,ID) +_T(";\n");
			OutString += Texture(VSTexture::TT_2D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFuntion) + _T("Texture")
				+ SetRegister(RT_T, ID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFuntion),VSTexture::TT_2D,ID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiLightFuntion++;
			ID++;
		}
	}
}
void VSDX11Renderer::CreateUserConstantShadow(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString)
{

	unsigned int uiShadowNum = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			if (pLocalLight->GetLightType() == VSLight::LT_POINT)
			{
				if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_CUB)
				{
					OutString +=Sampler(VSTexture::TT_CUBE) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Sampler")
						+ SetRegister(RT_S,ID) +_T(";\n");
					OutString += Texture(VSTexture::TT_CUBE) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),VSTexture::TT_CUBE,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					
				}
				if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_VOLUME)
				{
					OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Sampler")
						+ SetRegister(RT_S,ID) +_T(";\n");

					OutString += Texture(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),VSTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;

				}
				if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_DUAL_PARABOLOID)
				{
					OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)  + _T("Sampler") + _T("[2]")
						+ SetRegister(RT_S,ID) +_T(";\n");

					OutString += Texture(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)  + _T("Texture")+ _T("[2]")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),VSTexture::TT_2D,ID,2);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;

					uiShadowNum++;
					ID++;

				}
			}
			else if (pLocalLight->GetLightType() == VSLight::LT_DIRECTION)
			{
				if(((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_CSM)
				{
					OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Sampler") + _T("[3]")
						+ SetRegister(RT_S,ID) +_T(";\n");

					OutString += Texture(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Texture") + _T("[3]")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),VSTexture::TT_2D,ID,3);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					uiShadowNum++;
					ID++;
					uiShadowNum++;
					ID++;
				}
				else if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_PROJECT)
				{

				}
				else
				{
					OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Sampler")
						+ SetRegister(RT_S,ID) +_T(";\n");

					OutString += Texture(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),VSTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
				}
				
			}
			else if (pLocalLight->GetLightType() == VSLight::LT_SPOT)
			{
				if (((VSSpotLight*)pLocalLight)->GetShadowType() == VSSpotLight::ST_NORMAL)
				{
					OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Sampler")
						+ SetRegister(RT_S,ID) +_T(";\n");

					OutString += Texture(VSTexture::TT_2D) + _T(" ");
					OutString += VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),VSTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
				}
			}
			
		}
	}

}
void VSDX11Renderer::CreateGBufferSampler(VSPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, VSString & OutString)
{
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (pMaterial->GetPostEffectShaderFunction())
	{
		OutString += Sampler(VSTexture::TT_2D) + _T(" ");
		OutString += VSShaderStringFactory::ms_PSColorBufferSampler + _T("Sampler")
			+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");

		OutString += Texture(VSTexture::TT_2D) + _T(" ");
		OutString += VSShaderStringFactory::ms_PSColorBufferSampler + _T("Texture")
			+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

		VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSColorBufferSampler, VSTexture::TT_2D, uiTexRegisterID, 1);
		pPShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;

		OutString += Sampler(VSTexture::TT_2D) + _T(" ");
		OutString += VSShaderStringFactory::ms_PSDepthNormalBufferSampler + _T("Sampler")
			+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");

		OutString += Texture(VSTexture::TT_2D) + _T(" ");
		OutString += VSShaderStringFactory::ms_PSDepthNormalBufferSampler + _T("Texture")
			+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

		pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSDepthNormalBufferSampler, VSTexture::TT_2D, uiTexRegisterID, 1);
		pPShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;
	}
}
void VSDX11Renderer::CreateLightModleLookUpTable(VSPShader* pPShader,MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID,VSString & OutString)
{
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_OREN_NAYAR)
	{
		VSOrenNayarShaderFunction * pOrenNayarShaderFunction = (VSOrenNayarShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pOrenNayarShaderFunction->UseLookUpTable())
		{
			OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T("Sampler")
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");

			OutString += Texture(VSTexture::TT_2D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler,VSTexture::TT_2D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
	else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_STRAUSS)
	{
		VSStraussShaderFunction * pStraussShaderFunction = (VSStraussShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pStraussShaderFunction->UseLookUpTable())
		{
			OutString +=Sampler(VSTexture::TT_1D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Sampler")
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");

			OutString += Texture(VSTexture::TT_1D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSStraussFLookUpTableSampler,VSTexture::TT_1D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;

			OutString +=Sampler(VSTexture::TT_1D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Sampler")
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");

			OutString += Texture(VSTexture::TT_1D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSStraussSLookUpTableSampler,VSTexture::TT_1D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
	else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_COOKTOORANCE)
	{
		VSCookTorranceShaderFunction * pCookTorranceShaderFunction = (VSCookTorranceShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pCookTorranceShaderFunction->GetSpecularType() == VSCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			OutString +=Sampler(VSTexture::TT_2D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Sampler")
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");
			OutString += Texture(VSTexture::TT_2D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler,VSTexture::TT_2D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
	else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_ISOTROPICWARD)
	{
		VSIsotropicWardShaderFunction * pIsotropicWardShaderFunction = (VSIsotropicWardShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pIsotropicWardShaderFunction->UseLookUpTable())
		{
			OutString +=Sampler(VSTexture::TT_1D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Sampler")
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");
			OutString += Texture(VSTexture::TT_1D) + _T(" ");
			OutString += VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler,VSTexture::TT_1D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;


		}
	}
}
void VSDX11Renderer::CreateUserConstantFarZ(VSShader * pShader,unsigned int& uiRegisterID,
							VSString & OutString)
{

	OutString += _T("float ") + VSShaderStringFactory::ms_FarZ +_T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_FarZ,NULL,sizeof(VSREAL) * 1,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantSkyLightUpColor(VSShader * pShader,unsigned int& uiRegisterID,
									   VSString & OutString)
{
	VSString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float4 ") + VSShaderStringFactory::ms_SkyLightUpColor + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_SkyLightUpColor,NULL,sizeof(VSREAL) * 4,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantSkyLightDownColor(VSShader * pShader,unsigned int& uiRegisterID,
										 VSString & OutString)
{

	OutString += _T("float4 ") + VSShaderStringFactory::ms_SkyLightDownColor + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_SkyLightDownColor,NULL,sizeof(VSREAL) * 4,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantPointLightRange(VSShader * pShader,unsigned int& uiRegisterID,
									   VSString & OutString)
{

	OutString += _T("float ") + VSShaderStringFactory::ms_PointLightRange + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_PointLightRange,NULL,sizeof(VSREAL) * 1,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantInvRTWidth(VSShader * pShader,unsigned int& uiRegisterID,
							   VSString & OutString)
{

	OutString += _T("float ") + VSShaderStringFactory::ms_InvRTWidth + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_InvRTWidth,NULL,sizeof(VSREAL) * 1,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantLight(VSShader * pShader,MaterialShaderPara &MSPara,unsigned int& uiRegisterID,
							 VSString & OutString)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;

	int iLightNum[VSLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;
				}
			}
		}
	}
	
	CreateUserConstantPointLight(pShader,uiRegisterID,iLightNum[VSLight::LT_POINT],OutString);
	CreateUserConstantSpotLight(pShader,uiRegisterID,iLightNum[VSLight::LT_SPOT],OutString);
	CreateUserConstantDirectionLight(pShader,uiRegisterID,iLightNum[VSLight::LT_DIRECTION],OutString);
}
void VSDX11Renderer::CreatePUserConstant(VSPShader* pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										VSString & OutString)
{
	
	if (uiPassType == VSPass::PT_MATERIAL)
	{
		unsigned int uiRegisterID = 0;
		unsigned int uiLightNum = MSPara.LightArray.GetNum();
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantViewMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantFarZ(pPShader,uiRegisterID,OutString);
		if (uiLightNum)
		{			
			CreateUserConstantLight(pPShader,MSPara,uiRegisterID,OutString);	
		}
		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		CreateUserConstantShadow(pPShader,MSPara.LightArray,uiTexRegisterID,OutString);
		CreateUserConstantLightFuntion(pPShader,MSPara.LightArray,uiTexRegisterID,OutString);
		CreateLightModleLookUpTable(pPShader,MSPara,uiTexRegisterID,OutString);
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		unsigned int uiRegisterID = 0;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantViewMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantSkyLightUpColor(pPShader,uiRegisterID,OutString);
		CreateUserConstantSkyLightDownColor(pPShader,uiRegisterID,OutString);

		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantViewMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);
		CreateUserConstantFarZ(pPShader,uiRegisterID,OutString);
			
		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
		
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantPointLightRange(pPShader,uiRegisterID,OutString);
		

		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
		
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);


		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION
		|| uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		unsigned int uiRegisterID = 0;

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;

		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		unsigned int uiRegisterID = 0;

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantLightShadowMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);

		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;

		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_POSTEFFECT)
	{
		unsigned int uiRegisterID = 0;

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		CreateGBufferSampler(pPShader, MSPara, uiTexRegisterID, OutString);
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	
}
void VSDX11Renderer::CreatePFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
								   VSString & OutString)
{
	if (uiPassType == VSPass::PT_MATERIAL)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_MATERIAL,MSPara.uiPassId);
		
		VSString VSCustomDeclareString;
		pMaterial->GetCustomDeclareString(VSCustomDeclareString,MSPara.uiPassId);
		OutString += VSCustomDeclareString + _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_INDIRECT,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_NORMAL_DEPTH,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_CUB_SHADOW,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_SHADOW,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_DUAL_PARABOLOID_SHADOW,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		VSMAC_ASSERT(pMaterial->GetLightShaderFunction());
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,VSShaderMainFunction::OST_MAX,MSPara.uiPassId);
		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");		
	}
	else if (uiPassType == VSPass::PT_POSTEFFECT)
	{
		VSString FuntionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		VSMAC_ASSERT(pMaterial->GetPostEffectShaderFunction());
		pMaterial->GetShaderTreeString(FuntionBody, MSPara, VSShaderMainFunction::OST_MAX, MSPara.uiPassId);
		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");
	}
	else if (uiPassType == VSPass::PT_PREZ || uiPassType == VSPass::PT_POINT_VOLUME_SHADOW || uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		OutString = _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\n PS_OUTPUT Out = (PS_OUTPUT) 0;\nOut.Color0 = float4(0.0f,0.0f,0.0f,1.0f);\nreturn Out;\n};");
	}
}
void VSDX11Renderer::SkyLight(const VSString &WorldNormal,const VSString &UpColor,const VSString & DownColor,const VSString & DiffuseColor,VSString & OutString)
{
	OutString += _T(" + SkyLight(") + WorldNormal + _T(",") + UpColor + _T(",") + DownColor + _T(") * ") + DiffuseColor;
}
void VSDX11Renderer::CustomDirectionalLight(int iLightNum,const VSString &CustomString,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		//OutString += _T("+ float4(1.0f,1.0f,1.0f,1.0f)") + ShadowString[i];
		OutString += _T(" + CustomDirectionLightFun(") + CustomString
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void VSDX11Renderer::DirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							  const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + DirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
					+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
					+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
					+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
					+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}

void VSDX11Renderer::SchlickDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
									 const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + SchlickDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void VSDX11Renderer::IsotropicWardDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
										   const VSString &Roughness ,
										   const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + IsotropicWardDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",")
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + IsotropicWardDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",")
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::AnisotropicWardDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
											 const VSString &RoughnessX ,const VSString &RoughnessY ,
											 const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
											 VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + AnisotropicWardDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + RoughnessX + _T(",") + RoughnessY + _T(",")
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void VSDX11Renderer::CookTorranceDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular , 
										  const VSString &Roughness , const VSString &C , const VSString &R,
										  const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
										  VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (uiSpecularType != VSCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			OutString += _T(" + CookTorranceDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",") + C + _T(",") + R + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + CookTorranceDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",") + C + _T(",") + R + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::OrenNayarDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
									 const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + OrenNayarDirectionalLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + OrenNayarDirectionalLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}
		
	}
}
void VSDX11Renderer::MinnaertDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
									  const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									  VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);

		OutString += _T(" + MinnaertDirectionalLightFun(") + Diffuse + _T(",") + Power + _T(",") 
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];


	}
}
void VSDX11Renderer::StraussDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
									 const VSString &Metalness,const VSString &Transparency,const VSString &K,
									 const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
									 VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + StraussDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",")  
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + StraussDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Texture,")
				+ VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::ShirleyDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
									 const VSString &NU,const VSString &NV,const VSString &Rs,
									 const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									 VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);

		OutString += _T(" + ShirleyDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",")
			+ NU + _T(",") + NV + _T(",") + Rs + _T(",")
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	

	}
}
void VSDX11Renderer::PointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + PointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}
	
}
void VSDX11Renderer::CustomPointLight(int iLightNum, const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + CustomPointLightFun(") + CustomString
			+ WorldPos + _T(",")
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}
}
void VSDX11Renderer::SchlickPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + SchlickPointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}

}
void VSDX11Renderer::OrenNayarPointLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
										 const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										 VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + OrenNayarPointLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + OrenNayarPointLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,")
				+ VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + +_T("Sampler,") + VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}
	}
}
void VSDX11Renderer::MinnaertPointLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);

		OutString += _T(" + MinnaertPointLightFun(") + Diffuse + _T(",") + Power + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;

	}
}
void VSDX11Renderer::StraussPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
										   const VSString &Metalness,const VSString &Transparency,const VSString &K,
										   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + StraussPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
		}
		else
		{
			OutString += _T(" + StraussPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,")
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,")
				+ VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Texture,")
				+ VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}
	}
}
void VSDX11Renderer::IsotropicWardPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
									 const VSString &Roughness ,
									 const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
									 VSString & OutString)const
{

	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + IsotropicWardPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
		}
		else
		{
			OutString += _T(" + IsotropicWardPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,")
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}
	}
}
void VSDX11Renderer::AnisotropicWardPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
									   const VSString &RoughnessX ,const VSString &RoughnessY ,
									   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + AnisotropicWardPointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + RoughnessX + _T(",") + RoughnessY + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
	}
}
void VSDX11Renderer::CookTorrancePointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
									const VSString &Roughness , const VSString &C , const VSString &R,
									const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
									VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (uiSpecularType != VSCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			OutString += _T(" + CookTorrancePointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
		}
		else
		{
			OutString += _T(" + CookTorrancePointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,")
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}
	}
}
void VSDX11Renderer::ShirleyPointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
							   const VSString &NU,const VSString &NV,const VSString &Rs,
							   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);

		OutString += _T(" + ShirleyPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
			+ NU + _T(",") + NV + _T(",") + Rs + _T(",")
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
	
	}
}
void VSDX11Renderer::SpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
					   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
					   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + SpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void VSDX11Renderer::CustomSpotLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + CustomSpotLightFun(") + CustomString
			+ WorldPos + _T(",")
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void VSDX11Renderer::SchlickSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							  const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							  VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + SchlickSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,")  
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void VSDX11Renderer::OrenNayarSpotLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
								VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			VSString ID = IntToString(i);
			OutString += _T(" + OrenNayarSpotLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			VSString ID = IntToString(i);
			OutString += _T(" + OrenNayarSpotLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + +_T("Sampler,") + VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::MinnaertSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Power,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{

		VSString ID = IntToString(i);
		OutString += _T(" + MinnaertSpotLightFun(") + Diffuse + _T(",") + Power + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
	

	}
}
void VSDX11Renderer::StraussSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,const VSString &Smoothness,
							  const VSString &Metalness,const VSString &Transparency,const VSString &K,
							  const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
							  VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			VSString ID = IntToString(i);
			OutString += _T(" + StraussSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			VSString ID = IntToString(i);
			OutString += _T(" + StraussSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",")  
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T("Texture,")
				+ VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::IsotropicWardSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
									const VSString &Roughness ,
									const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
									VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			VSString ID = IntToString(i);
			OutString += _T(" + IsotropicWardSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			VSString ID = IntToString(i);
			OutString += _T(" + IsotropicWardSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::AnisotropicWardSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
									  const VSString &RoughnessX ,const VSString &RoughnessY ,
									  const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									  VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + AnisotropicWardSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + RoughnessX + _T(",")  + RoughnessY + _T(",")
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
	}

}
void VSDX11Renderer::CookTorranceSpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,
								   const VSString &Roughness , const VSString &C , const VSString &R,
								   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,unsigned int uiSpecularType,VSArray<VSString> ShadowString,
								   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (uiSpecularType != VSCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			VSString ID = IntToString(i);
			OutString += _T(" + CookTorranceSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			VSString ID = IntToString(i);
			OutString += _T(" + CookTorranceSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Sampler,") + VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::ShirleySpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular,
							  const VSString &NU,const VSString &NV,const VSString &Rs,
							  const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							  VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{

		VSString ID = IntToString(i);
		OutString += _T(" + ShirleySpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
			+ NU + _T(",") + NV + _T(",") + Rs + _T(",")
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		

	}
}
void VSDX11Renderer::LocalToWorldPos(const VSString & LocalPos,VSString &OutString)const
{
	OutString += _T("TransPos(") + LocalPos + _T(",") + 
					VSShaderStringFactory::ms_WorldMatrix + _T(")");
}
void VSDX11Renderer::WorldToViewPos(const VSString & WorldPos,VSString &OutString)const
{
	OutString += _T("TransPos(") + WorldPos + _T(",") + 
		VSShaderStringFactory::ms_ViewMatrix + _T(")");
}
void VSDX11Renderer::TransProjPos(const VSString & Pos,const VSString & Matrix,VSString &OutString)const
{
	OutString += _T("TransProjPos(") + Pos + _T(",") + Matrix + _T(")");
}
void VSDX11Renderer::LocalToViewNormal(const VSString & LocalNormal,VSString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") + 
		VSShaderStringFactory::ms_WorldViewMatrix + _T(")");
}
void VSDX11Renderer::LocalToWorldNormal(const VSString & LocalNormal,VSString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") + 
		VSShaderStringFactory::ms_WorldMatrix + _T(")");
}
void VSDX11Renderer::GetWorldViewDir(VSString &OutString)const
{
	OutString += _T("GetZDir(") + VSShaderStringFactory::ms_ViewMatrix + _T(")");
	
}
void VSDX11Renderer::WorldToViewNormal(const VSString & WorldNormal,VSString &OutString)const
{
	OutString += _T("TransNormal(") + WorldNormal + _T(",") + 
		VSShaderStringFactory::ms_ViewMatrix + _T(")");
}
void VSDX11Renderer::BumpNormal(const VSString &TexNormal,VSString &OutString)const
{
	OutString += _T("BumpNormal(") + VSShaderStringFactory::ms_PSInputLocalTangent + _T(",") 
				+ VSShaderStringFactory::ms_PSInputLocalBinormal + _T(",")
				+ VSShaderStringFactory::ms_PSInputLocalNormal + _T(",")
				+ TexNormal + _T(")");
}
void VSDX11Renderer::ComputeDir(const VSString & Orgin,const VSString & End,VSString &OutString)const
{
	OutString += _T("ComputeDir(") + Orgin + _T(",")
				+ End + _T(")");
}
void VSDX11Renderer::ComputeLength(const VSString & Orgin,const VSString & End,VSString &OutString)const
{
	OutString += _T("ComputeLength(") + Orgin + _T(",")
		+ End + _T(")");
}
void VSDX11Renderer::GetLightType(unsigned int i,VSString &OutString)const
{
	if (i == VSLight::LT_DIRECTION)
	{ 
		OutString = _T("DirLightType");
	}
	else if (i == VSLight::LT_POINT)
	{
		OutString = _T("PointLightType");
	}
	else if (i == VSLight::LT_SPOT)
	{
		OutString = _T("SpotLightType");
	}
}
void VSDX11Renderer::GetLightTypeTemp(VSString &OutString)const
{

	OutString = _T("LightTypeTemp");

}
void VSDX11Renderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,const VSString &BoneVector,
							  const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const
{
	OutString += _T("ComputeBoneVector(") + BoneIndex + _T(",")
		+ BoneWeight + _T(",") + BoneVector + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
void VSDX11Renderer::BoneTranPos( const VSString & LocalPos,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const
{
	OutString += _T("TransPos(") + LocalPos + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
 void VSDX11Renderer::BoneTranNormal( const VSString & LocalNormal,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
void VSDX11Renderer::DecodeNormal1(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("DecodeNormal1(") + Normal + _T(")");
}
void VSDX11Renderer::RecodeNormal1(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("RecodeNormal1(") + Normal + _T(")");
}
void VSDX11Renderer::DecodeNormal2(const VSString & Normal,const VSString & Tengent,VSString &OutString) const
{
	OutString += _T("DecodeNormal2(") + Normal + _T(",") + Tengent + _T(")");
}
void VSDX11Renderer::DecodeNormal3(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("DecodeNormal3(") + Normal + _T(")");
}
void VSDX11Renderer::RecodeNormal3(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("RecodeNormal3(") + Normal + _T(")");
}
void VSDX11Renderer::EncodeReflect(const VSString & Mip , const VSString & Pow , VSString& OutString)
{
	OutString += _T("EncodeReflect(") + Mip + _T(",") + Pow + _T(")");
}
void VSDX11Renderer::DecodeReflect(const VSString & Mip , const VSString & Pow , const VSString & Value,VSString& OutString)
{
	OutString += _T("DecodeReflect(") + Mip + _T(",") + Pow + _T(",") + Value + _T(")");
}
void VSDX11Renderer::GreaterZeroPow(const VSString & Value,VSREAL Exp,VSString &OutString) const
{
	VSString SExp = RealToString(Exp);
	OutString += _T("GreaterZeroPow(") + Value + _T(",") + SExp + _T(")");
}
void VSDX11Renderer::Reciprocal(const VSString & Value, VSString &OutString)const
{
	OutString += _T("1.0 / (") + Value + _T(")");
}
void VSDX11Renderer::Clip(const VSString & ClipValue,VSString &OutString)const
{
	OutString += _T("clip(") + ClipValue + _T(")");
}
void VSDX11Renderer::SaturatePow(const VSString & Value,VSREAL Exp,VSString &OutString) const
{
	VSString SExp = RealToString(Exp);
	OutString += _T("SaturatePow(") + Value + _T(",") + SExp + _T(")");
}
void VSDX11Renderer::Saturate(const VSString & Value,VSString &OutString)const
{
	OutString += _T("saturate(") + Value + _T(")");
}
void VSDX11Renderer::PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler , const VSString & ViewMatrix,
										 const VSString & WorldPos, VSString &OutString)const
{

	OutString += _T("PointLightCubShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + ViewMatrix + _T(",") + CubeShadowSampler + _T("Sampler,") + CubeShadowSampler + _T("Texture)");
	
}
void VSDX11Renderer::PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler ,
										 const VSString & WorldPos, VSString &OutString)const
{

	OutString += _T("PointLightCubShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + CubeShadowSampler + _T("Sampler,") + CubeShadowSampler + _T("Texture)");

}
void VSDX11Renderer::PointLightVolumeShadow(const VSString & PointLightName , const VSString & VolumeShadowSampler , 
									const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("PointLightVolumeShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + VolumeShadowSampler + _T("Sampler,") + VolumeShadowSampler + _T("Texture)");
}
void VSDX11Renderer::PointLightDualParaboloidShadow(const VSString & PointLightName , const VSString & ShadowSampler ,  
											const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("PointLightDualParaboloidShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + ShadowSampler + _T("Sampler,") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::DirectionLightVolumeShadow(const VSString & DirLightName , const VSString & VolumeShadowSampler , 
										const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("DirectionLightVolumeShadow(") + DirLightName + _T(",") + WorldPos + _T(",") + VolumeShadowSampler + _T("Sampler,") + VolumeShadowSampler + _T("Texture)");
}
void VSDX11Renderer::DirectionLightCSMShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
									 const VSString & WorldPos, const VSString & ViewPos,const VSString &FarZ,VSString& OutString)const
{
	OutString += _T("DirLightCSMShadow(") + DirLightName + _T(",") + WorldPos + _T(",") + ViewPos + _T(",") + FarZ + _T(",") + ShadowSampler + _T("Sampler,") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::DirectionLightShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
								  const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("DirLightShadow(") + DirLightName + _T(",") + WorldPos + _T(",") + ShadowSampler + _T("Sampler,") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::SpotLightShadow(const VSString & SpotLightName , const VSString & ShadowSampler , 
							 const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("SpotLightShadow(") + SpotLightName + _T(",") + WorldPos + _T(",") + ShadowSampler + _T("Sampler,") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::GetLightFunction(VSArray<VSLight *> LightArray,const VSString & WorldPos,VSString & OutString)const
{
	unsigned int iLightNum[VSLight::LT_MAX] = { 0 };
	unsigned int uiLightFunSampler = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{

		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight)
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{
					if (pLocalLight->HaveLightFun())
					{
						OutString += _T("float4 FunReslut = float4(0.0f,0.0f,0.0f,1.0f);\n");
						
						if (j == VSLight::LT_POINT)
						{
							OutString += _T("FunReslut = PointLightFunction(")
								+ VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightWorldPos.xyz,")
								+ WorldPos + _T(",")
								+ VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightFunParam,")
								+ VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFunSampler) + _T("Sampler,") 
								+ VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFunSampler) + _T("Texture") + _T(");\n");
						}
						else
						{
							OutString += _T("FunReslut = LightFunction(")
								+ VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].WVP,")
								+ WorldPos + _T(",")
								+ VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightFunParam,")
								+ VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFunSampler) + _T("Sampler,")
								+ VSShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFunSampler) + _T("Texture") + _T(");\n");
						}
						
						OutString += VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightDiffuse *= FunReslut;\n");
						OutString += VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightSpecular *= dot(FunReslut,float4(0.299,0.587,0.184,0));\n");
						uiLightFunSampler++;
					}
					
					iLightNum[j]++;
				}
			}
			
		}
	}
}
void VSDX11Renderer::TranLightToTemp(VSArray<VSLight *> LightArray,VSString & OutString)const
{
	unsigned int iLightNum[VSLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;

				}
			}
		}
	}
	VSString LightTypeTempString;
	GetLightTypeTemp(LightTypeTempString);
	for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
	{
		if (iLightNum[j] > 0)
		{
			OutString += LightTypeTempString + _T(" ") + VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("];\n"); 
		}
		
	}
	for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
	{

		for (unsigned int k = 0 ; k < iLightNum[j] ; k++)
		{
			OutString += VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(k) + _T("].LightDiffuse = ") + 
				VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(k) + _T("].LightDiffuse;\n");  

			OutString += VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(k) + _T("].LightSpecular = ") + 
				VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(k) + _T("].LightSpecular;\n");  
		}

		
	}
}
void VSDX11Renderer::CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	VSString & OutString){}
void VSDX11Renderer::CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	VSString & OutString){}
void VSDX11Renderer::CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
	VSString & OutString){}
void VSDX11Renderer::CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
	VSString & OutString){}
void VSDX11Renderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, VSGShader *pGShader){}