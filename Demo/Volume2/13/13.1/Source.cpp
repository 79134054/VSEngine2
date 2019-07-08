//建立一个异步加载测试场景，保存成Map文件
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSWorld.h"
#include "VSStaticActor.h"
#include "VSSkyLight.h"
#include "VSDirectionLight.h"
#include "VSLightActor.h"
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

		VSSceneMapPtr pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

		// texture 
		VSOutPutDebugString("Rand Create Static Actor\n");
		printf("Rand Create Static Actor\n");
		VSString Path[] = { _T("LightPhone.STMODEL"), _T("TablePhone.STMODEL"), _T("Table1Phone.STMODEL"), _T("Table2Phone.STMODEL"),
			_T("ChairPhone.STMODEL"), _T("Chair1Phone.STMODEL"), _T("Chair2Phone.STMODEL"), _T("ModelPhone.STMODEL"), _T("RadioPhone.STMODEL"),
			_T("FanPhone.STMODEL"), _T("ZhuZiPhone.STMODEL"), _T("ZhuZi1Phone.STMODEL") };
		for (unsigned int i = 0; i < 200; i++)
		{
			float x = VSFRand() * 40000.0f - 20000.0f;
			float z = VSFRand() * 40000.0f - 20000.0f;
			unsigned int s = VSRand() % 12;
			unsigned int m = VSRand() % 5 + 1;
			VSWorld::ms_pWorld->CreateActor(Path[s].GetBuffer(), VSVector3(x, 0.0f, z), VSMatrix3X3::ms_Identity, VSVector3(m * 1.0f, m * 1.0f, m * 1.0f), pTestMap);
		}

		VSStaticActor* pStaticActor = (VSStaticActor*)VSWorld::ms_pWorld->CreateActor(_T("NewOceanPlane.STMODEL"), VSVector3(0.0f, 0.0f, 0.0f), VSMatrix3X3::ms_Identity, VSVector3(500.0f, 500.f, 500.0f), pTestMap);
		pStaticActor->GetTypeNode()->CastShadow(false);

		VSOutPutDebugString("Create Light\n");
		printf("Create Light\n");
		VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(VSVector3::ms_Zero, VSMatrix3X3::ms_Identity, VSVector3::ms_One, pTestMap);
		pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(0.0f, 0.0f, 0.5f, 1.0f);
		pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

		VSMatrix3X3 Dir;
		Dir.CreateEluer(0, AngleToRadian(45), AngleToRadian(45));
		VSDirectionLightActor * pDirectionLightActor = (VSDirectionLightActor *)VSWorld::ms_pWorld->CreateActor<VSDirectionLightActor>(VSVector3::ms_Zero, Dir, VSVector3::ms_One, pTestMap);
		pDirectionLightActor->GetTypeNode()->SetShadowType(VSDirectionLight::ST_CSM);

		VSOutPutDebugString("Build Quad Tree\n");
		printf("Build Quad Tree\n");
		pTestMap->GetScene()->Build();

		VSOutPutDebugString("Save Map\n");
		printf("Save Map\n");
		VSResourceManager::NewSaveMap(pTestMap, _T("Test"),true);
		return true;
	}
}
#endif