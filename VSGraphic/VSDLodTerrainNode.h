#ifndef VSDLODTERRAINNODE_H
#define VSDLODTERRAINNODE_H
#include "VSTerrainNode.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSDLodTerrainNode : public VSTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		VSDLodTerrainNode();
		virtual ~VSDLodTerrainNode();
	public:
		FORCEINLINE void SetDLodExtend(int uiDLodExtend)
		{
			m_uiDLodExtend = uiDLodExtend;
		}
		FORCEINLINE int GetDLodExtend()const
		{
			return m_uiDLodExtend;
		}

		FORCEINLINE void SetDLodScale(VSREAL fDLodScale)
		{
			m_fDLodScale = fDLodScale;
		}

		FORCEINLINE VSREAL GetDLodScale()const { return m_fDLodScale; }
		
	protected:
		int m_uiDLodExtend;
		VSREAL m_fDLodScale;
		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		
	};
	DECLARE_Ptr(VSDLodTerrainNode);
	VSTYPE_MARCO(VSDLodTerrainNode);


}
#endif