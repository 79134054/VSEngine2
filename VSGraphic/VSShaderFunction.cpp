#include "VSShaderFunction.h"
#include "VSMaterial.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSLightShaderFunction.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSShaderFunction,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSShaderFunction,VSObject)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pOwner,Owner,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pInput,Input,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pOutput,Output,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
VSShaderFunction::VSShaderFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_bIsVisited = 0;
	m_pOwner = NULL;
}
VSShaderFunction::VSShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial)
{
	m_bIsVisited = 0;
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	VSMAC_ASSERT(pMaterial);
	m_pOwner = pMaterial;
	m_pOwner->AddShaderFuntion(this);
}
VSShaderFunction::~VSShaderFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		VSMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		VSMAC_DELETE(m_pOutput[i]);
	}
	m_pOwner->DeleteShaderFuntion(this);
}
bool VSShaderFunction::GetInputValueString(VSString &OutString)const
{
	if(!VSRenderer::ms_pRenderer)
		return 0;
	VSString Temp;
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		
		if(m_pInput[i]->GetValueType() == VSPutNode::VT_1)
		{
			OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
			Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
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
		{
			VSMAC_ASSERT(0);
		}
		if(!m_pInput[i]->GetOutputLink())
		{
			OutString += m_pInput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
			continue;
		}
// 		if(m_pInput[i]->GetValueType() != m_pInput[i]->GetOutputLink()->GetValueType())
// 			return 0;
		OutString += GetValueEqualString(m_pInput[i]->GetOutputLink(),m_pInput[i]);/*m_pInput[i]->GetNodeName() + _T(" = ") + m_pInput[i]->GetOutputLink()->GetNodeName() + _T(";\n");*/
	}
	return 1;
}
VSString VSShaderFunction::GetValueEqualString(const VSOutputNode * pOutPutNode,const VSInputNode * pInputNode)const
{
	if(!pInputNode || !pOutPutNode)
		return VSString();
	unsigned int uiMaxElement = pInputNode->GetValueType();
	if(uiMaxElement == pOutPutNode->GetValueType())
	{
		
		return pInputNode->GetNodeName().GetString() + _T(" = ") + pOutPutNode->GetNodeName().GetString() + _T(";\n");
	}
	
	VSString OutString;
	OutString = pInputNode->GetNodeName().GetString() + _T(" = ");
	VSString Value[4];
	unsigned int Mask[4];
	Mask[0] = VSRenderer::VE_R;
	Mask[1] = VSRenderer::VE_G;
	Mask[2] = VSRenderer::VE_B;
	Mask[3] = VSRenderer::VE_A;
	for(unsigned int i = 0 ; i < 4 ; i ++)
	{
		if ( i > pOutPutNode->GetValueType())
		{
			Value[i] = VSRenderer::ms_pRenderer->GetValueElement(pOutPutNode,Mask[pOutPutNode->GetValueType()]);
		}
		else
		{
			Value[i] = VSRenderer::ms_pRenderer->GetValueElement(pOutPutNode,Mask[i]);
		}

	}
	if (pInputNode->GetValueType() == VSPutNode::VT_1)
	{
		OutString +=VSRenderer::ms_pRenderer->FloatConst(Value[0]);
	}
	else if (pInputNode->GetValueType() == VSPutNode::VT_2)
	{
		OutString +=VSRenderer::ms_pRenderer->Float2Const(Value[0],Value[1]);
	}
	else if (pInputNode->GetValueType() == VSPutNode::VT_3)
	{
		OutString +=VSRenderer::ms_pRenderer->Float3Const(Value[0],Value[1],Value[2]);
	}
	else if (pInputNode->GetValueType() == VSPutNode::VT_4)
	{
		OutString +=VSRenderer::ms_pRenderer->Float4Const(Value[0],Value[1],Value[2],Value[3]);
	}
	else
		return VSString();
	OutString += _T(";\n");
	return OutString;
}
bool VSShaderFunction::GetOutPutValueString(VSString &OutString)const
{
	if(!VSRenderer::ms_pRenderer)
		return 0;
	VSString Temp;
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{

		if(m_pOutput[i]->GetValueType() == VSPutNode::VT_1)
		{
			OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
			Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
		}
		else if(m_pOutput[i]->GetValueType() == VSPutNode::VT_2)
		{
			OutString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
			Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
		}
		else if(m_pOutput[i]->GetValueType() == VSPutNode::VT_3)
		{
			OutString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
			Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
		}
		else if(m_pOutput[i]->GetValueType() == VSPutNode::VT_4)
		{
			OutString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
			Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
		}
		else
			return 0;

		OutString += m_pOutput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	return 1;
}
bool VSShaderFunction::GetShaderTreeString(VSString &OutString)
{
	if(m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;
		for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
		{
			if(m_pInput[i]->GetOutputLink() == NULL)
				continue;
			else
			{
				((VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
			
			}
		
		}
		if (!ResetValueType())
		{
			return 0;
		}
		if(!GetInputValueString(OutString))
			return 0;
		if(!GetOutPutValueString(OutString))
			return 0;
		if(!GetFuntionString(OutString))
			return 0;
		return 1;
	}
}
bool VSShaderFunction::ClearShaderTreeStringFlag()
{
	if(m_bIsVisited == 0)
		return 1;
	else
	{
		m_bIsVisited = 0;
		for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
		{
			if(m_pInput[i]->GetOutputLink() == NULL)
				continue;
			else
			{
				((VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->ClearShaderTreeStringFlag();

			}

		}
		return 1;
	}
}
VSInputNode * VSShaderFunction::GetInputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];
}
VSInputNode * VSShaderFunction::GetInputNode(const VSString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

VSOutputNode * VSShaderFunction::GetOutputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pOutput.GetNum())
		return NULL;
	return m_pOutput[uiNodeID];
}
VSOutputNode * VSShaderFunction::GetOutputNode(const VSString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}
bool VSShaderFunction::IsValidNodeToThis(VSShaderFunction * pShaderFunction)
{
	if (pShaderFunction == this)
	{
		return false;
	}
	return true;
}
bool VSShaderFunction::GetAllChildNode(VSArray<VSShaderFunction *> & ChildNodeArray)
{

	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			bool bFind = false;
			for (unsigned int j = 0 ; j < ChildNodeArray.GetNum() ; j++)
			{
				if (ChildNodeArray[j] == pOwner)
				{
					bFind = true;
				}
			}
			if(!bFind)
			{
				ChildNodeArray.AddElement(pOwner);
				pOwner->GetAllChildNode(ChildNodeArray);
			}
		}
	}
	return ChildNodeArray.GetNum() > 0;
}
bool VSShaderFunction::CheckChildNodeValidToThis(VSArray<VSShaderFunction *> & NoValidShaderFunctionArray)
{
	VSArray<VSShaderFunction *>  ChildNodeArray;
	if (GetAllChildNode(ChildNodeArray))
	{
		for (unsigned int i = 0 ;  i < ChildNodeArray.GetNum() ; i++)
		{
			if (!IsValidNodeToThis(ChildNodeArray[i]))
			{
				NoValidShaderFunctionArray.AddElement(ChildNodeArray[i]);
			}
		}
	}

	return NoValidShaderFunctionArray.GetNum() == 0;
}
bool VSShaderFunction::CheckChildNodeValidAll(VSMap<VSShaderFunction *,VSArray<VSShaderFunction *>> & NoValidMap)
{
	VSArray<VSShaderFunction *>  NoValidShaderFunctionArray;
	if (!CheckChildNodeValidToThis(NoValidShaderFunctionArray))
	{
		NoValidMap.AddElement(this,NoValidShaderFunctionArray);
	}
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			pOwner->CheckChildNodeValidAll(NoValidMap);
		}
	}

	return NoValidMap.GetNum() == 0;
}
bool VSShaderFunction::HaveThisChild(VSShaderFunction * pShaderFunction)
{
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pShaderFunction == pOwner)
			{
				return true;
			}
			else
			{
				if (pOwner->HaveThisChild(pShaderFunction) == true)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void VSShaderFunction::GetNoLightFunctionParentNode(VSArray<VSShaderFunction *> & NoLightFunctionParentNodeArray)
{
	VSArray<VSShaderFunction *> ChildNodeArray;
	GetAllChildNode(ChildNodeArray);

	bool bHaveLightNode = false;
	for (unsigned int i = 0 ; i < ChildNodeArray.GetNum() ; i++)
	{
		VSShaderFunction * pShaderFunction = ChildNodeArray[i];
		if (pShaderFunction->GetType().IsSameType(VSLightDir::ms_Type) || pShaderFunction->GetType().IsSameType(VSLightColor::ms_Type) || pShaderFunction->GetType().IsSameType(VSLightSpecular::ms_Type))
		{
			bHaveLightNode = true;
			break;
		}
		
	}
	if (!bHaveLightNode)
	{
		if (GetType().IsSameType(VSLightDir::ms_Type) || GetType().IsSameType(VSLightColor::ms_Type) || GetType().IsSameType(VSLightSpecular::ms_Type))
		{
			return;
		}	
		else
		{
			NoLightFunctionParentNodeArray.AddElement(this);
		}
		return ;
	}
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pOwner)
			{
				pOwner->GetNoLightFunctionParentNode(NoLightFunctionParentNodeArray);

			}
		}
	}
}
bool VSShaderFunction::ResetValueType()
{
	return true;
}
