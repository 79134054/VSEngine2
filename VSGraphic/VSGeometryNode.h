#ifndef VSGEOMETRYNODE_H
#define VSGEOMETRYNODE_H
#include "VSNode.h"
#include "VSMorphSet.h"
#include "VSGeometry.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSGeometryNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGeometryNode();
		virtual ~VSGeometryNode();
		void SetMorphSet(VSMorphSet * pMorphSet);
		FORCEINLINE const VSMorphSet *GetMorphSet()const
		{
			return m_pMorphSet;
		}
		VSGeometry * GetGeometry(unsigned int i);
		VSGeometry * GetNormalGeometry(unsigned int index);
		unsigned int GetNormalGeometryNum();
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		VSMorphSetPtr m_pMorphSet;
		void UpdateLocalAABB();
	};
	DECLARE_Ptr(VSGeometryNode);
	VSTYPE_MARCO(VSGeometryNode);
}
#endif