#pragma once

#include <windows.h>
#include <string>

#include "WinAPIManager.h"
#include "Engine.h"
#include "Config.h"

class AppMgr
{
public:
	AppMgr ();
	~AppMgr ();

	bool Initialize ();
	bool Run ();

private:
	bool InitializeWinAPIManager ();

	std::string m_appTitle;
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	WinAPIManager *m_winManager;
	Engine *m_engine;
};

