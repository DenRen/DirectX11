#pragma once

// Only WINAPI

#include <windows.h>
#include "Config.h"

class WinAPIManager
{
public:
	WinAPIManager ();
	~WinAPIManager ();

	bool Initialize (const char *title, unsigned width, unsigned height,
					 int locateX, int locateY, bool fullScreen, bool vSync);
	void  Show ();

	HWND GetHWnd ();
	HINSTANCE GetHInstance ();

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	char *m_appName;
};

LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);