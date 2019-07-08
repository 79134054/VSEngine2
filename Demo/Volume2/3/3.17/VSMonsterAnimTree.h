#ifndef VSMONSTERANIMTREE_H
#define VSMONSTERANIMTREE_H
#include "VSAnimTree.h"
namespace VSEngine2
{
	class VSMonsterAnimTree1 : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSMonsterAnimTree1();
		VSMonsterAnimTree1(const VSUsedName &ShowName);
		~VSMonsterAnimTree1();
	};
	DECLARE_Ptr(VSMonsterAnimTree1);
	VSTYPE_MARCO(VSMonsterAnimTree1);

	class VSMonsterAnimTree2 : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSMonsterAnimTree2();
		VSMonsterAnimTree2(const VSUsedName &ShowName);
		~VSMonsterAnimTree2();
	};
	DECLARE_Ptr(VSMonsterAnimTree2);
	VSTYPE_MARCO(VSMonsterAnimTree2);
}
#endif
