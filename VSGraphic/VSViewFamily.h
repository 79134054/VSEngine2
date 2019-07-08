#ifndef VSVIEWFAMILY_H
#define VSVIEWFAMILY_H
#include "VSSystem.h"
#include "VSReference.h"
#include "VSCamera.h"
#include "VSSceneRender.h"
#include "VSCubeTexture.h"
namespace VSEngine2
{
	DECLARE_Proxy(VSPostEffectSet);
	class VSGRAPHIC_API VSSceneRenderMethod : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		VSSceneRenderMethod();
		virtual ~VSSceneRenderMethod() = 0;
		void SetPostEffect(VSPostEffectSetR* pPostEffectSet);
		virtual void Draw(VSCuller & Culler,double dAppTime);
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		virtual void DisableRT();
		FORCEINLINE void SetDebugDrawInfo(bool bDrawDebugInfo, bool bBeforePostDebugDraw = true)
		{
			m_bBeforePostDebugDraw = bBeforePostDebugDraw;
			m_bDrawDebugInfo = bDrawDebugInfo;
		}
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		VSRenderTarget * GetFinalColorRT();

		VSPostEffectSet * GetPostEffectSet()const
		{
			return m_pPostEffectInstance;
		}
		VSDebugDraw * GetDebugDraw(unsigned int uiRenderGroup);
		virtual void SetUseState(VSRenderState & RenderState,unsigned int uiRenderStateInheritFlag) = 0;
		virtual void ClearUseState() = 0;

