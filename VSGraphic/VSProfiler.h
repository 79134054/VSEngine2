#ifndef VSPROFILER_H
#define VSPROFILER_H
#include "VSString.h"
#include "VSArray.h"
#include "VSObject.h"
namespace VSEngine2
{
	class VSRenderer;
	class VSProfilerNode;
	struct ProfilerString
	{
		int iX;
		int iY;
		VSColorRGBA rColor;
		VSString sText;
	};
	class VSGRAPHIC_API VSProfiler
	{
	public:
		DECLARE_INITIAL_ONLY
		static void AddProfileNode(VSProfilerNode * pProfilerNode);
		static void Draw();
		static void Draw(VSRenderer * pRenderer);
		static void GetProfilerNameString(const VSString & Name, unsigned int uiLayer,VSString & OutString);
		static void ClearAll();
		static unsigned int ms_uiCurCount;
	protected:
		static VSArray<VSProfilerNode *> & GetProfilerNode()
		{
			static VSArray<VSProfilerNode *> m_pProfilerNodes;
			return m_pProfilerNodes;
		}
		
		static bool InitialDefaultState();
		static VSArray<VSProfilerNode *> m_pRoot;
		static TCHAR * ms_LayerFlag[10];
	};

	class VSGRAPHIC_API VSProfilerNode
	{
	public:
		VSProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		
		virtual void Clear() = 0;
		void AddChild(VSProfilerNode * pChild);
		void Draw( unsigned int uiLayer);
	protected:
		virtual void OnDraw( unsigned int uiLayer) = 0;
		friend class VSProfilerNodeTImer;
		friend class VSProfiler;
		
		VSProfilerNode * m_pParentNode;
		VSArray<VSProfilerNode*>m_pChildNode;
		VSString m_Name;
		VSString m_ParentName;
	};
	class VSGRAPHIC_API VSTimeProfilerNode : public VSProfilerNode
	{
	public:
		class VSGRAPHIC_API VSProfilerNodeTImer
		{
		public:
			VSProfilerNodeTImer(VSTimeProfilerNode * pProfilerNode);
			~VSProfilerNodeTImer();
		protected:
			VSTimeProfilerNode * m_pOwner;
			VSREAL m_fBeginTime;
		};
		VSTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
			m_fProfilerTime = 0.0f;
		}
		virtual void OnDraw(unsigned int uiLayer);
	protected:
		VSREAL m_fProfilerTime;
	};
	class VSGRAPHIC_API VSCountProfilerNode : public VSProfilerNode
	{
	public:
		class VSGRAPHIC_API VSProfilerNodeCounter
		{
		public:
			VSProfilerNodeCounter(VSCountProfilerNode * pProfilerNode, int uiCount);
		};
		VSCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
			m_uiCounter = 0;
		}
		virtual void OnDraw( unsigned int uiLayer);
	protected:
		int m_uiCounter;
	};
	class VSGRAPHIC_API VSNoClearCountProfilerNode : public VSCountProfilerNode
	{
	public:
		VSNoClearCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
		}
	};
	class VSGRAPHIC_API VSOnlyTimeProfilerNode : public VSProfilerNode
	{
	public:
		class VSGRAPHIC_API VSProfilerNodeOnlyTImer
		{
		public:
			VSProfilerNodeOnlyTImer(VSOnlyTimeProfilerNode * pProfilerNode, float fProfilerTime);
		};
		VSOnlyTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
			m_fProfilerTime = 0.0f;
		}
		virtual void OnDraw( unsigned int uiLayer);
	protected:
		VSREAL m_fProfilerTime;
	};
#ifdef PROFILER
#define DECLEAR_TIME_PROFILENODE(Name,ParentName)  VSTimeProfilerNode TimeProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_TIME_PROFILE(Name) VSTimeProfilerNode::VSProfilerNodeTImer ProfilerNodeTimer_##Name(&TimeProfilerNode_##Name);

#define DECLEAR_COUNT_PROFILENODE(Name,ParentName)  VSCountProfilerNode CountProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define DECLEAR_NOCLEAR_COUNT_PROFILENODE(Name,ParentName)  VSNoClearCountProfilerNode CountProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_COUNT_PROFILE(Name,Count) VSCountProfilerNode::VSProfilerNodeCounter ProfilerNodeCounter_##Name(&CountProfilerNode_##Name,Count);

#define DECLEAR_ONLYTIME_PROFILENODE(Name,ParentName)  VSOnlyTimeProfilerNode OnlyTimeProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_ONLYTIME_PROFILE(Name,fProfilerTime) VSOnlyTimeProfilerNode::VSProfilerNodeOnlyTImer ProfilerNodeOnlyTimer_##Name(&OnlyTimeProfilerNode_##Name,fProfilerTime);
#else
#define DECLEAR_TIME_PROFILENODE(Name,ParentName)  
#define ADD_TIME_PROFILE(Name) 

#define DECLEAR_COUNT_PROFILENODE(Name,ParentName)  
#define DECLEAR_NOCLEAR_COUNT_PROFILENODE(Name,ParentName)  
#define ADD_COUNT_PROFILE(Name,Count) 

#define DECLEAR_ONLYTIME_PROFILENODE(Name,ParentName)  
#define ADD_ONLYTIME_PROFILE(Name,fProfilerTime) 
#endif
}
#endif