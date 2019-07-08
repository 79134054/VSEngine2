//展示了给导出的一个蒙皮模型添加材质的过程
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSMaterial.h"
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
		VSResourceManager::ms_bUpdateThread = false;
		VSResourceManager::ms_bRenderThread = false;
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
		VSTexAllStateRPtr  pDiffuseR =  VSResourceManager::LoadASYNTexture(_T("Monster//Monster_d.TEXTURE"), false);

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


		//material


		VSOutPutDebugString("Load VSEngine Material Phone\n");
		printf("Load VSEngine Material Phone\n");
		VSMaterialRPtr  pPhoneMaterialR = VSResourceManager::LoadASYNMaterial(_T("Phone.MATERIAL"), false);



		//Model
		VSOutPutDebugString("Load VSEngine Model Monster\n");
		printf("Load VSEngine Model Monster\n");
		VSSkelectonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNSkelectonMesh(_T("Monster.SKMODEL"), false);





		VSOutPutDebugString("Set Material\n");
		printf("Set Material\n");
		VSSkelectonMeshNode * pSkelectonMeshNode = pMonsterModel->GetResource();
		VSGeometryNode * pGeometryNode = pSkelectonMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pPhoneMaterialR);
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
		

	
		VSOutPutDebugString("Save VSEngine Model Monster\n");
		printf("Save VSEngine Model Monster\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonster"), true);

		
		return true;
	}
}
#endif