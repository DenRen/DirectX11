#include "AppMgr.h"
#include "DebugFunc.h"
#include "AddFunc.h"

AppMgr::AppMgr () :
	m_hWnd (nullptr),
	m_hInstance (nullptr),
	m_winManager (nullptr),
	m_engine (Engine::GetEngine ())
{}

AppMgr::~AppMgr ()
{
	delete m_winManager;
}

bool AppMgr::Initialize ()
{
	
	if (!InitializeWinManager ())						RETURN_FALSE;

	if (!m_engine->InitializeGraphics (m_hWnd))			RETURN_FALSE;
	
	if (!m_engine->Initialize (m_hInstance, m_hWnd))	RETURN_FALSE;

	m_winManager->Show ();

	return true;
}

bool AppMgr::Run ()
{
	MSG msg = { 0 };
	SET_IN_ZERO (msg);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		else
		{
			m_engine->Run ();
		}
	}

	return true;
}

bool AppMgr::InitializeWinManager ()
{
	WndCnf::WindowDesc wd (0, 0);

	m_winManager = new WinManager ();
	
	if (!m_winManager->Initialize (wd.title,
								   wd.width,      wd.height,
								   wd.locateX,    wd.locateY,
								   wd.fullScreen, wd.vSync))
	{
		RETURN_FALSE;
	}

	m_hWnd = m_winManager->GetHWnd ();
	m_hInstance = m_winManager->GetHInstance ();

	return true;
}
