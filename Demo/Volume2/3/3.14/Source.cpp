//输出11节中两个AnimTree资源
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSMaterial.h"
#include "VSMonsterAnimTree.h"
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

		VSMonsterAnimTree1Ptr  pMonsterAnimTree1 = VS_NEW VSMonsterAnimTree1(_T("MonsterAnimTree1"));


		VSOutPutDebugString("Save VSEngine AnimTree MonsterAnimTree1\n");
		printf("Save VSEngine AnimTree MonsterAnimTree1\n");
		VSResourceManager::NewSaveAnimTree(pMonsterAnimTree1, _T("MonsterAnimTree1"), true);

		VSMonsterAnimTree2Ptr  pMonsterAnimTree2 = VS_NEW VSMonsterAnimTree2(_T("MonsterAnimTree2"));
		VSOutPutDebugString("Save VSEngine AnimTree MonsterAnimTree2\n");
		printf("Save VSEngine AnimTree MonsterAnimTree2\n");
		VSResourceManager::NewSaveAnimTree(pMonsterAnimTree2, _T("MonsterAnimTree2"), true);
		return true;
	}
}
#endif