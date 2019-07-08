//这个Demo展示创建一个CLOD Mesh过程，并存储为新的模型。
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

	
		VSOutPutDebugString("Make CLOD Data\n");
		printf("Make CLOD Data\n");
		VSStaticMeshNode * pStaticMeshNodeLOD0 = pMonsterLOD0Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD0 = pStaticMeshNodeLOD0->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD0->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD0->GetNormalGeometry(i);
			pGeometry->CreateClodMesh();
		}
 		VSOutPutDebugString("Save VSEngine Model CLODMonster\n");
 		printf("Save VSEngine Model CLODMonster\n");
		VSResourceManager::NewSaveStaticMesh(pStaticMeshNodeLOD0, _T("CLODMonster"), true);
		
		return true;
	}
}
#endif