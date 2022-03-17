//---------------------------------------------------------------------
// GS2 project
// Bryn Stringer F6063476
//---------------------------------------------------------------------
#define VC_EXTRALEAN
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Vis_Core.h"
#include "World_Core.h"
#include "Control_Core.h"
#include "Media_Core.h"

//Declare our message handler
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//Globals
HDC mainhdc;//Handle to main window (device context)
LPDIRECT3D9 d3dObject = NULL;
IDirect3DDevice9* d3dDevice = NULL;
CVis_Core* graphics;
CWorld_Core* World;
CControl_Core* Controls;
CMedia_Core* Media;
bool playingintro;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Create the window
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc= (WNDPROC)WndProc;
	wcex.cbClsExtra= 0;
	wcex.cbWndExtra= 0;
	wcex.hInstance= hInstance;
	wcex.hIcon= 0;
	wcex.hCursor= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName= 0;
	wcex.lpszClassName= "MyWindowClass";
	wcex.hIconSm= 0;
	// Now we can go ahead and register our new window class
	RegisterClassEx(&wcex);
	//Create a copy of this window
	HWND hWnd = CreateWindow("MyWindowClass", "Pirates in the future!!!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	//Display and update the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//set the device context to our window
	mainhdc = GetDC(hWnd);

	//Create direct3d Object
	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);

	//Create Device
	D3DPRESENT_PARAMETERS presParams;
	ZeroMemory(&presParams,sizeof(presParams));
	presParams.Windowed=TRUE;
	presParams.SwapEffect=D3DSWAPEFFECT_DISCARD;
	presParams.BackBufferFormat=D3DFMT_UNKNOWN;
	presParams.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	presParams.EnableAutoDepthStencil = TRUE;
	presParams.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&presParams,&d3dDevice);
	d3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&presParams,&d3dDevice);
	//Create the Vis core
	graphics = new CVis_Core(&d3dObject,d3dDevice);
	//Create the media core
	Media = new CMedia_Core(hWnd);
	playingintro = true;
	//Create the world core
	World = new CWorld_Core(graphics);
	//Create the Control core
	//We chuck the controls the current cursor pos and he keeps track of it
	POINT temppoint;
	GetCursorPos(&temppoint);
	Controls = new CControl_Core(hInstance,hWnd,temppoint.x,temppoint.y,World);
	//Enter the message loop	
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message!=WM_QUIT )
	{
		if ( playingintro )//This stops the need for calling the isfinished more than once.
		{	
			if ( Media->isFinished() )
			{
				playingintro = false;
			}
		}
		else
		{
			Controls->Control_Main();//Ask the controls to handle input and pass actions to the world
			World->World_Main();//World handles it's own ticks, just run as fast as you can.
		}
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			graphics->Render();
		}
	}
	//End the program
	d3dDevice->Release();
	d3dObject->Release();
	delete graphics;
	delete World;
	delete Controls;
	delete Media;
	return (int)msg.wParam;
}

//Win proc, handle the stuff
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		if ( d3dDevice )
		{
			if ( !playingintro )
			{
				graphics->Render();
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
			case VK_SPACE:
				if ( playingintro )
				{	
					Media->ForceFinish();
					playingintro = false;
				}
				break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
