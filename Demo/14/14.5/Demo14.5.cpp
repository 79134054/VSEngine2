//给Monster的右手骨头创建一个Socket，通过用3DMax查看FBX文件骨架，可以知道右手骨头名字叫Bip01 R Hand，创建Socket名字叫HandSocket。
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
		VSOutPutDebugString("Load VSEngine Model NewMonsterWithAnim\n");
		printf("Load VSEngine Model NewMonsterWithAnim\n");
		VSSkelectonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNSkelectonMesh(_T("NewMonsterWithAnim.SKMODEL"), false);




		
		
		VSSkelectonMeshNode * pSkelectonMeshNode = pMonsterModel->GetResource();

		
		pSkelectonMeshNode->CreateSocket(_T("Bip01 R Hand"), _T("HandSocket"));

		VSOutPutDebugString("Save VSEngine Model NewMonsterWithSocket\n");
		printf("Save VSEngine Model NewMonsterWithSocket\n");
		VSResourceManager::NewSaveSkelectonMeshNode(pSkelectonMeshNode, _T("NewMonsterWithSocket"), true);

		
		return true;
	}
}
#endif