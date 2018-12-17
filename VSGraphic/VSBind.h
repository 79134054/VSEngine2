#ifndef VSBIND_H
#define VSBIND_H
#include "VSObject.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSResourceIdentifier
	{
	public:
		virtual ~VSResourceIdentifier () {/**/}

	protected:
		VSResourceIdentifier () {/**/}
	};
	class VSStream;
	class VSGRAPHIC_API VSBind : public VSObject
	{
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum	//Lock Flag
		{
			LF_NOOVERWRITE,
			LF_DISCARD,		
			LF_READONLY,
			LF_MAX
		};
		enum	//Mem Type
		{
			MT_BOTH,
			MT_RAM,
			MT_VRAM,
			MT_MAX	
		};
		enum	// MemType Clear State
		{
			MCS_NONE,
			MCS_READY,
			MCS_DONE,
			MCS_MAX
		};
		VSBind(unsigned int uiMemType = MT_BOTH);
		virtual ~VSBind() = 0;

		VSResourceIdentifier* GetIdentifier();
		unsigned int GetInfoQuantity () const;
		bool IsStatic()const {return m_bIsStatic;}
		//如果此资源需要lock，并且是多线程渲染，为了有效利用多线效率，lock函数如果在主线程使用请设置bmulthreadUse为true
		void SetStatic(bool bIsStatic,bool bMulThreadUse = false);
		static VSArray<VSBind *> ms_DynamicTwoBindArray;
		static VSArray<VSBind *> ms_BindArray;
		void ExChange();
		virtual void ASYNClearInfo();
		
	protected:
		virtual void ClearInfo();
		friend class VSRenderer;

		void Bind(VSResourceIdentifier* pID);

		struct INFO_TYPE
		{
			INFO_TYPE()
			{
				ID = NULL;
			}
			~INFO_TYPE()
			{

			}
			VSResourceIdentifier* ID;
		};
		VSArray<INFO_TYPE> m_InfoArray;
		unsigned int m_uiSwapChainNum;
		unsigned int m_uiCurID;
		bool m_bIsStatic;
		VSRenderer* m_pUser;
		

		unsigned int m_uiLockFlag;

		unsigned int m_uiMemType;
		unsigned int m_uiClearState;
	public:


		virtual	bool LoadResource(VSRenderer * pRender);
		virtual bool ReleaseResource();


		virtual unsigned int GetByteSize()const;

		FORCEINLINE void SetLockFlag(unsigned int uiLockFlag)
		{
			if (uiLockFlag > LF_MAX)
			{
				uiLockFlag = LF_NOOVERWRITE;
			}
			m_uiLockFlag = uiLockFlag;
		}
		FORCEINLINE unsigned int GetLockFlag()const
		{
			return m_uiLockFlag;
		}

		FORCEINLINE void SetMemType(unsigned int uiMemType)
		{
			if (uiMemType < MT_MAX)
			{
				m_uiMemType = uiMemType;
			}
		}

		FORCEINLINE unsigned int GetMemType()const
		{
			return m_uiMemType;
		}
		FORCEINLINE unsigned int GetSwapChainNum()const
		{
			return m_uiSwapChainNum;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID) = 0;		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID) = 0;
		
	};
	DECLARE_Ptr(VSBind);
	VSTYPE_MARCO(VSBind);

}
#endif