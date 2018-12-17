#ifndef VSSTEER_H
#define VSSTEER_H
#include "VSActor.h"
namespace VSEngine2
{
	
	class VSSteerComponent : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		friend class VSSteer;
	public:
		VSSteerComponent();
		virtual ~VSSteerComponent() = 0;
		FORCEINLINE VSSteer * GetOwner()const
		{
			return m_pSteer;
		}
		//º∆À„∫œ¡¶
		virtual VSVector3 Compute() = 0;
	protected:
		VSSteer * m_pSteer;
		unsigned int m_uiPrior;
		VSREAL		 m_fWeight;
	};
	DECLARE_Ptr(VSSteerComponent);

	class VSSteer : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		friend class VSActor;
		VSSteer();
		virtual	~VSSteer();
		FORCEINLINE VSActor * GetOwner()const
		{
			return m_pActor;
		}
		
		template<class T>
		T * GetSteerComponet();
		template<class T>
		void AddSteerComponent(unsigned int uiPrior ,VSREAL fWeight);

		template<class T>
		void DeleteSteerComponent();
		virtual VSVector3 Compute();
	protected:
		VSActor * m_pActor;
		VSArray<VSSteerComponentPtr> m_ComponentArray;
		bool AddForce(VSVector3 & CurForce,VSVector3 & AddForce);
		class PriorityCompare
		{
		public:
			FORCEINLINE bool operator()(VSSteerComponentPtr & e1,VSSteerComponentPtr& e2)
			{
				
				return e1->m_uiPrior <= e2->m_uiPrior;
			}

		};
		
	};
	DECLARE_Ptr(VSSteer);
	template<class T>
	void VSSteer::AddSteerComponent(unsigned int uiPrior ,VSREAL fWeight)
	{
		if (!T::ms_Type.IsDerived(VSSteerComponent::ms_Type))
		{
			return ;
		}
		for (unsigned int i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				continue;
			}
		}
		T * pComponent = VS_NEW T();
		VSSteerComponent * pTemp = StaticCast<VSSteerComponent>(pComponent);
		pTemp->m_pSteer = this;
		pTemp->m_fWeight = fWeight;
		pTemp->m_uiPrior = uiPrior;
		m_ComponentArray.AddElement(pComponent);
		m_ComponentArray.Sort(0,m_ComponentArray.GetNum(),PriorityCompare())
	}
	template<class T>
	T * VSSteer::GetSteerComponet()
	{
		for (unsigned int i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				return StaticCast<T>(m_ComponentArray[i]);
			}
		}
	}

	template<class T>
	void VSSteer::DeleteSteerComponent()
	{
		for (unsigned int i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				m_ComponentArray[i] = NULL;
				m_ComponentArray.Erase(i);
				return ;
			}
		}
	}
}
#endif