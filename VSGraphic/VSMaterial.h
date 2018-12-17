#ifndef VSMaterial_H
#define VSMaterial_H
#include "VSObject.h"
#include "VSShaderMainFunction.h"
#include "VSTexAllState.h"
#include "VSLight.h"
#include "VSResource.h"
#include "VSMaterialPass.h"
#include "VSNormalDepthPass.h"
#include "VSShaderKey.h"
#include "VSRenderState.h"
#include "VSPass.h"
#include "VSShadowPass.h"
#include "VSPrezPass.h"
#include "VSLightShaderFunction.h"
#include "VSPostEffectShaderFunction.h"
#include "VSLightFunPass.h"
#include "VSIndirectRenderPass.h"
#include "VSPostEffectPass.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API MaterialCustomPara : public VSObject
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
		MaterialCustomPara();
		virtual ~MaterialCustomPara() = 0;

		VSUsedName ConstValueName;

		MaterialCustomPara & operator =(const MaterialCustomPara &Para)
		{
			ConstValueName = Para.ConstValueName;
			return *this;
		}
		VSGRAPHIC_API friend bool operator >(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName > Para1.ConstValueName;
		}
		VSGRAPHIC_API friend bool operator <(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName < Para1.ConstValueName;
		}
		VSGRAPHIC_API friend bool operator ==(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName == Para1.ConstValueName;
		}
	};
	DECLARE_Ptr(MaterialCustomPara);
	VSTYPE_MARCO(MaterialCustomPara);
	class VSGRAPHIC_API CustomFloatValue : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY

		DECLARE_INITIAL
		CustomFloatValue();
		virtual ~CustomFloatValue();
		VSArray<VSREAL> Value;
		CustomFloatValue & operator =(const CustomFloatValue &Para)
		{
			MaterialCustomPara::operator =(Para);
			Value = Para.Value;
			return *this;
		}
		
	};
	DECLARE_Ptr(CustomFloatValue);
	VSTYPE_MARCO(CustomFloatValue);
	class CustomTexSampler : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
		CustomTexSampler();
		virtual ~CustomTexSampler();
		VSTexAllStateRPtr m_pTexture;
		CustomTexSampler & operator =(const CustomTexSampler &Para)
		{
			MaterialCustomPara::operator =(Para);
			m_pTexture = Para.m_pTexture;
			return *this;
		}
	};
	DECLARE_Ptr(CustomTexSampler);
	VSTYPE_MARCO(CustomTexSampler);
	class VSGRAPHIC_API VSMaterialInterface : public VSObject , public VSResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		
		VSMaterialInterface();
		virtual ~VSMaterialInterface() = 0;


		virtual unsigned int GetResourceType()const
		{
			return RT_MATERIAL;
		}
		
	};
	DECLARE_Ptr(VSMaterialInterface);
	VSTYPE_MARCO(VSMaterialInterface);
	class VSGRAPHIC_API VSCustomMaterial : public VSMaterialInterface
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSCustomMaterial(const VSUsedName & ResourceName,
						const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
						const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName);
		virtual ~VSCustomMaterial();
		bool UseMaterial();
		void PreLoad(const VSUsedName & ResourceName,
			const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
			const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName);
		friend class VSRenderer;
		static VSCustomMaterial * GetPostGray()
		{
			return ms_pPostGray;
		}
		static VSCustomMaterial * GetGammaCorrect()
		{
			return ms_pGammaCorrect;
		}
		static VSCustomMaterial * GetPostScreenQuad()
		{
			return ms_pPostScreenQuad;
		}
		static VSCustomMaterial * GetPostVolumeShadowMap()
		{
			return ms_pPostVolumeShadowMap;
		}
		static VSCustomMaterial * GetSSR()
		{
			return ms_pSSR;
		}
		static VSCustomMaterial * GetProjectShadowPre()
		{
			return ms_pProjectShadowPre;
		}
		static VSCustomMaterial * GetProjectShadow()
		{
			return ms_pProjectShadow;
		}
		static VSCustomMaterial * GetFont()
		{
			return ms_pFont;
		}
		virtual void SetVShaderValue(const VSUsedName & Name,void *fValue);

		virtual void SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex);

		virtual void SetPShaderValue(const VSUsedName & Name,void *fValue);

		virtual void SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex);

		
		VSVShader *GetCurVShader(VSShaderKey & VShaderKey);
		VSPShader *GetCurPShader(VSShaderKey & PShaderKey);
		VSGShader *GetCurGShader(VSShaderKey & GShaderKey);

		VSRenderState & GetRenderState();
		void SetBlendState(VSBlendState * pBlendState);
		void SetDepthStencilState(VSDepthStencilState *pDepthStencilState);
		void SetRasterizerState(VSRasterizerState *pRasterizerState);
		void AddClipPlane(const VSPlane3 & Plane);
		void AddScissorRect(const VSRect2 & Rect);
	protected:
		bool GetVShader(VSShaderKey & VShaderKey);
		bool GetPShader(VSShaderKey & PShaderKey);
		bool GetGShader(VSShaderKey & GShaderKey);
		VSCustomMaterial();
		VSVShaderPtr m_pVShader;
		VSPShaderPtr m_pPShader;
		VSGShaderPtr m_pGShader;
		VSRenderState	m_RenderState;

		VSUsedName m_VShaderName;
		VSUsedName m_PShaderName;
		VSUsedName m_GShaderName;
		VSString m_VMainFunName;
		VSString m_PMainFunName;
		VSString m_GMainFunName;
		VSShaderSet * m_pVShaderSet;
		VSShaderSet * m_pPShaderSet;
		VSShaderSet * m_pGShaderSet;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void LoadAllDeviceShader();
	protected:
		static VSPointer<VSCustomMaterial> ms_pPostGray;
		static VSPointer<VSCustomMaterial> ms_pPostScreenQuad;
		static VSPointer<VSCustomMaterial> ms_pPostVolumeShadowMap;
		static VSPointer<VSCustomMaterial> ms_pProjectShadowPre;
		static VSPointer<VSCustomMaterial> ms_pProjectShadow;
		static VSPointer<VSCustomMaterial> ms_pGammaCorrect;
		static VSPointer<VSCustomMaterial> ms_pSSR;
		static VSPointer<VSCustomMaterial> ms_pFont;
		static bool LoadDefault();
		void LoadDeviceShader();
		static VSShaderKeyPtr ms_NULLShaderKey;
	};
	DECLARE_Ptr(VSCustomMaterial);
	VSTYPE_MARCO(VSCustomMaterial);

	class VSGRAPHIC_API VSMaterialBase : public VSMaterialInterface
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		
		VSMaterialBase();
		virtual ~VSMaterialBase();

	protected:
		VSArrayOrder<CustomFloatValue> m_VShaderCustomValue;
		VSArrayOrder<CustomTexSampler>m_VShaderCustomTex;

		VSArrayOrder<CustomFloatValue> m_PShaderCustomValue;
		VSArrayOrder<CustomTexSampler>m_PShaderCustomTex;


		virtual void SetVShaderValue(const VSUsedName & Name,void *fValue);

		virtual void SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex);

		virtual void SetPShaderValue(const VSUsedName & Name,void *fValue);

		virtual void SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex);

	};
	DECLARE_Ptr(VSMaterialBase);
	VSTYPE_MARCO(VSMaterialBase);
	class VSGRAPHIC_API VSMaterial : public VSMaterialBase
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
		DECLARE_INITIAL
	protected:
		VSMaterial();
		VSUsedName m_ShowName;
		VSArray<VSShaderMainFunction*> m_pShaderMainFunction;
		VSArray<VSShaderFunction *> m_pShaderFunctionArray;
		VSLightShaderFunction * m_pLightShaderFunction;
		VSPostEffectShaderFunction * m_pPostEffectShaderFunction;

		bool m_bIsAllReadyLink;
		
		VSPassPtr m_pPass[VSPass::PT_MAX];
		bool m_bIsCombine;
		
	public:
		void SetBlendState(VSBlendState * pBlendState,unsigned int uiPassId = 0);
		void SetDepthStencilState(VSDepthStencilState *pDepthStencilState,unsigned int uiPassId = 0);
		void SetRasterizerState(VSRasterizerState *pRasterizerState,unsigned int uiPassId = 0);
		void AddClipPlane(const VSPlane3 & Plane,unsigned int uiPassId = 0);
		void AddScissorRect(const VSRect2 & Rect,unsigned int uiPassId = 0);
		void AddPass(unsigned int uiMUT);
		FORCEINLINE VSRenderState & GetRenderState(unsigned int uiPassId)
		{
			return m_pShaderMainFunction[uiPassId]->GetRenderState();
		}
		FORCEINLINE void SetCombine(bool IsCombine)
		{
			m_bIsCombine = IsCombine;
		}
		FORCEINLINE bool GetCombine()const
		{
			return m_bIsCombine;
		}
		friend class VSRenderer;
		friend class VSMaterialInstance;
		enum //MATERIAL USED TYPE
		{
			MUT_PHONE,
			MUT_OREN_NAYAR,
			MUT_MINNAERT,
			MUT_STRAUSS,
			MUT_SHIRLEY,
			MUT_SCHLICK,
			MUT_COOKTOORANCE,
			MUT_ISOTROPICWARD,
			MUT_ANISOTROPICWARD,
			MUT_CUSTOM,
			MUT_LIGHT,
			MUT_POSTEFFECT,
			MUT_MAX
		};
		VSMaterial(const VSUsedName &ShowName, unsigned int uiMUT = MUT_PHONE);
		virtual ~VSMaterial();
		FORCEINLINE VSIndirectRenderPass * GetIndirectRenderPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_INDIRECT];
			return (VSIndirectRenderPass *)pPass;
		}
		FORCEINLINE VSMaterialPass * GetMaterialPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_MATERIAL];
			return (VSMaterialPass *)pPass;
		}
		FORCEINLINE VSNormalDepthPass * GetNormalDepthPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_NORMALDEPTH];
			return (VSNormalDepthPass *)pPass;
		}
		FORCEINLINE VSCubeShadowPass *GetCubeShadowPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_POINT_CUBE_SHADOW];
			return (VSCubeShadowPass *)pPass;

		}
		FORCEINLINE VSPrezPass * GetPrezPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_PREZ];
			return (VSPrezPass *)pPass;
		}
		FORCEINLINE VSVolumeShadowPass * GetVolumeShadowPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_POINT_VOLUME_SHADOW];
			return (VSVolumeShadowPass *)pPass;
		}
		FORCEINLINE VSShadowPass * GetShadowMapPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_SHADOW];
			return (VSShadowPass *)pPass;
		}
		FORCEINLINE VSDualParaboloidShadowPass * GetDualParaboloidShadowPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_DUAL_PARABOLOID_SHADOW];
			return (VSDualParaboloidShadowPass *)pPass;
		}
		FORCEINLINE VSLightFunPass * GetLightFunPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_LIGHT_FUNCTION];
			return (VSLightFunPass *)pPass;
		}
		FORCEINLINE VSPostEffectPass * GetPostEffectPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_POSTEFFECT];
			return (VSPostEffectPass *)pPass;
		}
		void AddShaderFuntion(VSShaderFunction *pShaderFuntion);
		void DeleteShaderFuntion(VSShaderFunction *pShaderFuntion);
		FORCEINLINE VSShaderMainFunction * GetShaderMainFunction(unsigned char uPassId = 0)const
		{
			return m_pShaderMainFunction[uPassId];
		}
		FORCEINLINE VSLightShaderFunction * GetLightShaderFunction()const
		{
			return m_pLightShaderFunction;
		}

		FORCEINLINE VSPostEffectShaderFunction * GetPostEffectShaderFunction()const
		{
			return m_pPostEffectShaderFunction;
		}
		unsigned int GetShaderMainFunctionNum()const
		{
			return m_pShaderMainFunction.GetNum();
		}
		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		FORCEINLINE bool IsAllReadyLink()const
		{
			return m_bIsAllReadyLink;
		}

		void LinkAllResource();
		void ClearShaderFuntionVisitFlagFalse();
		bool GetShaderTreeString(VSString & OutString,MaterialShaderPara &MSPara,unsigned int uiOST,unsigned char uPassId = 0);
		bool GetCustomDeclareString(VSString & OutString,unsigned char uPassId = 0);
		void CreateCustomValue(VSPShader * pShader);

		void CreateConstValueDeclare(VSString & OutString,unsigned int uiRegisterID);

		void CreateCustomTexture(VSPShader * pShader);
		void CreateTextureDeclare(VSString & OutString,unsigned int uiRegisterID);

		void SetGlobleValue(VSPass * pPass, unsigned int uiPassId,VSVShader * pVShader , VSPShader * pPShader);
		
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		
		
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		static VSPointer<VSMaterial> Default;
		static VSPointer<VSMaterial> DefaultOnlyColor;
		static VSPointer<VSMaterial> DefaultOnlyVertexColor;

		static VSPointer<VSMaterial> DefaultOnlyColorDisableDepth;
		static VSPointer<VSMaterial> DefaultOnlyVertexColorDisableDepth;
		static bool LoadDefault();
	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		friend class VSRenderer;
		friend class VSResourceManager;
		static const VSMaterial *GetDefalut()
		{
			return Default;
		}
		
		static const VSMaterial *GetDefaultOnlyColor()
		{
			return DefaultOnlyColor;
		}
		
		static const VSMaterial *GetDefaultOnlyVertexColor()
		{
			return DefaultOnlyVertexColor;
		}
		
		static const VSMaterial *GetDefaultOnlyColorDisableDepth()
		{
			return DefaultOnlyColorDisableDepth;
		}

		static const VSMaterial *GetDefaultOnlyVertexColorDisableDepth()
		{
			return DefaultOnlyVertexColorDisableDepth;
		}
		// more bigger more later render
		unsigned int m_uiCustomLayer;
	};
	DECLARE_Ptr(VSMaterial);
	DECLARE_Proxy(VSMaterial);
	VSTYPE_MARCO(VSMaterial);

	class VSGRAPHIC_API VSMaterialInstance: public VSObject
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		VSMaterialInstance(VSMaterialR * pMaterial);
		virtual ~VSMaterialInstance();
	protected:
		VSMaterialInstance();
		VSArrayOrder<CustomFloatValue> m_VShaderCustomValue;
		VSArrayOrder<CustomFloatValue> m_PShaderCustomValue;
		VSArrayOrder<CustomTexSampler>	m_VShaderCustomTex;		
		VSArrayOrder<CustomTexSampler>	m_PShaderCustomTex;		


		VSVShaderPtr m_pCurVShader[VSPass::PT_MAX];
		VSPShaderPtr m_pCurPShader[VSPass::PT_MAX];
		VSGShaderPtr m_pCurGShader[VSPass::PT_MAX];
		VSMaterialRPtr m_pMaterial;

	public:
		friend class VSSpatial;

		void SetPShaderValue(VSPShader * pPShader);
		void SetVShaderValue(VSVShader * pVShader);
		void SetVShaderValue(const VSUsedName & Name,void *fValue,unsigned int uiSize);
		void DeleteVShaderValue(const VSUsedName &Name);

		void SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex);
		void DeleteVShaderTexture(const VSUsedName & TexSamplerNodeName);

		void SetPShaderValue(const VSUsedName & Name,void *fValue,unsigned int uiSize);
		void DeletePShaderValue(const VSUsedName &Name);

		void SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex);
		void DeletePShaderTexture(const VSUsedName & TexSamplerNodeName);

		void GetAllMaterialPara();
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, int Data = 0);
		FORCEINLINE VSMaterial* GetMaterial()const
		{
			return m_pMaterial->GetResource();
		}
		FORCEINLINE VSMaterialR * GetMaterialR()const
		{
			return m_pMaterial;
		}
		friend class VSMaterialPass;
		friend class VSNormalDepthPass;
		friend class VSPrezPass;
		friend class VSCubeShadowPass;
		friend class VSVolumeShadowPass;
		friend class VSShadowPass;
		friend class VSDualParaboloidShadowPass;
		friend class VSLightFunPass;
		friend class VSProjectPreShadowPass;
		friend class VSProjectShadowPass;
		friend class VSPass;
		friend class VSIndirectRenderPass;
		friend class VSPostEffectPass;
	};
	DECLARE_Ptr(VSMaterialInstance);
	VSTYPE_MARCO(VSMaterialInstance);
	class VSGRAPHIC_API VSMaterialOnlyTexture : public VSMaterial
	{
	protected:
		VSMaterialOnlyTexture();
	public:
		VSMaterialOnlyTexture(const VSUsedName &ShowName,VSTexAllStateR * pTexture);
		virtual ~VSMaterialOnlyTexture();
		
	};
	DECLARE_Ptr(VSMaterialOnlyTexture);
	VSTYPE_MARCO(VSMaterialOnlyTexture);
	class VSGRAPHIC_API VSMaterialTextureAndNormal : public VSMaterial
	{
	protected:
		VSMaterialTextureAndNormal();
	public:
		VSMaterialTextureAndNormal(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture);
		virtual ~VSMaterialTextureAndNormal();
	};
	DECLARE_Ptr(VSMaterialTextureAndNormal);
	VSTYPE_MARCO(VSMaterialTextureAndNormal);
	class VSGRAPHIC_API VSMaterialOnlyEmissive : public VSMaterial
	{
	protected:
		VSMaterialOnlyEmissive();
	public:
		VSMaterialOnlyEmissive(const VSUsedName &ShowName);
		virtual ~VSMaterialOnlyEmissive();

	};
	DECLARE_Ptr(VSMaterialOnlyEmissive);
	VSTYPE_MARCO(VSMaterialOnlyEmissive);
	class VSGRAPHIC_API VSMaterialNoLight : public VSMaterial
	{
	protected:
		VSMaterialNoLight();
	public:
		VSMaterialNoLight(const VSUsedName &ShowName);
		virtual ~VSMaterialNoLight();

	};
	DECLARE_Ptr(VSMaterialNoLight);
	VSTYPE_MARCO(VSMaterialNoLight);
	class VSGRAPHIC_API VSMaterialOrenNayar : public VSMaterial
	{
	protected:
		VSMaterialOrenNayar();
	public:
		VSMaterialOrenNayar(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialOrenNayar();

	};
	DECLARE_Ptr(VSMaterialOrenNayar);
	VSTYPE_MARCO(VSMaterialOrenNayar);
	class VSGRAPHIC_API VSMaterialPhone : public VSMaterial
	{
	protected:
		VSMaterialPhone();
	public:
		VSMaterialPhone(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture,bool bPhoneSpecular);
		virtual ~VSMaterialPhone();

	};
	DECLARE_Ptr(VSMaterialPhone);
	VSTYPE_MARCO(VSMaterialPhone);
	class VSGRAPHIC_API VSMaterialPhoneTwoPass : public VSMaterial
	{
	protected:
		VSMaterialPhoneTwoPass();
	public:
		VSMaterialPhoneTwoPass(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture, bool bPhoneSpecular);
		virtual ~VSMaterialPhoneTwoPass();

	};
	DECLARE_Ptr(VSMaterialPhoneTwoPass);
	VSTYPE_MARCO(VSMaterialPhoneTwoPass)
	class VSGRAPHIC_API VSMaterialMinnaert : public VSMaterial
	{
	protected:
		VSMaterialMinnaert();
	public:
		VSMaterialMinnaert(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialMinnaert();

	};
	DECLARE_Ptr(VSMaterialMinnaert);
	VSTYPE_MARCO(VSMaterialMinnaert);
	class VSGRAPHIC_API VSMaterialStrauss : public VSMaterial
	{
	protected:
		VSMaterialStrauss();
	public:
		VSMaterialStrauss(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialStrauss();

	};
	DECLARE_Ptr(VSMaterialStrauss);
	VSTYPE_MARCO(VSMaterialStrauss);
	class VSGRAPHIC_API VSMaterialShirley : public VSMaterial
	{
	protected:
		VSMaterialShirley();
	public:
		VSMaterialShirley(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialShirley();

	};
	DECLARE_Ptr(VSMaterialShirley);
	VSTYPE_MARCO(VSMaterialShirley);
	class VSGRAPHIC_API VSMaterialSchlick : public VSMaterial
	{
	protected:
		VSMaterialSchlick();
	public:
		VSMaterialSchlick(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialSchlick();

	};
	DECLARE_Ptr(VSMaterialSchlick);
	VSTYPE_MARCO(VSMaterialSchlick);
	class VSGRAPHIC_API VSMaterialCookTorrance : public VSMaterial
	{
	protected:
		VSMaterialCookTorrance();
	public:
		VSMaterialCookTorrance(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture,bool bSample);
		virtual ~VSMaterialCookTorrance();

	};
	DECLARE_Ptr(VSMaterialCookTorrance);
	VSTYPE_MARCO(VSMaterialCookTorrance);
	class VSGRAPHIC_API VSMaterialIsotropicWard : public VSMaterial
	{
	protected:
		VSMaterialIsotropicWard();
	public:
		VSMaterialIsotropicWard(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialIsotropicWard();

	};
	DECLARE_Ptr(VSMaterialIsotropicWard);
	VSTYPE_MARCO(VSMaterialIsotropicWard);
	class VSGRAPHIC_API VSMaterialAnisotropicWard : public VSMaterial
	{
	protected:
		VSMaterialAnisotropicWard();
	public:
		VSMaterialAnisotropicWard(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialAnisotropicWard();

	};
	DECLARE_Ptr(VSMaterialAnisotropicWard);
	VSTYPE_MARCO(VSMaterialAnisotropicWard);
	class VSGRAPHIC_API VSMaterialCustom : public VSMaterial
	{
	protected:
		VSMaterialCustom();
	public:
		VSMaterialCustom(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialCustom();

	};
	DECLARE_Ptr(VSMaterialCustom);
	VSTYPE_MARCO(VSMaterialCustom);
	class VSGRAPHIC_API VSLightMaterial : public VSMaterial
	{
	protected:
		VSLightMaterial();
	public:
		VSLightMaterial(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture);
		virtual ~VSLightMaterial();

	};
	DECLARE_Ptr(VSLightMaterial);
	VSTYPE_MARCO(VSLightMaterial);
	class VSGRAPHIC_API VSPostEffectMaterial : public VSMaterial
	{
	protected:
		VSPostEffectMaterial();
	public:
		VSPostEffectMaterial(const VSUsedName &ShowName);
		virtual ~VSPostEffectMaterial();

	};
	DECLARE_Ptr(VSPostEffectMaterial);
	VSTYPE_MARCO(VSPostEffectMaterial);
}
#endif