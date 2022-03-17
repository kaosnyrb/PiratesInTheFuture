//--------------------------------------------------------
// The Media Core, handles video/music
// Bryn Stringer 
//--------------------------------------------------------
#pragma once
#include "dshow.h"

class CMedia_Core
{
	public:
		CMedia_Core(HWND hWnd);
		~CMedia_Core();
		bool isFinished();
		void ForceFinish();
	private:
		IGraphBuilder *m_graphBuilder;
		IMediaControl *m_mediaControl;
		IMediaEvent  *m_mediaEvent;
		IMediaPosition *m_mediaPosition;
		IVideoWindow *m_videoWindow;
		HWND m_hWndAccess;
};	