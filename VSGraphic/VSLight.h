#ifndef VSLIGHT_H
#define VSLIGHT_H
#include "VSNodeComponent.h"
#include "VSVector3.h"
#include "VSVector3W.h"
#include "VSResource.h"
#include "VSVector2.h"
#include "VSCuller.h"
namespace VSEngine2
{
	class VSShadowMapSceneRender;
	class VSStream;
	class VS2DTexture;
	class VSCamera;
	DECLARE_Ptr(VSTexAllState);
	DECLARE_Ptr(VSMaterialInstance);
	DECLARE_Ptr(VSMaterial);
	DECLARE_Proxy(VSMaterial);
	DECLARE_Ptr(VSRenderTarget);
	class VSGRAPHIC_API VSLight : public VSNodeComponent
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class VSSpatial;
		friend class VSScene;
		enum	//LightType
		{
			LT_POINT,
			LT_SPOT,
			LT_DIRECTION,
			LT_SKY,
			LT_MAX
		};
		VSLight();
		virtual ~VSLight() = 0;
	public:
		virtual void UpdateAll(double dAppTime);
		virtual unsigned int GetLightType()const = 0;
		virtual bool IsRelative(VSGeometry * pGeometry);
		virtual bool Cullby(VSCuller & Culler);
		virtual void UpdateLightState(double dAppTime);
		virtual void GetLightRange() = 0;
		virtual void UpdateTransform(double dAppTime);
		virtual void BuildSceneInfo(VSScene * pScene);
	};
	DECLARE_Ptr(VSLight);
	VSTYPE_MARCO(VSLight);
	
	DECLARE_Ptr(VSLightMaterialSceneRender);
	class VSGRAPHIC_API VSLocalLight : public VSLight
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSLocalLight();
		virtual ~VSLocalLight() = 0;
		VSColorRGBA m_Diffuse;
		VSColorRGBA m_Specular;
		virtual void DrawNoDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableNoDepandenceShadowMap( double dAppTime);
		virtual void DrawDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(VSCuller & CurCuller, double dAppTime);
		virtual void UpdateAll(double dAppTime);
		virtual void DrawLightMaterial(double dAppTime);
		void SetCastShadow(bool bIsCastShadow);
		FORCEINLINE bool GetCastShadow()
		{
			return m_bIsCastShadow;
		}
		FORCEINLINE void AddShadowScene(VSScene * pSecen)
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
		
		FORCEINLINE void ClearAllShadowScene()
		{
			m_pScene.Clear();
		}
		virtual VSTexAllState * GetShadowTexture(unsigned int ID = 0)
		{
			return m_pShadowTexture[ID];	
		}
		unsigned int GetShadowTextureNum()
		{
			return m_pShadowTexture.GetNum();
		}
		unsigned int GetShadowResolution()const
		{
			return m_uiRTWidth;
		}
		FORCEINLINE VSTexAllState * GetLightFunDiffuseTexture()const
		{
			return m_pLightFunDiffuseTexture;
		}
		virtual bool SetLightMaterial(VSMaterialR * pMaterial);
		VSREAL m_ZBias;
		VSMatrix3X3W m_LightShadowMatrix;
		void SetLightMaterialRtWidth(unsigned int uiRtWidth);

		FORCEINLINE VSMaterialInstance * GetMaterialInstance()const
		{
			return m_pLightMaterial;
		}
		FORCEINLINE bool HaveLightFun()const
		{
			return (m_pLightMaterial != NULL);
		}
		unsigned int GetLightMaterialRtWidth()
		{
			return m_uiLightMaterialRTWidth;
		}
		FORCEINLINE VSMatrix3X3W &GetMVP()
		{
			return m_MVP;
		}
		virtual void BuildSceneInfo(VSScene * pScene);
		VSVector2 m_LightFunScale;
		VSVector2 m_LightFunOffset;
		VSColorRGBA m_ProjectShadowColor;
		VSREAL m_ProjectShadowFallOff;
	protected:  
		bool m_bIsCastShadow;
		unsigned int m_uiRTWidth;
		VSArray<VSScene *> m_pScene;
		VSArray<VSTexAllStatePtr> m_pShadowTexture;
		VSTexAllStatePtr m_pLightFunDiffuseTexture;
		VSMaterialInstancePtr	m_pLightMaterial;
		unsigned int m_uiLightMaterialRTWidth;
		VSLightMaterialSceneRenderPtr m_pLMSceneRender;
		VSRenderTargetPtr m_pLightFunDiffuseRenderTarget;
		VSRenderTargetPtr m_pLightFunSpecularRenderTarget;
		VSMatrix3X3W m_MVP;
		virtual void GetLightRange() = 0;
		void GetCullerAABBArray(VSCuller & Culler, VSArray<VSAABB3> & AABBArray, unsigned int GetFlag = VSCuller::GF_ALL);
		bool m_bShadowMapDrawEnd;
	};
	DECLARE_Ptr(VSLocalLight);
	VSTYPE_MARCO(VSLocalLight);
	class VSGRAPHIC_API VSIndirectLight : public VSLight
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSIndirectLight();
		virtual ~VSIndirectLight() = 0;
		virtual bool IsRelative(VSGeometry * pGeometry);
		virtual bool Cullby(VSCuller & Culler);
		virtual void SetLocalScale(const VSVector3 & fScale);
		virtual void SetLocalRotate(const VSMatrix3X3 & Rotate);
		virtual void SetLocalTransform(const VSTransform & LocalTransform);
		virtual void SetLocalMat(const VSMatrix3X3W VSMat);
		FORCEINLINE void SetRange(VSVector3 & Range)
		{
			m_Range.x = Range.x > 1.0f ? Range.x : m_Range.x;
			m_Range.y = Range.y > 1.0f ? Range.y : m_Range.y;
			m_Range.z = Range.z > 1.0f ? Range.z : m_Range.z;
			GetLightRange();
		}
	protected:
		VSVector3 m_Range;
		virtual void GetLightRange();
		VSAABB3 m_WorldRenderBV;
	};
	DECLARE_Ptr(VSIndirectLight);
	VSTYPE_MARCO(VSIndirectLight);
}
#endif