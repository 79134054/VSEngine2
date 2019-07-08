#ifndef VSMORPHBASEFUNCTION_H
#define VSMORPHBASEFUNCTION_H
#include "VSController.h"
#include "VSMorphTree.h"
#include "VSPutNode.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSMorphBaseFunction : public VSController
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class VSMorphTree;
		virtual ~VSMorphBaseFunction() = 0;
		VSMorphBaseFunction(const VSUsedName & ShowName,VSMorphTree * pMorphTree);
		virtual bool Update(double dAppTime);

	protected:
		VSMorphBaseFunction();

		VSArray<VSInputNode *> m_pInput;
		VSArray<VSOutputNode *> m_pOutput;
		VSMorphTree * m_pOwner;
		VSUsedName m_ShowName;
		VSGeometryNode * m_pGeomeNode;
		bool m_bIsVisited;

		FORCEINLINE void SetOwner(VSMorphTree * pOwner)
		{
			VSMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

		virtual void ClearFlag();
		virtual void ClearChangeFlag(){};
	public:
		

		VSInputNode * GetInputNode(unsigned int uiNodeID)const;
		VSInputNode * GetInputNode(const VSString & NodeName)const;

		VSOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		VSOutputNode * GetOutputNode(const VSString & NodeName)const;

		FORCEINLINE VSModelMeshNode * GetMeshNode()const
		{
			return DynamicCast<VSModelMeshNode>(m_pObject);
		}

		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		virtual void SetPara(void * pPara){};
		FORCEINLINE void SetCurGeometryNodeNode(VSGeometryNode * pGeomeNode)
		{
			m_pGeomeNode = pGeomeNode;
		}

	};
	DECLARE_Ptr(VSMorphBaseFunction);
	VSTYPE_MARCO(VSMorphBaseFunction);
}
#endif