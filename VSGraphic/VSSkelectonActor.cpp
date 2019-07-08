#include "VSSkelectonActor.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSkelectonActor,VSActor)
BEGIN_ADD_PROPERTY(VSSkelectonActor,VSActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkelectonActor)
IMPLEMENT_INITIAL_END
VSSkelectonActor::VSSkelectonActor()
{

}
VSSkelectonActor::~VSSkelectonActor()
{

}
void VSSkelectonActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponet<VSSkelectonMeshComponent>();
}
