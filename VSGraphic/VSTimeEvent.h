#ifndef VSTIMEEVENT_H
#define VSTIMEEVENT_H
#include "VSObject.h"
#include "VSDelegateList.h"
namespace VSEngine2
{
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef VSDelegateEvent<void(void)> TriggerAnimEventType;
#else
	typedef VSDelegateEvent0<void> TriggerAnimEventType;
#endif
	class VSStream;
	class VSController;

	class VSGRAPHIC_API VSTimeEvent : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~VSTimeEvent() = 0;
		FORCEINLINE VSController * GetObject()const;

		VSTimeEvent();
		VSTimeEvent(VSController *pController);
		virtual void Trigger();
	public:
		bool m_bEnable;
		VSREAL m_fTriggerPercent;
		TriggerAnimEventType m_TriggerAnimEvent;
	protected:
		VSController * m_pController;
		
	};
	DECLARE_Ptr(VSTimeEvent);
	VSTYPE_MARCO(VSTimeEvent);

}
#endif