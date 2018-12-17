//这个demo给导出的Stone.STMODEL创建一个材质资源，并赋予它，然后导出新的模型叫做NewStone.STMODEL。
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
	protected:
		VSMaterialPtr m_SaveMaterial;
	
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
		m_SaveMaterial = NULL;
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
		m_SaveMaterial = NULL;
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

		VSOutPutDebugString("Load diffuse tga texture\n");
		printf("Load diffuse tga texture\n");
		VSFileName DiffuseFileName = VSResourceManager::ms_TexturePath + _T("stone_d.tga");
		VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTextureCompress(DiffuseFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture Stone.Diffuse\n");
		printf("Save VSEngine texture Stone.Diffuse\n");
		VSResourceManager::NewSaveTexture(pDiffuse, _T("Stone//Diffuse"),true);

		VSOutPutDebugString("Load normal tga texture\n");
		printf("Load normal tga texture\n");
		VSFileName NormalFileName = VSResourceManager::ms_TexturePath + _T("stone_n.tga");
		VSTexAllStatePtr pNormal = VSResourceManager::Load2DTextureCompress(NormalFileName.GetBuffer(), pTriLineSamplerState, 2, true, false);

		VSOutPutDebugString("Save VSEngine texture Stone.Normal\n");
		printf("Save VSEngine texture Stone.Normal\n");
		VSResourceManager::NewSaveTexture(pNormal, _T("Stone//Normal"), true);

		VSOutPutDebugString("Load VSEngine texture Stone.Diffuse\n");
		printf("Load VSEngine texture Stone.Diffuse\n");
		VSTexAllStateRPtr  pDiffuseR =  VSResourceManager::LoadASYNTexture(_T("Stone//Diffuse.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture Stone.Normal\n");
		printf("Load VSEngine texture Stone.Normal\n");
		VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNTexture(_T("Stone//Normal.TEXTURE"), false);

		VSOutPutDebugString("Save VSEngine Material TextureAndNormal\n");
		printf("Save VSEngine Material TextureAndNormal\n");
		m_SaveMaterial = VS_NEW VSMaterialTextureAndNormal(_T("TextureAndNormal"), pDiffuseR,pNormalR);
		VSResourceManager::NewSaveMaterial(m_SaveMaterial, _T("TextureAndNormal"),true);


		VSOutPutDebugString("Load VSEngine Material TextureAndNormal\n");
		printf("Load VSEngine Material TextureAndNormal\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNMaterial(_T("TextureAndNormal.MATERIAL"), false);

		VSOutPutDebugString("Load VSEngine Model Stone\n");
		printf("Load VSEngine Model Stone\n");
		VSStaticMeshNodeRPtr pModel = VSResourceManager::LoadASYNStaticMesh(_T("Stone.STMODEL"), false);

		VSOutPutDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNode = pModel->GetResource();
		VSGeometryNode * pGeometryNode = pStaticMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pMaterialR);
		}
		VSOutPutDebugString("Save VSEngine Model NewStone\n");
		printf("Save VSEngine Model NewStone\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNode, _T("NewStone"), true);
		return true;
	}
}
#endif