#ifndef VSMORPHTREE_H
#define VSMORPHTREE_H
#include "VSController.h"
#include "VSModelMeshNode.h"
namespace VSEngine2
{
	class VSMorphBaseFunction;
	class VSMorphFunction;
	class VSMorphMainFunction;
	class VSStream;
	class VSGRAPHIC_API VSMorphTree : public VSController,public VSResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSMorphTree();
		virtual bool Update(double dAppTime);

		VSMorphTree(const VSUsedName &ShowName);
	protected:
		VSMorphTree();
	protected:

		VSArray<VSMorphBaseFunction *> m_pMorphFunctionArray;

		VSMorphMainFunction*  m_pMorphMainFunction;
		static VSPointer<VSMorphTree> Default;
		VSUsedName m_ShowName;
		bool m_bCompressData;
	public:
		FORCEINLINE bool IsCompressData()const
		{
			return m_bCompressData;
		}
		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		static const VSMorphTree *GetDefalut()
		{
			return Default;
		}
		FORCEINLINE VSModelMeshNode * GetMeshNode()const
		{
			return DynamicCast<VSModelMeshNode>(m_pObject);
		}
		void ClearChangeFlag();
		void AddMorphFunction(VSMorphBaseFunction * pMorphFunction);
		void DeleteMorphFunction(VSMorphBaseFunction * pMorphFunction);
		FORCEINLINE VSMorphMainFunction * GetMorphMainFunction()const
		{
			return m_pMorphMainFunction;
		}
		virtual bool SetObject(VSObject * pObject);
		virtual unsigned int GetResourceType()const
		{
			return RT_MORPHTREE;
		}
		VSMorphBaseFunction *  GetMorphFunctionFromShowName(const VSUsedName & ShowName);
		void SetNodePara(const VSUsedName & ShowName, void * pPara);
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	};
	DECLARE_Ptr(VSMorphTree);
	VSTYPE_MARCO(VSMorphTree);
	DECLARE_Proxy(VSMorphTree);

	class VSGRAPHIC_API VSMorphTreeTwoSmoothBlendAnim : public VSMorphTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		VSMorphTreeTwoSmoothBlendAnim();
		VSMorphTreeTwoSmoothBlendAnim(const VSUsedName &ShowName, const VSString &MorphName);
		~VSMorphTreeTwoSmoothBlendAnim();
	};
	DECLARE_Ptr(VSMorphTreeTwoSmoothBlendAnim);
	VSTYPE_MARCO(VSMorphTreeTwoSmoothBlendAnim);
}
#endif