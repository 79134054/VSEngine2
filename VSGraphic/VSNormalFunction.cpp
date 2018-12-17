#include "VSNormalFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSWorldNormal,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSWorldNormal,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSWorldNormal)
IMPLEMENT_INITIAL_END
VSWorldNormal::VSWorldNormal(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("WorldNormalOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_3,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_R);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameR, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameG = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_G);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameG, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameB = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_B);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameB, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
VSWorldNormal::VSWorldNormal()
{

}
VSWorldNormal::~VSWorldNormal()
{

}
void VSWorldNormal::ResetInShaderName()
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("WorldNormalOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	VSString OutputNameG = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	VSString OutputNameB = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool VSWorldNormal::GetFuntionString(VSString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_WorldNormal + _T(";\n");

	return 1;
}
bool VSWorldNormal::GetOutPutValueString(VSString &OutString)const
{
	VSString Temp;
	if (m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString += VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}

IMPLEMENT_RTTI(VSViewNormal,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSViewNormal,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSViewNormal)
IMPLEMENT_INITIAL_END
VSViewNormal::VSViewNormal(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ViewNormalOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_R);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameR, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameG = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_G);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameG, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameB = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_B);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameB, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
VSViewNormal::VSViewNormal()
{

}
VSViewNormal::~VSViewNormal()
{

}
void VSViewNormal::ResetInShaderName()
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ViewNormalOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	VSString OutputNameG = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	VSString OutputNameB = VSRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), VSRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool VSViewNormal::GetFuntionString(VSString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_ViewNormal + _T(";\n");

	return 1;
}
bool VSViewNormal::GetOutPutValueString(VSString &OutString)const
{
	VSString Temp;
	if (m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString += VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}