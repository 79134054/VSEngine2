//创建了11个新的引擎格式模型
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSPostEffectSet.h"
#include "VSCaptureTexAllState.h"
namespace VSEngine2
{

	//SourcePath DestPath -s(static mesh)/-d(dynamic mesh)/-a(action) -c(compress) -v(shadow volume)
	class MaterialSaverDemo : public VSConsoleApplication
	{
	public:
		MaterialSaverDemo();
		virtual ~MaterialSaverDemo();
		DLCARE_APPLICATION(MaterialSaverDemo);
	public:
		virtual bool PreInitial();
		virtual bool OnInitial();
		virtual bool OnTerminal();


	};
	IMPLEMENT_APPLICATION(MaterialSaverDemo);

	MaterialSaverDemo::MaterialSaverDemo()
	{

	}

	MaterialSaverDemo::~MaterialSaverDemo()
	{


	}
	bool MaterialSaverDemo::OnTerminal()
	{
		if (!VSConsoleApplication::OnTerminal())
		{
			return false;
		}
		return true;
	}
	bool MaterialSaverDemo::PreInitial()
	{
		if (!VSConsoleApplication::PreInitial())
		{
			return false;
		}

		return true;
	}
	bool MaterialSaverDemo::OnInitial()
	{
		if (!VSConsoleApplication::OnInitial())
		{
			return false;
		}
		m_bIsRunning = false;

		VSSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
		VSSamplerStatePtr pTriLineSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);


		VSOutPutDebugString("Load VSEngine Material MaterialPhone\n");
		printf("Load VSEngine Material MaterialPhone\n");
		VSMaterialRPtr  pMaterialPhoneR = VSResourceManager::LoadASYNMaterial(_T("MaterialPhone.MATERIAL"), false);
		{
			// texture 
			VSOutPutDebugString("Load diffuse Light_D texture\n");
			printf("Load diffuse Light_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Light_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Light.Light_D\n");
			printf("Save VSEngine texture Light.Light_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Light//Light_D"), true);



