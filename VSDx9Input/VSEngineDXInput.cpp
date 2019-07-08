#include "VSEngineDXInput.h"
using namespace VSEngine2;
DWORD VSEngineDXInput::ms_BoardKey[BK_MAX] = 
{
	DIK_ESCAPE			,          
	DIK_TAB				,
	DIK_SPACE           ,
	DIK_RETURN          ,
	DIK_BACK            ,
	DIK_CAPITAL         ,

	DIK_MINUS           ,
	DIK_EQUALS          ,
	DIK_LBRACKET        ,
	DIK_RBRACKET        ,
	DIK_SEMICOLON       ,
	DIK_APOSTROPHE      ,
	DIK_GRAVE           ,
	DIK_BACKSLASH       ,
	DIK_COMMA           ,
	DIK_PERIOD          ,
	DIK_SLASH           ,

	// CHARACTER KEYS
	DIK_A               ,
	DIK_S               ,
	DIK_D               ,
	DIK_F               ,
	DIK_G               ,
	DIK_H               ,
	DIK_J               ,
	DIK_K               ,
	DIK_L               ,
	DIK_Z               ,
	DIK_X               ,
	DIK_C               ,
	DIK_V              ,
	DIK_B               ,
	DIK_N               ,
	DIK_M               ,
	DIK_Q               ,
	DIK_W               ,
	DIK_E               ,
	DIK_R               ,
	DIK_T               ,
	DIK_Y               ,
	DIK_U               ,
	DIK_I               ,
	DIK_O               ,
	DIK_P               ,

	//数字键
	DIK_1               ,
	DIK_2               ,
	DIK_3               ,
	DIK_4               ,
	DIK_5               ,
	DIK_6               ,
	DIK_7               ,
	DIK_8               ,
	DIK_9               ,
	DIK_0               ,

	// FUNCTION 键
	DIK_F1              ,
	DIK_F2              ,
	DIK_F3              ,
	DIK_F4              ,
	DIK_F5              ,
	DIK_F6              ,
	DIK_F7              ,
	DIK_F8              ,
	DIK_F9              ,
	DIK_F10             ,
	DIK_F11             ,
	DIK_F12             ,

	// ON NUMPAD
	DIK_ADD				,
	DIK_SUBTRACT        ,
	DIK_DECIMAL			,
	DIK_NUMPADCOMMA     ,
	DIK_DIVIDE			,
	DIK_MULTIPLY		,
	DIK_NUMPADENTER     ,
	DIK_NUMLOCK         ,
	DIK_NUMPAD1             ,
	DIK_NUMPAD2             ,
	DIK_NUMPAD3             ,
	DIK_NUMPAD4             ,
	DIK_NUMPAD5             ,
	DIK_NUMPAD6             ,
	DIK_NUMPAD7             ,
	DIK_NUMPAD8             ,
	DIK_NUMPAD9             ,
	DIK_NUMPAD0             ,

	// CTRL, ALT, SHFT and WINDWS
	DIK_RSHIFT          ,
	DIK_LSHIFT          ,
	DIK_RCONTROL           ,
	DIK_LCONTROL           ,
	DIK_RALT            ,
	DIK_LALT            ,
	DIK_LWIN            ,
	DIK_RWIN            ,

	// ON ARROW KEYPAD
	DIK_UP              ,
	DIK_DOWN            ,
	DIK_LEFT            ,
	DIK_RIGHT           ,
	DIK_INSERT          ,
	DIK_DELETE          ,
	DIK_HOME            ,
	DIK_END             ,
	DIK_PGDN			,
	DIK_PGUP            ,

	DIK_PAUSE           ,    /* Pause */
	DIK_SCROLL              /* Scroll Lock */
};
VSEngineDXInput::VSEngineDXInput(HINSTANCE hInstance,HWND hWnd,const MouseRange *pCage)
				:VSEngineInput()
{
	m_hInstance = hInstance;
	m_hWndMain = hWnd;
	m_pDI = NULL;
	m_pMouseDevice = NULL;
	m_pKeyBoardDevice = NULL;
	
	m_hResult = DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&m_pDI,NULL);
	VSMAC_ASSERT(!FAILED(m_hResult));

	GUID guid;
	DWORD dwFlags;

	//创建键盘设备
	guid = GUID_SysKeyboard;
	dwFlags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

	m_hResult = m_pDI->CreateDevice(guid, &m_pKeyBoardDevice, NULL);
	VSMAC_ASSERT(!FAILED(m_hResult));

	m_hResult = m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);
	VSMAC_ASSERT(!FAILED(m_hResult));

	m_hResult = m_pKeyBoardDevice->SetCooperativeLevel(m_hWndMain, dwFlags);
	VSMAC_ASSERT(!FAILED(m_hResult));

	m_pKeyBoardDevice->Acquire();

	//创建鼠标设备
	guid = GUID_SysMouse;
	dwFlags = DISCL_FOREGROUND | DISCL_EXCLUSIVE;

	m_hResult = m_pDI->CreateDevice(guid, &m_pMouseDevice, NULL);
	VSMAC_ASSERT(!FAILED(m_hResult));

	m_hResult = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
	VSMAC_ASSERT(!FAILED(m_hResult));

	m_hResult = m_pMouseDevice->SetCooperativeLevel(m_hWndMain, dwFlags);
	VSMAC_ASSERT(!FAILED(m_hResult));
	
	HANDLE	m_hEvent = NULL;
	//创建通知事件
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	VSMAC_ASSERT(m_hEvent);

	m_hResult = m_pMouseDevice->SetEventNotification(m_hEvent);
	VSMAC_ASSERT(!FAILED(m_hResult));

	//为鼠标建立缓冲区
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = BUFFER_SIZE;

	//设置事件个数缓冲区大小
	m_hResult = m_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	VSMAC_ASSERT(!FAILED(m_hResult));
	m_pMouseDevice->Acquire();

	SetCage(pCage);
}
VSEngineDXInput::~VSEngineDXInput()
{
	if (m_pKeyBoardDevice) 
	{
		m_pKeyBoardDevice->Unacquire();
		m_pKeyBoardDevice->Release();
		m_pKeyBoardDevice = NULL;
	}
	
	if (m_pMouseDevice) 
	{
		m_pMouseDevice->Unacquire();
		m_pMouseDevice->Release();
		m_pMouseDevice = NULL;
	}

	if (m_pDI) 
	{
		m_pDI->Release();
		m_pDI = NULL;
	}
}
bool VSEngineDXInput::IsKeyBoardPressed(unsigned int  uiBoardKey)
{
	if(uiBoardKey < BK_MAX)
	{
		if (m_Keys[ms_BoardKey[uiBoardKey]] & 0x80)
			return true;
	}
	return false;
}
bool VSEngineDXInput::IsKeyBoardReleased(unsigned int  uiBoardKey)
{
	if(uiBoardKey < BK_MAX)
	{
		if ( (m_KeysOld[ms_BoardKey[uiBoardKey]]&0x80) && !(m_Keys[ms_BoardKey[uiBoardKey]]&0x80) )
			return true;
	}
	return false;
}
void VSEngineDXInput::KeyBoardUpdate()
{
	DWORD size = 0;
	VSMemcpy(m_KeysOld, m_Keys, sizeof(m_Keys));

	size = sizeof(char) * 256;

	m_hResult = m_pKeyBoardDevice->GetDeviceState(size, &m_Keys[0]);

	if (FAILED(m_hResult)) 
	{
		//设备丢失
		if ( (m_hResult == DIERR_NOTACQUIRED) || (m_hResult == DIERR_INPUTLOST) ) 
		{

			m_hResult = m_pKeyBoardDevice->Acquire();
			//如果设备一直丢失，直到得到设备
			while (m_hResult==DIERR_INPUTLOST)
				m_hResult = m_pKeyBoardDevice->Acquire();

			//如果有其他应用程序用，则下一Frame在取得
			if (m_hResult == DIERR_OTHERAPPHASPRIO) return;

			//如果重新得到设备
			if (SUCCEEDED(m_hResult))
			{		
				m_hResult = m_pKeyBoardDevice->GetDeviceState(size, &m_Keys[0]);
			}
			if (FAILED(m_hResult)) 
					VSMAC_ASSERT(0);
		}
		else
		{
			VSMAC_ASSERT(0);
		}
	}
}

