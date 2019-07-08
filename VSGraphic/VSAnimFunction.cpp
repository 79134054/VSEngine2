#include "VSAnimFunction.h"
#include "VSBoneNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSAnimFunction,VSAnimBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSAnimFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSAnimFunction,VSAnimBaseFunction)
END_ADD_PROPERTY
VSAnimFunction::VSAnimFunction()
{
	m_BoneOutPut.Clear();

}
VSAnimFunction::VSAnimFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBaseFunction(ShowName,pAnimTree)
{
	
}
bool VSAnimFunction::SetObject(VSObject * pObject)
{
	if(VSAnimBaseFunction::SetObject(pObject))
	{
		const VSSkelectonMeshNode* pMesh = GetSkelectonMeshNode(); 
		VSMAC_ASSERT(pMesh);

		VSSkelecton * pSkelecton = pMesh->GetSkelecton();
		VSMAC_ASSERT(pSkelecton);

		unsigned int BoneNum = pSkelecton->GetBoneNum();
		m_BoneOutPut.SetBufferNum(BoneNum);
		for(unsigned int i = 0 ; i < m_BoneOutPut.GetNum() ; i++)
		{
			m_BoneOutPut[i].FromTransfrom(pSkelecton->GetBoneNode(i)->GetLocalTransform());

		}
		return true;
	}

	
	return false;
}
VSAnimFunction::~VSAnimFunction()
{
	m_BoneOutPut.Clear();
}

bool VSAnimFunction::Update(double dAppTime)
{
	if (!VSAnimBaseFunction::Update(dAppTime))
		return false;

	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction)
			{
				pAnimBaseFunction->Update(dAppTime);
			}
		}
	}
	return true;
}
VSAnimAtom& VSAnimFunction::GetRootTranfform()
{
	return m_RootAtom;
}
