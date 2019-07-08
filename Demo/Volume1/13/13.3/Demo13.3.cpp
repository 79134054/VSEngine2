//展示了把5个 Mesh合并成一个具有LOD效果的Mesh，并存储成新Mesh。
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
		VSOutPutDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD0Model = VSResourceManager::LoadASYNStaticMesh(_T("NewMonsterLOD0.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD1\n");
		printf("Load VSEngine Model MonsterLOD1\n");
		VSStaticMeshNodeRPtr pMonsterLOD1Model = VSResourceManager::LoadASYNStaticMesh(_T("NewMonsterLOD1.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD2\n");
		printf("Load VSEngine Model MonsterLOD2\n");
		VSStaticMeshNodeRPtr pMonsterLOD2Model = VSResourceManager::LoadASYNStaticMesh(_T("NewMonsterLOD2.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD3\n");
		printf("Load VSEngine Model MonsterLOD3\n");
		VSStaticMeshNodeRPtr pMonsterLOD3Model = VSResourceManager::LoadASYNStaticMesh(_T("NewMonsterLOD3.STMODEL"), false);

		VSOutPutDebugString("Load VSEngine Model MonsterLOD4\n");
		printf("Load VSEngine Model MonsterLOD4\n");
		VSStaticMeshNodeRPtr pMonsterLOD4Model = VSResourceManager::LoadASYNStaticMesh(_T("NewMonsterLOD4.STMODEL"), false);
	
		VSStaticMeshNode * pStaticMeshNodeLOD0 = pMonsterLOD0Model->GetResource();
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD1Model);
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD2Model);
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD3Model);
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD4Model);
 		VSOutPutDebugString("Save VSEngine Model DLODMonster\n");
 		printf("Save VSEngine Model DLODMonster\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD0, _T("DLODMonster"), true);
		
		return true;
	}
}
#endif