#include "VSShaderStringFactory.h"
#include "VSGeometry.h"
#include "VSLog.h"
#include "VSResourceManager.h"
#include "VSBoneNode.h"
#include "VSDirectionLight.h"
#include "VSPointLight.h"
#include "VSSpotLight.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
unsigned int VSShaderStringFactory::ms_ShaderValueIndex = 0;
VSString VSShaderStringFactory::ms_PSTextureInputCoordValue[TEXLEVEL];
VSString VSShaderStringFactory::ms_PSColor[2];
VSString VSShaderStringFactory::ms_PSOutputColorValue;
VSString VSShaderStringFactory::ms_PSOutputColorValue1;
VSString VSShaderStringFactory::ms_PSInputLocalNormal;
VSString VSShaderStringFactory::ms_PSInputLocalBinormal;
VSString VSShaderStringFactory::ms_PSInputLocalTangent;
VSString VSShaderStringFactory::ms_PSInputLocalPos;
VSString VSShaderStringFactory::ms_PSInputProjectZ;
VSString VSShaderStringFactory::ms_PSInputWorldPos;
VSString VSShaderStringFactory::ms_PSInputProjectPos;

VSString VSShaderStringFactory::ms_PSOrenNayarLookUpTableSampler = _T("Tex_OrenNayarLookUpTable");
VSString VSShaderStringFactory::ms_PSStraussFLookUpTableSampler = _T("Tex_StraussFLookUpTable");
VSString VSShaderStringFactory::ms_PSStraussSLookUpTableSampler = _T("Tex_StraussSLookUpTable");
VSString VSShaderStringFactory::ms_PSCookTorranceLookUpTableSampler = _T("Tex_CookTorranceLookUpTable");
VSString VSShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler = _T("Tex_IsotropicWardLookUpTable");
unsigned int VSShaderStringFactory::ms_uiCreateVShaderNum = 0;
unsigned int VSShaderStringFactory::ms_uiCreatePShaderNum = 0;

VSString VSShaderStringFactory::ms_WorldViewProjectMatrix = _T("WorldViewProjectMatrix");
VSString VSShaderStringFactory::ms_ViewProjectMatrix = _T("ViewProjectMatrix");
VSString VSShaderStringFactory::ms_ViewMatrix = _T("ViewMatrix");
VSString VSShaderStringFactory::ms_WorldViewMatrix = _T("WorldViewMatrix");
VSString VSShaderStringFactory::ms_BoneMatrix = _T("BoneVector");
VSString VSShaderStringFactory::ms_WorldMatrix = _T("WorldMatrix");
VSString VSShaderStringFactory::ms_ProjectMatrix = _T("ProjectMatrix");

VSString VSShaderStringFactory::ms_LightName[VSLight::LT_MAX] = {_T("PointLight"),_T("SpotLight"),_T("DirLight")};
VSString VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_MAX] = {_T("PointLightTemp"),_T("SpotLightTemp"),_T("DirLightTemp")};
VSString VSShaderStringFactory::ms_LightColor = _T("LightColor");
VSString VSShaderStringFactory::ms_LightSpecular = _T("LightSpecular");
VSString VSShaderStringFactory::ms_LightDir = _T("LightDir");
VSString VSShaderStringFactory::ms_FarZ = _T("FarZ");
VSString VSShaderStringFactory::ms_PointLightRange =_T("PointLightRange");
VSString VSShaderStringFactory::ms_LightWorldPos = _T("LightWorldPos");
VSString VSShaderStringFactory::ms_LightWorldDirection = _T("LightWorldDirection");
VSString VSShaderStringFactory::ms_WorldNormal = _T("WorldNormal");
VSString VSShaderStringFactory::ms_ViewNormal = _T("ViewNormal");
VSString VSShaderStringFactory::ms_WorldPos = _T("WorldPos");
VSString VSShaderStringFactory::ms_ViewPos = _T("ViewPos");
VSString VSShaderStringFactory::ms_ProjectPos = _T("ProjectPos");
VSString VSShaderStringFactory::ms_ViewWorldDir = _T("ViewWorldDir");
VSString VSShaderStringFactory::ms_CameraWorldPos = _T("CameraWorldPos");

