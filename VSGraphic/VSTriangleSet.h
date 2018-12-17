#ifndef VSTRIANGLESET_H
#define VSTRIANGLESET_H
#include "VSMeshDate.h"

namespace VSEngine2
{

	

	class VSGRAPHIC_API VSTriangleSet : public VSMeshDate
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSTriangleSet();
		virtual ~VSTriangleSet();
		bool CreateFanIndex();
		bool CreateNormal(unsigned int uiVertexLevel = 0);
		bool CreateTangent(unsigned int uiTexCoordLevel = 0);
		virtual unsigned int GetTotleNum()const;
		virtual unsigned int GetMeshDateType(){return MDT_TRIANGLE;}
		virtual bool CreateCollapseRecord();
		virtual void UpDateClodMesh();
		virtual unsigned int GetActiveNum()const;
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;
		
	
	};
	DECLARE_Ptr(VSTriangleSet);
	VSTYPE_MARCO(VSTriangleSet);
	
}
#endif