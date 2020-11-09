#pragma once

// Only WINAPI

#include <windows.h>
#include <string>
#include "Config.h"

class WinManager
{
public:
	WinManager ();
	~WinManager ();
	bool Initialize (WndCnf::WindowDesc &winDesc);
	HWND GetHWnd ();
	HINSTANCE GetHInstance ();

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	char *m_appName;
};

LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);