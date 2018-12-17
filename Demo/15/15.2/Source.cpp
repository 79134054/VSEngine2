//创建和存储后期链，分别为屏幕灰色后期链、屏幕灰色加自定义材质链。
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSPostEffectSet.h"
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
		VSOutPutDebugString("Create PostEffect Gray\n");
		printf("Create PostEffect Gray\n");
		
		VSPESetGrayPtr pPESetGray = VS_NEW VSPESetGray(_T("PostEffect_Gray"));

		VSOutPutDebugString("Save VSEngine PostEffect PostEffect_Gray\n");
		printf("Save VSEngine PostEffect PostEffect_Gray\n");
		VSResourceManager::NewSavePostEffect(pPESetGray, _T("PostEffect_Gray"), true);


		VSOutPutDebugString("Save VSEngine Material PostEffectMaterial\n");
		printf("Save VSEngine Material PostEffectMaterial\n");
		VSMaterialPtr pSaveMaterial = VS_NEW VSPostEffectMaterial(_T("PostEffectMaterial"));
		VSResourceManager::NewSaveMaterial(pSaveMaterial, _T("PostEffectMaterial"), true);

		VSOutPutDebugString("Load VSEngine Material PostEffectMaterial\n");
		printf("Load VSEngine Material PostEffectMaterial\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNMaterial(_T("PostEffectMaterial.MATERIAL"), false);

		VSOutPutDebugString("Create PostEffect MaterialAndGray\n");
		printf("Create PostEffect MaterialAndGray\n");

		VSPESetMaterialAndGrayPtr pPESetMaterialAndGray = VS_NEW VSPESetMaterialAndGray(_T("PostEffect_MaterialAndGray"),pMaterialR);

		VSOutPutDebugString("Save VSEngine PostEffect PostEffect_Gray\n");
		printf("Save VSEngine PostEffect PostEffect_Gray\n");
		VSResourceManager::NewSavePostEffect(pPESetMaterialAndGray, _T("PostEffect_MaterialAndGray"), true);
		return true;
	}
}
#endif