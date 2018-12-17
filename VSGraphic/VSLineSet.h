#ifndef VSLINESET_H
#define VSLINESET_H
#include "VSMeshDate.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSLineSet:public VSMeshDate
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSLineSet();
		virtual ~VSLineSet();
		enum	//Line Type
		{
			LT_OPEN,
			LT_CLOSE,
			LT_SEGMENT,
			LT_MAX
		
		};
		bool CreateIndex(unsigned int uiLineType);
		virtual unsigned int GetTotleNum()const;
		virtual unsigned int GetMeshDateType(){return MDT_LINE;}

		virtual bool CreateCollapseRecord();

		virtual void UpDateClodMesh();
		virtual unsigned int GetActiveNum()const;

		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;

		
	};
	DECLARE_Ptr(VSLineSet);
	VSTYPE_MARCO(VSLineSet);



}
#endif