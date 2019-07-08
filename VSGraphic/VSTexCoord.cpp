#include "VSTexCoord.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTexCoord, VSShaderFunction)
BEGIN_ADD_PROPERTY(VSTexCoord, VSShaderFunction)
REGISTER_PROPERTY(m_uiTexCoordIndex, exCoordIndex, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTexCoord)
IMPLEMENT_INITIAL_END
VSTexCoord::VSTexCoord(const VSUsedName & ShowName, VSMaterial * pMaterial)
:VSShaderFunction(ShowName, pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("TexCoordOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_2, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
	m_uiTexCoordIndex = 0;
}
VSTexCoord::VSTexCoord()
{
	m_uiTexCoordIndex = 0;
}
VSTexCoord::~VSTexCoord()
{

}
void VSTexCoord::ResetInShaderName()
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("TexCoordOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSTexCoord::GetFuntionString(VSString &OutString)const
{
	if (VSShaderStringFactory::ms_PSTextureInputCoordValue[m_uiTexCoordIndex].GetLength() > 0)
	{
		OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_PSTextureInputCoordValue[m_uiTexCoordIndex] + _T(";\n");
	}
	return 1;
}
