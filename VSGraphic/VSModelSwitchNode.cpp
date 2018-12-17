#include "VSModelSwitchNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSModelSwitchNode,VSSwitchNode)
BEGIN_ADD_PROPERTY(VSModelSwitchNode,VSSwitchNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSModelSwitchNode)
IMPLEMENT_INITIAL_END
VSModelSwitchNode::VSModelSwitchNode()
{

}
VSModelSwitchNode::~VSModelSwitchNode()
{

}
VSGeometryNode * VSModelSwitchNode::GetGeometryNode(unsigned int uiLodLevel)
{
	if (uiLodLevel >= m_pChild.GetNum())
	{
		return NULL;
	}
	else
	{
		VSGeometryNode * pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[uiLodLevel]);
		return pGeometryNode;
	}
}
void VSModelSwitchNode::UpDateView(VSCuller & Culler, double dAppTime)
{
	VSSwitchNode::UpDateView(Culler, dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		VSREAL ZFar = pCamera->GetZFar();
		VSVector3 DistVector = pCamera->GetWorldTranslate() - GetWorldTranslate();
		VSREAL Dist = DistVector.GetLength();
		VSREAL LastTemp = ZFar * 0.5f;
		VSREAL CurTemp = LastTemp;

		m_uiActiveNode = 0;
		while (true)
		{
			if (Dist > CurTemp)
			{		
				if (m_uiActiveNode + 1 >= m_pChild.GetNum() || m_pChild[m_uiActiveNode + 1] == NULL)
				{
					break;
				}
				LastTemp = LastTemp * 0.5f;
				CurTemp = CurTemp + LastTemp;
				m_uiActiveNode++;
			}
			else
			{
				break;
			}
		}
	}


}