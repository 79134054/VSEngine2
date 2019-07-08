#include "VSApplication.h"
#include "VSDx9Renderer.h"
#include "VSEngineDXInput.h"
#include "VSTimer.h"
#include "VSSceneManager.h"
#include "VSMonitor.h"
#include "VSASYNLoader.h"
#include "VSRenderThread.h"
#include "VSUpdateThread.h"
#include "VSWorld.h"
#include "VSProfiler.h"
#include "VSNullRenderer.h"
#include "VSDx11Renderer.h"
#ifdef WINDOWS
#include <WINDOWSX.H>
#endif
using namespace VSEngine2;
#ifdef WINDOWS
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow) 
{
	VSInitSystem();
	VSInitMath();
	VSMain::Initialize();
	if (VSApplication::ms_pApplication)
	{
		if (!VSApplication::ms_pApplication->Main(hInst,lpCmdLine,nCmdShow))
		{
			VSMAC_ASSERT(0);
		}
	}
	
	VSMain::Terminate();

	return 1;
}
int  main(int argc, char* argv[])
{
	VSInitSystem();
	VSInitMath();
	VSMain::Initialize();
	if (VSApplication::ms_pApplication)
	{
		if (!VSApplication::ms_pApplication->Main(argc, argv))
		{
			VSMAC_ASSERT(0);
		}
	}

	VSMain::Terminate();

	return 1;
}
#endif
DECLEAR_ONLYTIME_PROFILENODE(FPS,)
DECLEAR_TIME_PROFILENODE(ApplicationUpdate,)
DECLEAR_TIME_PROFILENODE(Update, ApplicationUpdate)
VSApplication *VSApplication::ms_pApplication = NULL;
VSApplication::VSApplication()
{

	m_iUpdateThreadNum = 0;
}
VSApplication::~VSApplication()
{

}

