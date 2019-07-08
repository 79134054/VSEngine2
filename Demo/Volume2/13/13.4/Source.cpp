//多线程更新
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
	bool	m_bWireSwith;
	VSRasterizerStatePtr  m_pWireFrameStateEnable;
	

};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{
	m_bWireSwith = false;

}
VSDemoWindowsApplication::~VSDemoWindowsApplication()
{

}
bool VSDemoWindowsApplication::PreInitial()
{
	VSWindowApplication::PreInitial();
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	//m_uiInputAPIType = VSEngineInput::IAT_DX;
	VSResourceManager::ms_bRenderThread = false;
	m_bLMouseDowning = false;

	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
	m_iLastMouseX = 0;
	m_iLastMouseY = 0;
	m_pWireFrameStateEnable = NULL;
	VSEngineFlag::EnableCLODMesh = false;

	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();

	m_p1stCameraController = VSObject::GetInstance<VS1stCameraController>();
	m_pCameraActor = (VSCameraActor *)VSWorld::ms_pWorld->CreateActor<VSCameraActor>();
	m_pCameraActor->GetTypeNode()->AddController(m_p1stCameraController);
	VSVector3 CameraPos(0.0f, 1000.0f, -300.0f);
	VSVector3 CameraDir(0.0f, -1.0f, 1.0f);
	m_pCameraActor->GetTypeNode()->CreateFromLookDir(CameraPos, CameraDir);
	m_pCameraActor->GetTypeNode()->SetPerspectiveFov(AngleToRadian(90.0f), (m_uiScreenWidth * 1.0f) / (m_uiScreenHeight), 1.0f, 8000.0f);

	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

	VSString AnimName[] = { _T("Idle"), _T("Walk"), _T("Attack"), _T("RootMotion"),
		_T("Attack2"), _T("Pos"), _T("AddtiveAnim")};

	for (unsigned int i = 0; i < 10; i++)
	{
		for (unsigned int j = 0; j < 10; j++)
		{
			unsigned int s = VSRand() % 7;
			VSSkelectonActor * pSKActor = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(j * 400.0f - 1500.0f, 0, i * 400.0f + 1000.0f), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
			pSKActor->GetTypeNode()->PlayAnim(AnimName[s], 1.0f, VSController::RT_WRAP);
		}
		
	}


	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(VSVector3::ms_Zero, VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(1.0f, 0.0f, 0.0f, 1.0f);

	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));
	VSWorld::ms_pWorld->AttachWindowViewFamilyToCamera(m_pCameraActor, VSWindowViewFamily::VT_WINDOW_NORMAL,
		_T("WindowUse"), SceneMap, VSForwordEffectSceneRenderMethod::ms_Type.GetName().GetBuffer());
	VSRasterizerDesc WireLessDesc;
	WireLessDesc.m_bWireEnable = true;
	m_pWireFrameStateEnable = VSResourceManager::CreateRasterizerState(WireLessDesc);

	return true;
}
bool VSDemoWindowsApplication::OnTerminal()
{
	VSWindowApplication::OnTerminal();
	m_pWireFrameStateEnable = NULL;
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
	else if (uiKey == VSEngineInput::BK_W)
	{
		m_bWireSwith = !m_bWireSwith;

		VSViewFamily * pViewFamily = VSSceneManager::ms_pSceneManager->GetViewFamily(_T("WindowUse"));
		VSSceneRenderMethod * pRM = pViewFamily->m_pSceneRenderMethod;
		if (m_bWireSwith)
		{
			VSRenderState RenderState;
			RenderState.SetRasterizerState(m_pWireFrameStateEnable);

			pRM->SetUseState(RenderState, VSRenderState::IF_WIRE_ENABLE);
		}
		else
		{
			pRM->ClearUseState();
		}


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
