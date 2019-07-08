#ifndef VSANIMTREE_H
#define VSANIMTREE_H
#include "VSController.h"
#include "VSAnimSet.h"
namespace VSEngine2
{
	class VSAnimFunction;
	class VSAnimMainFunction;
	class VSAnimBaseFunction;
	class VSSkelectonMeshNode;
	class VSStream;
	class VSGRAPHIC_API VSAnimTree : public VSController , public VSResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSAnimTree();
		virtual bool Update(double dAppTime);
	
		VSAnimTree(const VSUsedName &ShowName);
	protected:
		VSAnimTree();
	protected:
		VSArray<VSAnimBaseFunction *> m_pAnimFunctionArray;

		VSAnimMainFunction*  m_pAnimMainFunction;
		static VSPointer<VSAnimTree> Default;
		VSUsedName m_ShowName;
	public:
		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		static const VSAnimTree *GetDefalut()
		{
			return Default;
		}
		VSSkelectonMeshNode * GetSkelectonMeshNode()const;


		void AddAnimFunction(VSAnimBaseFunction * pAnimFunction);
		void DeleteAnimFunction(VSAnimBaseFunction * pAnimFunction);
		FORCEINLINE VSAnimMainFunction * GetAnimMainFunction()const
		{
			return m_pAnimMainFunction;
		}
		virtual bool SetObject(VSObject * pObject);
		void ResetAnimFunction();

		VSAnimBaseFunction *  GetAnimFunctionFromShowName(const VSUsedName & ShowName);
		virtual unsigned int GetResourceType()const
		{
			return RT_ANIMTREE;
		}
		void SetNodePara(const VSUsedName & ShowName,void * pPara);
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	};
	DECLARE_Ptr(VSAnimTree);
	VSTYPE_MARCO(VSAnimTree);
	DECLARE_Proxy(VSAnimTree);
	class VSGRAPHIC_API VSAnimTreeOneAnim : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSAnimTreeOneAnim();
		VSAnimTreeOneAnim(const VSUsedName &ShowName,const VSString &AnimName);
		~VSAnimTreeOneAnim();
	};
	DECLARE_Ptr(VSAnimTreeOneAnim);
	VSTYPE_MARCO(VSAnimTreeOneAnim);

	class VSGRAPHIC_API VSAnimTreeTwoSmoothBlendAnim : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSAnimTreeTwoSmoothBlendAnim();
		VSAnimTreeTwoSmoothBlendAnim(const VSUsedName &ShowName, const VSString &AnimName1, const VSString &AnimName2);
		~VSAnimTreeTwoSmoothBlendAnim();
	};
	DECLARE_Ptr(VSAnimTreeTwoSmoothBlendAnim);
	VSTYPE_MARCO(VSAnimTreeTwoSmoothBlendAnim);

	class VSGRAPHIC_API VSAnimTreeTwoImmediateAnim : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSAnimTreeTwoImmediateAnim();
		VSAnimTreeTwoImmediateAnim(const VSUsedName &ShowName, const VSString &AnimName1, const VSString &AnimName2);
		~VSAnimTreeTwoImmediateAnim();
	};
	DECLARE_Ptr(VSAnimTreeTwoImmediateAnim);
	VSTYPE_MARCO(VSAnimTreeTwoImmediateAnim);

	class VSGRAPHIC_API VSAnimTreePartialAnim : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSAnimTreePartialAnim();
		VSAnimTreePartialAnim(const VSUsedName &ShowName, const VSString &AnimName1, const VSString &AnimName2);
		~VSAnimTreePartialAnim();
	};
	DECLARE_Ptr(VSAnimTreePartialAnim);
	VSTYPE_MARCO(VSAnimTreePartialAnim);

	class VSGRAPHIC_API VSAnimTreeRectBlendAnim : public VSAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSAnimTreeRectBlendAnim();
		VSAnimTreeRectBlendAnim(const VSUsedName &ShowName,
							const VSString &AnimName1,
							const VSString &AnimName2,
							const VSString &AnimName3,
							const VSString &AnimName4);
		~VSAnimTreeRectBlendAnim();
	};
	DECLARE_Ptr(VSAnimTreeRectBlendAnim);
	VSTYPE_MARCO(VSAnimTreeRectBlendAnim);

}
#endif