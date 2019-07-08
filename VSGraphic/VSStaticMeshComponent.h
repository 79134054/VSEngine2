#ifndef VSSTATICMESHCOMPONENT_H
#define VSSTATICMESHCOMPONENT_H
#include "VSMeshComponent.h"
namespace VSEngine2
{

	DECLARE_Ptr(VSStaticMeshNode);
	DECLARE_Proxy(VSStaticMeshNode);
	class VSGRAPHIC_API VSStaticMeshComponent : public VSMeshComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSStaticMeshComponent();
		virtual ~VSStaticMeshComponent();
		void SetStaticMeshResouce(VSStaticMeshNodeR * pStaticMeshResource);
		VSStaticMeshNode * GetStaticMeshNode();
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		virtual void PostCreate();
		virtual bool BeforeSave(void * pDate = NULL);
		virtual bool PostLoad(void * pDate = NULL);
		
	protected:
		VSStaticMeshNodeRPtr m_pStaticMeshResource;
		

	};
	DECLARE_Ptr(VSStaticMeshComponent);
	VSTYPE_MARCO(VSStaticMeshComponent);
}
#endif