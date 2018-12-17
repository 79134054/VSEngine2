//创建和存储两个ShaderFunction混合材质，10中光照模型材质，1种自定义光照材质，LightFunction材质
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


		VSOutPutDebugString("Load VSEngine texture Monster.Monster_d\n");
		printf("Load VSEngine texture Monster.Monster_d\n");
		VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_d.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_n\n");
		printf("Load VSEngine texture Monster.Monster_n\n");
		VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_n.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_s\n");
		printf("Load VSEngine texture Monster.Monster_s\n");
		VSTexAllStateRPtr  pSpecularR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_s.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_e\n");
		printf("Load VSEngine texture Monster.Monster_e\n");
		VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_e.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_d\n");
		printf("Load VSEngine texture Monster.Monster_d\n");
		VSTexAllStateRPtr  pDiffuse_wR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_w_d.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_n\n");
		printf("Load VSEngine texture Monster.Monster_n\n");
		VSTexAllStateRPtr  pNormal_wR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_w_n.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_s\n");
		printf("Load VSEngine texture Monster.Monster_s\n");
		VSTexAllStateRPtr  pSpecular_wR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_w_s.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Monster.Monster_e\n");
		printf("Load VSEngine texture Monster.Monster_e\n");
		VSTexAllStateRPtr  pEmissive_wR = VSResourceManager::LoadASYNTexture(_T("Monster//Monster_w_e.TEXTURE"), false);
		
		VSOutPutDebugString("Load VSEngine Model Monster\n");
		printf("Load VSEngine Model Monster\n");
		VSSkelectonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNSkelectonMesh(_T("Monster.SKMODEL"), false);

		// PhoneTwoPass
		VSOutPutDebugString("Save VSEngine Material MaterialPhoneTwoPass\n");
		printf("Save VSEngine Material MaterialPhoneTwoPass\n");
		VSMaterialPhoneTwoPassPtr  pMaterial = VS_NEW VSMaterialPhoneTwoPass(_T("MaterialPhoneTwoPass"),pDiffuseR, pNormalR, pSpecularR, pEmissiveR,true);
		VSResourceManager::NewSaveMaterial(pMaterial, _T("MaterialPhoneTwoPass"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialPhoneTwoPass\n");
		printf("Load VSEngine Material MaterialPhoneTwoPass\n");
		VSMaterialRPtr  pMaterialPhoneTwoPassR = VSResourceManager::LoadASYNMaterial(_T("MaterialPhoneTwoPass.MATERIAL"), false);


		VSSkelectonMeshNode * pSkelectonMeshNode = pMonsterModel->GetResource();
		VSGeometryNode * pGeometryNode = pSkelectonMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pMaterialPhoneTwoPassR);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterPhoneTwoPass\n");
		printf("Save VSEngine Model NewMonsterPhoneTwoPass\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterPhoneTwoPass"), true);

		// Phone
		VSOutPutDebugString("Save VSEngine Material MaterialPhone\n");
		printf("Save VSEngine Material MaterialPhone\n");
		VSMaterialPhonePtr  pMaterialPhone = VS_NEW VSMaterialPhone(_T("MaterialPhone"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR, true);
		VSResourceManager::NewSaveMaterial(pMaterialPhone, _T("MaterialPhone"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialPhone\n");
		printf("Load VSEngine Material MaterialPhone\n");
		VSMaterialRPtr  pMaterialPhoneR = VSResourceManager::LoadASYNMaterial(_T("MaterialPhone.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialPhoneR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterPhone\n");
		printf("Save VSEngine Model NewMonsterPhone\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterPhone"), true);

		// BlinnPhone
		VSOutPutDebugString("Save VSEngine Material MaterialBlinnPhone\n");
		printf("Save VSEngine Material MaterialBlinnPhone\n");
		VSMaterialPhonePtr  pMaterialBlinnPhone = VS_NEW VSMaterialPhone(_T("MaterialBlinnPhone"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR, false);
		VSResourceManager::NewSaveMaterial(pMaterialBlinnPhone, _T("MaterialBlinnPhone"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialBlinnPhone\n");
		printf("Load VSEngine Material MaterialBlinnPhone\n");
		VSMaterialRPtr  pMaterialBlinnPhoneR = VSResourceManager::LoadASYNMaterial(_T("MaterialBlinnPhone.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialBlinnPhoneR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterBlinnPhone\n");
		printf("Save VSEngine Model NewMonsterBlinnPhone\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterBlinnPhone"), true);



		//OrenNayar

		VSOutPutDebugString("Save VSEngine Material MaterialOrenNayar\n");
		printf("Save VSEngine Material MaterialOrenNayar\n");
		VSMaterialOrenNayarPtr  pMaterialOrenNayar = VS_NEW VSMaterialOrenNayar(_T("MaterialOrenNayar"), pDiffuseR, pNormalR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialOrenNayar, _T("MaterialOrenNayar"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialOrenNayar\n");
		printf("Load VSEngine Material MaterialOrenNayar\n");
		VSMaterialRPtr  pMaterialOrenNayarR = VSResourceManager::LoadASYNMaterial(_T("MaterialOrenNayar.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialOrenNayarR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterOrenNayar\n");
		printf("Save VSEngine Model NewMonsterOrenNayar\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterOrenNayar"), true);


		//Minnaert
		VSOutPutDebugString("Save VSEngine Material MaterialMinnaert\n");
		printf("Save VSEngine Material MaterialMinnaert\n");
		VSMaterialMinnaertPtr  pMaterialMinnaert = VS_NEW VSMaterialMinnaert(_T("MaterialMinnaert"), pDiffuseR, pNormalR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialMinnaert, _T("MaterialMinnaert"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialMinnaert\n");
		printf("Load VSEngine Material MaterialMinnaert\n");
		VSMaterialRPtr  pMaterialMinnaertR = VSResourceManager::LoadASYNMaterial(_T("MaterialMinnaert.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialMinnaertR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterMinnaert\n");
		printf("Save VSEngine Model NewMonsterMinnaert\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterMinnaert"), true);



		// Strauss
		VSOutPutDebugString("Save VSEngine Material MaterialStrauss\n");
		printf("Save VSEngine Material MaterialStrauss\n");
		VSMaterialStraussPtr  pMaterialStrauss = VS_NEW VSMaterialStrauss(_T("MaterialStrauss"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialStrauss, _T("MaterialStrauss"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialStrauss\n");
		printf("Load VSEngine Material MaterialStrauss\n");
		VSMaterialRPtr  pMaterialStraussR = VSResourceManager::LoadASYNMaterial(_T("MaterialStrauss.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialStraussR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterStrauss\n");
		printf("Save VSEngine Model NewMonsterStrauss\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterStrauss"), true);


		//Shirley
		VSOutPutDebugString("Save VSEngine Material MaterialShirley\n");
		printf("Save VSEngine Material MaterialShirley\n");
		VSMaterialShirleyPtr  pMaterialShirley = VS_NEW VSMaterialShirley(_T("MaterialShirley"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialShirley, _T("MaterialShirley"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialShirley\n");
		printf("Load VSEngine Material MaterialShirley\n");
		VSMaterialRPtr  pMaterialShirleyR = VSResourceManager::LoadASYNMaterial(_T("MaterialShirley.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialShirleyR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterShirley\n");
		printf("Save VSEngine Model NewMonsterShirley\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterShirley"), true);




		//Schlick
		VSOutPutDebugString("Save VSEngine Material MaterialSchlick\n");
		printf("Save VSEngine Material MaterialSchlick\n");
		VSMaterialSchlickPtr  pMaterialSchlick = VS_NEW VSMaterialSchlick(_T("MaterialSchlick"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialSchlick, _T("MaterialSchlick"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialSchlick\n");
		printf("Load VSEngine Material MaterialSchlick\n");
		VSMaterialRPtr  pMaterialSchlickR = VSResourceManager::LoadASYNMaterial(_T("MaterialSchlick.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialSchlickR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterSchlick\n");
		printf("Save VSEngine Model NewMonsterSchlick\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterSchlick"), true);





		//CookTorrance
		VSOutPutDebugString("Save VSEngine Material MaterialCookTorrance\n");
		printf("Save VSEngine Material MaterialCookTorrance\n");
		VSMaterialCookTorrancePtr  pMaterialCookTorrance = VS_NEW VSMaterialCookTorrance(_T("MaterialCookTorrance"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR,true);
		VSResourceManager::NewSaveMaterial(pMaterialCookTorrance, _T("MaterialCookTorrance"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialCookTorrance\n");
		printf("Load VSEngine Material MaterialCookTorrance\n");
		VSMaterialRPtr  pMaterialCookTorranceR = VSResourceManager::LoadASYNMaterial(_T("MaterialCookTorrance.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialCookTorranceR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterCookTorrance\n");
		printf("Save VSEngine Model NewMonsterCookTorrance\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterCookTorrance"), true);




		//IsotropicWard
		VSOutPutDebugString("Save VSEngine Material MaterialIsotropicWard\n");
		printf("Save VSEngine Material MaterialIsotropicWard\n");
		VSMaterialIsotropicWardPtr  pMaterialIsotropicWard = VS_NEW VSMaterialIsotropicWard(_T("MaterialIsotropicWard"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialIsotropicWard, _T("MaterialIsotropicWard"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialIsotropicWard\n");
		printf("Load VSEngine Material MaterialIsotropicWard\n");
		VSMaterialRPtr  pMaterialIsotropicWardR = VSResourceManager::LoadASYNMaterial(_T("MaterialIsotropicWard.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialIsotropicWardR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterIsotropicWard\n");
		printf("Save VSEngine Model NewMonsterIsotropicWard\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterIsotropicWard"), true);



		//AnisotropicWard
		VSOutPutDebugString("Save VSEngine Material MaterialAnisotropicWard\n");
		printf("Save VSEngine Material MaterialAnisotropicWard\n");
		VSMaterialAnisotropicWardPtr  pMaterialAnisotropicWard = VS_NEW VSMaterialAnisotropicWard(_T("MaterialAnisotropicWard"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialAnisotropicWard, _T("MaterialAnisotropicWard"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialAnisotropicWard\n");
		printf("Load VSEngine Material MaterialAnisotropicWard\n");
		VSMaterialRPtr  pMaterialAnisotropicWardR = VSResourceManager::LoadASYNMaterial(_T("MaterialAnisotropicWard.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialAnisotropicWardR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterAnisotropicWard\n");
		printf("Save VSEngine Model NewMonsterAnisotropicWard\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterAnisotropicWard"), true);


		//Custom
		VSOutPutDebugString("Save VSEngine Material MaterialCustom\n");
		printf("Save VSEngine Material MaterialCustom\n");
		VSMaterialCustomPtr  pMaterialCustom = VS_NEW VSMaterialCustom(_T("MaterialCustom"), pDiffuseR, pNormalR, pEmissiveR);
		VSResourceManager::NewSaveMaterial(pMaterialCustom, _T("MaterialCustom"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialCustom\n");
		printf("Load VSEngine Material MaterialCustom\n");
		VSMaterialRPtr  pMaterialCustomR = VSResourceManager::LoadASYNMaterial(_T("MaterialCustom.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialCustomR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutPutDebugString("Save VSEngine Model NewMonsterCustom\n");
		printf("Save VSEngine Model NewMonsterCustom\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterCustom"), true);

		//LightFunction

		// texture 
		VSOutPutDebugString("Load LightFunction texture\n");
		printf("Load LightFunction texture\n");
		VSFileName LightFunctionFileName = VSResourceManager::ms_TexturePath + _T("LightFunction.tga");
		VSTexAllStatePtr pLightFunction = VSResourceManager::Load2DTextureCompress(LightFunctionFileName.GetBuffer(), (VSSamplerState *)VSSamplerState::GetTriLine(), 2, false, true);

		VSOutPutDebugString("Save VSEngine texture LightFunction.LightFunction\n");
		printf("Save VSEngine texture LightFunction.LightFunction\n");
		VSResourceManager::NewSaveTexture(pLightFunction, _T("LightFunction//LightFunction"), true);

		VSOutPutDebugString("Load VSEngine texture LightFunction.LightFunction\n");
		printf("Load VSEngine texture LightFunction.LightFunction\n");
		VSTexAllStateRPtr  pLightFunctionR = VSResourceManager::LoadASYNTexture(_T("LightFunction//LightFunction.TEXTURE"), false);

		VSOutPutDebugString("Save VSEngine Material LightFunction\n");
		printf("Save VSEngine Material LightFunction\n");
		VSLightMaterialPtr  pLightMaterial = VS_NEW VSLightMaterial(_T("LightFunction"), pLightFunctionR);
		VSResourceManager::NewSaveMaterial(pLightMaterial, _T("LightFunction"), true);

		return true;
	}
}
#endif