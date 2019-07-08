//保存一个AnimTree资源，只是单一的播放动画，没有混合。
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

		VSAnimTreeOneAnimPtr  pAnimTreeOneAnim = VS_NEW VSAnimTreeOneAnim(_T("AnimTreeOneAnim"), _T("RootMotion"));


		VSOutPutDebugString("Save VSEngine AnimTree AnimTreeOneAnim\n");
		printf("Save VSEngine AnimTree AnimTreeOneAnim\n");
		VSResourceManager::NewSaveAnimTree(pAnimTreeOneAnim, _T("AnimTreeOneAnim"), true);

		return true;
	}
}
#endif