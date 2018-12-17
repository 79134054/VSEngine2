#ifndef VSSKELECTONACTOR_H
#define VSSKELECTONACTOR_H
#include "VSActor.h"
#include "VSSkelectonMeshComponent.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSSkelectonActor : public VSActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		friend class VSWorld;
		VSSkelectonActor();
		virtual ~VSSkelectonActor();

		DECLARE_INITIAL


		GET_TYPE_NODE(VSSkelectonMeshComponent)
		//virtual void Update(double dAppTime);
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(VSSkelectonActor);
	VSTYPE_MARCO(VSSkelectonActor);
}
#endif