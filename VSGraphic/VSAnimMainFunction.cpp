#include "VSAnimMainFunction.h"
#include "VSBoneNode.h"
#include "VSAnimFunction.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSAnimMainFunction,VSAnimBaseFunction)
BEGIN_ADD_PROPERTY(VSAnimMainFunction,VSAnimBaseFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimMainFunction)
IMPLEMENT_INITIAL_END
VSAnimMainFunction::VSAnimMainFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBaseFunction(ShowName,pAnimTree)
{
	VSString InputName = _T("Anim");
	VSInputNode * pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("IK");
	pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_IK,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
}
VSAnimMainFunction::VSAnimMainFunction()
{

}
VSAnimMainFunction::~VSAnimMainFunction()
{
	
}
bool VSAnimMainFunction::Update(double dAppTime)
{
	if (!VSAnimBaseFunction::Update(dAppTime))
		return false;

	const VSSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	VSMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return false;
	}
	VSSkelecton * pSkelecton = pMesh->GetSkelecton();
	VSMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return false;
	}
	if(m_pInput[0]->GetOutputLink())
	{
		VSAnimFunction *pAnimFunction = (VSAnimFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
		if(pAnimFunction)
		{
			pAnimFunction->Update(dAppTime);
			for(unsigned int i = 0 ; i < pSkelecton->GetBoneNum() ; i++)
			{
				VSBoneNode * pBone = pSkelecton->GetBoneNode(i);
				if(pBone)
				{
					VSMatrix3X3W Mat;
					pAnimFunction->m_BoneOutPut[i].GetMatrix(Mat);
					pBone->SetLocalMat(Mat);
				}
			}
			VSSpatial * pMeshComponent = pMesh->GetParent();
			if (pMeshComponent)
			{

				VSMatrix3X3W RootMatrix;
				pAnimFunction->m_RootAtom.GetMatrix(RootMatrix);
				pMeshComponent->SetLocalMat(RootMatrix);

			}
		}
	}
	
	if(m_pInput[1]->GetOutputLink())
	{
		VSAnimFunction *pAnimFunction = (VSAnimFunction *)m_pInput[1]->GetOutputLink()->GetOwner();
		if(pAnimFunction)
		{
			pAnimFunction->Update(dAppTime);
			
		}
	}
	
	return true;

}
// void VSAnimMainFunction::CreateNode(const VSString & NodeName,unsigned int uiNodeType)
// {
// 	if (uiNodeType > VSPutNode::AVT_ANIM && uiNodeType < VSPutNode::AVT_MAX)
// 	{
// 		VSString InputName = NodeName;
// 		VSInputNode * pInputNode = NULL;
// 
// 		pInputNode = VS_NEW VSInputNode(uiNodeType,InputName,this);
// 		VSMAC_ASSERT(pInputNode);
// 		m_pInput.AddElement(pInputNode);
// 	}
// 
// }