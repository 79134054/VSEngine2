#ifndef VSANIMBASEFUNCTION_H
#define VSANIMBASEFUNCTION_H
#include "VSController.h"
#include "VSAnimTree.h"
#include "VSPutNode.h"
namespace VSEngine2
{	
	class VSStream;
	class VSGRAPHIC_API VSAnimBaseFunction : public VSController
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class VSAnimTree;
		virtual ~VSAnimBaseFunction() = 0;
		VSAnimBaseFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

		virtual bool Update(double dAppTime);
		virtual void ClearFlag();

	protected:
		VSAnimBaseFunction();
	protected:
		VSArray<VSInputNode *> m_pInput;
		VSArray<VSOutputNode *> m_pOutput;
		VSAnimTree * m_pOwner;
		VSUsedName m_ShowName;

		bool m_bIsVisited;
		bool m_bNoLeafStart;
		FORCEINLINE void SetOwner(VSAnimTree * pOwner)
		{
			VSMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

		
	public:
		VSSkelectonMeshNode * GetSkelectonMeshNode()const;
		VSInputNode * GetInputNode(unsigned int uiNodeID)const;
		VSInputNode * GetInputNode(const VSString & NodeName)const;

		VSOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		VSOutputNode * GetOutputNode(const VSString & NodeName)const;

		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		virtual void SetPara(void * pPara){};

		virtual bool IsLeafNode();
		void GetLeafArray(VSArray<VSAnimBaseFunction *> & LeafNode);

		void EnableLeafStart();
		void NoLeafStart();
	};
	DECLARE_Ptr(VSAnimBaseFunction);
	VSTYPE_MARCO(VSAnimBaseFunction);
}
#endif