		// must use in viewfamily OnDraw , every frame you call this , and pFinalColorRT  must youself manage.
		virtual void SetColorRT(VSRenderTarget * pFinalColorRT,unsigned int uiWidth,unsigned int uiHeight);
	protected:
		VSPostEffectSetRPtr m_pPostEffectSet;
		VSPostEffectSetPtr	m_pPostEffectInstance;
		VSDebugDrawSceneRenderPtr m_pDebugDrawSceneRender;
		bool m_bBeforePostDebugDraw;
		bool m_bDrawDebugInfo;
		VSDepthStencilPtr m_pDepthStencil[VSCuller::RG_MAX];
		VSRenderTargetPtr m_pColorRT;
		bool ColorRTIsOutSet;
	};
	DECLARE_Ptr(VSSceneRenderMethod);
	VSTYPE_MARCO(VSSceneRenderMethod);
	class VSGRAPHIC_API VSForwordHighEffectSceneRenderMethod : public VSSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSForwordHighEffectSceneRenderMethod();
		~VSForwordHighEffectSceneRenderMethod();
		virtual void Draw(VSCuller & Culler,double dAppTime);
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(VSRenderState & RenderState,unsigned int uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:	
		VSMaterialSceneRenderPtr m_pMaterialSceneRenderder;
		VSNormalDepthSceneRenderPtr m_pNormalDepthSceneRender;
		VSGammaCorrectSceneRenderPtr m_pGammaCorrectSceneRender;
		VSPESSRSceneRenderPtr m_pSSRSceneRender;
		VSRenderTargetPtr m_pMaterialRT;
		VSRenderTargetPtr m_pNormalDepthRT;
		VSRenderTargetPtr m_pSSRRT;
	};
	DECLARE_Ptr(VSForwordHighEffectSceneRenderMethod);
	VSTYPE_MARCO(VSForwordHighEffectSceneRenderMethod);
	
	class VSGRAPHIC_API VSForwordEffectSceneRenderMethod : public VSSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSForwordEffectSceneRenderMethod();
		~VSForwordEffectSceneRenderMethod();
		virtual void Draw(VSCuller & Culler, double dAppTime);
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:
		VSMaterialSceneRenderPtr m_pMaterialSceneRenderder;
		VSGammaCorrectSceneRenderPtr m_pGammaCorrectSceneRender;
		VSRenderTargetPtr m_pMaterialRT;
	};
	DECLARE_Ptr(VSForwordEffectSceneRenderMethod);
	VSTYPE_MARCO(VSForwordEffectSceneRenderMethod);

	class VSGRAPHIC_API VSSimpleForwordEffectSceneRenderMethod : public VSSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSimpleForwordEffectSceneRenderMethod();
		~VSSimpleForwordEffectSceneRenderMethod();
		virtual void Draw(VSCuller & Culler, double dAppTime);
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:
		VSMaterialSceneRenderPtr m_pMaterialSceneRenderder;
	};
	DECLARE_Ptr(VSSimpleForwordEffectSceneRenderMethod);
	VSTYPE_MARCO(VSSimpleForwordEffectSceneRenderMethod);
	class VSGRAPHIC_API VSViewFamily : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		
		VSViewFamily(const VSString &ViewFamilyName, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName);
		VSViewFamily(const VSString &ViewFamilyName,VSCamera * pCamera);
		virtual ~VSViewFamily();
		FORCEINLINE void AddScene(VSScene* pScene);
		FORCEINLINE VSScene * GetScene(unsigned int i)const;
		FORCEINLINE void ClearAllScene();
		FORCEINLINE void DeleteScene(unsigned int i);
		FORCEINLINE void DeleteScene(VSScene * pScene);
		FORCEINLINE unsigned int GetSceneNum()const;
		virtual void Draw(double dAppTime);
		virtual void OnDraw(double dAppTime);
		virtual void Update(double dAppTime);

		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		bool IsReCreate(unsigned int uiWidth,unsigned int uiHeight);

		VSString m_ViewFamilyName;
		bool m_bEnable;

		VSSceneRenderMethodPtr m_pSceneRenderMethod;

		virtual VSRenderTarget * GetFinalColorRT();

		void SetCullUseID(int USeID = -1)
		{
			m_Culler.SetUseMaterialIndex(USeID);
		}
	protected:
		VSCamera* m_pCamera;
		VSArray<VSScene *> m_pScene;
		VSCuller	m_Culler;

	protected:
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;	


		
	};
	FORCEINLINE void VSViewFamily::AddScene(VSScene * pSecen)
	{
		if(pSecen)
		{
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				if (m_pScene[i] == pSecen)
				{
					return ;
				}
			}
			m_pScene.AddElement(pSecen);
		}
	}
	FORCEINLINE VSScene * VSViewFamily::GetScene(unsigned int i)const
	{
		if(i >= m_pScene.GetNum())
			return NULL;
		return m_pScene[i];
	}
	FORCEINLINE void VSViewFamily::ClearAllScene()
	{
		m_pScene.Clear();
	}
	FORCEINLINE void VSViewFamily::DeleteScene(unsigned int i)
	{	
		if(i >= m_pScene.GetNum())
			return ;
		m_pScene.Erase(i);

	}
	FORCEINLINE void VSViewFamily::DeleteScene(VSScene * pScene)
	{
		if (!pScene)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pScene.GetNum() ; i++)
		{
			if(pScene == m_pScene[i])
			{
				m_pScene.Erase(i);
			}
		}
	}

	FORCEINLINE unsigned int VSViewFamily::GetSceneNum()const
	{
		return m_pScene.GetNum();
	}
	class VSGRAPHIC_API VSWindowViewFamily : public VSViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		enum 
		{
			VT_WINDOW_NORMAL,
			VT_WINDOW_MAX
		};
		VSWindowViewFamily(const VSString &ViewFamilyName, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName, int iWindowID = -1);
		VSWindowViewFamily(const VSString &ViewFamilyName,VSCamera * pCamera,int iWindowID = -1);
		virtual ~VSWindowViewFamily();
		virtual void Draw(double dAppTime);
		virtual void OnDraw(double dAppTime);
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);

	protected:
		int m_iWindowID;
		VSPEScreenQuadSceneRenderPtr  m_pScreenQuadRenderer;
	};

	class VSGRAPHIC_API VSCaptureViewFamily : public VSViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		enum //ViewFamily Type
		{
			VT_CAPTURE_2D_CAPTURE_NORMAL,
			VT_CAPTURE_CUB_CAPTURE_NORMAL,
			VT_MAX
		};
		VSCaptureViewFamily(const VSString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName);
		VSCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera);
		virtual ~VSCaptureViewFamily();
		virtual void OnDraw(double dAppTime);
		void SetSize(unsigned int uiWidth,unsigned int uiHeight);
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		virtual VSTexture * GetTexture()const = 0;
		
		virtual void Draw(double dAppTime);
		friend class VSCaptureTexAllState;
	protected:
		VSPEScreenQuadSceneRenderPtr  m_pScreenQuadRenderer;
		VSCaptureTexAllState*  m_pTexOwner;
		bool m_OnlyUpdateOneTime;
		
	};
	class VSGRAPHIC_API VSCubCaptureViewFamily : public VSCaptureViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSCubCaptureViewFamily(const VSString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName);
		VSCubCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera);
		virtual ~VSCubCaptureViewFamily();
		virtual void OnDraw(double dAppTime);
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		virtual void Update(double dAppTime);
		virtual VSTexture * GetTexture()const;
	protected:
		VSCameraPtr CubCameraPtr[VSCubeTexture::F_MAX];
		VSCuller m_CubCuller[VSCubeTexture::F_MAX];
		VSRenderTargetPtr m_pCubRenderTarget[VSCubeTexture::F_MAX];
		
	};

	class VSGRAPHIC_API VS2DCaptureViewFamily : public VSCaptureViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VS2DCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera,VSPostEffectSetR* pPostEffectSet,const TCHAR * RenderMethodRTTIName);
		VS2DCaptureViewFamily(const VSString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,VSCamera * pCamera);
		virtual ~VS2DCaptureViewFamily();
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		virtual VSTexture * GetTexture()const;
		virtual void OnDraw(double dAppTime);
	protected:
		VSRenderTargetPtr m_pRenderTarget;

	};

}
#endif