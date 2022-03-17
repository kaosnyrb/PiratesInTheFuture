//--------------------------------------------------------
// The Control Core, handles user input
// Bryn Stringer 
//--------------------------------------------------------
#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include "World_Core.h"

class CControl_Core
{
	public:
		CControl_Core(HINSTANCE inst,HWND hWnd, LONG p_startx, LONG p_starty, CWorld_Core* p_World_Core);
		~CControl_Core();
		void Control_Main();
	private:
		bool GetKeyState(BYTE Key);
		void GetMouseMoveDelta(LONG &DeltaX,LONG &DeltaY);
		bool GetMouseButtonState(BYTE Button);
		LPDIRECTINPUT8 m_diObject;
		LPDIRECTINPUTDEVICE8 m_diKeyboardDevice;
		LPDIRECTINPUTDEVICE8 m_diMouseDevice;
		CWorld_Core* m_World_core;
		int tick;
		LONG ScreenX;
		LONG ScreenY;		
};	