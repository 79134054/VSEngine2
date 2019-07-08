#ifndef VSANIMFUNCTION_H
#define VSANIMFUNCTION_H

#include "VSController.h"
#include "VSAnimTree.h"
#include "VSAnimBaseFunction.h"
#include "VSAnimAtom.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSAnimFunction : public VSAnimBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~VSAnimFunction() = 0;
		//必须判断是否是SkelectonMeshNode,必须有skelecton
		VSAnimFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		virtual bool Update(double dAppTime);
	protected:
		VSAnimFunction();
	public:
		VSArray<VSAnimAtom> m_BoneOutPut;
		VSAnimAtom m_RootAtom;
	public:
		virtual bool SetObject(VSObject * pObject);
		virtual VSAnimAtom & GetRootTranfform();
	};
	DECLARE_Ptr(VSAnimFunction);
	VSTYPE_MARCO(VSAnimFunction);

}
#endif