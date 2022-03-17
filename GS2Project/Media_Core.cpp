//--------------------------------------------------------
// The Media Core, handles video/music
// Bryn Stringer 
//--------------------------------------------------------

#include "Media_Core.h"

CMedia_Core::CMedia_Core(HWND hWnd)
{
	m_hWndAccess = hWnd;
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_graphBuilder);
	//hr = m_graphBuilder->QueryInterface (IID_IMediaEvent, (void **)&m_mediaEvent);
	hr = m_graphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_mediaControl);
	hr = m_graphBuilder->QueryInterface (IID_IMediaPosition, (void **)&m_mediaPosition);

	hr = m_graphBuilder->RenderFile(L"intro.avi", NULL);
	hr = m_mediaControl->Run();
	
	hr = m_graphBuilder->QueryInterface(IID_IVideoWindow, (void **)&m_videoWindow);

	hr = m_videoWindow->put_Owner((OAHWND)hWnd);
	hr = m_videoWindow->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	RECT rect;
	GetClientRect(hWnd, &rect);
	hr = m_videoWindow->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom);
	m_videoWindow->put_Visible(OATRUE);
	
}

CMedia_Core::~CMedia_Core()
{

}

bool CMedia_Core::isFinished()
{
	SetActiveWindow(m_hWndAccess);//pass focus back to the main window.
	REFTIME curposition,duration;
	m_mediaPosition->get_CurrentPosition(&curposition);
	m_mediaPosition->get_Duration(&duration);
	if ( curposition >= duration )
	{
		m_mediaControl->Release();
		m_graphBuilder->Release();
		m_videoWindow->Release();
		m_mediaControl->Stop();
		return true;
	}
	return false;
}

void CMedia_Core::ForceFinish()
{
	SetActiveWindow(m_hWndAccess);//pass focus back to the main window.
	m_mediaControl->Release();
	m_graphBuilder->Release();
	m_videoWindow->Release();
	m_mediaControl->Stop();
}