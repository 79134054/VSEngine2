#ifndef VSMORPHMAINFUNCTION_H
#define VSMORPHMAINFUNCTION_H
#include "VSMorphBaseFunction.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSMorphMainFunction : public VSMorphBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSMorphMainFunction();
		virtual bool Update(double dAppTime);

		VSMorphMainFunction(const VSUsedName & ShowName,VSMorphTree * pMorphTree);
		//void CreateNode(const VSString & NodeName,unsigned int uiNodeType);
	protected:
		VSMorphMainFunction();
		void MainThreadUpdate(VSMorphFunction *pMorphFunction);
		void RenderThreadUpdate(VSMorphFunction *pMorphFunction);


	};
	DECLARE_Ptr(VSMorphMainFunction);
	VSTYPE_MARCO(VSMorphMainFunction);

}
#endif