bool VSEngineDXInput::IsMousePressed(UINT nBtn)
{
	if (nBtn < MK_MAX) return m_bPressed[nBtn]; 
	return false;
}
bool VSEngineDXInput::IsMouseReleased(UINT nBtn)
{
	if (nBtn < MK_MAX) return m_bReleased[nBtn]; 
	return false;
}
void VSEngineDXInput::MouseUpdate()
{
	DIDEVICEOBJECTDATA od[BUFFER_SIZE];
	DWORD        dwNumElem = BUFFER_SIZE;
	DWORD size = 0;
	bool bRightCage = (m_Cage.right != -1);
	bool bLeftCage = (m_Cage.left != -1);
	bool bBottomCage = (m_Cage.bottom != -1);
	bool bTopCage = (m_Cage.top!= -1);
	bool bFrontCage = (m_Cage.front != -1);
	bool bBackCage = (m_Cage.back != -1);

	m_Delta.x = m_Delta.y = m_Delta.z = 0 ;

	size = sizeof(DIDEVICEOBJECTDATA);

	m_hResult =m_pMouseDevice->GetDeviceData(size, (DIDEVICEOBJECTDATA*)od, &dwNumElem, 0);

	if (FAILED(m_hResult)) 
	{
		//设备丢失
		if ( (m_hResult == DIERR_NOTACQUIRED) || (m_hResult == DIERR_INPUTLOST) ) 
		{

			m_hResult = m_pMouseDevice->Acquire();
			//如果设备一直丢失，直到得到设备
			while (m_hResult == DIERR_INPUTLOST)
				m_hResult = m_pMouseDevice->Acquire();

			//如果有其他应用程序用，则下一Frame在取得
			if (m_hResult == DIERR_OTHERAPPHASPRIO) return;

			//如果重新得到设备
			if (SUCCEEDED(m_hResult))
			{
				
				m_hResult = m_pMouseDevice->GetDeviceData(size, (DIDEVICEOBJECTDATA*)od, &dwNumElem, 0);
	
			}
			if (FAILED(m_hResult)) 
				VSMAC_ASSERT(0);
		}
		else 
			VSMAC_ASSERT(0);
	}

	m_bReleased[0] = m_bReleased[1] = m_bReleased[2] = false;

	// 遍历所有鼠标事件鼠标
	for (DWORD i = 0 ; i < dwNumElem; i++) 
	{
		switch (od[i].dwOfs) 
		{
			// X移动
		case DIMOFS_X: 
			{
				m_lX += od[i].dwData;
				m_Delta.x = od[i].dwData;

				if (bLeftCage & (m_lX < m_Cage.left))
					m_lX = m_Cage.left;
				else if (bRightCage & (m_lX > m_Cage.right))
					m_lX = m_Cage.right;
			} break;

		case DIMOFS_Y: 
			{
				m_lY += od[i].dwData;
				m_Delta.y = od[i].dwData;

				if (bTopCage & (m_lY < m_Cage.top))
					m_lY = m_Cage.top;
				else if (bBottomCage & (m_lY > m_Cage.bottom))
					m_lY = m_Cage.bottom;
			} break;
		case DIMOFS_Z: 
			{
				m_lZ += od[i].dwData;
				m_Delta.z = od[i].dwData;

				if (bFrontCage & (m_lZ < m_Cage.front))
					m_lZ = m_Cage.front;
				else if (bBackCage & (m_lZ > m_Cage.back))
					m_lZ = m_Cage.back;
			} break;
			// 按键移动
		case DIMOFS_BUTTON0: 
			{
				if (od[i].dwData & 0x80) 
				{
					m_bPressed[0] = true;
				}
				else 
				{
					if (m_bPressed[0])
						m_bReleased[0] = true;

					m_bPressed[0] = false;
				}
			} break;

		case DIMOFS_BUTTON1: 
			{
				if (od[i].dwData & 0x80)
					m_bPressed[1] = true;
				else 
				{
					if (m_bPressed[1])
						m_bReleased[1] = true;

					m_bPressed[1] = false;
				}
			} break;

		case DIMOFS_BUTTON2: 
			{
				if (od[i].dwData & 0x80)
					m_bPressed[2] = true;
				else 
				{
					if (m_bPressed[2])
						m_bReleased[2] = true;

					m_bPressed[2] = false;
				}
			} break;
		}; // switch
	} // for
}