VSString VSShaderStringFactory::ms_PSConstantShadowSampler = _T("Tex_ConstantShadow");
VSString VSShaderStringFactory::ms_PSConstantLightFunSampler = _T("Tex_PSConstantLightFun");
VSString VSShaderStringFactory::ms_InvRTWidth = _T("InvRTWidth");
VSString VSShaderStringFactory::ms_SkyLightUpColor = _T("SkyLightUpColor");
VSString VSShaderStringFactory::ms_SkyLightDownColor = _T("SkyLightDownColor");

VSString VSShaderStringFactory::ms_PSColorBufferSampler = _T("Tex_ColorBufferSampler");
VSString VSShaderStringFactory::ms_PSDepthNormalBufferSampler = _T("Tex_DepthNormalBufferSampler");
void VSShaderStringFactory::ClearAllString()
{
	for(unsigned int i = 0 ; i < TEXLEVEL ; i++)
		ms_PSTextureInputCoordValue[i] = VSRenderer::ms_pRenderer->Float2Const(_T("-1"),_T("-1"));
	ms_PSOutputColorValue.Clear();
	ms_PSOutputColorValue1.Clear();
	ms_PSInputLocalNormal.Clear();
	ms_PSInputLocalBinormal.Clear();
	ms_PSInputLocalTangent.Clear();
	ms_PSInputLocalPos.Clear();

	ms_PSColor[0].Clear();
	ms_PSColor[1].Clear();

	ms_PSInputProjectZ.Clear();
}

bool VSShaderStringFactory::CreateVShaderString(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,VSString & VShaderString
#ifdef _DEBUG
												,VSString &OutShaderFileName)
#else
												)
#endif
{
	if (!MSPara.pGeometry || !pVShader || !VSRenderer::ms_pRenderer || !MSPara.pMaterialInstance)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}
	ClearAllString();
	ms_uiCreateVShaderNum++;

	VSString VInclude;
	VSString VDynamic;
	VSString VInputDeclare;
	VSString VOutputDeclare;
	VSString VUserConstantString;
	VSString VFuntionString;
	VSLog VShaderText;
	VSString VShaderTextName = VSResourceManager::ms_OutputShaderCodePath + VSRenderer::ms_pRenderer->GetRendererStringType() + _T("_");
	VSString ShaderID = IntToString(uiShaderID);
	VSRenderer::ms_pRenderer->GetIncludeShader(VInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(VDynamic);
	VSRenderer::ms_pRenderer->CreateVUserConstant(pVShader,MSPara,uiPassType,VUserConstantString);
	VSRenderer::ms_pRenderer->CreateVInputDeclare(MSPara,uiPassType,VInputDeclare);
	VSRenderer::ms_pRenderer->CreateVOutputDeclare(MSPara,uiPassType,VOutputDeclare);
	VSRenderer::ms_pRenderer->CreateVFuntion(MSPara,uiPassType,VFuntionString);
	if (uiPassType == VSPass::PT_MATERIAL || uiPassType == VSPass::PT_INDIRECT)
	{			
		
		VShaderTextName += pMaterial->GetShowName().GetString() + ShaderID + _T("VShader.txt");
		
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
				
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + _T("VShader.txt");

	}
	else if (uiPassType == VSPass::PT_PREZ)
	{

		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + _T("VShader.txt");

	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + _T("VShader.txt");
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + _T("VShader.txt");
	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + _T("VShader.txt");
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + _T("VShader.txt");
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + _T("VShader.txt");
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + _T("VShader.txt");
	}

	VShaderText.Open(VShaderTextName.GetBuffer());
	VShaderString = VInclude + VDynamic + VUserConstantString + VInputDeclare + VOutputDeclare + VFuntionString;
	VShaderText.WriteInfo(VShaderString.GetBuffer());
#ifdef _DEBUG
	OutShaderFileName = VShaderTextName;
#endif
	return 1;
}
bool VSShaderStringFactory::CreatePShaderString(VSPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,VSString & PShaderString
#ifdef _DEBUG
												,VSString &OutShaderFileName)
#else
												)
