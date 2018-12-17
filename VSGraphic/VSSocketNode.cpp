#include "VSSocketNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSocketNode, VSSpatial)
BEGIN_ADD_PROPERTY(VSSocketNode, VSSpatial)
REGISTER_PROPERTY(m_cName, Name, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSocketNode)
IMPLEMENT_INITIAL_END
VSSocketNode::VSSocketNode()
{
	m_bIsStatic = false;
}
VSSocketNode::~VSSocketNode()
{

}
void VSSocketNode::UpdateNodeAll(double dAppTime)
{
	UpdateTransform(dAppTime);
}