#ifndef VSPUTNODE_H
#define VSPUTNODE_H
#include "VSObject.h"
#include "VSName.h"
namespace VSEngine2
{
	class VSShaderFunction;
	class VSInputNode;
	class VSOutputNode;
	class VSStream;
	class VSGRAPHIC_API VSPutNode : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum // Value Type
		{
			VT_1,
			VT_2,
			VT_3,
			VT_4,
			VT_MAX
		};
		enum		//ANIM VALUE TYPE
		{
			AVT_ANIM,
			AVT_MORPH,
			AVT_IK,
			AVT_MAX
		};
		enum	//Post Effect Type
		{
			PET_OUT
		};
		virtual ~VSPutNode() = 0;
	protected:
		VSPutNode(unsigned int uiValueType,const VSUsedName & NodeName,VSObject * pShaderFunction);
		VSPutNode();
		VSObject * m_pOwner;
		VSUsedName m_NodeName;
		unsigned int m_uiValueType;
		FORCEINLINE void SetOwner(VSObject *pOwner)
		{
			m_pOwner = pOwner;
		}
	public:
		friend class VSShaderFunction;
		friend class VSAnimBaseFunction;
		friend class VSMorphBaseFunction;
		friend class VSPostEffectFunction;
	
		
		

		FORCEINLINE unsigned int GetValueType()const
		{
			return m_uiValueType;
		}
		FORCEINLINE void SetValueType(unsigned int uiValueType)
		{
			m_uiValueType = uiValueType;
		}
		FORCEINLINE const VSUsedName & GetNodeName()const
		{
			return m_NodeName;
		}
		FORCEINLINE void SetNodeName(const VSUsedName & NodeName)
		{
			m_NodeName = NodeName;
		}
		FORCEINLINE VSObject * GetOwner()const
		{
			return m_pOwner;
		}

	};
	DECLARE_Ptr(VSPutNode);
	VSTYPE_MARCO(VSPutNode);
	class VSGRAPHIC_API VSInputNode : public VSPutNode
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		VSInputNode(unsigned int uiValueType,const VSUsedName &NodeName,VSObject * pObject);
		virtual ~VSInputNode();
	protected:
		VSInputNode();
		VSOutputNode * m_pOutputLink;
		

	public:
		friend class VSOutputNode;
		bool Connection(VSOutputNode * pOutputNode,bool bIsJudgeType = false);
		void UnConnection();
		FORCEINLINE const VSOutputNode *GetOutputLink()const
		{
			return m_pOutputLink;
		}
		bool IsConnection()
		{
			return m_pOutputLink != NULL;
		}
	};
	DECLARE_Ptr(VSInputNode);
	VSTYPE_MARCO(VSInputNode);
	class VSGRAPHIC_API VSOutputNode:public VSPutNode
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSOutputNode(unsigned int uiValueType,const VSUsedName &NodeName,VSObject * pShaderFunction);
		virtual ~VSOutputNode();
	protected:
		VSOutputNode();
		VSArray<VSInputNode *> m_pInputLink;
		

	public:
		friend class VSInputNode;
		bool Connection(VSInputNode * pInputNode,bool bIsJudgeType = false);
		void UnConnection();
		bool IsConnection()
		{
			return m_pInputLink.GetNum() > 0;
		}
		VSInputNode * GetInputLink(unsigned int i)
		{
			return m_pInputLink[i];
		}
		unsigned int GetInputNum()const
		{
			return m_pInputLink.GetNum();
		}
	protected:
		bool CheckIsConnection(VSInputNode * pInputNode)const;
	
	};
	DECLARE_Ptr(VSOutputNode);
	VSTYPE_MARCO(VSOutputNode);
}
#endif