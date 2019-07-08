#ifndef VSENGINEINPUT_H
#define VSENGINEINPUT_H
#include "VSSystem.h"
#ifdef VSINPUT_EXPORTS
#define  VSINPUT_API __declspec(dllexport) 
#else
#define  VSINPUT_API __declspec(dllimport)
#endif
namespace VSEngine2
{
	typedef void (* InputMsgProc)(unsigned int uiInputType,unsigned int uiEvent,unsigned int uiKey,int x, int y, int z);
	class VSINPUT_API VSEngineInput
	{
	public:

		enum //Input Type 
		{
			IT_KEYBOARD,
			IT_MOUSE,
			IT_JOYSTICK,
			IT_MAX
			
		};
		enum // Input Event
		{
			IE_DOWN,
			IE_UP,
			IE_MOUSE_MOVE,
			IE_WHEEL_MOVE,
			IE_MAX
		};
		enum	//Board Key
		{
			//º¸≈Ãº¸…®√Ë¬Î
			BK_ESCAPE			,	//0         
			BK_TAB				,	//1
			BK_SPACE           ,	//2
			BK_RETURN          ,	//3
			BK_BACK            ,	//4
			BK_CAPITAL         ,	//5

			BK_MINUS           ,	//6
			BK_EQUALS          ,	//7
			BK_LBRACKET        ,	//8
			BK_RBRACKET        ,	//9
			BK_SEMICOLON       ,	//10
			BK_APOSTROPHE      ,	//11
			BK_GRAVE           ,	//12
			BK_BACKSLASH       ,	//13
			BK_COMMA           ,	//14	
			BK_PERIOD          ,	//15
			BK_SLASH           ,	//16

			// CHARACTER KEYS
			BK_A               ,	//17
			BK_S               ,	//18
			BK_D               ,	//19
			BK_F               ,	//20
			BK_G               ,	//21
			BK_H               ,	//22
			BK_J               ,	//23
			BK_K               ,	//24
			BK_L               ,	//25
			BK_Z               ,	//26
			BK_X               ,	//27
			BK_C               ,	//28
			BK_V              ,		//29
			BK_B               ,	//30
			BK_N               ,	//31
			BK_M               ,	//32
			BK_Q               ,	//33
			BK_W               ,	//34
			BK_E               ,	//35
			BK_R               ,	//36
			BK_T               ,	//37
			BK_Y               ,	//38
			BK_U               ,	//39
			BK_I               ,	//40
			BK_O               ,	//41
			BK_P               ,	//42

			// ˝◊÷º¸
			BK_1               ,	//43
			BK_2               ,	//44
			BK_3               ,	//45
			BK_4               ,	//46
			BK_5               ,	//47
			BK_6               ,	//48
			BK_7               ,	//49
			BK_8               ,	//50
			BK_9               ,	//51
			BK_0               ,	//52

			// FUNCTION º¸
			BK_F1              ,	//53
			BK_F2              ,	//54
			BK_F3              ,	//55
			BK_F4              ,	//56
			BK_F5              ,	//57
			BK_F6              ,	//58
			BK_F7              ,	//59
			BK_F8              ,	//60
			BK_F9              ,	//61
			BK_F10             ,	//62
			BK_F11             ,	//63
			BK_F12             ,	//64

			// ON NUMPAD
			BK_NPPLUS         ,		//65
			BK_NPMINUS         ,	//66
			BK_NPDECIMAL       ,	//67
			BK_NPCOMMA         ,	//68
			BK_NPDIVIDE        ,	//69
			BK_NPMULTIPLY      ,	//70
			BK_NPENTER         ,	//71
			BK_NUMLOCK         ,	//72
			BK_NP1             ,	//73
			BK_NP2             ,	//74
			BK_NP3             ,	//75
			BK_NP4             ,	//76
			BK_NP5             ,	//77
			BK_NP6             ,	//78
			BK_NP7             ,	//79
			BK_NP8             ,	//80
			BK_NP9             ,	//81
			BK_NP0             ,	//82

			// CTRL, ALT, SHFT and WINDWS
			BK_RSHIFT          ,	//83
			BK_LSHIFT          ,	//84
			BK_RCTRL           ,	//85
			BK_LCTRL           ,	//86
			BK_RALT            ,	//87
			BK_LALT            ,	//88
			BK_LWIN            ,	//89
			BK_RWIN            ,	//90

			// ON ARROW KEYPAD
			BK_UP              ,	//91
			BK_DOWN            ,	//92
			BK_LEFT            ,	//93
			BK_RIGHT           ,	//94
			BK_INSERT          ,	//95
			BK_DELETE          ,	//96
			BK_HOME            ,	//97
			BK_END             ,	//98
			BK_PGDOWN          ,	//99
			BK_PGUP            ,	//100

			BK_PAUSE           ,    //101 /* Pause */
			BK_SCROLL           ,   //102 /* Scroll Lock */

			BK_MAX

		};
		enum //Mouse Key
		{
			MK_RIGHT,
			MK_LEFT,
			MK_MIDDLE,
			MK_MAX
		};
		enum	//Input API TYPE
		{
			IAT_WINDOWS,
			IAT_DX,
			IAT_MAX
		};
		struct MouseRange
		{
			int    left;
			int    top;
			int    right;
			int    bottom;
			int	   front;
			int	   back;
		};
		struct MousePoint
		{
			int  x;
			int  y;
			int	 z;
		};
		VSEngineInput();
		virtual ~VSEngineInput() = 0;
		virtual unsigned int GetInputAPIType()const = 0;

		void Update();

		unsigned int GetMouseX(){return m_lX;}
		unsigned int GetMouseY(){return m_lY;}
		void	SetPosition(int x , int y, int z){m_lX = x;m_lY = y; m_lZ = z;}
		MousePoint   GetMouseDelta(){ return m_Delta; }
		void	SetCage(const MouseRange * pCage) { if(pCage) m_Cage = *pCage; }

		bool    IsPressed(unsigned int InputType, UINT nBtn);
		bool    IsReleased(unsigned int InputType, UINT nBtn);

		static	VSEngineInput * ms_pInput;

		void	SetMsgProc(InputMsgProc pMsgProc);
	protected:
		virtual bool IsKeyBoardPressed(unsigned int  uiBoardKey) = 0;
		virtual bool IsKeyBoardReleased(unsigned int  uiBoardKey) = 0;
		virtual void KeyBoardUpdate() = 0;
		

		virtual bool IsMousePressed(UINT nBtn) = 0;
		virtual	bool IsMouseReleased(UINT nBtn) = 0;
		virtual void MouseUpdate() = 0;
		
		// Û±Í
		
		MouseRange      m_Cage;							// Û±Í∑∂Œß
		bool			m_bPressed[3];					//¥Ê¥¢ Û±Í «∑Ò∞¥œ¬
		bool			m_bReleased[3];					//≈–∂œ Û±Í «∑Ò Õ∑≈
		MousePoint      m_Delta;							// Û±Í∆´“∆¡ø
		int                m_lX;
		int                m_lY;
		int					m_lZ;

		//º¸≈Ã
		TCHAR  m_Keys[256];					//¥Ê¥¢º¸≈Ãº¸¬Î «∑Ò∞¥œ¬
		TCHAR  m_KeysOld[256];				//…œ“ª¥Œº¸≈Ãº¸¬Î£¨”√¿¥≈–∂œº¸≈Ã «∑Ò Õ∑≈

		InputMsgProc	m_pMsgProc;
	};


};
#endif