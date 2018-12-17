#ifndef VSSHADERMAINFUNCTION_H
#define VSSHADERMAINFUNCTION_H
#include "VSShaderFunction.h"
#include "VSRenderState.h"
#include "VSShaderKey.h"
namespace VSEngine2
{	class VSLight;
	DECLARE_Ptr(VSLight);
	class VSGRAPHIC_API VSShaderMainFunction : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum
		{
			SM_PHONE,
			SM_OREN_NAYAR,
			SM_MINNAERT,
			SM_STRAUSS,
			SM_SHIRLEY,
			SM_SCHLICK,
			SM_COOKTOORANCE,
			SM_ISOTROPICWARD,
			SM_ANISOTROPICWARD,
			SM_CUSTOM,
			SM_MAX
		};
		
		enum // OutPut String Type
		{
			OST_MATERIAL,
			OST_NORMAL_DEPTH,
			OST_CUB_SHADOW,
			OST_SHADOW,
			OST_DUAL_PARABOLOID_SHADOW,
			OST_INDIRECT,
			OST_MAX
		};
		VSShaderMainFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSShaderMainFunction() = 0;
		virtual bool GetFuntionString(VSString &OutString)const = 0;
		virtual bool GetNormalDepthString(VSString &OutString)const;
		virtual bool GetIndirectRenderString(VSString &OutString)const;
		virtual bool GetCubShadowString(VSString &OutString)const;
		virtual bool GetShadowString(VSString &OutString)const;
		virtual bool GetSDualParaboloidhadowString(VSString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const = 0;
		FORCEINLINE void SetMaterialShaderPara(MaterialShaderPara &MSPara)
		{
			m_MSPara = MSPara;
		}
		FORCEINLINE void SetSRGBWrite(unsigned char uiVESRGBWrite)
		{
			m_uiVESRGBWrite = uiVESRGBWrite;
		}
		FORCEINLINE unsigned char GetSRGBWrite()const
		{
			return m_uiVESRGBWrite;
		}
		FORCEINLINE void SetPassId(unsigned char uiPassId)
		{
			m_uiPassId = uiPassId;
		}
		FORCEINLINE unsigned char GetPassId()const
		{
			return m_uiPassId;
		}
		virtual bool HasNormal()const
		{
			if (GetNormalNode()->GetOutputLink())
			{
				return true;
			}
			return false;
		}

		virtual VSInputNode * GetNormalNode()const = 0;
		virtual VSInputNode * GetAlphaNode()const = 0;
		virtual VSInputNode * GetEmissiveNode()const = 0;
		virtual VSInputNode * GetDiffuseNode()const = 0;
		virtual VSInputNode * GetReflectMipNode()const = 0;
		virtual VSInputNode * GetReflectPowNode()const = 0;
		virtual bool GetShaderTreeString(VSString &OutString,unsigned int uiOutPutStringType);
		virtual bool GetInputValueString(VSString &OutString,unsigned int uiOutPutStringType)const;
		FORCEINLINE VSRenderState & GetRenderState()
		{
			return m_RenderState;
		}
		FORCEINLINE void SetBlendState(VSBlendState * pBlendState)
		{
			m_RenderState.SetBlendState(pBlendState);
		}
		FORCEINLINE void SetDepthStencilState(VSDepthStencilState *pDepthStencilState)
		{
			m_RenderState.SetDepthStencilState(pDepthStencilState);
		}
		FORCEINLINE void SetRasterizerState(VSRasterizerState *pRasterizerState)
		{
			m_RenderState.SetRasterizerState(pRasterizerState);
		}
		FORCEINLINE void AddClipPlane(const VSPlane3 & Plane)
		{
			m_RenderState.AddPlane(Plane);
		}
		FORCEINLINE void AddScissorRect(const VSRect2 & Rect)
		{
			m_RenderState.AddRect(Rect);
		}
		virtual void SetGlobleValue(unsigned int uiOutPutStringType , VSVShader * pVShader , VSPShader * pPShader){}
		virtual void ResetInShaderName(){}
		virtual void GetLightShadow(const MaterialShaderPara& MSPara,VSArray<VSString> ShadowStringArray[VSLight::LT_MAX])const;
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction);

		FORCEINLINE void SetAlphaTestValue(VSREAL AlphaTestValue)
		{
			m_fAlphaTestValue = AlphaTestValue;
		}
	protected:
		VSShaderMainFunction();
		MaterialShaderPara m_MSPara;
		unsigned char m_uiVESRGBWrite;
		unsigned char m_uiPassId;
		enum //Value Use String
		{
			VUS_WORLD_POS = BIT(1),
			VUS_VIEW_POS = BIT(2) | VUS_WORLD_POS,
			VUS_PROJ_POS = BIT(3) | VUS_VIEW_POS,

			VUS_WORLD_NORMAL = BIT(4),
			VUS_VIEW_NORMAL = BIT(5) | VUS_WORLD_NORMAL,

			VUS_VIEW_WORLD_DIR = BIT(6),

			VUS_ALL = VUS_PROJ_POS | VUS_VIEW_NORMAL | VUS_VIEW_WORLD_DIR,
		};
		void GetValueUseDeclareString(VSString &OutString, unsigned int uiValueUseString);
		VSRenderState	m_RenderState;

		VSREAL m_fAlphaTestValue;

		void GetValueUseString(VSString &OutString, unsigned int uiValueUseString);
		void GetAlphaTestString(VSString &OutString)const;
		void GetSRGBWriteString(VSString &OutString)const;

		void GetNormalString(VSString &OutString)const;
		
		void GetAlphaString(VSString &OutString)const;
		
		void GetEmissiveString(VSString &OutString)const;
		
		void GetDiffuseString(VSString &OutString)const;
		
		void GetReflectMipString(VSString &OutString)const;
		
		void GetReflectPowString(VSString &OutString)const;
		
		bool GetNormalInputValueString(VSString &OutString)const;
		
		bool GetAlphaInputValueString(VSString &OutString)const;
		
		bool GetEmissiveInputValueString(VSString &OutString)const;
		
		bool GetDiffuseInputValueString(VSString &OutString)const;
		
		bool GetReflectMipInputValueString(VSString &OutString)const;
		
		bool GetReflectPowInputValueString(VSString &OutString)const;;
		
	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		
		};
		
	public:
		
	};
	DECLARE_Ptr(VSShaderMainFunction);
	VSTYPE_MARCO(VSShaderMainFunction);
}
#endif