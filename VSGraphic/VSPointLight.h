#ifndef VSPOINTLIGHT_H
#define VSPOINTLIGHT_H
#include "VSLight.h"
#include "VSCubeTexture.h"
#include "VSRenderTarget.h"
#include "VSDepthStencil.h"
#include "VSSceneRender.h"
namespace VSEngine2
{
	//cube shadow 限制在于光源距离物体太远，rt分辨率不够，存放具体精度不够
	//volume 限制在于参与投影的物体必须是闭合的，表面是光滑的，不同光滑组有棱角的地方，要求内嵌三角形。
	class VSStream;
	class VSGRAPHIC_API VSPointLight : public VSLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPointLight();
		virtual ~VSPointLight();
	public:
		
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual bool PostLoad(void * pDate = NULL);
		
		FORCEINLINE void SetRange(VSREAL Range)
		{
			m_Range = Range;
			GetLightRange();
		}
		FORCEINLINE VSREAL GetRange()const
		{
			return m_Range;
		}
		virtual void BuildSceneInfo(VSScene * pScene);
		virtual unsigned int GetLightType()const{return LT_POINT;}
		virtual void DrawDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DrawNoDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableNoDepandenceShadowMap(double dAppTime);
		enum //ShadowType
		{
			ST_CUB,
			ST_VOLUME,
			ST_DUAL_PARABOLOID,
			ST_PROJECT,
			ST_MAX
		};
		virtual bool Cullby(VSCuller & Culler);
		void SetShadowType(unsigned int uiShadowType);
		FORCEINLINE unsigned int GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual bool IsRelative(VSGeometry * pGeometry);
		virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(VSCuller & CurCuller, double dAppTime);
	protected:
		void ResetShadow();

		void DrawNormalCubShadow(VSCuller & CurCuller,double dAppTime);
		void DrawFovCubShadow(VSCuller & CurCuller,double dAppTime);

		void DrawNormalProjectShadow(VSCuller & CurCuller,double dAppTime);
		void DrawFovProjectShadow(VSCuller & CurCuller,double dAppTime);

		void DrawDualParaboloidShadow(VSCuller & CurCuller, double dAppTime);


		VSProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;
		
		unsigned int m_uiShadowType;
		VSRenderTargetPtr m_pCubRenderTarget[VSCubeTexture::F_MAX];
		
		VSShadowMapSceneRenderPtr m_pShadowMapSceneRender;

		VSPEVolumeShadowMapSceneRenderPtr m_pPEVolumeSMSceneRender;
		VSVolumeShadowSceneRenderPtr m_pVolumeShadowSceneRender;

		VSRenderTargetPtr m_pVolumeShadowRenderTarget;
		VSRenderTargetPtr m_pVolumeShadowFirstPassRenderTarget;

#define m_pDualParaboloidRenderTarget0 m_pVolumeShadowFirstPassRenderTarget
#define m_pDualParaboloidRenderTarget1 m_pVolumeShadowRenderTarget

#define m_pProjectShadowRenderTarget m_pVolumeShadowRenderTarget

		
		virtual void GetLightRange();
		VSSphere3 m_WorldRenderBV;
		VSREAL		m_Range;
	public:
		friend class VSVolumeShadowMapCuller;
		friend class VSDualParaboloidCuller;
	};
	DECLARE_Ptr(VSPointLight);
	VSTYPE_MARCO(VSPointLight);


}
#endif