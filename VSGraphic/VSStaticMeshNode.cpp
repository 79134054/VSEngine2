#include "VSStaticMeshNode.h"
#include "VSGraphicInclude.h"
#include "VSModelSwitchNode.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSStaticMeshNode,VSModelMeshNode)
BEGIN_ADD_PROPERTY(VSStaticMeshNode,VSModelMeshNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStaticMeshNode)
ADD_PRIORITY(VSGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSStaticMeshNodePtr VSStaticMeshNode::Default = NULL;
bool VSStaticMeshNode::ms_bIsEnableASYNLoader = true;
bool VSStaticMeshNode::ms_bIsEnableGC = true;
VSStaticMeshNode::VSStaticMeshNode()
{
}
VSStaticMeshNode::~VSStaticMeshNode()
{

}
bool VSStaticMeshNode::InitialDefaultState()
{
	Default = VS_NEW VSStaticMeshNode();
	VSGeometryNodePtr GeometryNode = VS_NEW VSGeometryNode();
	Default->AddChild(GeometryNode);
	VSGeometryPtr Geometry = (VSGeometry *)VSObject::CloneCreateObject(VSGeometry::GetDefaultRenderCube());
	GeometryNode->AddChild(Geometry);
	Default->CreateLocalAABB();
	GeometryNode->SetLocalScale(VSVector3(100.0f,100.0f,100.0f));
	
	return true;
}
bool VSStaticMeshNode::TerminalDefaultState()
{
	Default = NULL;
	return true;
}
void VSStaticMeshNode::AddLodMesh(VSStaticMeshNodeR * pStaticMeshResource)
{
	if (pStaticMeshResource)
	{
		VSModelSwitchNode* LodNode = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
		if (!LodNode)
		{
			LodNode = VS_NEW VSModelSwitchNode();
			VSSpatialPtr GeoNode = m_pChild[0];	
			DeleteAllChild();
			LodNode->AddChild(GeoNode);
			AddChild(LodNode);
		}
		while (!pStaticMeshResource->IsLoaded())
		{
		}
		VSGeometryNode * pGeoNode = pStaticMeshResource->GetResource()->GetGeometryNode(0);
		LodNode->AddChild(pGeoNode);
	}
}
void VSStaticMeshNode::SetLodMesh(unsigned int i, VSStaticMeshNodeR * pStaticMeshResource)
{
	if (pStaticMeshResource)
	{
		VSModelSwitchNode* LodNode = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
		if (!LodNode)
		{
			return;
		}
		if (i >= LodNode->GetNodeNum())
		{
			return;
		}
		while (!pStaticMeshResource->IsLoaded())
		{
		}
		VSGeometryNode * pGeoNode = pStaticMeshResource->GetResource()->GetGeometryNode(0);
		(*LodNode->GetChildList())[i] = pGeoNode;
	}
}
void VSStaticMeshNode::DeleteLodMesh(unsigned int i)
{
	VSModelSwitchNode* LodNode = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
	if (!LodNode)
	{
		return;
	}
	if (i >= LodNode->GetNodeNum())
	{
		return;
	}
	LodNode->GetChildList()->Erase(i);
}