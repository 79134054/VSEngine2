#ifndef VSMODELMESHNODE_H
#define VSMODELMESHNODE_H
#include "VSMeshNode.h"
#include "VSGeometryNode.h"
namespace VSEngine2
{
	class VSGeometryNode;
	class VSSwitchNode;
	class VSStream;
	DECLARE_Ptr(VSMorphTree);
	DECLARE_Proxy(VSMorphTree);
	class VSGRAPHIC_API VSModelMeshNode : public VSMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//LOD TYPE
		{
			LT_NONE,
			LT_DLOD,
			LT_CLOD,
			LT_MAX
		};
		VSModelMeshNode();
		virtual ~VSModelMeshNode();

		FORCEINLINE unsigned int GetLodType()const{return m_uiLodType;}
		FORCEINLINE void SetLodType(unsigned int uiLodType)
		{
			if (uiLodType >= LT_MAX)
			{
				return;
			}
			m_uiLodType = uiLodType;
		}


		virtual VSSwitchNode * GetDlodNode()const;
		virtual unsigned int GetResourceType()const
		{
			return RT_MAX;
		}
		virtual VSGeometryNode * GetGeometryNode(unsigned int uiLodLevel);
		void SetMorphTree(VSMorphTreeR * pMorphTree);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateController(double dAppTime);
		void SetMorphTreeNodePara(const VSUsedName & ShowName, void * pPara);
	protected:

		unsigned int m_uiLodType;
		VSMorphTreeRPtr m_pMorphTree;
		VSMorphTreePtr m_pMorphTreeInstance;
	};
	DECLARE_Ptr(VSModelMeshNode);
	VSTYPE_MARCO(VSModelMeshNode);
}
#endif