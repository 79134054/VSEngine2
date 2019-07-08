#ifndef VSACTOR_H
#define VSACTOR_H
#include "VSLogic.h"
#include "VSObject.h"
#include "VSVector3.h"
#include "VSQuat.h"
#include "VSNode.h"
#include "VSNodeComponent.h"
#define GET_TYPE_NODE(nodeclass)\
	friend class nodeclass;\
	nodeclass * GetTypeNode()const\
	{\
		VSNode * pNode = m_pNode; \
		return(nodeclass*)pNode;\
	}
namespace VSEngine2
{
	DECLARE_Ptr(VSFSM);
	DECLARE_Ptr(VSSteer);
	class VSSceneMap;

	class VSGRAPHIC_API VSActor : public VSObject,public VSResource
	{
		//RTTI
		DECLARE_RTTI;

	public:
		VSActor();
		virtual ~VSActor();

		DECLARE_INITIAL
	public:
		virtual void SetWorldPos(const VSVector3 & Pos);
		virtual void SetWorldScale(const VSVector3 &Scale);
		virtual void SetWorldRotate(const VSMatrix3X3 & Rotate);

		virtual void SetLocalPos(const VSVector3 & Pos);
		virtual void SetLocalScale(const VSVector3 &Scale);
		virtual void SetLocalRotate(const VSMatrix3X3 & Rotate);

		VSVector3 GetWorldPos();
		VSVector3 GetWorldScale();
		VSMatrix3X3 GetWorldRotate();

		VSVector3 GetLocalPos();
		VSVector3 GetLocalScale();
		VSMatrix3X3 GetLocalRotate();

		
		virtual void ProcessInput(unsigned int uiInputType,unsigned int uiEvent,unsigned int uiKey,int x, int y, int z);
		
		virtual bool HandleMessage(VSMessage & Message);



		VSNode * GetActorNode()const
		{
			return m_pNode;
		}
		
		VSVector3 GetVelocity()const
		{
			return m_Velocity;
		}
		VSREAL GetMaxVelocity()const
		{
			return m_fMaxVelocity;
		}
		VSREAL GetMaxDriverForce()const
		{
			return m_fMaxDriverForce;
		}

		virtual void Update(double dAppTime);
		virtual bool PostClone(VSObject * pObjectSrc);
		friend class VSWorld;
		friend class VSSceneMap;
	protected:
		VSVector3	m_Velocity;
		VSREAL		m_fMaxVelocity;
		VSREAL		m_fMaxDriverForce;

		VSNodePtr		m_pNode;
		VSFSMPtr		m_pFSM;
		VSSteerPtr		m_pSteerPtr;
		
		VSActor *		m_pOwner;
	public:
		VSUsedName		m_ActorName;
		VSSceneMap *	m_pSceneMap;

	public:
		virtual void OnDestory();
		VSActor *GetOwner();
		virtual void AddChildActor(VSActor * pActor);
		virtual void DeleteChildActor(VSActor * pActor);
		virtual VSActor * GetChildActor(unsigned int uiActorIndex);
		virtual void DeleteChildActor(unsigned int uiActorIndex);
		template<typename T>
		T * AddComponentNode();
		template<typename T>
		void GetComponentNode(VSArray<T*>& Node);
		void DeleteComponentNode(VSNodeComponent * pComponent);
		void ChangeComponentNodeParent(VSNodeComponent * pSource, VSNode * pParent = NULL);
		void AddActorNodeToNode(VSActor * pActor, VSNodeComponent * pNode);
	protected:
		virtual void AddToSceneMap(VSSceneMap * pSceneMap);
		virtual void CreateDefaultComponentNode();

	protected:
		VSArray<VSActor *> m_ChildActor;
		
		VSArray<VSNodeComponentPtr> m_pNodeComponentArray; // not include root node
		static VSPointer<VSActor> Default;
	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual unsigned int GetResourceType()const
		{
			return RT_ACTOR;
		}
		static const VSActor *GetDefalut()
		{
			return Default;
		}
	};
	DECLARE_Ptr(VSActor);
	VSTYPE_MARCO(VSActor);
	DECLARE_Proxy(VSActor);
	template<typename T>
	T * VSActor::AddComponentNode()
	{
		T * pNode = VSNodeComponent::CreateComponet<T>();
		m_pNode->AddChild(pNode);
		m_pNodeComponentArray.AddElement(pNode);
		return pNode;
	}
	template<typename T>
	void VSActor::GetComponentNode(VSArray<T*>& Node)
	{
		for (unsigned int i = 0; i < m_pNodeComponentArray.GetNum();i++)
		{
			if (m_pNodeComponentArray[i]->GetType().IsSameType(T::GetType()))
			{
				Node.AddElement(m_pNodeComponentArray[i]);
			}
		}
	}
}
#endif