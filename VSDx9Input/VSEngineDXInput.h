#ifndef VSENGINEDXINPUT_H
#define VSENGINEDXINPUT_H
#include <dinput.h>
#include "VSEngineInput.h"
#ifdef VSDX9INPUT_EXPORTS
#define  VSDX9INPUT_API __declspec(dllexport) 
#else
#define  VSDX9INPUT_API __declspec(dllimport)
#endif
namespace VSEngine2
{
	class VSDX9INPUT_API VSEngineDXInput : public VSEngineInput
	{
	public:
		VSEngineDXInput(HINSTANCE hInstance,HWND hWnd,const MouseRange *pCage);
		virtual ~VSEngineDXInput();
		virtual unsigned int GetInputAPIType()const
		{
			return IAT_DX;
		}
	protected:
		enum
		{
			BUFFER_SIZE = 16
		};
		virtual bool IsKeyBoardPressed(unsigned int  uiBoardKey);
		virtual bool IsKeyBoardReleased(unsigned int  uiBoardKey);
		virtual void KeyBoardUpdate();


		virtual bool IsMousePressed(UINT nBtn);
		virtual	bool IsMouseReleased(UINT nBtn);
		virtual void MouseUpdate();

		HRESULT					m_hResult;
		LPDIRECTINPUT8			m_pDI;
		LPDIRECTINPUTDEVICE8	m_pMouseDevice;
		LPDIRECTINPUTDEVICE8	m_pKeyBoardDevice;
		static DWORD ms_BoardKey[BK_MAX];
		HWND				m_hWndMain;         //´°¿Ú¾ä±ú
		HINSTANCE			m_hInstance;        //ÊµÀý¾ä±ú
	};


}
#endif