bool VSApplication::CreateRenderer()
{
	return true;
}
bool VSApplication::CreateTimer()
{
	if (!VSTimer::ms_pTimer)
	{
		VSTimer * pTimer = VS_NEW VSTimer();
		if (!pTimer)
		{
			return false;
		}
	}
	VSRandInit(VSTimer::ms_pTimer->GetRandSeed());
	return true;
}
bool VSApplication::CreateSceneManager()
{
	if (!VSSceneManager::ms_pSceneManager)
	{
		VSSceneManager * pSceneManager = VS_NEW VSSceneManager();
		if (!pSceneManager)
		{
			return false;
		}
	}
	return true;
}
bool VSApplication::CreateWorld()
{
	if (!VSWorld::ms_pWorld)
	{
		VSWorld * pWorld = VS_NEW VSWorld();
		if (!pWorld)
		{
			return false;
		}
	}
	return true;
}
bool VSApplication::CreateMonitor()
{
	return true;
}
bool VSApplication::CreateASYNLoader()
{
	if(!VSASYNLoadManager::ms_pASYNLoadManager)
	{
		VSASYNLoadManager * pASYNLoadManager = VS_NEW VSASYNLoadManager();
		if (!pASYNLoadManager)
		{
			return false;
		}
	}
	return true;
}
bool VSApplication::CreateUpdateThread()
{
	if (!VSResourceManager::ms_bUpdateThread || !m_iUpdateThreadNum)
	{
		return true;
	}
	if(!VSUpdateThreadSys::ms_pUpdateThreadSys)
	{
		VSUpdateThreadSys * pUpdateThreadSys = VS_NEW VSUpdateThreadSys(m_iUpdateThreadNum);
		if (!pUpdateThreadSys)
		{
			return false;
		}
	}
	return true;
}
bool VSApplication::CreateRenderThread()
{
	if (!VSResourceManager::ms_bRenderThread)
	{
		return true;
	}
	if(!VSRenderThreadSys::ms_pRenderThreadSys)
	{
		VSRenderThreadSys * pRenderTreadSys = VS_NEW VSRenderThreadSys();
		if (!pRenderTreadSys)
		{
			return false;
		}
	}
	return true;
}
bool VSApplication::CreateInput()
{
	
	return true;
}
bool VSApplication::ReleaseRenderer()
{
	VSMAC_DELETE(VSRenderer::ms_pRenderer);
	return true;
}
bool VSApplication::ReleaseInput()
{
	VSMAC_DELETE(VSEngineInput::ms_pInput);
	return true;
}
bool VSApplication::ReleaseTimer()
{
	VSMAC_DELETE(VSTimer::ms_pTimer);
	return true;
}
bool VSApplication::ReleaseSceneManager()
{
	VSMAC_DELETE(VSSceneManager::ms_pSceneManager);
	return true;
}
bool VSApplication::ReleaseWorld()
{
	VSMAC_DELETE(VSWorld::ms_pWorld);
	return true;
}
bool VSApplication::ReleaseASYNLoader()
{
	VSMAC_DELETE(VSASYNLoadManager::ms_pASYNLoadManager);
	return true;
}
bool VSApplication::ReleaseRenderThread()
{
	if (!VSResourceManager::ms_bRenderThread)
	{
		return true;
	}
	VSMAC_DELETE(VSRenderThreadSys::ms_pRenderThreadSys);
	return true;
}
bool VSApplication::ReleaseUpdateThread()
{
	if (!VSResourceManager::ms_bUpdateThread)
	{
		return true;
	}
	VSMAC_DELETE(VSUpdateThreadSys::ms_pUpdateThreadSys);
	return true;
}
bool VSApplication::ReleaseMonitor()
{
	return true;
}
bool VSApplication::Run()
{
	double fTime = 0.0f;
	double fFPS = 0.0f;
	double DetTime = 0.0f;
	ADD_TIME_PROFILE(ApplicationUpdate)
	GetStackMemManager().Clear();
	if (VSTimer::ms_pTimer)
	{
		VSTimer::ms_pTimer->UpdateFPS();
		fTime = VSTimer::ms_pTimer->GetGamePlayTime();
		fFPS = VSTimer::ms_pTimer->GetFPS();
		ADD_ONLYTIME_PROFILE(FPS,fFPS)
	}
	if (VSRenderer::ms_pRenderer)
	{

		if(VSRenderer::ms_pRenderer->CooperativeLevel())
		{
			if (VSRenderThreadSys::ms_pRenderThreadSys && VSResourceManager::ms_bRenderThread)
			{
				VSRenderThreadSys::ms_pRenderThreadSys->Begin();
			}
			{
				ADD_TIME_PROFILE(Update)
				if (VSEngineInput::ms_pInput)
				{
					VSEngineInput::ms_pInput->Update();
				}

#ifdef WINDOWS
				if (VSResourceMonitor::ms_pResourceMonitor)
				{
					VSResourceMonitor::ms_pResourceMonitor->Update(fTime);
				}
#endif
				if (VSASYNLoadManager::ms_pASYNLoadManager)
				{
					VSASYNLoadManager::ms_pASYNLoadManager->Update(fTime);
				}

				
				PreUpdate();

				if (VSSceneManager::ms_pSceneManager)
				{
					VSSceneManager::ms_pSceneManager->Update(fTime);
				}	

				if (VSWorld::ms_pWorld)
				{
					VSWorld::ms_pWorld->Update(fTime);
				}
				PostUpdate();
				VSRenderer::ms_pRenderer->BeginRendering();

				if (VSSceneManager::ms_pSceneManager)
				{
					VSSceneManager::ms_pSceneManager->Draw(fTime);
				}

				if (!OnDraw())
				{
					return false;
				}
				VSProfiler::Draw();
				VSRenderer::ms_pRenderer->EndRendering();
		
			}
			if (VSRenderThreadSys::ms_pRenderThreadSys && VSResourceManager::ms_bRenderThread)
			{
				VSRenderThreadSys::ms_pRenderThreadSys->ExChange();
			}		
		}
		else
		{
			if (VSRenderThreadSys::ms_pRenderThreadSys)
			{
				VSRenderThreadSys::ms_pRenderThreadSys->Clear();
			}
		}
		VSResourceManager::ClearDynamicBufferGeometry();
		VSResourceManager::DelayUpdate(fTime);
		VSResourceManager::GC();
		
	}
	return true;
}
bool VSApplication::CreateEngine()
{
	if (!CreateMonitor())
	{
		return false;
	}
	if (!CreateASYNLoader())
	{
		return false;
	}
	if (!CreateRenderThread())
	{
		return false;
	}
	if (!CreateUpdateThread())
	{
		return false;
	}
	if (!CreateRenderer())
	{
		return false;
	}
	if (!CreateInput())
	{
		return false;
	}
	if (!CreateSceneManager())
	{
		return false;
	}
	if (!CreateWorld())
	{
		return false;
	}
	return true;
}
bool VSApplication::ReleaseEngine()
{
	if (!ReleaseWorld())
	{
		return false;
	}
	if (!ReleaseSceneManager())
	{
		return false;
	}
	if (!ReleaseInput())
		return false;
	if (!ReleaseRenderer())
	{
		return false;
	}
	if (!ReleaseUpdateThread())
	{
		return false;
	}
	if (!ReleaseRenderThread())
	{
		return false;
	}
	if (!ReleaseASYNLoader())
	{
		return false;
	}
	if (!ReleaseMonitor())
	{
		return false;
	}
	return true;
}
bool VSApplication::Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow)
{
	return false;
}
bool VSApplication::Main(int argc, char* argv[])
{
	return false;
}
bool VSApplication::OnTerminal()
{
	return true;
}
bool VSApplication::PostUpdate()
{
	return true;
}
bool VSApplication::PreUpdate()
{
	return true;
}
bool VSApplication::OnDraw()
{
	return true;
}
bool VSApplication::OnInitial()
{
	VSProfiler::ClearAll();
	return true;
}
bool VSApplication::PreInitial()
{

	m_uiScreenWidth = 1024;
	m_uiScreenHeight = 768;
	m_uiAnisotropy = 1;
	m_uiMultisample = VSRenderer::MS_NONE;

	int iUpdateMaxThreadNum = VSGetCpuNum();
	iUpdateMaxThreadNum--;// Main Thread all time run
	iUpdateMaxThreadNum--;// AsynLoad Thread ,Monitor Thread

	if (iUpdateMaxThreadNum < 0)// Render Thread all time run
	{
		VSResourceManager::ms_bRenderThread = false;
	}
	else
	{
		VSResourceManager::ms_bRenderThread = true;
		iUpdateMaxThreadNum--;
	}

	iUpdateMaxThreadNum--; // for physics , audio or else third part lib


	if (iUpdateMaxThreadNum <= 0)
	{

		VSResourceManager::ms_bUpdateThread = false;
	}
	else
	{
		m_iUpdateThreadNum = iUpdateMaxThreadNum;
		VSResourceManager::ms_bUpdateThread = true;

	}
	return true;
}
bool VSApplication::CreateAppWindow()
{	
	return true;
}
void VSApplication::ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow,bool IsMaxScreen)
{
	if (VSRenderThreadSys::ms_pRenderThreadSys)
	{
		VSRenderThreadSys::ms_pRenderThreadSys->Clear();
	}
	
	if(VSRenderer::ms_pRenderer->ChangeScreenSize(uiWidth,uiHeight,bWindow))
	{
		m_uiScreenWidth = uiWidth;
		m_uiScreenHeight = uiHeight;
	}
}
void VSApplication::OnMove(int xPos,int yPos)
{

}
void VSApplication::OnReSize(int iWidth,int iHeight)
{

}
void VSApplication::OnKeyDown(unsigned int uiKey)
{

}
void VSApplication::OnKeyUp(unsigned int uiKey)
{

}
void VSApplication::OnLButtonDown(int xPos,int yPos)
{

}
void VSApplication::OnLButtonUp(int xPos,int yPos)
{

}
void VSApplication::OnRButtonDown(int xPos,int yPos)
{

}
void VSApplication::OnRButtonUp(int xPos,int yPos)
{

}
void VSApplication::OnMButtonDown(int xPos,int yPos)
{

}
void VSApplication::OnMButtonUp(int xPos,int yPos)
{

}
void VSApplication::OnMouseMove(int xPos,int yPos)
{

}
void VSApplication::OnMouseWheel(int xPos,int yPos,int zDet)
{

}
#ifdef WINDOWS
DWORD VSWindowApplication::ms_WinodwKeyToVS[] =
{
	VSEngineInput::BK_MAX,		//					0x00
	VSEngineInput::BK_MAX,		//VK_LBUTTON        0x01
	VSEngineInput::BK_MAX,		//VK_RBUTTON        0x02
	VSEngineInput::BK_MAX,		//VK_CANCEL         0x03
	VSEngineInput::BK_MAX,		//VK_MBUTTON        0x04
	VSEngineInput::BK_MAX,		//VK_XBUTTON1       0x05
	VSEngineInput::BK_MAX,		//VK_XBUTTON2       0x06
	VSEngineInput::BK_MAX,		//				    0x07
	VSEngineInput::BK_BACK,		//VK_BACK           0x08
	VSEngineInput::BK_TAB,		//VK_TAB            0x09
	VSEngineInput::BK_MAX,		//					0x0A
	VSEngineInput::BK_MAX,		//					0x0B
	VSEngineInput::BK_MAX,		//VK_CLEAR          0x0C
	VSEngineInput::BK_RETURN,	//VK_RETURN         0x0D
	VSEngineInput::BK_MAX,		//VK_CLEAR          0x0E
	VSEngineInput::BK_MAX,		//VK_CLEAR          0x0F
	VSEngineInput::BK_MAX,		//VK_SHIFT          0x10
	VSEngineInput::BK_MAX,		//VK_CONTROL        0x11
	VSEngineInput::BK_MAX,		//VK_MENU           0x12
	VSEngineInput::BK_PAUSE,	//VK_PAUSE          0x13
	VSEngineInput::BK_CAPITAL,	//VK_CAPITAL        0x14
	VSEngineInput::BK_MAX,		//					0x15
	VSEngineInput::BK_MAX,		//					0x16
	VSEngineInput::BK_MAX,		//					0x17
	VSEngineInput::BK_MAX,		//					0x18
	VSEngineInput::BK_MAX,		//					0x19
	VSEngineInput::BK_MAX,		//					0x1A
	VSEngineInput::BK_ESCAPE,	//VK_ESCAPE         0x1B
	VSEngineInput::BK_MAX,		//					0x1C
	VSEngineInput::BK_MAX,		//					0x1D
	VSEngineInput::BK_MAX,		//					0x1E
	VSEngineInput::BK_MAX,		//					0x1F
	VSEngineInput::BK_SPACE,	//VK_SPACE          0x20
	VSEngineInput::BK_MAX,		//VK_PRIOR          0x21
	VSEngineInput::BK_MAX,		//VK_NEXT           0x22
	VSEngineInput::BK_END,		//VK_END			0x23
	VSEngineInput::BK_HOME,		//VK_HOME           0x24
	VSEngineInput::BK_LEFT,		//VK_LEFT           0x25
	VSEngineInput::BK_UP,		//VK_UP             0x26
	VSEngineInput::BK_RIGHT,	//VK_RIGHT          0x27
	VSEngineInput::BK_DOWN,		//VK_DOWN           0x28
	VSEngineInput::BK_MAX,		//VK_SELECT         0x29
	VSEngineInput::BK_MAX,		//					0x2A
	VSEngineInput::BK_MAX,		//					0x2B
	VSEngineInput::BK_MAX,		//					0x2C
	VSEngineInput::BK_INSERT,	//VK_INSERT         0x2D
	VSEngineInput::BK_DELETE,	//VK_DELETE         0x2E
	VSEngineInput::BK_MAX,		//					0x2F
	VSEngineInput::BK_0,		//VK_0				0x30
	VSEngineInput::BK_1,		//VK_1				0x31
	VSEngineInput::BK_2,		//VK_2				0x32
	VSEngineInput::BK_3,		//VK_3				0x33
	VSEngineInput::BK_4,		//VK_4				0x34
	VSEngineInput::BK_5,		//VK_5				0x35
	VSEngineInput::BK_6,		//VK_6				0x36
	VSEngineInput::BK_7,		//VK_7				0x37
	VSEngineInput::BK_8,		//VK_8				0x38
	VSEngineInput::BK_9,		//VK_9				0x39
	VSEngineInput::BK_MAX,		//					0x4A
	VSEngineInput::BK_MAX,		//					0x4B
	VSEngineInput::BK_MAX,		//					0x4C
	VSEngineInput::BK_MAX,		//					0x4D
	VSEngineInput::BK_MAX,		//					0x4E
	VSEngineInput::BK_MAX,		//					0x4F
	VSEngineInput::BK_MAX,		//					0x40
	VSEngineInput::BK_A,		//VK_A				0x41
	VSEngineInput::BK_B,		//VK_B				0x42
	VSEngineInput::BK_C,		//VK_C				0x43
	VSEngineInput::BK_D,		//VK_D				0x44
	VSEngineInput::BK_E,		//VK_E				0x45
	VSEngineInput::BK_F,		//VK_F				0x46
	VSEngineInput::BK_G,		//VK_G				0x47
	VSEngineInput::BK_H,		//VK_H				0x48
	VSEngineInput::BK_I,		//VK_I				0x49
	VSEngineInput::BK_J,		//VK_J				0x4A
	VSEngineInput::BK_K,		//VK_K				0x4B
	VSEngineInput::BK_L,		//VK_L				0x4C
	VSEngineInput::BK_M,		//VK_M				0x4D
	VSEngineInput::BK_N,		//VK_N				0x4E
	VSEngineInput::BK_O,		//VK_O				0x4F
	VSEngineInput::BK_P,		//VK_P				0x50
	VSEngineInput::BK_Q,		//VK_Q				0x51
	VSEngineInput::BK_R,		//VK_R				0x52
	VSEngineInput::BK_S,		//VK_S				0x53
	VSEngineInput::BK_T,		//VK_T				0x54
	VSEngineInput::BK_U,		//VK_U				0x55
	VSEngineInput::BK_V,		//VK_V				0x56
	VSEngineInput::BK_W,		//VK_W				0x57
	VSEngineInput::BK_X,		//VK_X				0x58
	VSEngineInput::BK_Y,		//VK_Y				0x59
	VSEngineInput::BK_Z,		//VK_Z				0x5A
	VSEngineInput::BK_MAX,		//					0x5B
	VSEngineInput::BK_MAX,		//					0x5C
	VSEngineInput::BK_MAX,		//					0x5D
	VSEngineInput::BK_MAX,		//					0x5E
	VSEngineInput::BK_MAX,		//					0x5F
	VSEngineInput::BK_NP0,		//VK_NUMPAD0        0x60
	VSEngineInput::BK_NP1,		//VK_NUMPAD1        0x61
	VSEngineInput::BK_NP2,		//VK_NUMPAD2        0x62
	VSEngineInput::BK_NP3,		//VK_NUMPAD3        0x63
	VSEngineInput::BK_NP4,		//VK_NUMPAD4        0x64
	VSEngineInput::BK_NP5,		//VK_NUMPAD5        0x65
	VSEngineInput::BK_NP6,		//VK_NUMPAD6        0x66
	VSEngineInput::BK_NP7,		//VK_NUMPAD7        0x67
	VSEngineInput::BK_NP8,		//VK_NUMPAD8        0x68
	VSEngineInput::BK_NP9,		//VK_NUMPAD9        0x69
	VSEngineInput::BK_NPMULTIPLY,//VK_MULTIPLY       0x6A
	VSEngineInput::BK_NPPLUS,	//VK_ADD            0x6B
	VSEngineInput::BK_MAX,		//VK_SEPARATOR      0x6C
	VSEngineInput::BK_NPMINUS,	//VK_SUBTRACT       0x6D
	VSEngineInput::BK_NPDECIMAL,//VK_DECIMAL        0x6E
	VSEngineInput::BK_NPDIVIDE,	//VK_DIVIDE         0x6F
	VSEngineInput::BK_F1,		//VK_F1             0x70
	VSEngineInput::BK_F2,		//VK_F2             0x71
	VSEngineInput::BK_F3,		//VK_F3             0x72
	VSEngineInput::BK_F4,		//VK_F4             0x73
	VSEngineInput::BK_F5,		//VK_F5             0x74
	VSEngineInput::BK_F6,		//VK_F6             0x75
	VSEngineInput::BK_F7,		//VK_F7             0x76
	VSEngineInput::BK_F8,		//VK_F8             0x77
	VSEngineInput::BK_F9,		//VK_F9             0x78
	VSEngineInput::BK_F10,		//VK_F10            0x79
	VSEngineInput::BK_F11,		//VK_F11            0x7A
	VSEngineInput::BK_F12,		//VK_F12            0x7B
	VSEngineInput::BK_MAX,		//					0x7C
	VSEngineInput::BK_MAX,		//					0x7D
	VSEngineInput::BK_MAX,		//					0x7E
	VSEngineInput::BK_MAX,		//					0x7F
	VSEngineInput::BK_MAX,		//					0x80
	VSEngineInput::BK_MAX,		//					0x81
	VSEngineInput::BK_MAX,		//					0x82
	VSEngineInput::BK_MAX,		//					0x83
	VSEngineInput::BK_MAX,		//					0x84
	VSEngineInput::BK_MAX,		//					0x85
	VSEngineInput::BK_MAX,		//					0x86
	VSEngineInput::BK_MAX,		//					0x87
	VSEngineInput::BK_MAX,		//					0x88
	VSEngineInput::BK_MAX,		//					0x89
	VSEngineInput::BK_MAX,		//					0x8A
	VSEngineInput::BK_MAX,		//					0x8B
	VSEngineInput::BK_MAX,		//					0x8C
	VSEngineInput::BK_MAX,		//					0x8D
	VSEngineInput::BK_MAX,		//					0x8E
	VSEngineInput::BK_MAX,		//					0x8F
	VSEngineInput::BK_NUMLOCK,	//VK_NUMLOCK        0x90
	VSEngineInput::BK_SCROLL,	//VK_SCROLL         0x91
	VSEngineInput::BK_MAX,		//					0x92
	VSEngineInput::BK_MAX,		//					0x93
	VSEngineInput::BK_MAX,		//					0x94
	VSEngineInput::BK_MAX,		//					0x95
	VSEngineInput::BK_MAX,		//					0x96
	VSEngineInput::BK_MAX,		//					0x97
	VSEngineInput::BK_MAX,		//					0x98
	VSEngineInput::BK_MAX,		//					0x99
	VSEngineInput::BK_MAX,		//					0x9A
	VSEngineInput::BK_MAX,		//					0x9B
	VSEngineInput::BK_MAX,		//					0x9C
	VSEngineInput::BK_MAX,		//					0x9D
	VSEngineInput::BK_MAX,		//					0x9E
	VSEngineInput::BK_MAX,		//					0x9F
	VSEngineInput::BK_LSHIFT,		//VK_LSHIFT         0xA0
	VSEngineInput::BK_RSHIFT,		//VK_RSHIFT         0xA1
	VSEngineInput::BK_LCTRL,		//VK_LCONTROL       0xA2
	VSEngineInput::BK_RCTRL,		//VK_RCONTROL       0xA3
	VSEngineInput::BK_MAX,			//VK_LMENU			0xA4
	VSEngineInput::BK_MAX,			//VK_RMENU			0xA5
	VSEngineInput::BK_MAX,			//VK_BROWSER_BACK	0xA6
	VSEngineInput::BK_MAX,			//VK_BROWSER_FORWARD			0xA7
	VSEngineInput::BK_MAX,			//VK_BROWSER_REFRESH			0xA8
	VSEngineInput::BK_MAX,			//VK_BROWSER_STOP			0xA9
	VSEngineInput::BK_MAX,			//VK_BROWSER_SEARCH			0xAA
	VSEngineInput::BK_MAX,			//VK_BROWSER_FAVORITES			0xAB
	VSEngineInput::BK_MAX,			//VK_BROWSER_HOME			0xAC
	VSEngineInput::BK_MAX,			//VK_VOLUME_MUTE			0xAD
	VSEngineInput::BK_MAX,			//VK_VOLUME_DOWN			0xAE
	VSEngineInput::BK_MAX,			//VK_VOLUME_UP				0xAF
	VSEngineInput::BK_MAX,			// VK_MEDIA_NEXT_TRACK    0xB0
	VSEngineInput::BK_MAX,			// VK_MEDIA_PREV_TRACK    0xB1
	VSEngineInput::BK_MAX,			// VK_MEDIA_STOP          0xB2
	VSEngineInput::BK_MAX,			// VK_MEDIA_PLAY_PAUSE    0xB3
	VSEngineInput::BK_MAX,			// VK_LAUNCH_MAIL         0xB4
	VSEngineInput::BK_MAX,			// VK_LAUNCH_MEDIA_SELECT 0xB5
	VSEngineInput::BK_MAX,			// VK_LAUNCH_APP1         0xB6
	VSEngineInput::BK_MAX,			// VK_LAUNCH_APP2         0xB7
	VSEngineInput::BK_MAX,			//						0xB8
	VSEngineInput::BK_MAX,			//				         0xB9
	VSEngineInput::BK_SEMICOLON,			// VK_OEM_1          0xBA   // ';:' for US
	VSEngineInput::BK_EQUALS,			// VK_OEM_PLUS       0xBB   // '+' any country
	VSEngineInput::BK_COMMA,			// VK_OEM_COMMA      0xBC   // ',' any country
	VSEngineInput::BK_MINUS,			// VK_OEM_MINUS      0xBD   // '-' any country
	VSEngineInput::BK_PERIOD,			// VK_OEM_PERIOD     0xBE   // '.' any country
	VSEngineInput::BK_MAX,			// VK_OEM_2          0xBF   // '/?' for US
	VSEngineInput::BK_MAX,			// VK_OEM_3          0xC0   // '`~' for US
	VSEngineInput::BK_MAX,			//           0xC1
	VSEngineInput::BK_MAX,			//           0xC2
	VSEngineInput::BK_MAX,			//           0xC3
	VSEngineInput::BK_MAX,			//	         0xC4
	VSEngineInput::BK_MAX,			//           0xC5
	VSEngineInput::BK_MAX,			//           0xC6
	VSEngineInput::BK_MAX,			//           0xC7
	VSEngineInput::BK_MAX,			//           0xC8
	VSEngineInput::BK_MAX,			//           0xC9
	VSEngineInput::BK_MAX,			//           0xCA
	VSEngineInput::BK_MAX,			//           0xCB
	VSEngineInput::BK_MAX,			//           0xCC
	VSEngineInput::BK_MAX,			//           0xCD
	VSEngineInput::BK_MAX,			//           0xCE
	VSEngineInput::BK_MAX,			//           0xCF
	VSEngineInput::BK_MAX,			//           0xD0
	VSEngineInput::BK_MAX,			//           0xD1
	VSEngineInput::BK_MAX,			//           0xD2
	VSEngineInput::BK_MAX,			//           0xD3
	VSEngineInput::BK_MAX,			//           0xD4
	VSEngineInput::BK_MAX,			//           0xD5
	VSEngineInput::BK_MAX,			//           0xD6
	VSEngineInput::BK_MAX,			//           0xD7
	VSEngineInput::BK_MAX,			//           0xD8
	VSEngineInput::BK_MAX,			//           0xD9
	VSEngineInput::BK_MAX,			//           0xDA
	VSEngineInput::BK_LBRACKET,		//           0xDB  //  '[{' for US
	VSEngineInput::BK_MAX,			//           0xDC  //  '\|' for US
	VSEngineInput::BK_RBRACKET,		//           0xDD  //  ']}' for US
	VSEngineInput::BK_MAX,			//           0xDE  //  ''"' for US
	VSEngineInput::BK_MAX			//           0xDF


};
VSWindowApplication::VSWindowApplication()
{
	m_hInst = NULL;
	m_MainHwnd = NULL;

}
VSWindowApplication::~VSWindowApplication()
{

}
bool VSWindowApplication::CreateDx9()
{
	if (VSRenderer::ms_pRenderer)
	{
		if (VSRenderer::ms_pRenderer->GetRendererType() == VSRenderer::RAT_DIRECTX9)
		{
			return true;
		}
		else
		{
			VSMAC_DELETE(VSRenderer::ms_pRenderer);
			VSDX9Renderer * pRenderer =
				VS_NEW VSDX9Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
				m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
			if (VSRenderer::ms_pRenderer)
			{
				return true;
			}
		}
	}
	else
	{
		VSDX9Renderer * pRenderer =
			VS_NEW VSDX9Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
			m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
		if (VSRenderer::ms_pRenderer)
		{
			return true;
		}
	}

	return true;

}
bool VSWindowApplication::CreateDx10()
{
	return false;
}
bool VSWindowApplication::CreateDx11()
{
	if (VSRenderer::ms_pRenderer)
	{
		if (VSRenderer::ms_pRenderer->GetRendererType() == VSRenderer::RAT_DIRECTX11)
		{
			return true;
		}
		else
		{
			VSMAC_DELETE(VSRenderer::ms_pRenderer);
			VSDX11Renderer * pRenderer =
				VS_NEW VSDX11Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
				m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
			if (VSRenderer::ms_pRenderer)
			{
				return true;
			}
		}
	}
	else
	{
		VSDX11Renderer * pRenderer =
			VS_NEW VSDX11Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
			m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
		if (VSRenderer::ms_pRenderer)
		{
			return true;
		}
	}

	return false;
}
bool VSWindowApplication::CreateOPGL()
{
	return false;
}
bool VSWindowApplication::CreateRenderer()
{
	VSApplication::CreateRenderer();
	if (m_uiRenderAPIType == VSRenderer::RAT_DIRECTX9)
	{
		return CreateDx9();
	}
	else if (m_uiRenderAPIType == VSRenderer::RAT_OPENGL)
	{
		return CreateOPGL();
	}
	else if (m_uiRenderAPIType == VSRenderer::RAT_DIRECTX10)
	{
		return CreateDx10();
	}
	else if (m_uiRenderAPIType == VSRenderer::RAT_DIRECTX11)
	{
		return CreateDx11();
	}
	else
	{
		return false;
	}
}
bool VSWindowApplication::CreateInput()
{
	if (VSEngineInput::ms_pInput)
	{
		if (VSEngineInput::ms_pInput->GetInputAPIType() == m_uiInputAPIType)
		{
			return true;
		}
		else
		{
			if (m_uiInputAPIType == VSEngineInput::IAT_DX)
			{
				VSEngineInput * pInput = VS_NEW VSEngineDXInput(m_hInst, m_MainHwnd, NULL);
				if (!pInput)
				{
					return false;
				}
				pInput->SetMsgProc(VSApplication::InputMsgProc);
			}
		}

	}
	else
	{
		if (m_uiInputAPIType == VSEngineInput::IAT_DX)
		{
			VSEngineInput * pInput = VS_NEW VSEngineDXInput(m_hInst, m_MainHwnd, NULL);
			if (!pInput)
			{
				return false;
			}
			pInput->SetMsgProc(VSApplication::InputMsgProc);
		}
	}

	return true;
}
bool VSWindowApplication::Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow)
{
	m_pCommand = VS_NEW VSCommand(lpCmdLine);
	m_hInst = hInst;
	m_bIsRunning = true;
	m_bIsActive = false;
	if (!CreateTimer())
	{
		return false;
	}

	if (!PreInitial())
	{
		return false;
	}

	if (!CreateAppWindow())
	{
		return false;
	}

	if (!CreateEngine())
	{
		return false;
	}

	if (!OnInitial())
	{
		return false;
	}
	MSG			msg;
	bool bError = false;
	while (m_bIsRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (m_bIsActive)
		{
			if (!Run())
			{
				m_bIsRunning = false;
				bError = true;
			}

		}
		else
		{
			if (VSRenderThreadSys::ms_pRenderThreadSys)
			{
				VSRenderThreadSys::ms_pRenderThreadSys->Clear();
			}
		}
	}
	if (!OnTerminal())
	{
		bError = true;
	}
	if (!ReleaseEngine())
	{
		bError = true;
	}
	if (!ReleaseTimer())
	{
		bError = true;
	}
	VSMAC_DELETE(m_pCommand);
	UnregisterClass(m_WindowClassName.GetBuffer(), m_hInst);
	return !bError;
}
bool VSWindowApplication::PreInitial()
{
	VSApplication::PreInitial();
	m_uiRenderAPIType = VSRenderer::RAT_DIRECTX11;
	m_uiScreenWidth = 1024;
	m_uiScreenHeight = 768;
	m_bIsWindowed = true;
	m_uiAnisotropy = 1;
	m_uiMultisample = VSRenderer::MS_NONE;
	m_Tile = _T("VSEngine2.0 by azhe --- Demo");
	m_WindowClassName = _T("FrameWorktest");
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	m_dwWindowedStyle = WS_OVERLAPPEDWINDOW & (~WS_OVERLAPPED) | WS_VISIBLE;
	m_dwFullscreenStyle = WS_POPUP | WS_SYSMENU | WS_VISIBLE;
	
	return true;
}
bool VSWindowApplication::CreateAppWindow()
{
	WNDCLASSEX	wndclass;
	HWND		   hWnd = NULL;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wndclass.lpfnWndProc = VSWindowApplication::MsgProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_WindowClassName.GetBuffer();
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (RegisterClassEx(&wndclass) == 0)
		return 0;

	// create window
	if (!(hWnd = CreateWindowEx(NULL, m_WindowClassName.GetBuffer(),
		m_Tile.GetBuffer(),
		m_dwWindowedStyle,
		0,
		0,
		m_uiScreenWidth, m_uiScreenHeight, NULL, NULL, m_hInst, NULL)))
		return 0;

	m_MainHwnd = hWnd;




	if (m_bIsWindowed)
	{
		RECT window_rect = { 0, 0, m_uiScreenWidth, m_uiScreenHeight };
		AdjustWindowRect(&window_rect, m_dwWindowedStyle, GetMenu(m_MainHwnd) != NULL);
		int NewWidth = window_rect.right - window_rect.left;
		int NewHeight = window_rect.bottom - window_rect.top;
		//移动窗口
		MoveWindow(m_MainHwnd,
			0, // x position
			0, // y position
			NewWidth, // width
			NewHeight, // height
			TRUE);
	}



	return true;
}
void VSWindowApplication::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen)
{
	VSApplication::ChangeScreenSize(uiWidth, uiHeight, bWindow, IsMaxScreen);
	if (bWindow)
	{
		if (IsMaxScreen)
		{
			RECT window_rect = { 0, 0, m_uiScreenWidth, m_uiScreenHeight };
			AdjustWindowRect(&window_rect, m_dwFullscreenStyle, GetMenu(m_MainHwnd) != NULL);
			SetWindowLong(m_MainHwnd, GWL_STYLE, m_dwFullscreenStyle);
			SetWindowPos(m_MainHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			//移动窗口
			MoveWindow(m_MainHwnd,
				0, // x position
				0, // y position
				GetSystemMetrics(SM_CXSCREEN), // width
				GetSystemMetrics(SM_CYSCREEN), // height
				TRUE);
		}
		else
		{
			RECT window_rect = { 0, 0, m_uiScreenWidth, m_uiScreenHeight };
			AdjustWindowRect(&window_rect, m_dwWindowedStyle, GetMenu(m_MainHwnd) != NULL);
			SetWindowLong(m_MainHwnd, GWL_STYLE, m_dwWindowedStyle);
			unsigned int NewWidth = window_rect.right - window_rect.left;
			unsigned int NewHeight = window_rect.bottom - window_rect.top;
			//移动窗口
			MoveWindow(m_MainHwnd,
				0, // x position
				0, // y position
				NewWidth, // width
				NewHeight, // height
				TRUE);
		}
	}


}
void VSApplication::InputMsgProc(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z)
{
	if (uiInputType == VSEngineInput::IT_KEYBOARD)
	{
		if (uiEvent == VSEngineInput::IE_DOWN)
		{
			VSApplication::ms_pApplication->OnKeyDown(uiKey);
		}
		else if (uiEvent == VSEngineInput::IE_UP)
		{
			VSApplication::ms_pApplication->OnKeyUp(uiKey);
		}

	}
	else if (uiInputType == VSEngineInput::IT_MOUSE)
	{
		if (uiEvent == VSEngineInput::IE_DOWN)
		{
			if (uiKey == VSEngineInput::MK_RIGHT)
			{
				VSApplication::ms_pApplication->OnRButtonDown(x, y);
			}
			else if (uiKey == VSEngineInput::MK_LEFT)
			{
				VSApplication::ms_pApplication->OnLButtonDown(x, y);
			}
			else if (uiKey == VSEngineInput::MK_MIDDLE)
			{
				VSApplication::ms_pApplication->OnMButtonDown(x, y);
			}
		}
		else if (uiEvent == VSEngineInput::IE_UP)
		{
			if (uiKey == VSEngineInput::MK_RIGHT)
			{
				VSApplication::ms_pApplication->OnRButtonUp(x, y);
			}
			else if (uiKey == VSEngineInput::MK_LEFT)
			{
				VSApplication::ms_pApplication->OnLButtonUp(x, y);
			}
			else if (uiKey == VSEngineInput::MK_MIDDLE)
			{
				VSApplication::ms_pApplication->OnMButtonUp(x, y);
			}
		}
		else if (uiEvent == VSEngineInput::IE_MOUSE_MOVE)
		{
			VSApplication::ms_pApplication->OnMouseMove(x, y);
		}
		else if (uiEvent == VSEngineInput::IE_WHEEL_MOVE)
		{
			VSApplication::ms_pApplication->OnMouseWheel(x, y, z);
		}
	}
	if (VSWorld::ms_pWorld)
	{
		VSWorld::ms_pWorld->ProcessInput(uiInputType, uiEvent, uiKey, x, y, z);
	}
}
unsigned int VSWindowApplication::CheckVirtualKeyDown(unsigned int VK)
{
	if (GetKeyState(VK_LSHIFT) < 0)
	{
		return VK_LSHIFT;
	}
	else if (GetKeyState(VK_RSHIFT) < 0)
	{
		return VK_RSHIFT;
	}
	else if (GetKeyState(VK_LCONTROL) < 0)
	{
		return VK_LCONTROL;
	}
	else if (GetKeyState(VK_RCONTROL) < 0)
	{
		return VK_RCONTROL;
	}
	else
	{
		return 0;
	}

}
unsigned int VSWindowApplication::CheckVirtualKeyUp(unsigned int VK)
{
	if (GetKeyState(VK_LSHIFT) > 0)
	{
		return VK_LSHIFT;
	}
	else if (GetKeyState(VK_RSHIFT) > 0)
	{
		return VK_RSHIFT;
	}
	else if (GetKeyState(VK_LCONTROL) > 0)
	{
		return VK_LCONTROL;
	}
	else if (GetKeyState(VK_RCONTROL) > 0)
	{
		return VK_RCONTROL;
	}
	else
	{
		return 0;
	}
}
LRESULT WINAPI VSWindowApplication::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!VSWindowApplication::ms_pApplication)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	switch (msg)
	{
		case WM_KILLFOCUS:
		{
			VSWindowApplication::ms_pApplication->m_bIsActive = false;
			break;
		}
		case WM_SETFOCUS:
		{
			VSWindowApplication::ms_pApplication->m_bIsActive = true;
			break;
		}

		case WM_ACTIVATE:
		{
			VSWindowApplication::ms_pApplication->m_bIsActive = (bool)(wParam);
			break;
		}
		case WM_MOVE:
		{
			int xPos = (int)(LOWORD(lParam));
			int yPos = (int)(HIWORD(lParam));
			VSApplication::ms_pApplication->OnMove(xPos, yPos);
			break;
		}
		case WM_SIZE:
		{
			int width = (int)(LOWORD(lParam));
			int height = (int)(HIWORD(lParam));
			VSApplication::ms_pApplication->OnReSize(width, height);
			break;
		}
		case WM_KEYDOWN:
		{
			if (!VSEngineInput::ms_pInput)
			{
				unsigned int virtKey = (unsigned int)wParam;
				if (virtKey < sizeof(ms_WinodwKeyToVS) / sizeof(DWORD))
				{ 
					VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_KEYBOARD, VSEngineInput::IE_DOWN, ms_WinodwKeyToVS[virtKey],0,0,0);
				}

			}

			switch (wParam)
			{
				case VK_ESCAPE:
				{
					VSApplication::ms_pApplication->m_bIsRunning = false;
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0;
				}

			}
			break;
		}
		case WM_KEYUP:
		{
			if (!VSEngineInput::ms_pInput)
			{
				unsigned int virtKey = (unsigned int)wParam;
				if (virtKey < sizeof(ms_WinodwKeyToVS) / sizeof(DWORD))
				{
					VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_KEYBOARD, VSEngineInput::IE_UP, ms_WinodwKeyToVS[virtKey], 0, 0, 0);
				}

			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_DOWN, VSEngineInput::MK_LEFT, xPos, yPos, 0);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_UP, VSEngineInput::MK_LEFT, xPos, yPos, 0);
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_DOWN, VSEngineInput::MK_MIDDLE, xPos, yPos, 0);
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_UP, VSEngineInput::MK_MIDDLE, xPos, yPos, 0);
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_DOWN, VSEngineInput::MK_RIGHT, xPos, yPos, 0);
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_UP, VSEngineInput::MK_RIGHT, xPos, yPos, 0);
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_MOUSE_MOVE, VSEngineInput::MK_MAX, xPos, yPos, 0);
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			if (!VSEngineInput::ms_pInput)
			{
				int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				VSApplication::ms_pApplication->InputMsgProc(VSEngineInput::IT_MOUSE, VSEngineInput::IE_WHEEL_MOVE, VSEngineInput::MK_MAX, xPos, yPos, delta);
			}
			break;
		}
		case WM_DESTROY:
		{
			VSApplication::ms_pApplication->m_bIsRunning = false;
			PostQuitMessage(0);
			return 1;
		} break;

		default: break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
