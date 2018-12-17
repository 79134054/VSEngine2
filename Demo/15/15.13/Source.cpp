//演示一个Strauss光照模型
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
	VSMaterialInstance * pMaterialInstance0;
	VSMaterialInstance * pMaterialInstance1;
	float Smoothness0;
	float Smoothness1;

	float Metalness0;
	float Metalness1;

	float Transparency0;
	float Transparency1;

	float K0;
	float K1;
};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{

	Smoothness0 = 0.3f;
	Smoothness1 = 0.3f;

	Metalness0 = 0.6f;
	Metalness1 = 0.6f;

	Transparency0 = 0.2f;
	Transparency1 = 0.2f;

	K0 = 16.0f;
	K1 = 16.0f;
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
	VSVector3 CameraPos(0.0f, 100.0f, -300.0f);
	VSVector3 CameraDir(0.0f, 0.0f, 1.0f);
	m_pCameraActor->GetTypeNode()->CreateFromLookDir(CameraPos, CameraDir);
	m_pCameraActor->GetTypeNode()->SetPerspectiveFov(AngleToRadian(90.0f), (m_uiScreenWidth * 1.0f) / (m_uiScreenHeight), 1.0f, 8000.0f);

	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

	VSSkelectonActor * pSKActor1 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterStrauss.SKMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pMaterialInstance0 = pSKActor1->GetTypeNode()->GetUseMaterialInstance(0);
	pMaterialInstance1 = pSKActor1->GetTypeNode()->GetUseMaterialInstance(1);

	VSStaticActor* pStaticActor = (VSStaticActor*)VSWorld::ms_pWorld->CreateActor(_T("NewOceanPlane.STMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3(100.0f, 100.f, 100.0f), m_pTestMap);
	pStaticActor->GetTypeNode()->CastShadow(false);

	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(VSVector3::ms_Zero, VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(0.0f, 0.0f, 0.5f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

	VSMatrix3X3 Dir;
	Dir.CreateEluer(0, AngleToRadian(45), AngleToRadian(45));
	VSDirectionLightActor * pDirectionLightActor = (VSDirectionLightActor *)VSWorld::ms_pWorld->CreateActor<VSDirectionLightActor>(VSVector3::ms_Zero, Dir, VSVector3::ms_One, m_pTestMap);

	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));

	VSWorld::ms_pWorld->AttachWindowViewFamilyToCamera(m_pCameraActor, VSWindowViewFamily::VT_WINDOW_NORMAL,
		_T("WindowUse"), SceneMap, VSForwordEffectSceneRenderMethod::ms_Type.GetName().GetBuffer(), -1);


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

	pMaterialInstance0->SetPShaderValue(_T("Smoothness"), &Smoothness0, sizeof(Smoothness0));
	pMaterialInstance1->SetPShaderValue(_T("Smoothness"), &Smoothness1, sizeof(Smoothness1));

	pMaterialInstance0->SetPShaderValue(_T("Metalness"), &Metalness0, sizeof(Metalness0));
	pMaterialInstance1->SetPShaderValue(_T("Metalness"), &Metalness1, sizeof(Metalness1));

	pMaterialInstance0->SetPShaderValue(_T("Transparency"), &Transparency0, sizeof(Transparency0));
	pMaterialInstance1->SetPShaderValue(_T("Transparency"), &Transparency1, sizeof(Transparency1));

	pMaterialInstance0->SetPShaderValue(_T("K"), &K0, sizeof(K0));
	pMaterialInstance1->SetPShaderValue(_T("K"), &K1, sizeof(K1));
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
	VSRenderer::ms_pRenderer->DrawText(500, 400, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key '+' And '-' to Change Smoothness0"));
	VSRenderer::ms_pRenderer->DrawText(500, 420, VSDWCOLORABGR(255, 255, 255, 255), _T("Smoothness0:%f"), Smoothness0);
	VSRenderer::ms_pRenderer->DrawText(500, 440, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key '[' And ']' to Change Smoothness1"));
	VSRenderer::ms_pRenderer->DrawText(500, 460, VSDWCOLORABGR(255, 255, 255, 255), _T("Smoothness1:%f"), Smoothness1);

	VSRenderer::ms_pRenderer->DrawText(500, 480, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key '1' And '2' to Change Metalness0"));
	VSRenderer::ms_pRenderer->DrawText(500, 500, VSDWCOLORABGR(255, 255, 255, 255), _T("Metalness0:%f"), Metalness0);
	VSRenderer::ms_pRenderer->DrawText(500, 520, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key 'F1' And 'F2' to Change Metalness1"));
	VSRenderer::ms_pRenderer->DrawText(500, 540, VSDWCOLORABGR(255, 255, 255, 255), _T("Metalness1:%f"), Metalness1);

	VSRenderer::ms_pRenderer->DrawText(500, 560, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key '3' And '4' to Change Transparency0"));
	VSRenderer::ms_pRenderer->DrawText(500, 580, VSDWCOLORABGR(255, 255, 255, 255), _T("Transparency0:%f"), Transparency0);
	VSRenderer::ms_pRenderer->DrawText(500, 600, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key 'F3' And 'F4' to Change Transparency1"));
	VSRenderer::ms_pRenderer->DrawText(500, 620, VSDWCOLORABGR(255, 255, 255, 255), _T("Transparency1:%f"), Transparency1);

	VSRenderer::ms_pRenderer->DrawText(500, 640, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key '5' And '6' to Change K0"));
	VSRenderer::ms_pRenderer->DrawText(500, 660, VSDWCOLORABGR(255, 255, 255, 255), _T("K0:%f"), K0);
	VSRenderer::ms_pRenderer->DrawText(500, 680, VSDWCOLORABGR(255, 255, 255, 255), _T("Use Key 'F5' And 'F6' to Change K1"));
	VSRenderer::ms_pRenderer->DrawText(500, 700, VSDWCOLORABGR(255, 255, 255, 255), _T("K1:%f"), K1);
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
	else if (uiKey == VSEngineInput::BK_MINUS)
	{
		Smoothness0 -= 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_EQUALS)
	{
		Smoothness0 += 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_LBRACKET)
	{
		Smoothness1 -= 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_RBRACKET)
	{
		Smoothness1 += 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_1)
	{
		Metalness0 -= 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_2)
	{
		Metalness0 += 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_F1)
	{
		Metalness1 -= 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_F2)
	{
		Metalness1 += 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_3)
	{
		Transparency0 -= 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_4)
	{
		Transparency0 += 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_F3)
	{
		Transparency1 -= 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_F4)
	{
		Transparency1 += 0.01f;
	}
	else if (uiKey == VSEngineInput::BK_5)
	{
		K0 -= 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_6)
	{
		K0 += 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_F7)
	{
		K1 -= 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_F8)
	{
		K1 += 0.1f;
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
