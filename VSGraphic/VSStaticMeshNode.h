#ifndef VSSTATICMESHNODE_H
#define VSSTATICMESHNODE_H
#include "VSModelMeshNode.h"
namespace VSEngine2
{
	DECLARE_Proxy(VSStaticMeshNode);
	class VSGRAPHIC_API VSStaticMeshNode : public VSModelMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSStaticMeshNode();
		virtual ~VSStaticMeshNode();
		
		virtual unsigned int GetResourceType()const
		{
			return RT_STATIC_MODEL;
		}
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		void AddLodMesh(VSStaticMeshNodeR * pStaticMeshResource);
		void SetLodMesh(unsigned int i, VSStaticMeshNodeR * pStaticMeshResource);
		void DeleteLodMesh(unsigned int i);
	protected:
		static VSPointer<VSStaticMeshNode> Default;
	public:
		static const VSStaticMeshNode *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;

	};
	DECLARE_Ptr(VSStaticMeshNode);
	VSTYPE_MARCO(VSStaticMeshNode);
	DECLARE_Proxy(VSStaticMeshNode);
}
#endif