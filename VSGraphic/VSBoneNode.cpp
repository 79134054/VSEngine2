#include "VSBoneNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSBoneNode,VSNode)
BEGIN_ADD_PROPERTY(VSBoneNode,VSNode)
REGISTER_PROPERTY(m_OffSetMatrix, OffSetMatrix, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bIsEffector, IsEffector, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_TargetPosInWorld, TargetPosInWorld, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bAllowTranslation, AllowTranslation, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMinTranslation, MinTranslation, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMaxTranslation, MaxTranslation, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bAllowRotation, AllowRotation, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMinRotation, MinRotation, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMaxRotation, MaxRotation, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fIKWeight,IKWeight,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName, Name, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBoneNode)
IMPLEMENT_INITIAL_END
VSBoneNode::VSBoneNode()
{
	m_bIsEffector = false;
	m_bAllowTranslation.SetBufferNum(3);
	m_fMinTranslation.SetBufferNum(3);
	m_fMaxTranslation.SetBufferNum(3);
	m_bAllowRotation.SetBufferNum(3);
	m_fMinRotation.SetBufferNum(3);
	m_fMaxRotation.SetBufferNum(3);
	for (int i = 0; i < 3; i++)
	{
		m_bAllowTranslation[i] = false;
		m_fMinTranslation[i] = -VSMAX_REAL;
		m_fMaxTranslation[i] = VSMAX_REAL;
		m_bAllowRotation[i] = true;
		m_fMinRotation[i] = -VSPI;
		m_fMaxRotation[i] = VSPI;
	}
	m_fIKWeight = 1.0f;
}
VSBoneNode::~VSBoneNode()
{

}
void VSBoneNode::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	return;
}
void VSBoneNode::GetIKMoveAxis(VSVector3 Axis[3])const
{
	if (m_pParent)
	{
		m_pParent->GetWorldRotate().GetUVN(Axis);
	}
	else
	{
		Axis[0] = VSVector3(1.0f,0.0f,0.0f);
		Axis[1] = VSVector3(0.0f,1.0f,0.0f);
		Axis[2] = VSVector3(0.0f,0.0f,1.0f);
	}

}
void VSBoneNode::ComputeIKLocalRotDelta(const VSMatrix3X3 & WorldRot,VSMatrix3X3 & LocalRotDelta)const
{
	if (m_pParent)
	{
		VSMatrix3X3 Inver;
		Inver.InverseOf(m_pParent->GetWorldRotate());
		LocalRotDelta = m_pParent->GetWorldRotate() * WorldRot * Inver;
	}
	else
	{
		LocalRotDelta = WorldRot;
	}

}
VSBoneNode * VSBoneNode::GetBoneNodeFromLevel(const VSUsedName & BoneName)
{
	VSBoneNode * pFind = NULL;
	if (m_cName == BoneName)
	{
		pFind = this;
	}
	else
	{
		for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			VSSpatial * pChild = m_pChild[i];
			pFind = ((VSBoneNode *)pChild)->GetBoneNodeFromLevel(BoneName);
			if (!pFind)
			{
				return pFind;
			}
		}
	}
	return pFind;
}
unsigned int VSBoneNode::GetAllBoneNum()const
{
	unsigned int uiLeafNum  = 1;
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{

		VSSpatial * pBone = m_pChild[i];
		uiLeafNum += ((VSBoneNode *)pBone)->GetAllBoneNum();


	}

	return uiLeafNum;
}
void VSBoneNode::SetLocalMat(const VSMatrix3X3W VSMat)
{
	m_bIsChanged = true;
	m_Local.SetMatrix(VSMat);
}
void VSBoneNode::GetAllBoneArray(VSArray<VSBoneNode *> & AllNodeArray)
{
	AllNodeArray.AddElement(this);
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		VSBoneNode * pBoneNode = DynamicCast<VSBoneNode>(m_pChild[i]);
		if (pBoneNode)
		{
			pBoneNode->GetAllBoneArray(AllNodeArray);
		}

		

	}
}