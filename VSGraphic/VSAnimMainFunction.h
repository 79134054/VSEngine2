#ifndef VSANIMMAINFUNCTION_H
#define VSANIMMAINFUNCTION_H
#include "VSAnimBaseFunction.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSAnimMainFunction : public VSAnimBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSAnimMainFunction();
		virtual bool Update(double dAppTime);
		VSAnimMainFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
	protected:
		VSAnimMainFunction();


	};
	DECLARE_Ptr(VSAnimMainFunction);
	VSTYPE_MARCO(VSAnimMainFunction);
}
#endif