bool VSWindowApplication::CreateMonitor()
{
	if (!VSResourceMonitor::ms_pResourceMonitor)
	{
		VSResourceMonitor * pResourceMonitor = VS_NEW VSResourceMonitor();
		if (!pResourceMonitor)
		{
			return false;
		}
		pResourceMonitor->Open();
	}
	return true;
}
bool VSWindowApplication::ReleaseMonitor()
{

	VSResourceMonitor::ms_pResourceMonitor->Close();
	VSMAC_DELETE(VSResourceMonitor::ms_pResourceMonitor);
	return true;
}



VSConsoleApplication::VSConsoleApplication()
{
}
VSConsoleApplication::~VSConsoleApplication()
{
}
bool VSConsoleApplication::CreateRenderer()
{
	VSApplication::CreateRenderer();
	if (!VSRenderer::ms_pRenderer)
	{
		VSNullRenderer * pRenderer = VS_NEW VSNullRenderer();
		if (VSRenderer::ms_pRenderer)
		{
			return true;
		}
	}

	return true;
}
bool VSConsoleApplication::CreateInput()
{
	return true;
}
bool VSConsoleApplication::Main(int argc, char* argv[])
{
	m_pCommand = VS_NEW VSCommand(argc, argv);
	m_bIsRunning = true;
	m_bIsActive = false;
	if (!CreateTimer())
	{
		return false;
	}

	if (!PreInitial())
	{
		return false;
	}

	if (!CreateAppWindow())
	{
		return false;
	}

	if (!CreateEngine())
	{
		return false;
	}

	if (!OnInitial())
	{
		return false;
	}
	bool bError = false;
	while (m_bIsRunning)
	{
		if (m_bIsActive)
		{
			if (!Run())
			{
				m_bIsRunning = false;
				bError = true;
			}

		}
	}
	if (!OnTerminal())
	{
		bError = true;
	}
	if (!ReleaseEngine())
	{
		bError = true;
	}
	if (!ReleaseTimer())
	{
		bError = true;
	}
	VSMAC_DELETE(m_pCommand);
	return !bError;
}
bool VSConsoleApplication::PreInitial()
{
	VSApplication::PreInitial();

	return true;
}
bool VSConsoleApplication::CreateAppWindow()
{
	
	return true;
}
void VSConsoleApplication::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen)
{
	VSApplication::ChangeScreenSize(uiWidth, uiHeight, bWindow, IsMaxScreen);
}

#endif