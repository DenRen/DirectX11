#pragma once

#include <windows.h>
#include <string>

#include "WinManager.h"
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
	std::string m_appTitle;
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	WinManager *m_winManager;
	Engine *m_engine;
};

