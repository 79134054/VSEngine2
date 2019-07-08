#include "VSDLodTerrainNode.h"
#include "VSDLodTerrainGeometry.h"
#include "VSDLodTerrainSwitchNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDLodTerrainNode,VSTerrainNode)
BEGIN_ADD_PROPERTY(VSDLodTerrainNode,VSTerrainNode)
REGISTER_PROPERTY(m_uiDLodExtend, DLodExtend, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fDLodScale, DLodScale, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainNode)
IMPLEMENT_INITIAL_END
VSDLodTerrainNode::VSDLodTerrainNode()
{
	m_uiDLodExtend = 500;
	m_fDLodScale = 50000000.0f;
}

VSDLodTerrainNode::~VSDLodTerrainNode()
{
}
bool VSDLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	unsigned int uiChildNumX = GetChildNumX();
	unsigned int uiChildNumZ = GetChildNumZ();
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{

			VSDLodTerrainSwitchNode * pDTS = NULL;
			pDTS = VS_NEW VSDLodTerrainSwitchNode(i,j);
			if (!pDTS)
			{
				return 0;
			}
			AddChild(pDTS);
			for (unsigned int k = 0 ; k < m_uiTesselationLevel ; k++)
			{
				VSDLodTerrainGeometry * pChild = NULL;
				pChild = VS_NEW VSDLodTerrainGeometry();
				if (!pChild)
				{
					return 0;
				}
				pDTS->AddChild(pChild);
				pChild->CreateMesh(i,j,k,m_uiTesselationLevel);

				pChild->AddMaterialInstance(VSResourceManager::ms_DefaultOnlyColorMaterialResource);
				VSREAL green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
				pChild->GetMaterialInstance(0)->SetPShaderValue(_T("EmissiveColor"), green, 4);
			}

		}
	}

	return 1;
}

void VSDLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	
	VSTerrainNode::UpdateNodeAll(dAppTime);

}