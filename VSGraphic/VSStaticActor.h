#ifndef VSSTATICACTOR_H
#define VSSTATICACTOR_H
#include "VSActor.h"
#include "VSStaticMeshComponent.h"
namespace VSEngine2
{
	DECLARE_Proxy(VSStaticMeshNode);
	class VSGRAPHIC_API VSStaticActor : public VSActor
	{
		//PRIORITY
	
		//RTTI
		DECLARE_RTTI;
	public:
		friend class VSWorld;
		VSStaticActor();
		virtual ~VSStaticActor();
		DECLARE_INITIAL


		GET_TYPE_NODE(VSStaticMeshComponent)
		virtual void CreateDefaultComponentNode();

	};
	DECLARE_Ptr(VSStaticActor);
	VSTYPE_MARCO(VSStaticActor);
}
#endif