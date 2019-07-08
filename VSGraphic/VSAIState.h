#ifndef VSAISTATE_H
#define VSAISTATE_H
#include "VSObject.h"
#include "VSLogic.h"
#include "VSPutNode.h"
#include "VSName.h"
#include "VSResource.h"
namespace VSEngine2
{
	class VSStream;
	//״̬�任һ����������ڲ�����������任����һ�������ġ�
	//����������ͨ��update���任��������ͨ������Ϣ���任�����ǿ�ȷ���������Ϣ������任״̬������ǰ����״̬���ܲ�Ҫ�任
	//������������¾Ͳ��ܱ任��
	class  VSAIStateInputNode : public VSInputNode
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSAIStateInputNode();
		virtual ~VSAIStateInputNode();
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		
	public:	

		virtual bool CheckState() = 0;

	};
	DECLARE_Ptr(VSAIStateInputNode);
	VSTYPE_MARCO(VSAIStateInputNode);
	class VSFSM;
	class  VSAIState : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSAIState(const VSUsedName &StateName,VSFSM * pOwner);
		virtual ~VSAIState() = 0;
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		
	public:	

		virtual bool BeginState() = 0;
		virtual bool EndState() = 0;
		virtual bool HandleMessage(VSMessage & Message);
		virtual bool Update(double Time);
		const VSUsedName & GetStateName()const
		{
			return m_StateName;
		}
	protected:
		VSOutputNode * m_pStateOutPut;
		VSArray<VSAIStateInputNode*> m_pStateInputNode;
		VSUsedName	m_StateName;
		VSAIState();
		VSFSM * m_pOwner;

	};
	DECLARE_Ptr(VSAIState);
	VSTYPE_MARCO(VSAIState);

	class  VSBeginState : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSBeginState(const VSUsedName &StateName);
		virtual ~VSBeginState() = 0;
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		
	public:	

		virtual bool BeginState();
		virtual bool EndState();
		virtual bool Update(double Time);
	protected:
		VSBeginState();
	};
	DECLARE_Ptr(VSBeginState);
	VSTYPE_MARCO(VSBeginState);

	DECLARE_Ptr(VSActor);

	class  VSFSM : public VSObject,public VSResource
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSFSM();
		virtual ~VSFSM() = 0;
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		

	public:	
		friend class VSAIState;
		virtual unsigned int GetResourceType()const
		{
			return RT_FSM;
		}
		virtual bool HandleMessage(VSMessage & Message)
		{
			if (m_pGlobleState)
			{
				m_pGlobleState->HandleMessage(Message);
			}
			if (m_pCurState)
			{
				m_pCurState->HandleMessage(Message);
			}	
		}
		virtual bool Update(double Time)
		{
			if (m_pGlobleState)
			{
				m_pGlobleState->Update(Time);
			}
			if (m_pCurState)
			{
				m_pCurState->Update(Time);
			}
		}

		VSAIState * GetState(const VSUsedName	& StateName)
		{
			for (unsigned int i = 0 ; i < m_AllState.GetNum() ; i++)
			{
				if (StateName == m_AllState[i]->GetStateName())
				{
					return m_AllState[i];
				}
			}
			return NULL;
		}
		VSAIState * GetBeginState()
		{
			for (unsigned int i = 0 ; i < m_AllState.GetNum() ; i++)
			{
				VSBeginState * pState = DynamicCast<VSBeginState>(m_AllState[i]);
				if (pState)
				{
					return m_AllState[i];
				}
			}
			return NULL;
		}

		virtual bool Reset()
		{
			m_pCurState = GetBeginState();
			m_pCurState->BeginState();
		}

	protected:
		bool ChangeState(VSAIState *m_pState)
		{
			if (m_pState)
			{
				m_pCurState->EndState();
				m_pState->BeginState();
				m_pCurState = m_pState;
				return true;
			}
			return false;
		}
		bool ChangeState(const VSUsedName & StateName)
		{
			VSAIState * pState = GetState(StateName);
			if (pState)
			{
				m_pCurState->EndState();
				pState->BeginState();
				m_pCurState = pState;
				return true;
			}
			return false;
		}
		VSArray<VSAIState *> m_AllState;
		VSAIState *m_pCurState;
		VSAIState *m_pGlobleState;
		VSAIState *m_pBeginState;
		VSUsedName	m_FSMName;
		VSActorPtr m_pActor;
		
	};
	DECLARE_Ptr(VSFSM);
	VSTYPE_MARCO(VSFSM);

}
#endif