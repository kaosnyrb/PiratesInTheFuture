//--------------------------------------------------------
// The Control Core, handles user input
// Bryn Stringer 
//--------------------------------------------------------
#include "Control_Core.h"

CControl_Core::CControl_Core(HINSTANCE inst, HWND hWnd, LONG p_startx, LONG p_starty, CWorld_Core* p_World_Core)
{	
	DirectInput8Create(inst, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&m_diObject, NULL);
	m_diObject->CreateDevice(GUID_SysKeyboard, &m_diKeyboardDevice, NULL);
	m_diObject->CreateDevice(GUID_SysMouse, &m_diMouseDevice, NULL);
	m_diKeyboardDevice->SetDataFormat( &c_dfDIKeyboard );
	m_diMouseDevice->SetDataFormat( &c_dfDIMouse2 );
	m_diKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_diMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//Finally acquire both
	m_diKeyboardDevice->Acquire();
	m_diMouseDevice->Acquire();
	//And remember where the mouse started, so we can keep track of it
	ScreenX = p_startx;
	ScreenY = p_starty;
	m_World_core = p_World_Core;
	tick = 0;
}

CControl_Core::~CControl_Core()
{
	if ( m_diKeyboardDevice )
	{
		m_diKeyboardDevice->Unacquire();
		m_diKeyboardDevice->Release();
	}
	if ( m_diMouseDevice )
	{
		m_diMouseDevice->Unacquire();
		m_diMouseDevice->Release();
	}
	if (m_diObject)
	{
		m_diObject->Release();
	}
}

void CControl_Core::Control_Main()
{
	if ( GetKeyState(DIK_W) || GetKeyState(DIK_UP))
	{
		m_World_core->Player_Move(0);
	}
	if ( GetKeyState(DIK_D) || GetKeyState(DIK_RIGHT))
	{
		m_World_core->Player_Move(1);
	}
	if ( GetKeyState(DIK_S) || GetKeyState(DIK_DOWN))
	{
		m_World_core->Player_Move(2);
	}
	if ( GetKeyState(DIK_A) || GetKeyState(DIK_LEFT))
	{
		m_World_core->Player_Move(3);
	}
	if ( GetKeyState(DIK_SPACE) || GetMouseButtonState(0))
	{
		m_World_core->Player_FireMissle();
	}
}

bool CControl_Core::GetKeyState(BYTE Key)
{
	BYTE  keys[256];
	ZeroMemory(keys, sizeof(keys) );
	HRESULT hr = m_diKeyboardDevice->GetDeviceState( sizeof(keys), keys );
	if ( FAILED(hr) )
	{
		hr = m_diKeyboardDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{     
			hr = m_diKeyboardDevice->Acquire();
		}
		// Could be we failed for some other reason
		if (FAILED(hr))
		{
			return false;
		}
		// Now read the state again
		m_diKeyboardDevice->GetDeviceState( sizeof(keys), keys );
	}
	if ( keys[Key] & 0x80 )
	{
		return true;
	}
	return false;
}

void CControl_Core::GetMouseMoveDelta(LONG &DeltaX,LONG &DeltaY)
{
	DIMOUSESTATE2 m_mouseState;
	ZeroMemory( &m_mouseState, sizeof(m_mouseState) );
	HRESULT hr = m_diMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &m_mouseState );
	if ( FAILED(hr) )
	{
		hr = m_diMouseDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{     
			hr = m_diMouseDevice->Acquire();
		}
		// Could be we failed for some other reason
		if (FAILED(hr))
		{
			return;
		}
		// Now read the state again
		m_diMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &m_mouseState );
	}
	DeltaX = m_mouseState.lX;
	DeltaY = m_mouseState.lY;
	ScreenX += m_mouseState.lX;
	ScreenY += m_mouseState.lY;
}

bool CControl_Core::GetMouseButtonState(BYTE Button)
{
	DIMOUSESTATE2 m_mouseState;
	ZeroMemory( &m_mouseState, sizeof(m_mouseState) );
	HRESULT hr = m_diMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &m_mouseState );
	if ( FAILED(hr) )
	{
		hr = m_diMouseDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{     
			hr = m_diMouseDevice->Acquire();
		}
		// Could be we failed for some other reason
		if (FAILED(hr))
		{
			return false;
		}
		// Now read the state again
		m_diMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &m_mouseState );
	}
	ScreenX += m_mouseState.lX;
	ScreenY += m_mouseState.lY;
	if ( Button < 8 )
	{
		if ( m_mouseState.rgbButtons[Button] & 0x80 )
		{
			return true;
		}
	}
	return false;
}