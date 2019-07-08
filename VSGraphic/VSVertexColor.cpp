#include "VSVertexColor.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSVertexColor,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSVertexColor,VSShaderFunction)
REGISTER_PROPERTY(m_uiColorIndex,ColorIndex,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVertexColor)
IMPLEMENT_INITIAL_END
VSVertexColor::VSVertexColor(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("VertexColorOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
	m_uiColorIndex = 0;
}
VSVertexColor::VSVertexColor()
{
	m_uiColorIndex = 0;
}
VSVertexColor::~VSVertexColor()
{

}
void VSVertexColor::ResetInShaderName()
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("VertexColorOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSVertexColor::GetFuntionString(VSString &OutString)const
{
	if (VSShaderStringFactory::ms_PSColor[m_uiColorIndex].GetLength() > 0)
	{
		OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_PSColor[m_uiColorIndex] + _T(";\n");
	}
	return 1;
}
