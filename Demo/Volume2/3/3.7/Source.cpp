//创建走路和待机动画立即混合的AnimTree，并保存。
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

		VSAnimTreeTwoImmediateAnimPtr  pAnimTreeTwoBlendAnim = VS_NEW VSAnimTreeTwoImmediateAnim(_T("AnimTreeTwoSmoothBlendAnim"), _T("Idle"), _T("Walk"));


		VSOutPutDebugString("Save VSEngine AnimTreeTwoImmediateAnim\n");
		printf("Save VSEngine AnimTreeTwoImmediateAnim\n");
		VSResourceManager::NewSaveAnimTree(pAnimTreeTwoBlendAnim, _T("AnimTreeTwoImmediateAnim"), true);

		return true;
	}
}
#endif