//部分混合动画，分别混合攻击动画的上半身和走路动画的下半身，实现可以边走边攻击动画
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSPartialAnimBlend.h"

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

		VSAnimTreePartialAnimPtr  pAnimTreePartialAnim1 = VS_NEW VSAnimTreePartialAnim(_T("AnimTreePartialAnim"), _T("Walk"),_T("Attack2"));
		VSPartialAnimBlend * pAnimBlend1 = (VSPartialAnimBlend * )pAnimTreePartialAnim1->GetAnimFunctionFromShowName(_T("PartialAnimBlend"));
		pAnimBlend1->AddControllBone(_T("Bip01 Neck"));
		pAnimBlend1->SetBoneWeight(0, _T("Bip01 Neck"), 0.0f);
		pAnimBlend1->SetBoneWeight(1, _T("Bip01 Neck"), 1.0f);

		VSOutPutDebugString("Save VSEngine AnimTree AnimTreePartialAnim1\n");
		printf("Save VSEngine AnimTree AnimTreePartialAnim1\n");
		VSResourceManager::NewSaveAnimTree(pAnimTreePartialAnim1, _T("AnimTreePartialAnim1"), true);


		VSAnimTreePartialAnimPtr  pAnimTreePartialAnim2 = VS_NEW VSAnimTreePartialAnim(_T("AnimTreePartialAnim"), _T("Attack2"), _T("Walk"));
		VSPartialAnimBlend * pAnimBlend2 = (VSPartialAnimBlend *)pAnimTreePartialAnim2->GetAnimFunctionFromShowName(_T("PartialAnimBlend"));
		pAnimBlend2->AddControllBone(_T("Bip01 L Thigh"));
		pAnimBlend2->SetBoneWeight(0, _T("Bip01 L Thigh"), 0.0f);
		pAnimBlend2->SetBoneWeight(1, _T("Bip01 L Thigh"), 1.0f);

		pAnimBlend2->AddControllBone(_T("Bip01 R Thigh"));
		pAnimBlend2->SetBoneWeight(0, _T("Bip01 R Thigh"), 0.0f);
		pAnimBlend2->SetBoneWeight(1, _T("Bip01 R Thigh"), 1.0f);

		pAnimBlend2->AddControllBone(_T("Bip01 Tail"));
		pAnimBlend2->SetBoneWeight(0, _T("Bip01 Tail"), 0.0f);
		pAnimBlend2->SetBoneWeight(1, _T("Bip01 Tail"), 1.0f);
		VSOutPutDebugString("Save VSEngine AnimTree AnimTreePartialAnim2\n");
		printf("Save VSEngine AnimTree AnimTreePartialAnim2\n");
		VSResourceManager::NewSaveAnimTree(pAnimTreePartialAnim2, _T("AnimTreePartialAnim2"), true);

		return true;
	}
}
#endif