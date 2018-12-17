//给导出的Morph模型添加材质
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

		VSOutPutDebugString("Load Black tga texture\n");
		printf("Load Black tga texture\n");
		VSFileName BlackFileName = VSResourceManager::ms_TexturePath + _T("Black.tga");
		VSTexAllStatePtr pBlack = VSResourceManager::Load2DTextureCompress(BlackFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture FaceMorph.Black\n");
		printf("Save VSEngine texture FaceMorph.Black\n");
		VSResourceManager::NewSaveTexture(pBlack, _T("FaceMorph//Black"), true);

		VSOutPutDebugString("Load Eye tga texture\n");
		printf("Load Eye tga texture\n");
		VSFileName EyeFileName = VSResourceManager::ms_TexturePath + _T("Eye.tga");
		VSTexAllStatePtr pEye = VSResourceManager::Load2DTextureCompress(EyeFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture FaceMorph.Eye\n");
		printf("Save VSEngine texture FaceMorph.Eye\n");
		VSResourceManager::NewSaveTexture(pEye, _T("FaceMorph//Eye"), true);

		VSOutPutDebugString("Load Head tga texture\n");
		printf("Load Head tga texture\n");
		VSFileName HeadFileName = VSResourceManager::ms_TexturePath + _T("Head.tga");
		VSTexAllStatePtr pHead = VSResourceManager::Load2DTextureCompress(HeadFileName.GetBuffer(), pTriLineSamplerState, 2, false, true);

		VSOutPutDebugString("Save VSEngine texture FaceMorph.Head\n");
		printf("Save VSEngine texture FaceMorph.Head\n");
		VSResourceManager::NewSaveTexture(pHead, _T("FaceMorph//Head"), true);


		VSOutPutDebugString("Load VSEngine texture FaceMorph.Black\n");
		printf("Load VSEngine texture FaceMorph.Black\n");
		VSTexAllStateRPtr  pBlackR = VSResourceManager::LoadASYNTexture(_T("FaceMorph//Black.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture FaceMorph.Eye\n");
		printf("Load VSEngine texture FaceMorph.Eye\n");
		VSTexAllStateRPtr  pEyeR = VSResourceManager::LoadASYNTexture(_T("FaceMorph//Eye.TEXTURE"), false);

		VSOutPutDebugString("Load VSEngine texture FaceMorph.Head\n");
		printf("Load VSEngine texture FaceMorph.Head\n");
		VSTexAllStateRPtr  pHeadR = VSResourceManager::LoadASYNTexture(_T("FaceMorph//Head.TEXTURE"), false);



		VSOutPutDebugString("Load VSEngine Material OnlyTexture\n");
		printf("Load VSEngine Material OnlyTexture\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNMaterial(_T("OnlyTexture.MATERIAL"), false);

		VSOutPutDebugString("Load VSEngine Model FaceMorph\n");
		printf("Load VSEngine Model FaceMorph\n");
		VSStaticMeshNodeRPtr pModel = VSResourceManager::LoadASYNStaticMesh(_T("FaceMorph.STMODEL"), false);

		VSOutPutDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNode = pModel->GetResource();
		VSGeometryNode * pGeometryNode = pStaticMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pMaterialR);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pEyeR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pBlackR);
			}
			else if (i == 2)
			{
				pGeometry->GetMaterialInstance(0)->SetPShaderTexture(_T("DiffuseTexture"), pHeadR);
			}
		}
		printf("Save VSEngine Model NewFaceMorph\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNode, _T("NewFaceMorph"), true);
		return true;
	}
}
#endif