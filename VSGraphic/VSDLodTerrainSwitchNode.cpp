#include "VSDLodTerrainSwitchNode.h"
#include "VSDLodTerrainNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDLodTerrainSwitchNode,VSSwitchNode)
BEGIN_ADD_PROPERTY(VSDLodTerrainSwitchNode,VSSwitchNode)
REGISTER_PROPERTY(m_uiIndexXInTerrain,IndexXInTerrain,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiIndexZInTerrain,IndexZInTerrain,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainSwitchNode)
IMPLEMENT_INITIAL_END
VSDLodTerrainSwitchNode::VSDLodTerrainSwitchNode(unsigned int uiIndexXInTerrain,unsigned int uiIndexZInTerrain)
{
	m_uiIndexXInTerrain = uiIndexXInTerrain;
	m_uiIndexZInTerrain = uiIndexZInTerrain;
}
VSDLodTerrainSwitchNode::VSDLodTerrainSwitchNode()
{
	
}
VSDLodTerrainSwitchNode::~VSDLodTerrainSwitchNode()
{

}
void VSDLodTerrainSwitchNode::UpDateView(VSCuller & Culler,double dAppTime)
{
	VSSwitchNode::UpDateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		VSDLodTerrainNode * pTerrainNode = DynamicCast<VSDLodTerrainNode>(m_pParent);
		if (!pTerrainNode)
		{
			return ;
		}
		VSTransform Tran = pTerrainNode->GetWorldTransform();
		VSVector3 Loc = pCamera->GetWorldTranslate() * Tran.GetCombineInverse();
		unsigned int uiLength = 1 << pTerrainNode->GetTesselationLevel();
		VSVector3 Pos;
		unsigned int uiIndenX = uiLength * m_uiIndexXInTerrain;
		unsigned int uiIndenZ = uiLength * m_uiIndexZInTerrain;
		Pos.x = VSTerrainNode::WIDTH_SCALE * (uiIndenX + (uiLength >> 1)) * 1.0f;
		Pos.z = VSTerrainNode::WIDTH_SCALE * (uiIndenZ + (uiLength >> 1)) * 1.0f;
		Pos.y = pTerrainNode->GetHeight(uiIndenX,uiIndenZ);

		VSVector3 Length = Loc - Pos;
		VSREAL fSqrLen = Length.GetSqrLength();
		VSREAL fDLodScale = pTerrainNode->GetDLodScale();
		for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			m_uiActiveNode = i;
			if (fSqrLen < (1 << i) * fDLodScale)
			{		
				break;
			}
		}
	}


}
