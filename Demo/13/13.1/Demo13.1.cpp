//这个Demo演示了加载5个层级LOD Mesh并赋予材质，存储成新的Mesh。这个Demo可以看见Material Instance的使用，具体的会在后面章节介绍。
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
		VSSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
		VSSamplerStatePtr pTriLineSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);
		// texture 
		VSOutPutDebugString("Load diffuse tga texture\n");
		printf("Load diffuse tga texture\n");
		VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("Monster_d.tga");
		VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_d\n");
		printf("Save VSEngine texture Monster.Monster_d\n");
		VSResourceManager::NewSaveTexture(pDiffuse, _T("Monster//Monster_d"),true);

		VSOutPutDebugString("Load normal tga texture\n");
		printf("Load normal tga texture\n");
		VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("Monster_n.tga");
		VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_n\n");
		printf("Save VSEngine texture Monster.Monster_n\n");
		VSResourceManager::NewSaveTexture(pNormal, _T("Monster//Monster_n"), true);


		VSOutPutDebugString("Load specular tga texture\n");
		printf("Load specular tga texture\n");
		VSFileName SpecularFileName = VSResourceManager::ms_TexturePath + _T("Monster_s.tga");
		VSTexAllStatePtr pSpecular = VSResourceManager::Load2DTextureCompress(SpecularFileName.GetBuffer(), pTriLineSamplerState, 2, false, false);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_s\n");
		printf("Save VSEngine texture Monster.Monster_s\n");
		VSResourceManager::NewSaveTexture(pSpecular, _T("Monster//Monster_s"), true);


		VSOutPutDebugString("Load emissive tga texture\n");
		printf("Load emissive tga texture\n");
		VSFileName EmissiveFileName = VSResourceManager::ms_TexturePath + _T("Monster_e.tga");
		VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTextureCompress(EmissiveFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_e\n");
		printf("Save VSEngine texture Monster.Monster_e\n");
		VSResourceManager::NewSaveTexture(pEmissive, _T("Monster//Monster_e"), true);



		VSOutPutDebugString("Load diffuse tga texture\n");
		printf("Load diffuse tga texture\n");
		VSFileName DiffuseWFileName = VSResourceManager::ms_TexturePath + _T("Monster_w_d.tga");
		VSTexAllStatePtr pDiffuseW = VSResourceManager::Load2DTextureCompress(DiffuseWFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_w_d\n");
		printf("Save VSEngine texture Monster.Monster_w_d\n");
		VSResourceManager::NewSaveTexture(pDiffuseW, _T("Monster//Monster_w_d"), true);

		VSOutPutDebugString("Load normal tga texture\n");
		printf("Load normal tga texture\n");
		VSFileName NormalWFileName = VSResourceManager::ms_TexturePath + _T("Monster_w_n.tga");
		VSTexAllStatePtr pNormalW = VSResourceManager::Load2DTextureCompress(NormalWFileName.GetBuffer(), pTriLineSamplerState, 2, true);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_w_n\n");
		printf("Save VSEngine texture Monster.Monster_w_n\n");
		VSResourceManager::NewSaveTexture(pNormalW, _T("Monster//Monster_w_n"), true);


		VSOutPutDebugString("Load specular tga texture\n");
		printf("Load specular tga texture\n");
		VSFileName SpecularWFileName = VSResourceManager::ms_TexturePath + _T("Monster_w_s.tga");
		VSTexAllStatePtr pSpecularW = VSResourceManager::Load2DTextureCompress(SpecularWFileName.GetBuffer(), pTriLineSamplerState, 2, false, false);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_w_s\n");
		printf("Save VSEngine texture Monster.Monster_w_s\n");
		VSResourceManager::NewSaveTexture(pSpecularW, _T("Monster//Monster_w_s"), true);


		VSOutPutDebugString("Load emissive tga texture\n");
		printf("Load emissive tga texture\n");
		VSFileName EmissiveWFileName = VSResourceManager::ms_TexturePath + _T("Monster_w_e.tga");
		VSTexAllStatePtr pEmissiveW = VSResourceManager::Load2DTextureCompress(EmissiveWFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture Monster.Monster_w_e\n");
		printf("Save VSEngine texture Monster.Monster_w_e\n");
		VSResourceManager::NewSaveTexture(pEmissiveW, _T("Monster//Monster_w_e"), true);




		
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
		VSOutPutDebugString("Save VSEngine Material OnlyTexture\n");
		printf("Save VSEngine Material OnlyTexture\n");
		VSMaterialPtr pOnlyTextureMaterial = VS_NEW VSMaterialOnlyTexture(_T("OnlyTexture"), pDiffuseR);
		VSResourceManager::NewSaveMaterial(pOnlyTextureMaterial, _T("OnlyTexture"), true);

		VSOutPutDebugString("Save VSEngine Material Phone\n");
		printf("Save VSEngine Material Phone\n");
		VSMaterialPtr pPhoneMaterial = VS_NEW VSMaterialPhone(_T("Phone"), pDiffuseR,pNormalR,pSpecularR,pEmissiveR,true);
		VSResourceManager::NewSaveMaterial(pPhoneMaterial, _T("Phone"), true);

		VSOutPutDebugString("Load VSEngine Material OnlyTexture\n");
		printf("Load VSEngine Material OnlyTexture\n");
		VSMaterialRPtr  pOnlyTextureMaterialR = VSResourceManager::LoadASYNMaterial(_T("OnlyTexture.MATERIAL"), false);

		VSOutPutDebugString("Load VSEngine Material TextureAndNormal\n");
		printf("Load VSEngine Material TextureAndNormal\n");
		VSMaterialRPtr  pTextureAndNormalMaterialR = VSResourceManager::LoadASYNMaterial(_T("TextureAndNormal.MATERIAL"), false);

		VSOutPutDebugString("Load VSEngine Material Phone\n");
		printf("Load VSEngine Material Phone\n");
		VSMaterialRPtr  pPhoneMaterialR = VSResourceManager::LoadASYNMaterial(_T("Phone.MATERIAL"), false);



		//Model
		VSOutPutDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD0Model = VSResourceManager::LoadASYNStaticMesh(_T("MonsterLOD0.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD1\n");
		printf("Load VSEngine Model MonsterLOD1\n");
		VSStaticMeshNodeRPtr pMonsterLOD1Model = VSResourceManager::LoadASYNStaticMesh(_T("MonsterLOD1.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD2Model = VSResourceManager::LoadASYNStaticMesh(_T("MonsterLOD2.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD3Model = VSResourceManager::LoadASYNStaticMesh(_T("MonsterLOD3.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD4Model = VSResourceManager::LoadASYNStaticMesh(_T("MonsterLOD4.STMODEL"), false);




		VSOutPutDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNodeLOD0 = pMonsterLOD0Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD0 = pStaticMeshNodeLOD0->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD0->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD0->GetNormalGeometry(i);
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
		

		VSStaticMeshNode * pStaticMeshNodeLOD1 = pMonsterLOD1Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD1 = pStaticMeshNodeLOD1->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD1->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD1->GetNormalGeometry(i);
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

		VSStaticMeshNode * pStaticMeshNodeLOD2 = pMonsterLOD2Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD2 = pStaticMeshNodeLOD2->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD2->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD2->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pTextureAndNormalMaterialR);
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

		VSStaticMeshNode * pStaticMeshNodeLOD3 = pMonsterLOD3Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD3 = pStaticMeshNodeLOD3->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD3->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD3->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pTextureAndNormalMaterialR);
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

		VSStaticMeshNode * pStaticMeshNodeLOD4 = pMonsterLOD4Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD4 = pStaticMeshNodeLOD4->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD4->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD4->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pOnlyTextureMaterialR);
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
		VSOutPutDebugString("Save VSEngine Model MonsterLOD0\n");
		printf("Save VSEngine Model MonsterLOD0\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD0, _T("NewMonsterLOD0"), true);

		VSOutPutDebugString("Save VSEngine Model MonsterLOD1\n");
		printf("Save VSEngine Model MonsterLOD1\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD1, _T("NewMonsterLOD1"), true);

		VSOutPutDebugString("Save VSEngine Model MonsterLOD2\n");
		printf("Save VSEngine Model MonsterLOD2\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD2, _T("NewMonsterLOD2"), true);

		VSOutPutDebugString("Save VSEngine Model MonsterLOD3\n");
		printf("Save VSEngine Model MonsterLOD3\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD3, _T("NewMonsterLOD3"), true);

		VSOutPutDebugString("Save VSEngine Model MonsterLOD4\n");
		printf("Save VSEngine Model MonsterLOD4\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD4, _T("NewMonsterLOD4"), true);
		return true;
	}
}
#endif