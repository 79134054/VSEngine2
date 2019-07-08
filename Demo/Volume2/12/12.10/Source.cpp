//ÑÝÊ¾¾Û¹âµÆµÄProjectShadow
#include <VSApplication.h>
#include <VSEngineInput.h>
#include <VSCameraActor.h>
#include <VSWorld.h>
#include <VS1stCameraController.h>
#include "VSStaticActor.h"
#include "VSLightActor.h"
#include "VSSkyLight.h"
#include "VSViewFamily.h"
#include "VSTimer.h"
#include "VSSceneManager.h"
#include "VSSkelectonActor.h"
#include "VSPostEffectFunction.h"
#include "VSPointLight.h"
#include "VSDirectionLight.h"
#include "VSSpotLight.h"
using namespace VSEngine2;
class VSDemoWindowsApplication : public VSWindowApplication
{

	DLCARE_APPLICATION(VSDemoWindowsApplication);
public:
	VSDemoWindowsApplication();
	~VSDemoWindowsApplication();
	virtual bool PreInitial();
	virtual bool OnInitial();
	virtual bool OnTerminal();
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool OnDraw();
	virtual void OnMove(int xPos, int yPos);
	virtual void OnReSize(int iWidth, int iHeight);
	virtual void OnKeyDown(unsigned int uiKey);
	virtual void OnKeyUp(unsigned int uiKey);
	virtual void OnLButtonDown(int xPos, int yPos);
	virtual void OnLButtonUp(int xPos, int yPos);
	virtual void OnRButtonDown(int xPos, int yPos);
	virtual void OnRButtonUp(int xPos, int yPos);
	virtual void OnMButtonDown(int xPos, int yPos);
	virtual void OnMButtonUp(int xPos, int yPos);
	virtual void OnMouseMove(int xPos, int yPos);
	virtual void OnMouseWheel(int xPos, int yPos, int zDet);
protected:
	VSCameraActor* m_pCameraActor;
	VS1stCameraController* m_p1stCameraController;
	VSSceneMap * m_pTestMap;
	bool m_bLMouseDowning;
	int		m_iDetMouseX;
	int		m_iDetMouseY;
	int		m_iLastMouseX;
	int		m_iLastMouseY;
};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{


}
VSDemoWindowsApplication::~VSDemoWindowsApplication()
{

}
bool VSDemoWindowsApplication::PreInitial()
{
	VSWindowApplication::PreInitial();
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	//m_uiInputAPIType = VSEngineInput::IAT_DX;
	VSResourceManager::ms_bUpdateThread = false;
	VSResourceManager::ms_bRenderThread = false;
	m_bLMouseDowning = false;

	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
	m_iLastMouseX = 0;
	m_iLastMouseY = 0;

	VSEngineFlag::EnableCLODMesh = false;
	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();

	m_p1stCameraController = VSObject::GetInstance<VS1stCameraController>();
	m_pCameraActor = (VSCameraActor *)VSWorld::ms_pWorld->CreateActor<VSCameraActor>();
	m_pCameraActor->GetTypeNode()->AddController(m_p1stCameraController);
	VSVector3 CameraPos(-1000.0f, 300.0f, 0.0f);
	VSVector3 CameraDir(1.0f, 0.0f, 0.0f);
	m_pCameraActor->GetTypeNode()->CreateFromLookDir(CameraPos, CameraDir);
	m_pCameraActor->GetTypeNode()->SetPerspectiveFov(AngleToRadian(90.0f), (m_uiScreenWidth * 1.0f) / (m_uiScreenHeight), 1.0f, 8000.0f);

	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

	VSSkelectonActor * pSKActor1 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(-300, 0, 300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor1->GetTypeNode()->PlayAnim(_T("Idle"), 1.0f, VSController::RT_WRAP);

	VSSkelectonActor * pSKActor2 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(0, 0, 300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor2->GetTypeNode()->PlayAnim(_T("Walk"), 1.0f, VSController::RT_WRAP);

	VSSkelectonActor * pSKActor3 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(300, 0, 300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor3->GetTypeNode()->PlayAnim(_T("Attack"), 1.0f, VSController::RT_WRAP);

	VSSkelectonActor * pSKActor4 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor4->GetTypeNode()->PlayAnim(_T("RootMotion"), 1.0f, VSController::RT_WRAP);

	VSSkelectonActor * pSKActor5 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(-300, 0,-300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor5->GetTypeNode()->PlayAnim(_T("Attack2"), 1.0f, VSController::RT_WRAP);

	VSSkelectonActor * pSKActor6 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(0, 0, -300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor6->GetTypeNode()->PlayAnim(_T("Pos"), 1.0f, VSController::RT_WRAP);

	VSSkelectonActor * pSKActor7 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(300, 0, -300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor7->GetTypeNode()->PlayAnim(_T("AddtiveAnim"), 1.0f, VSController::RT_WRAP);

	VSStaticActor* pStaticActor = (VSStaticActor*)VSWorld::ms_pWorld->CreateActor(_T("NewOceanPlane.STMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3(100.0f, 100.f, 100.0f), m_pTestMap);
	pStaticActor->GetTypeNode()->CastShadow(false);

	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(VSVector3::ms_Zero, VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(0.0f, 0.0f, 0.5f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

	VSMatrix3X3 Dir;
	Dir.CreateFromDirection(VSVector3(-1.0f,-1.0f,0.0f));
	VSSpotLightActor * pSpotLightActor = (VSSpotLightActor*)VSWorld::ms_pWorld->CreateActor<VSSpotLightActor>(VSVector3(700.0f, 700.0f, 0.0f), Dir, VSVector3::ms_One, m_pTestMap);
	pSpotLightActor->GetTypeNode()->m_Diffuse = VSColorRGBA(5.0f, 5.0f, 5.0f, 1.0f);
	pSpotLightActor->GetTypeNode()->m_Specular = VSColorRGBA(5.0f, 5.0f, 5.0f, 1.0f);
	pSpotLightActor->GetTypeNode()->SetShadowType(VSSpotLight::ST_PROJECT);
	pSpotLightActor->GetTypeNode()->m_ProjectShadowFallOff = 3.0f;
	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));

	VSWorld::ms_pWorld->AttachWindowViewFamilyToCamera(m_pCameraActor, VSWindowViewFamily::VT_WINDOW_NORMAL,
		_T("WindowUse"), SceneMap, VSForwordHighEffectSceneRenderMethod::ms_Type.GetName().GetBuffer(), -1);


	return true;
}
bool VSDemoWindowsApplication::OnTerminal()
{
	VSWindowApplication::OnTerminal();

	return true;
}
bool VSDemoWindowsApplication::PostUpdate()
{
	VSWindowApplication::PostUpdate();
	if (m_bLMouseDowning)
	{
		m_p1stCameraController->m_RotXDelta = ((m_iDetMouseY)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_p1stCameraController->m_RotYDelta = ((m_iDetMouseX)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_iDetMouseX = 0;
		m_iDetMouseY = 0;
	}
	else
	{
		m_p1stCameraController->m_RotXDelta = 0.0f;
		m_p1stCameraController->m_RotYDelta = 0.0f;
	}


	return true;
}
bool VSDemoWindowsApplication::PreUpdate()
{
	VSWindowApplication::PreUpdate();
	return true;
}
bool VSDemoWindowsApplication::OnDraw()
{
	VSWindowApplication::OnDraw();
	
	return true;
}
void VSDemoWindowsApplication::OnMove(int xPos, int yPos)
{

}
void VSDemoWindowsApplication::OnReSize(int iWidth, int iHeight)
{

}
void VSDemoWindowsApplication::OnKeyDown(unsigned int uiKey)
{
	VSApplication::OnKeyDown(uiKey);
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 1.0f;
	}

}
void VSDemoWindowsApplication::OnKeyUp(unsigned int uiKey)
{
	VSApplication::OnKeyUp(uiKey);
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
	
}
void VSDemoWindowsApplication::OnLButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonDown(xPos, yPos);
	m_bLMouseDowning = true;
	m_iLastMouseX = xPos;
	m_iLastMouseY = yPos;
	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
}
void VSDemoWindowsApplication::OnLButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonUp(xPos, yPos);
	m_bLMouseDowning = false;
}
void VSDemoWindowsApplication::OnRButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnRButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMouseMove(int xPos, int yPos)
{
	VSWindowApplication::OnMouseMove(xPos, yPos);
	if (m_bLMouseDowning)
	{
		m_iDetMouseX = xPos - m_iLastMouseX;
		m_iDetMouseY = yPos - m_iLastMouseY;
		m_iLastMouseX = xPos;
		m_iLastMouseY = yPos;
	}

}
void VSDemoWindowsApplication::OnMouseWheel(int xPos, int yPos, int zDet)
{
	VSWindowApplication::OnMouseWheel(xPos, yPos, zDet);
}
