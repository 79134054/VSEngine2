#include "VSCameraShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;

IMPLEMENT_RTTI(VSViewWorldDir,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSViewWorldDir,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSViewWorldDir)
IMPLEMENT_INITIAL_END
VSViewWorldDir::VSViewWorldDir(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ViewWorldDirOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
VSViewWorldDir::VSViewWorldDir()
{

}
VSViewWorldDir::~VSViewWorldDir()
{

}
void VSViewWorldDir::ResetInShaderName()
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ViewWorldDirOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSViewWorldDir::GetFuntionString(VSString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_ViewWorldDir + _T(";\n");

	return 1;
}


IMPLEMENT_RTTI(VSCameraWorldPos,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSCameraWorldPos,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCameraWorldPos)
IMPLEMENT_INITIAL_END
VSCameraWorldPos::VSCameraWorldPos(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("CameraWorldPosOutPut") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
VSCameraWorldPos::VSCameraWorldPos()
{

}
VSCameraWorldPos::~VSCameraWorldPos()
{

}
void VSCameraWorldPos::ResetInShaderName()
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("CameraWorldPosOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSCameraWorldPos::GetFuntionString(VSString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_CameraWorldPos + _T(";\n");

	return 1;
}

