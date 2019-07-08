#include "VSSteer.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSSteerComponent,VSObject)
VSSteerComponent::VSSteerComponent()
{
	m_pSteer = NULL;
}
VSSteerComponent::~VSSteerComponent()
{
	m_pSteer = NULL;
}
IMPLEMENT_RTTI_NoCreateFun(VSSteer,VSObject)
VSSteer::VSSteer()
{
	m_pActor = NULL;
}
VSSteer::~VSSteer()
{
	m_pActor = NULL;
	m_ComponentArray.Clear();
}
VSVector3 VSSteer::Compute()
{
	VSVector3 Force;
	for (unsigned int i = 0 ; i < m_ComponentArray.GetNum() ;i++)
	{
		if (!AddForce(Force,m_ComponentArray[i]->Compute() * m_ComponentArray[i]->m_fWeight))
		{
			return Force;
		}
	}
	return Force;
}
bool VSSteer::AddForce(VSVector3 & CurForce,VSVector3 & AddForce)
{
	VSREAL CurForceLen = CurForce.GetLength();
	VSREAL MaxDriverForce = GetOwner()->GetMaxDriverForce();
	VSREAL RemainForce = MaxDriverForce - CurForceLen;
	if (RemainForce <= 0.0f)
	{
		return false;
	}
	VSREAL AddForceLen = AddForce.GetLength();
	if (AddForceLen < RemainForce)
	{
		CurForce += AddForce;
	}
	else
	{
		AddForce.Normalize();
		CurForce += AddForce * RemainForce;
	}
	return true;
}