#endif
{
	if (!pPShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !VSRenderer::ms_pRenderer)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	ClearAllString();
	ms_uiCreatePShaderNum++;
	VSString PInclude;
	VSString PDynamic;
	VSString PInputDeclare;
	VSString POutputDeclare;
	VSString PUserConstantstring;
	VSString PFuntionString;
	VSLog PShaderText;
	VSString PShaderTextName = VSResourceManager::ms_OutputShaderCodePath + VSRenderer::ms_pRenderer->GetRendererStringType() + _T("_");
	VSString ShaderID = IntToString(uiShaderID);

	

	VSRenderer::ms_pRenderer->GetIncludeShader(PInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(PDynamic);
	
	VSRenderer::ms_pRenderer->CreatePInputDeclare(MSPara,uiPassType,PInputDeclare);
	VSRenderer::ms_pRenderer->CreatePOutputDeclare(MSPara,uiPassType,POutputDeclare);
	
	//must first before CreatePUserConstant to decide unused shader funtion
	VSRenderer::ms_pRenderer->CreatePFuntion(MSPara,uiPassType,PFuntionString);

	VSRenderer::ms_pRenderer->CreatePUserConstant(pPShader,MSPara,uiPassType,PUserConstantstring);
	if (uiPassType == VSPass::PT_MATERIAL)
	{
		
	
		
		PShaderTextName += pMaterial->GetShowName().GetString() + ShaderID +_T("PShader.txt");


		
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{

		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + _T("PShader.txt");

	}
	else if (uiPassType == VSPass::PT_PREZ)
	{


		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + _T("PShader.txt");

	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{


		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + _T("PShader.txt");

	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + _T("PShader.txt");
	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + _T("PShader.txt");
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + _T("PShader.txt");
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + _T("PShader.txt");
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + _T("PShader.txt");
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_Indirect") + ShaderID + _T("PShader.txt");
	}
	else if (uiPassType == VSPass::PT_POSTEFFECT)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_PostEffect") + ShaderID + _T("PShader.txt");
	}
	PShaderText.Open(PShaderTextName.GetBuffer());
	PShaderString = PInclude + PDynamic + PUserConstantstring + PInputDeclare + POutputDeclare + PFuntionString;
	PShaderText.WriteInfo(PShaderString.GetBuffer());
#ifdef _DEBUG
	OutShaderFileName = PShaderTextName;
#endif
	return 1;
}
bool VSShaderStringFactory::CreateGShaderString(VSGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID, VSString & GShaderString
#ifdef _DEBUG
	, VSString &OutShaderFileName)
#else
	)
#endif
{
	if (!pGShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !VSRenderer::ms_pRenderer)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	ClearAllString();
	ms_uiCreatePShaderNum++;
	VSString GInclude;
	VSString GDynamic;
	VSString GInputDeclare;
	VSString GOutputDeclare;
	VSString GUserConstantstring;
	VSString GFuntionString;
	VSLog GShaderText;
	VSString GShaderTextName = VSResourceManager::ms_OutputShaderCodePath + VSRenderer::ms_pRenderer->GetRendererStringType() + _T("_");
	VSString ShaderID = IntToString(uiShaderID);



	VSRenderer::ms_pRenderer->GetIncludeShader(GInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(GDynamic);

	VSRenderer::ms_pRenderer->CreateGUserConstant(pGShader, MSPara, uiPassType, GUserConstantstring);
	VSRenderer::ms_pRenderer->CreateGInputDeclare(MSPara, uiPassType, GInputDeclare);
	VSRenderer::ms_pRenderer->CreateGOutputDeclare(MSPara, uiPassType, GOutputDeclare);
	VSRenderer::ms_pRenderer->CreateGFuntion(MSPara, uiPassType, GFuntionString);
	if (uiPassType == VSPass::PT_MATERIAL)
	{



		GShaderTextName += pMaterial->GetShowName().GetString() + ShaderID + _T("GShader.txt");



	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{

		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + _T("GShader.txt");

	}
	else if (uiPassType == VSPass::PT_PREZ)
	{


		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + _T("GShader.txt");

	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{


		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + _T("GShader.txt");

	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + _T("GShader.txt");
	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + _T("GShader.txt");
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + _T("GShader.txt");
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + _T("GShader.txt");
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + _T("GShader.txt");
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_Indirect") + ShaderID + _T("GShader.txt");
	}
	else if (uiPassType == VSPass::PT_POSTEFFECT)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_PostEffect") + ShaderID + _T("GShader.txt");
	}
	GShaderText.Open(GShaderTextName.GetBuffer());
	GShaderString = GInclude + GDynamic + GUserConstantstring + GInputDeclare + GOutputDeclare + GFuntionString;
	GShaderText.WriteInfo(GShaderString.GetBuffer());
#ifdef _DEBUG
	OutShaderFileName = GShaderTextName;
#endif
	return 1;
}




