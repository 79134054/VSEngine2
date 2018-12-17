#ifndef VSSHADERCREATEFACTORY_H
#define VSSHADERCREATEFACTORY_H
#include "VSMaterial.h"
#include "VSVShader.h"
#include "VSPShader.h"
#include "VSGShader.h"
#include "VSDShader.h"
#include "VSHShader.h"
#include "VSLight.h"
#include "VSResourceManager.h"
namespace VSEngine2
{
#define ms_LightShadowMatrix ms_WorldViewProjectMatrix
	class VSGRAPHIC_API VSShaderStringFactory
	{
	public:	

		static VSString ms_PSTextureInputCoordValue[TEXLEVEL];
		static VSString ms_PSOutputColorValue;
		static VSString ms_PSOutputColorValue1;
		static VSString ms_PSColor[2];
		static VSString ms_PSInputProjectZ;
		static VSString ms_PSInputLocalNormal;
		static VSString ms_PSInputLocalBinormal;
		static VSString ms_PSInputLocalTangent;
		static VSString ms_PSInputLocalPos;
		static VSString ms_PSInputWorldPos;
		static VSString ms_PSInputProjectPos;

		static VSString ms_PSConstantShadowSampler;
		static VSString ms_PSConstantLightFunSampler;

		static VSString ms_PSOrenNayarLookUpTableSampler;
		static VSString ms_PSStraussFLookUpTableSampler;
		static VSString ms_PSStraussSLookUpTableSampler;
		static VSString ms_PSCookTorranceLookUpTableSampler;
		static VSString ms_PSIsotropicWardLookUpTableSampler;

		static VSString ms_PSColorBufferSampler;
		static VSString ms_PSDepthNormalBufferSampler;

		static VSString ms_WorldViewProjectMatrix;
		static VSString ms_WorldViewMatrix;
		static VSString ms_ViewProjectMatrix;
		static VSString ms_ViewMatrix;
		static VSString ms_BoneMatrix;
		static VSString ms_WorldMatrix;
		static VSString ms_ProjectMatrix;



		static VSString ms_LightWorldPos;
		static VSString ms_LightWorldDirection;

		static VSString ms_WorldNormal;
		static VSString ms_ViewNormal;
		static VSString ms_WorldPos;
		static VSString ms_ViewPos;
		static VSString ms_ProjectPos;

		static VSString ms_ViewWorldDir;
		static VSString ms_CameraWorldPos;

		static VSString ms_LightColor;
		static VSString ms_LightSpecular;
		static VSString ms_LightDir;
		static VSString ms_LightName[VSLight::LT_MAX];
		static VSString ms_LightNameTemp[VSLight::LT_MAX];

		static VSString ms_FarZ;
		static VSString ms_PointLightRange;

		static VSString ms_InvRTWidth;

		static VSString ms_SkyLightUpColor;
		static VSString ms_SkyLightDownColor;

		static unsigned int ms_uiCreateVShaderNum;
		static unsigned int ms_uiCreatePShaderNum;

		static unsigned int ms_ShaderValueIndex;

		
		//Create Shader


		static bool CreateVShaderString(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,VSString & VShaderString
#ifdef _DEBUG
			,VSString &OutShaderFileName);
#else
			);
#endif
		static bool CreatePShaderString(VSPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,VSString & PShaderString
#ifdef _DEBUG
			,VSString &OutShaderFileName);
#else
			);
#endif
		
		static bool CreateGShaderString(VSGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID, VSString & GShaderString
#ifdef _DEBUG
			, VSString &OutShaderFileName);
#else
			);
#endif
	protected:

 		static void ClearAllString();


		
	};

}
#endif