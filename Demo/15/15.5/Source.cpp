//创建自定义材质，用于演示反射效果
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

		//Model
		
		VSOutPutDebugString("Save VSEngine Material MaterialNoLight\n");
		printf("Save VSEngine Material MaterialNoLight\n");
		VSMaterialPtr  pMaterial = VS_NEW VSMaterialNoLight(_T("MaterialNoLight"));
		VSResourceManager::NewSaveMaterial(pMaterial, _T("MaterialNoLight"), true);

		VSOutPutDebugString("Load VSEngine Material MaterialNoLight\n");
		printf("Load VSEngine Material MaterialNoLight\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNMaterial(_T("MaterialNoLight.MATERIAL"), false);

		VSOutPutDebugString("Load VSEngine Model OceanPlane\n");
		printf("Load VSEngine Model OceanPlane\n");
		VSStaticMeshNodeRPtr pModel = VSResourceManager::LoadASYNStaticMesh(_T("OceanPlane.STMODEL"), false);

		VSOutPutDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNode = pModel->GetResource();
		VSGeometryNode * pGeometryNode = pStaticMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->AddMaterialInstance(pMaterialR);
		}
		VSOutPutDebugString("Save VSEngine Model NewOceanPlane\n");
		printf("Save VSEngine Model NewOceanPlane\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNode, _T("ReflectPlane"), true);
		return true;
	}
}
#endif