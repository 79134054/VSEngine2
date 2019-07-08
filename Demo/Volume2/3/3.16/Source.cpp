//把Demo14.24中输出的AnimTree资源设置到模型中，并保存。
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
		VSOutPutDebugString("Load VSEngine Model NewMonsterWithSocket\n");
		printf("Load VSEngine Model NewMonsterWithSocket\n");
		VSSkelectonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNSkelectonMesh(_T("NewMonsterWithSocket.SKMODEL"), false);


		VSAnimTreeRPtr pAnimTree1 = VSResourceManager::LoadASYNAnimTree(_T("MonsterAnimTree1.ANIMTREE"), false);



		VSSkelectonMeshNode * pSkelectonMeshNode = pMonsterModel->GetResource();


		pSkelectonMeshNode->SetAnimTree(pAnimTree1);

		VSOutPutDebugString("Save VSEngine Model AnimTreeMonster1\n");
		printf("Save VSEngine Model AnimTreeMonster1\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("AnimTreeMonster1"), true);


		VSAnimTreeRPtr pAnimTree2 = VSResourceManager::LoadASYNAnimTree(_T("MonsterAnimTree2.ANIMTREE"), false);
		pSkelectonMeshNode->SetAnimTree(pAnimTree2);

		VSOutPutDebugString("Save VSEngine Model AnimTreeMonster2\n");
		printf("Save VSEngine Model AnimTreeMonster2\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("AnimTreeMonster2"), true);
		return true;
	}
}
#endif