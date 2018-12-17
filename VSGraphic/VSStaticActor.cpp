#include "VSStaticActor.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSStaticActor,VSActor)
BEGIN_ADD_PROPERTY(VSStaticActor,VSActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStaticActor)
IMPLEMENT_INITIAL_END
VSStaticActor::VSStaticActor()
{
	
}
VSStaticActor::~VSStaticActor()
{

}
void VSStaticActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponet<VSStaticMeshComponent>();
}
