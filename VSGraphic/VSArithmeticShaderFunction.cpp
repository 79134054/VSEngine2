#include "VSArithmeticShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;

IMPLEMENT_RTTI(VSMul,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSMul,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMul)
IMPLEMENT_INITIAL_END
VSMul::VSMul(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("MulInputA") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
	
	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("MulInputB") + InputID;
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("MulOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
VSMul::VSMul()
{

}
VSMul::~VSMul()
{

}
bool VSMul::ResetValueType()
{
	unsigned int MaxType = VSPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);

	return true;
}
void VSMul::ResetInShaderName()
{
	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("MulInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("MulInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("MulOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSMul::GetFuntionString(VSString &OutString)const
{

	OutString += VSRenderer::ms_pRenderer->Mul(this);

	return 1;
}


IMPLEMENT_RTTI(VSDot,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSDot,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDot)
IMPLEMENT_INITIAL_END
VSDot::VSDot(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("DotInputA") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DotInputB") + InputID;
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;



	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("DotOutPut") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
VSDot::VSDot()
{

}
VSDot::~VSDot()
{

}
void VSDot::ResetInShaderName()
{

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("DotInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DotInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("DotOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSDot::GetFuntionString(VSString &OutString)const
{

	OutString += VSRenderer::ms_pRenderer->Dot(this);

	return 1;
}
bool VSDot::ResetValueType()
{
	unsigned int MaxType = VSPutNode::VT_2;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}

	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	return true;
}

IMPLEMENT_RTTI(VSSub,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSSub,VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSub)
IMPLEMENT_INITIAL_END
VSSub::VSSub(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("SubInputA") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("SubInputB") + InputID;
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("SubOutPut") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

}
VSSub::VSSub()
{

}
VSSub::~VSSub()
{

}
void VSSub::ResetInShaderName()
{

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("SubInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("SubInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("SubOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSSub::GetFuntionString(VSString &OutString)const
{

	OutString += VSRenderer::ms_pRenderer->Sub(this);

	return 1;
}
bool VSSub::ResetValueType()
{
	unsigned int MaxType = VSPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);
	return true;
}

IMPLEMENT_RTTI(VSAdd, VSShaderFunction)
BEGIN_ADD_PROPERTY(VSAdd, VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAdd)
IMPLEMENT_INITIAL_END
VSAdd::VSAdd(const VSUsedName & ShowName, VSMaterial * pMaterial)
:VSShaderFunction(ShowName, pMaterial)
{


	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("AddInputA") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("AddInputB") + InputID;
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("AddOutPut") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

}
VSAdd::VSAdd()
{

}
VSAdd::~VSAdd()
{

}
void VSAdd::ResetInShaderName()
{

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("AddInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("AddInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("AddOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSAdd::GetFuntionString(VSString &OutString)const
{

	OutString += VSRenderer::ms_pRenderer->Add(this);

	return 1;
}
bool VSAdd::ResetValueType()
{
	unsigned int MaxType = VSPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);
	return true;
}
IMPLEMENT_RTTI(VSMakeValue, VSShaderFunction)
BEGIN_ADD_PROPERTY(VSMakeValue, VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMakeValue)
IMPLEMENT_INITIAL_END
VSMakeValue::VSMakeValue(const VSUsedName & ShowName, VSMaterial * pMaterial, unsigned int uiInputNum)
:VSShaderFunction(ShowName, pMaterial)
{

	if (uiInputNum >= 1)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputA") + InputID;
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (uiInputNum >= 2)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputB") + InputID;
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (uiInputNum >= 3)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputC") + InputID;
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (uiInputNum >= 4)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputD") + InputID;
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}
	

	

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("OutPut") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;


}
VSMakeValue::VSMakeValue()
{

}
VSMakeValue::~VSMakeValue()
{

}
void VSMakeValue::ResetInShaderName()
{
	if (m_pInput.GetNum() >= 1)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputA") + InputID;
		m_pInput[IN_A]->SetNodeName(InputName);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (m_pInput.GetNum() >= 2)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputB") + InputID;
		m_pInput[IN_B]->SetNodeName(InputName);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (m_pInput.GetNum() >= 3)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputC") + InputID;
		m_pInput[IN_B]->SetNodeName(InputName);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}
	if (m_pInput.GetNum() >= 4)
	{
		VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
		VSString InputName = _T("InputD") + InputID;
		m_pInput[IN_B]->SetNodeName(InputName);
		VSShaderStringFactory::ms_ShaderValueIndex++;
	}
	

	

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("OutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSMakeValue::GetFuntionString(VSString &OutString)const
{
	OutString += VSRenderer::ms_pRenderer->MakeValue(this);
	return 1;
}
bool VSMakeValue::ResetValueType()
{
	unsigned int MaxType = 0;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			m_pInput[i]->SetValueType(m_pInput[i]->GetOutputLink()->GetValueType());
		}
		else
		{
			m_pInput[i]->SetValueType(VSPutNode::VT_1);
		}
		MaxType += m_pInput[i]->GetValueType() + 1;
	}

	m_pOutput[0]->SetValueType(MaxType - 1);
	return true;
}
IMPLEMENT_RTTI(VSDiv, VSShaderFunction)
BEGIN_ADD_PROPERTY(VSDiv, VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDiv)
IMPLEMENT_INITIAL_END
VSDiv::VSDiv(const VSUsedName & ShowName, VSMaterial * pMaterial)
:VSShaderFunction(ShowName, pMaterial)
{

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("DivInputA") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DivInputB") + InputID;
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;



	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("DivOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
VSDiv::VSDiv()
{

}
VSDiv::~VSDiv()
{

}
bool VSDiv::ResetValueType()
{
	unsigned int MaxType = VSPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);

	return true;
}
void VSDiv::ResetInShaderName()
{
	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("DivInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DivInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("DivOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSDiv::GetFuntionString(VSString &OutString)const
{

	OutString += VSRenderer::ms_pRenderer->Div(this);
	return 1;
}


IMPLEMENT_RTTI(VSSaturate, VSShaderFunction)
BEGIN_ADD_PROPERTY(VSSaturate, VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSaturate)
IMPLEMENT_INITIAL_END
VSSaturate::VSSaturate(const VSUsedName & ShowName, VSMaterial * pMaterial)
:VSShaderFunction(ShowName, pMaterial)
{

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("Input") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;



	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("Output") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
VSSaturate::VSSaturate()
{

}
VSSaturate::~VSSaturate()
{

}
bool VSSaturate::ResetValueType()
{
	unsigned int MaxType = VSPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);

	return true;
}
void VSSaturate::ResetInShaderName()
{
	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("Input") + InputID;
	m_pInput[IN_VALUE]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("Output") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
}
bool VSSaturate::GetFuntionString(VSString &OutString)const
{

	OutString += VSRenderer::ms_pRenderer->Saturate(this);
	return 1;
}