#include "AppMgr.h"
#include "DebugFuncs.h"

AppMgr::AppMgr () :
	m_hWnd		 (nullptr),
	m_hInstance	 (nullptr),
	m_winManager (nullptr),
	m_engine	 (nullptr)
{}

AppMgr::~AppMgr ()
{
	delete m_winManager;
	delete m_engine;
}

bool AppMgr::Initialize ()
{
	WndCnf::WindowDesc winDesc (0, 0);
	m_winManager = new WinManager ();
	if (!m_winManager->Initialize (winDesc))
	{
		RETURN_FALSE;
	}

	m_hWnd		= m_winManager->GetHWnd ();
	m_hInstance = m_winManager->GetHInstance ();

	m_engine = new Engine ();
	if (!m_engine->InitializeGraphics (m_hWnd))
	{
		RETURN_FALSE;
	}


}