			VSOutPutDebugString("Load normal Light_N texture\n");
			printf("Load normal Light_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Light_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Light.Light_N\n");
			printf("Save VSEngine texture Light.Light_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Light//Light_N"), true);


			VSOutPutDebugString("Load emissive Light_E texture\n");
			printf("Load Light_E tga texture\n");
			VSFileName EmissiveFileName = VSResourceManager::ms_TexturePath + _T("Light_E.tga");
			VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTextureCompress(EmissiveFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Light.Light_E\n");
			printf("Save VSEngine texture Light.Light_E\n");
			VSResourceManager::NewSaveTexture(pEmissive, _T("Light//Light_E"), true);


			VSOutPutDebugString("Load VSEngine texture Light.Light_D\n");
			printf("Load VSEngine texture Light.Light_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Light//Light_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Light.Light_N\n");
			printf("Load VSEngine texture Light.Light_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Light//Light_N.TEXTURE"), false);


			VSOutPutDebugString("Load VSEngine texture Light.Light_E\n");
			printf("Load VSEngine texture Light.Light_E\n");
			VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNTexture(_T("Light//Light_E.TEXTURE"), false);



			//Model
			VSOutPutDebugString("Load VSEngine Model Light\n");
			printf("Load VSEngine Model Light\n");
			VSStaticMeshNodeRPtr pLightModel= VSResourceManager::LoadASYNStaticMesh(_T("Light.STMODEL"), false);
			VSStaticMeshNode * pLightModelNode = pLightModel->GetResource();
			VSGeometryNode * pGeometryNode = pLightModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);

			VSOutPutDebugString("Save VSEngine Model LightPhone\n");
			printf("Save VSEngine Model LightPhone\n");
			VSResourceManager::NewSaveStaticMesh(pLightModelNode, _T("LightPhone"), true);
		}

		{
			// texture 
			VSOutPutDebugString("Load diffuse Table_D texture\n");
			printf("Load diffuse Table_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Table_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Table.Table_D\n");
			printf("Save VSEngine texture Table.Table_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Table//Table_D"), true);



			VSOutPutDebugString("Load normal Table_N texture\n");
			printf("Load normal Table_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Table_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Table.Table_N\n");
			printf("Save VSEngine texture Table.Table_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Table//Table_N"), true);


			VSOutPutDebugString("Load emissive Table_E texture\n");
			printf("Load Table_E tga texture\n");
			VSFileName EmissiveFileName = VSResourceManager::ms_TexturePath + _T("Table_E.tga");
			VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTextureCompress(EmissiveFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Table.Table_E\n");
			printf("Save VSEngine texture Table.Table_E\n");
			VSResourceManager::NewSaveTexture(pEmissive, _T("Table//Table_E"), true);


			VSOutPutDebugString("Load VSEngine texture Table.Table_D\n");
			printf("Load VSEngine texture Table.Table_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Table//Table_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Table.Table_N\n");
			printf("Load VSEngine texture Table.Table_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Table//Table_N.TEXTURE"), false);


			VSOutPutDebugString("Load VSEngine texture Table.Table_E\n");
			printf("Load VSEngine texture Table.Table_E\n");
			VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNTexture(_T("Table//Table_E.TEXTURE"), false);



			//Model
			VSOutPutDebugString("Load VSEngine Model Table\n");
			printf("Load VSEngine Model Table\n");
			VSStaticMeshNodeRPtr pTableModel= VSResourceManager::LoadASYNStaticMesh(_T("Table.STMODEL"), false);
			VSStaticMeshNode * pTableModelNode = pTableModel->GetResource();
			VSGeometryNode * pGeometryNode = pTableModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);

			VSOutPutDebugString("Save VSEngine Model TablePhone\n");
			printf("Save VSEngine Model TablePhone\n");
			VSResourceManager::NewSaveStaticMesh(pTableModelNode, _T("TablePhone"), true);
		}

		{
			// texture 
			VSOutPutDebugString("Load diffuse Radio_D texture\n");
			printf("Load diffuse Radio_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Radio_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Radio.Radio_D\n");
			printf("Save VSEngine texture Radio.Radio_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Radio//Radio_D"), true);



			VSOutPutDebugString("Load normal Radio_N texture\n");
			printf("Load normal Radio_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Radio_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Radio.Radio_N\n");
			printf("Save VSEngine texture Radio.Radio_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Radio//Radio_N"), true);


			VSOutPutDebugString("Load emissive Radio_E texture\n");
			printf("Load Radio_E tga texture\n");
			VSFileName EmissiveFileName = VSResourceManager::ms_TexturePath + _T("Radio_E.tga");
			VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTextureCompress(EmissiveFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Radio.Radio_E\n");
			printf("Save VSEngine texture Radio.Radio_E\n");
			VSResourceManager::NewSaveTexture(pEmissive, _T("Radio//Radio_E"), true);


			VSOutPutDebugString("Load VSEngine texture Radio.Radio_D\n");
			printf("Load VSEngine texture Radio.Radio_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Radio//Radio_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Radio.Radio_N\n");
			printf("Load VSEngine texture Radio.Radio_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Radio//Radio_N.TEXTURE"), false);


			VSOutPutDebugString("Load VSEngine texture Radio.Radio_E\n");
			printf("Load VSEngine texture Radio.Radio_E\n");
			VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNTexture(_T("Radio//Radio_E.TEXTURE"), false);



			//Model
			VSOutPutDebugString("Load VSEngine Model Radio\n");
			printf("Load VSEngine Model Radio\n");
			VSStaticMeshNodeRPtr pRadioModel = VSResourceManager::LoadASYNStaticMesh(_T("Radio.STMODEL"), false);
			VSStaticMeshNode * pRadioModelNode = pRadioModel->GetResource();
			VSGeometryNode * pGeometryNode = pRadioModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);

			VSOutPutDebugString("Save VSEngine Model RadioPhone\n");
			printf("Save VSEngine Model RadioPhone\n");
			VSResourceManager::NewSaveStaticMesh(pRadioModelNode, _T("RadioPhone"), true);
		}
		{
			// texture 
			VSOutPutDebugString("Load diffuse Fan_D texture\n");
			printf("Load diffuse Fan_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Fan_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Fan.Fan_D\n");
			printf("Save VSEngine texture Fan.Fan_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Fan//Fan_D"), true);



			VSOutPutDebugString("Load normal Fan_N texture\n");
			printf("Load normal Fan_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Fan_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Fan.Fan_N\n");
			printf("Save VSEngine texture Fan.Fan_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Fan//Fan_N"), true);


			VSOutPutDebugString("Load emissive Fan_E texture\n");
			printf("Load Fan_E tga texture\n");
			VSFileName EmissiveFileName = VSResourceManager::ms_TexturePath + _T("Fan_E.tga");
			VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTextureCompress(EmissiveFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Fan.Fan_E\n");
			printf("Save VSEngine texture Fan.Fan_E\n");
			VSResourceManager::NewSaveTexture(pEmissive, _T("Fan//Fan_E"), true);


			VSOutPutDebugString("Load VSEngine texture Fan.Fan_D\n");
			printf("Load VSEngine texture Fan.Fan_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Fan//Fan_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Fan.Fan_N\n");
			printf("Load VSEngine texture Fan.Fan_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Fan//Fan_N.TEXTURE"), false);


			VSOutPutDebugString("Load VSEngine texture Fan.Fan_E\n");
			printf("Load VSEngine texture Fan.Fan_E\n");
			VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNTexture(_T("Fan//Fan_E.TEXTURE"), false);



			//Model
			VSOutPutDebugString("Load VSEngine Model Fan\n");
			printf("Load VSEngine Model Fan\n");
			VSStaticMeshNodeRPtr pFanModel = VSResourceManager::LoadASYNStaticMesh(_T("Fan.STMODEL"), false);
			VSStaticMeshNode * pFanModelNode = pFanModel->GetResource();
			VSGeometryNode * pGeometryNode = pFanModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);

			VSOutPutDebugString("Save VSEngine Model FanPhone\n");
			printf("Save VSEngine Model FanPhone\n");
			VSResourceManager::NewSaveStaticMesh(pFanModelNode, _T("FanPhone"), true);
		}
		{
			// texture 
			VSOutPutDebugString("Load diffuse ZhuZi_D texture\n");
			printf("Load diffuse ZhuZi_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("ZhuZi_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture ZhuZi.ZhuZi_D\n");
			printf("Save VSEngine texture ZhuZi.ZhuZi_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("ZhuZi//ZhuZi_D"), true);



			VSOutPutDebugString("Load normal ZhuZi_N texture\n");
			printf("Load normal ZhuZi_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("ZhuZi_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture ZhuZi.ZhuZi_N\n");
			printf("Save VSEngine texture ZhuZi.ZhuZi_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("ZhuZi//ZhuZi_N"), true);


			VSOutPutDebugString("Load emissive ZhuZi_E texture\n");
			printf("Load ZhuZi_E tga texture\n");
			VSFileName EmissiveFileName = VSResourceManager::ms_TexturePath + _T("ZhuZi_E.tga");
			VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTextureCompress(EmissiveFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture ZhuZi.ZhuZi_E\n");
			printf("Save VSEngine texture ZhuZi.ZhuZi_E\n");
			VSResourceManager::NewSaveTexture(pEmissive, _T("ZhuZi//ZhuZi_E"), true);


			VSOutPutDebugString("Load VSEngine texture ZhuZi.ZhuZi_D\n");
			printf("Load VSEngine texture ZhuZi.ZhuZi_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("ZhuZi//ZhuZi_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture ZhuZi.ZhuZi_N\n");
			printf("Load VSEngine texture ZhuZi.ZhuZi_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("ZhuZi//ZhuZi_N.TEXTURE"), false);


			VSOutPutDebugString("Load VSEngine texture ZhuZi.ZhuZi_E\n");
			printf("Load VSEngine texture ZhuZi.ZhuZi_E\n");
			VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNTexture(_T("ZhuZi//ZhuZi_E.TEXTURE"), false);



			//Model
			VSOutPutDebugString("Load VSEngine Model ZhuZi\n");
			printf("Load VSEngine Model ZhuZi\n");
			VSStaticMeshNodeRPtr pZhuZiModel = VSResourceManager::LoadASYNStaticMesh(_T("ZhuZi.STMODEL"), false);
			VSStaticMeshNode * pZhuZiModelNode = pZhuZiModel->GetResource();
			VSGeometryNode * pGeometryNode = pZhuZiModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);

			VSOutPutDebugString("Save VSEngine Model ZhuZiPhone\n");
			printf("Save VSEngine Model ZhuZiPhone\n");
			VSResourceManager::NewSaveStaticMesh(pZhuZiModelNode, _T("ZhuZiPhone"), true);
		}
		{
			// texture 
			VSOutPutDebugString("Load diffuse Chair_D texture\n");
			printf("Load diffuse Chair_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Chair_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Chair.Chair_D\n");
			printf("Save VSEngine texture Chair.Chair_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Chair//Chair_D"), true);



			VSOutPutDebugString("Load normal Chair_N texture\n");
			printf("Load normal Chair_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Chair_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Chair.Chair_N\n");
			printf("Save VSEngine texture Chair.Chair_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Chair//Chair_N"), true);




			VSOutPutDebugString("Load VSEngine texture Chair.Chair_D\n");
			printf("Load VSEngine texture Chair.Chair_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Chair//Chair_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Chair.Chair_N\n");
			printf("Load VSEngine texture Chair.Chair_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Chair//Chair_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model Chair\n");
			printf("Load VSEngine Model Chair\n");
			VSStaticMeshNodeRPtr pChairModel = VSResourceManager::LoadASYNStaticMesh(_T("Chair.STMODEL"), false);
			VSStaticMeshNode * pChairModelNode = pChairModel->GetResource();
			VSGeometryNode * pGeometryNode = pChairModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model ChairPhone\n");
			printf("Save VSEngine Model ChairPhone\n");
			VSResourceManager::NewSaveStaticMesh(pChairModelNode, _T("ChairPhone"), true);
		}
		{
			// texture 
			VSOutPutDebugString("Load diffuse Chair1_D texture\n");
			printf("Load diffuse Chair1_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Chair1_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Chair1.Chair1_D\n");
			printf("Save VSEngine texture Chair1.Chair1_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Chair1//Chair1_D"), true);



			VSOutPutDebugString("Load normal Chair1_N texture\n");
			printf("Load normal Chair1_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Chair1_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Chair1.Chair1_N\n");
			printf("Save VSEngine texture Chair1.Chair1_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Chair1//Chair1_N"), true);




			VSOutPutDebugString("Load VSEngine texture Chair1.Chair1_D\n");
			printf("Load VSEngine texture Chair1.Chair1_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Chair1//Chair1_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Chair1.Chair1_N\n");
			printf("Load VSEngine texture Chair1.Chair1_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Chair1//Chair1_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model Chair1\n");
			printf("Load VSEngine Model Chair1\n");
			VSStaticMeshNodeRPtr pChair1Model = VSResourceManager::LoadASYNStaticMesh(_T("Chair1.STMODEL"), false);
			VSStaticMeshNode * pChair1ModelNode = pChair1Model->GetResource();
			VSGeometryNode * pGeometryNode = pChair1ModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model Chair1Phone\n");
			printf("Save VSEngine Model Chair1Phone\n");
			VSResourceManager::NewSaveStaticMesh(pChair1ModelNode, _T("Chair1Phone"), true);
		}

		{
			// texture 
			VSOutPutDebugString("Load diffuse Model_D texture\n");
			printf("Load diffuse Model_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Model_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Model.Model_D\n");
			printf("Save VSEngine texture Model.Model_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Model//Model_D"), true);



			VSOutPutDebugString("Load normal Model_N texture\n");
			printf("Load normal Model_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Model_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Model.Model_N\n");
			printf("Save VSEngine texture Model.Model_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Model//Model_N"), true);




			VSOutPutDebugString("Load VSEngine texture Model.Model_D\n");
			printf("Load VSEngine texture Model.Model_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Model//Model_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Model.Model_N\n");
			printf("Load VSEngine texture Model.Model_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Model//Model_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model Model\n");
			printf("Load VSEngine Model Model\n");
			VSStaticMeshNodeRPtr pModelModel = VSResourceManager::LoadASYNStaticMesh(_T("Model.STMODEL"), false);
			VSStaticMeshNode * pModelModelNode = pModelModel->GetResource();
			VSGeometryNode * pGeometryNode = pModelModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model ModelPhone\n");
			printf("Save VSEngine Model ModelPhone\n");
			VSResourceManager::NewSaveStaticMesh(pModelModelNode, _T("ModelPhone"), true);
		}

		{
			// texture 
			VSOutPutDebugString("Load diffuse Table1_D texture\n");
			printf("Load diffuse Table1_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Table1_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Table1.Table1_D\n");
			printf("Save VSEngine texture Table1.Table1_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Table1//Table1_D"), true);



			VSOutPutDebugString("Load normal Table1_N texture\n");
			printf("Load normal Table1_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Table1_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Table1.Table1_N\n");
			printf("Save VSEngine texture Table1.Table1_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Table1//Table1_N"), true);




			VSOutPutDebugString("Load VSEngine texture Table1.Table1_D\n");
			printf("Load VSEngine texture Table1.Table1_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Table1//Table1_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Table1.Table1_N\n");
			printf("Load VSEngine texture Table1.Table1_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Table1//Table1_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model Table1\n");
			printf("Load VSEngine Model Table1\n");
			VSStaticMeshNodeRPtr pTable1Model = VSResourceManager::LoadASYNStaticMesh(_T("Table1.STMODEL"), false);
			VSStaticMeshNode * pTable1ModelNode = pTable1Model->GetResource();
			VSGeometryNode * pGeometryNode = pTable1ModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model Table1Phone\n");
			printf("Save VSEngine Model Table1Phone\n");
			VSResourceManager::NewSaveStaticMesh(pTable1ModelNode, _T("Table1Phone"), true);
		}


		{
			// texture 
			VSOutPutDebugString("Load diffuse Table2_D texture\n");
			printf("Load diffuse Table2_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Table2_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Table2.Table2_D\n");
			printf("Save VSEngine texture Table2.Table2_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Table2//Table2_D"), true);



			VSOutPutDebugString("Load normal Table2_N texture\n");
			printf("Load normal Table2_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Table2_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Table2.Table2_N\n");
			printf("Save VSEngine texture Table2.Table2_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Table2//Table2_N"), true);




			VSOutPutDebugString("Load VSEngine texture Table2.Table2_D\n");
			printf("Load VSEngine texture Table2.Table2_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Table2//Table2_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Table2.Table2_N\n");
			printf("Load VSEngine texture Table2.Table2_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Table2//Table2_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model Table2\n");
			printf("Load VSEngine Model Table2\n");
			VSStaticMeshNodeRPtr pTable2Model = VSResourceManager::LoadASYNStaticMesh(_T("Table2.STMODEL"), false);
			VSStaticMeshNode * pTable2ModelNode = pTable2Model->GetResource();
			VSGeometryNode * pGeometryNode = pTable2ModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model Table2Phone\n");
			printf("Save VSEngine Model Table2Phone\n");
			VSResourceManager::NewSaveStaticMesh(pTable2ModelNode, _T("Table2Phone"), true);
		}

		{
			// texture 
			VSOutPutDebugString("Load diffuse Chair2_D texture\n");
			printf("Load diffuse Chair2_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Chair2_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture Chair2.Chair2_D\n");
			printf("Save VSEngine texture Chair2.Chair2_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("Chair2//Chair2_D"), true);



			VSOutPutDebugString("Load normal Chair2_N texture\n");
			printf("Load normal Chair2_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Chair2_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture Chair2.Chair2_N\n");
			printf("Save VSEngine texture Chair2.Chair2_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("Chair2//Chair2_N"), true);




			VSOutPutDebugString("Load VSEngine texture Chair2.Chair2_D\n");
			printf("Load VSEngine texture Chair2.Chair2_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Chair2//Chair2_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture Chair2.Chair2_N\n");
			printf("Load VSEngine texture Chair2.Chair2_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Chair2//Chair2_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model Chair2\n");
			printf("Load VSEngine Model Chair2\n");
			VSStaticMeshNodeRPtr pChair2Model = VSResourceManager::LoadASYNStaticMesh(_T("Chair2.STMODEL"), false);
			VSStaticMeshNode * pChair2ModelNode = pChair2Model->GetResource();
			VSGeometryNode * pGeometryNode = pChair2ModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model Chair2Phone\n");
			printf("Save VSEngine Model Chair2Phone\n");
			VSResourceManager::NewSaveStaticMesh(pChair2ModelNode, _T("Chair2Phone"), true);
		}


		{
			// texture 
			VSOutPutDebugString("Load diffuse ZhuZi1_D texture\n");
			printf("Load diffuse ZhuZi1_D texture\n");
			VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("ZhuZi1_D.tga");
			VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

			VSOutPutDebugString("Save VSEngine texture ZhuZi1.ZhuZi1_D\n");
			printf("Save VSEngine texture ZhuZi1.ZhuZi1_D\n");
			VSResourceManager::NewSaveTexture(pDiffuse, _T("ZhuZi1//ZhuZi1_D"), true);



			VSOutPutDebugString("Load normal ZhuZi1_N texture\n");
			printf("Load normal ZhuZi1_N texture\n");
			VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("ZhuZi1_N.tga");
			VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

			VSOutPutDebugString("Save VSEngine texture ZhuZi1.ZhuZi1_N\n");
			printf("Save VSEngine texture ZhuZi1.ZhuZi1_N\n");
			VSResourceManager::NewSaveTexture(pNormal, _T("ZhuZi1//ZhuZi1_N"), true);




			VSOutPutDebugString("Load VSEngine texture ZhuZi1.ZhuZi1_D\n");
			printf("Load VSEngine texture ZhuZi1.ZhuZi1_D\n");
			VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("ZhuZi1//ZhuZi1_D.TEXTURE"), false);

			VSOutPutDebugString("Load VSEngine texture ZhuZi1.ZhuZi1_N\n");
			printf("Load VSEngine texture ZhuZi1.ZhuZi1_N\n");
			VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("ZhuZi1//ZhuZi1_N.TEXTURE"), false);




			//Model
			VSOutPutDebugString("Load VSEngine Model ZhuZi1\n");
			printf("Load VSEngine Model ZhuZi1\n");
			VSStaticMeshNodeRPtr pZhuZi1Model = VSResourceManager::LoadASYNStaticMesh(_T("ZhuZi1.STMODEL"), false);
			VSStaticMeshNode * pZhuZi1ModelNode = pZhuZi1Model->GetResource();
			VSGeometryNode * pGeometryNode = pZhuZi1ModelNode->GetGeometryNode(0);

			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(0);
			pGeometry->AddMaterialInstance(pMaterialPhoneR);

			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
			pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);


			VSOutPutDebugString("Save VSEngine Model ZhuZi1Phone\n");
			printf("Save VSEngine Model ZhuZi1Phone\n");
			VSResourceManager::NewSaveStaticMesh(pZhuZi1ModelNode, _T("ZhuZi1Phone"), true);
		}
		return true;
	}
}
#endif