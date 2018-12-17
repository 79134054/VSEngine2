#ifndef VSSKELECTONMESHNODE_H
#define VSSKELECTONMESHNODE_H
#include "VSModelMeshNode.h"
#include "VSSkelecton.h"
#include "VSAnimSet.h"
#include "VSAnimSequenceFunc.h"
#include "VSSocketNode.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSSkelectonMeshNode : public VSModelMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkelectonMeshNode();
		virtual ~VSSkelectonMeshNode();
		
		virtual unsigned int GetResourceType()const
		{
			return RT_SKELECTON_MODEL;
		}
	protected:
		VSSkelectonPtr m_pSkelecton;
		VSAnimSetPtr   m_pAnimSet;
		VSAnimSequenceFuncPtr m_pAnimSequence;

	public:
		void SetSkelecton(VSSkelecton * pSkelecton);
		void SetAnimSet(VSAnimSet * pAnimSet);
		void SetAnimTree(VSAnimTreeR * pAnimTree);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		FORCEINLINE VSSkelecton * GetSkelecton()const
		{
			return m_pSkelecton;
		}
		FORCEINLINE VSAnimSet * GetAnimSet()const
		{
			return m_pAnimSet;
		}
		FORCEINLINE void SetIsDrawSkelecton(bool bIsDrawSkelecton)
		{
			if (m_pSkelecton)
			{
				m_pSkelecton->m_bIsDrawSkelecton = bIsDrawSkelecton;
			}
		}

		FORCEINLINE bool IsDrawSkelecton()const
		{
			if (m_pSkelecton)
			{
				return m_pSkelecton->m_bIsDrawSkelecton;
			}
			return 0;
		}
		virtual void UpdateController(double dAppTime);
		//can not use with anim tree
		bool PlayAnim(const VSString & AnimName,VSREAL fRatio = 1.0f,unsigned int uiRepeatType = VSController::RT_NONE);
		void StopAnim();

		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateAll(double dAppTime);
		VSSocketNode * CreateSocket(const VSUsedName & BoneName,const VSUsedName &SocketName);
		void DeleteSocket(const VSUsedName &SocketName);
		VSSocketNode * GetSocket(const VSUsedName &SocketName);
		void SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara);
	protected:
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
	protected:
		static VSPointer<VSSkelectonMeshNode> Default;
		VSAnimTreeRPtr m_pAnimTree;
		
		VSAnimTreePtr m_pAnimTreeInstance;
		
		void UpdateLocalAABB();
		VSArray<VSSocketNodePtr> m_pSocketArray;
	public:
		static const VSSkelectonMeshNode *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	};
	DECLARE_Ptr(VSSkelectonMeshNode);
	VSTYPE_MARCO(VSSkelectonMeshNode);
}
#endif