#ifndef VSSKELECTONCOMPONENT_H
#define VSSKELECTONCOMPONENT_H
#include "VSMeshComponent.h"
namespace VSEngine2
{
	DECLARE_Proxy(VSSkelectonMeshNode);
	DECLARE_Ptr(VSSkelectonMeshNode);
	class VSGRAPHIC_API VSSkelectonMeshComponent : public VSMeshComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkelectonMeshComponent();
		virtual ~VSSkelectonMeshComponent();
		void SetSkelectonMeshResource(VSSkelectonMeshNodeR * pSkelectonMeshResource);
		VSSkelectonMeshNode * GetSkelectonMeshNode();
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		virtual void PostCreate();
		void SetIsDrawSkelecton(bool bIsDrawSkelecton);
		
		virtual bool BeforeSave(void * pDate = NULL);
		bool PlayAnim(const VSString & AnimName, VSREAL fRatio = 1.0f, unsigned int uiRepeatType = VSController::RT_NONE);
		VSSocketNode * GetSocketNode(const VSUsedName & SocketName);
		void SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara);
		virtual bool PostLoad(void * pDate);
	protected:
		VSSkelectonMeshNodeRPtr m_pSkelectonMeshResource;
		

	};
	DECLARE_Ptr(VSSkelectonMeshComponent);
	VSTYPE_MARCO(VSSkelectonMeshComponent);
}
#endif