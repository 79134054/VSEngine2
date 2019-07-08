#ifndef  VSAPPLICATION_H
#define  VSAPPLICATION_H
#include "VSSystem.h"
#include "VSMain.h"
#include "VSMath.h"
#include "VSMemManager.h"
#include "VSCommand.h"
#include "VSRenderer.h"
#include "VSGraphicInclude.h"
namespace VSEngine2
{
//-------------------------------------------------------
#define DLCARE_APPLICATION(classname)\
	public:\
		static bool RegisterMainFactory();\
	private:\
		static bool InitialApplation();\
		static bool TerminalApplation();\
		static bool ms_bRegisterMainFactory;
//-------------------------------------------------------
#define IMPLEMENT_APPLICATION(classname)\
	static bool gs_bStreamRegistered_classname= classname::RegisterMainFactory ();\
 	bool classname::ms_bRegisterMainFactory = false;\
	bool classname::RegisterMainFactory()\
	{\
		if (!ms_bRegisterMainFactory)\
		{\
			VSMain::AddInitialFuntion(classname::InitialApplation);\
			VSMain::AddTerminalFuntion(classname::TerminalApplation);\
			ms_bRegisterMainFactory = true;\
		}\
		return ms_bRegisterMainFactory;\
	}\
	bool classname::InitialApplation()\
	{\
		classname::ms_pApplication = VS_NEW classname();\
		if(!classname::ms_pApplication)\
			return false;\
		return true;\
	}\
	bool classname::TerminalApplation()\
	{\
		if (classname::ms_pApplication)\
		{\
			VS_DELETE(classname::ms_pApplication);\
		}\
		return true;\
	}
//-------------------------------------------------------
	class VSApplication : public VSMemObject
	{
	public:
		VSApplication();
		virtual ~VSApplication();
		static VSApplication * ms_pApplication;

	protected:

		VSCommand * m_pCommand;
		unsigned int m_uiRenderAPIType;
		unsigned int m_uiScreenWidth;
		unsigned int m_uiScreenHeight;	
		unsigned int m_uiAnisotropy;
		unsigned int m_uiMultisample;
		unsigned int m_iUpdateThreadNum;
		
		virtual bool CreateAppWindow();

		virtual bool CreateRenderer();
		virtual bool ReleaseRenderer();

		virtual bool CreateInput();
		virtual bool ReleaseInput();

		virtual bool CreateTimer();
		virtual bool ReleaseTimer();

		virtual bool CreateSceneManager();
		virtual bool ReleaseSceneManager();

		virtual bool CreateWorld();
		virtual bool ReleaseWorld();
		
		virtual bool CreateMonitor();
		virtual bool ReleaseMonitor();

		virtual bool CreateASYNLoader();
		virtual bool ReleaseASYNLoader();

		virtual bool CreateRenderThread();
		virtual bool ReleaseRenderThread();

		virtual bool CreateUpdateThread();
		virtual bool ReleaseUpdateThread();
		
		
		virtual bool Run();
		
		virtual  bool CreateEngine();
		virtual  bool ReleaseEngine();
	public:
		virtual bool Main(HINSTANCE hInst,LPSTR lpCmdLine, int nCmdShow);
		virtual bool Main(int argc, char* argv[]);
		virtual bool PreInitial();
		virtual bool OnInitial();	
		virtual bool OnTerminal();
		virtual bool PostUpdate();
		virtual bool PreUpdate();
		virtual bool OnDraw();
		virtual void OnMove(int xPos,int yPos);
		virtual void OnReSize(int iWidth,int iHeight);
		virtual void OnKeyDown(unsigned int uiKey);
		virtual void OnKeyUp(unsigned int uiKey);
		virtual void OnLButtonDown(int xPos,int yPos);
		virtual void OnLButtonUp(int xPos,int yPos);
		virtual void OnRButtonDown(int xPos,int yPos);
		virtual void OnRButtonUp(int xPos,int yPos);
		virtual void OnMButtonDown(int xPos,int yPos);
		virtual void OnMButtonUp(int xPos,int yPos);
		virtual void OnMouseMove(int xPos,int yPos);
		virtual void OnMouseWheel(int xPos,int yPos,int zDet);

		virtual void ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow,bool IsMaxScreen = false);
		static void InputMsgProc(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z);
		bool m_bIsActive;
		bool m_bIsRunning;
	};
#ifdef WINDOWS
	class VSWindowApplication : public VSApplication
	{
	public:
		VSWindowApplication();
		virtual ~VSWindowApplication();
	protected:
		virtual bool CreateRenderer();
		virtual bool CreateInput();


		virtual bool CreateMonitor();
		virtual bool ReleaseMonitor();

		virtual bool CreateDx9();
		virtual bool CreateDx10();
		virtual bool CreateDx11();
		virtual bool CreateOPGL();
		virtual bool CreateAppWindow();

		static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		

		

		VSString m_Tile;
		VSString m_WindowClassName;
		unsigned int m_uiInputAPIType;
		static DWORD ms_WinodwKeyToVS[];


		DWORD m_dwWindowedStyle;
		DWORD m_dwFullscreenStyle;
		bool		m_bIsWindowed;
		HWND	m_MainHwnd;
		VSArray<VSRenderer::ChildWindowInfo>	m_ArrayChildHwnd;
		HINSTANCE m_hInst;
	public:
		virtual bool Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow);
		virtual bool PreInitial();
		virtual void ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen = false);
		unsigned int CheckVirtualKeyDown(unsigned int VK);
		unsigned int CheckVirtualKeyUp(unsigned int VK);
	};
	class VSConsoleApplication : public VSApplication
	{
	public:
		VSConsoleApplication();
		virtual ~VSConsoleApplication();
	protected:
		virtual bool CreateRenderer();
		virtual bool CreateInput();

		virtual bool CreateAppWindow();

	public:
		virtual bool Main(int argc, char* argv[]);
		virtual bool PreInitial();
		virtual void ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen = false);

	};
#endif
}
#endif