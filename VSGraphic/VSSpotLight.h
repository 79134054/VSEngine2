#ifndef VSSPOTLIGHT_H
#define VSSPOTLIGHT_H
#include "VSLight.h"
#include "VSSceneRender.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSSpotLight : public VSLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //ShadowType
		{
			ST_NORMAL,
			ST_VOLUME,
			ST_PROJECT,
			ST_MAX
		};
		VSSpotLight();
		virtual ~VSSpotLight();

	public:
		FORCEINLINE void Set(VSREAL		Range,
			VSREAL		Falloff,
			VSREAL		Theta,
			VSREAL		Phi)
		{
			m_Range = Range;
			m_Falloff = Falloff;
			m_Theta = Theta;
			m_Phi = Phi;
			GetLightRange();
		}
		FORCEINLINE VSREAL GetRange()const
		{
			return m_Range;
		}
		FORCEINLINE VSREAL GetFalloff()const
		{
			return m_Falloff;
		}
		FORCEINLINE VSREAL GetTheta()const
		{
			return m_Theta;
		}
		FORCEINLINE VSREAL GetPhi()const
		{
			return m_Phi;
		}
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual bool PostLoad(void * pDate = NULL);
		virtual unsigned int GetLightType()const{return LT_SPOT;}
		virtual bool Cullby(VSCuller & Culler);
		virtual bool IsRelative(VSGeometry * pGeometry);
		void ResetShadow();
		void SetShadowType(unsigned int uiShadowType);
		virtual void DrawDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DrawNoDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableNoDepandenceShadowMap(double dAppTime);
		FORCEINLINE unsigned int GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
	protected:
		VSShadowMapSceneRenderPtr m_pShadowMapSceneRender;
		unsigned int m_uiShadowType;
		void DrawNormalShadowMap(VSCuller & CurCuller,double dAppTime);		

		VSRenderTargetPtr m_pShadowRenderTarget;
		VSProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;

		virtual void GetLightRange();
		VSAABB3 m_WorldRenderBV;
		VSREAL		m_Range;
		VSREAL		m_Falloff;
		VSREAL		m_Theta;
		VSREAL		m_Phi;
	};
	DECLARE_Ptr(VSSpotLight);
	VSTYPE_MARCO(VSSpotLight);

}
#endif