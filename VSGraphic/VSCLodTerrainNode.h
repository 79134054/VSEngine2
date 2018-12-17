#ifndef VSCLODTERRAINNODE_H
#define VSCLODTERRAINNODE_H
#include "VSTerrainNode.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSCLodTerrainNode : public VSTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Terrain Node Type
		{
			TNT_ROAM,
			TNT_QUAD,
			TNT_MAX
		};
		virtual ~VSCLodTerrainNode();
		VSCLodTerrainNode();
	protected:
		

		
		VSREAL	m_fCLODScale;

		unsigned int m_uiTerrainNodeType;
		void LinkNeighbor();
		void ComputeVariance();

		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		virtual unsigned int AddChild(VSSpatial * pChild);

		void SetCLODScale(VSREAL fCLODScale);

		FORCEINLINE VSREAL GetCLODScale()const { return m_fCLODScale; }

		void SetTerrainNodeType(unsigned int uiTerrainNodeType);
	};
	DECLARE_Ptr(VSCLodTerrainNode);
	VSTYPE_MARCO(VSCLodTerrainNode);
}
#endif