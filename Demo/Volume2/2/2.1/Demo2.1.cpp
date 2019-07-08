//加载引擎格式的动作到动画集合中，一共导入7个动画，第四个RootMotion为根骨动画，并一起保存在Mesh里面。
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
		VSOutPutDebugString("Load VSEngine Model NewMonster\n");
		printf("Load VSEngine Model NewMonster\n");
		VSSkelectonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNSkelectonMesh(_T("NewMonster.SKMODEL"), false);




		
		
		VSSkelectonMeshNode * pSkelectonMeshNode = pMonsterModel->GetResource();
		VSAnimSet* pAnimSet = pSkelectonMeshNode->GetAnimSet();
		if (pAnimSet == NULL)
		{
			pAnimSet = VS_NEW VSAnimSet();
		}
		VSOutPutDebugString("Load VSEngine Action Idle\n");
		printf("Load VSEngine Action Idle\n");
		VSAnimRPtr  pAnim0 = VSResourceManager::LoadASYNAction(_T("Idle"), false);

		VSOutPutDebugString("Load VSEngine Action Walk\n");
		printf("Load VSEngine Action Walk\n");
 		VSAnimRPtr  pAnim1 = VSResourceManager::LoadASYNAction("Walk", false);

		VSOutPutDebugString("Load VSEngine Action Attack\n");
		printf("Load VSEngine Action Attack\n");
 		VSAnimRPtr  pAnim2 = VSResourceManager::LoadASYNAction("Attack", false);

		VSOutPutDebugString("Load VSEngine Action RootMotion\n");
		printf("Load VSEngine Action RootMotion\n");
 		VSAnimRPtr  pAnim3 = VSResourceManager::LoadASYNAction("RootMotion", false);

		VSOutPutDebugString("Load VSEngine Action Attack2\n");
		printf("Load VSEngine Action Attack2\n");
		VSAnimRPtr  pAnim4 = VSResourceManager::LoadASYNAction("Attack2", false);
		
		VSOutPutDebugString("Load VSEngine Action AddtiveAnim\n");
		printf("Load VSEngine Action AddtiveAnim\n");
		VSAnimRPtr  pAnim5 = VSResourceManager::LoadASYNAction("AddtiveAnim", false);

		VSOutPutDebugString("Load VSEngine Action Pos\n");
		printf("Load VSEngine Action Pos\n");
		VSAnimRPtr  pAnim6 = VSResourceManager::LoadASYNAction("Pos", false);

		pAnimSet->AddAnim(_T("Idle"), pAnim0);
		pAnimSet->AddAnim(_T("Walk"), pAnim1);
		pAnimSet->AddAnim(_T("Attack"), pAnim2);
		pAnimSet->AddAnim(_T("RootMotion"), pAnim3);
		pAnimSet->AddAnim(_T("Attack2"), pAnim4);
		pAnimSet->AddAnim(_T("AddtiveAnim"), pAnim5);
		pAnimSet->AddAnim(_T("Pos"), pAnim6);

		pSkelectonMeshNode->SetAnimSet(pAnimSet);
		

		VSOutPutDebugString("Save VSEngine Model NewMonsterWithAnim\n");
		printf("Save VSEngine Model NewMonsterWithAnim\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterWithAnim"), true);

		
		return true;
	}
}
#endif