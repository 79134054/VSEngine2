//给Demo14.21模型添加MorphTree资源
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





		//Model
		VSOutPutDebugString("Load VSEngine Model NewFaceMorph\n");
		printf("Load VSEngine Model NewFaceMorph\n");
		VSStaticMeshNodeRPtr pMorphModel = VSResourceManager::LoadASYNStaticMesh(_T("NewFaceMorph.STMODEL"), false);


		VSMorphTreeRPtr pMorphTree = VSResourceManager::LoadASYNMorphTree(_T("MorphTreeTwoSmoothBlendAnim.MORPHTREE"), false);



		VSStaticMeshNode * pStaticMeshNode = pMorphModel->GetResource();


		pStaticMeshNode->SetMorphTree(pMorphTree);

		VSOutPutDebugString("Save VSEngine Model NewFaceMorphWithTwoMorph\n");
		printf("Save VSEngine Model NewFaceMorphWithTwoMorph\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNode, _T("NewFaceMorphWithTwoMorph"), true);

		return true;
	}
}
#endif