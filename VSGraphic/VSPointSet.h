#ifndef VSPOINTSET_H
#define VSPOINTSET_H
#include "VSMeshDate.h"
#include "VSVector3.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSPointSet : public VSMeshDate
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPointSet();
		VSPointSet(const VSVector3 & Point,VSREAL fSize);
		virtual ~VSPointSet();
		bool CreateIndex();
		virtual unsigned int GetTotleNum()const;
		virtual unsigned int GetMeshDateType(){return MDT_POINT;}
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;


	
	
	};
	DECLARE_Ptr(VSPointSet);
	VSTYPE_MARCO(VSPointSet);

}
#endif