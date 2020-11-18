#include "WinManager.h"
#include "Config.h"
#include "DebugFunc.h"
#include "AddFunc.h"

bool WinManager::Initialize (const char *title, unsigned width, unsigned height,
							 int locateX, int locateY, bool fullScreen, bool vSync)
{
	WNDCLASSEX wcex = {};
	SET_IN_ZERO (wcex);

	m_hInstance = GetModuleHandle (nullptr);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = nullptr;
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor (nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = title;
	wcex.cbSize = sizeof (WNDCLASSEX);

	if (!RegisterClassEx (&wcex))
	{
		RETURN_FALSE;
	}

	m_appName = const_cast <char *> (title);

	int screenWidth = -1;
	int screenHeight = -1;

	if (fullScreen)
	{
		screenWidth = GetSystemMetrics (SM_CXSCREEN);
		screenHeight = GetSystemMetrics (SM_CYSCREEN);

		DEVMODE dmScreenSettings = { 0 };
		SET_IN_ZERO (dmScreenSettings);

		dmScreenSettings.dmSize = sizeof (dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		screenWidth = width;
		screenHeight = height;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;
	m_hWnd = CreateWindowEx (WS_EX_APPWINDOW, title, title,
							 nStyle,
							 locateX, locateY, screenWidth, screenHeight,
							 nullptr, nullptr, m_hInstance, nullptr);

	if (m_hWnd == nullptr)
	{
		RETURN_FALSE;
	}

	Show ();

	return true;
}

void WinManager::Show ()
{
	ShowWindow (m_hWnd, SW_SHOW);
	SetForegroundWindow (m_hWnd);
	SetFocus (m_hWnd);
}

HWND WinManager::GetHWnd ()
{
	return m_hWnd;
}

HINSTANCE WinManager::GetHInstance ()
{
	return m_hInstance;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {};
	HDC hdc = {};

	switch (Msg)
	{
	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage (0);
				DestroyWindow (hWnd);
			}
		} break;
	case WM_PAINT:
		{
			hdc = BeginPaint (hWnd, &ps);
			EndPaint (hWnd, &ps);
		} break;
	case WM_CLOSE:
		{
			PostQuitMessage (0);
			DestroyWindow (hWnd);
		} break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		{
			printf ("%d %d\n", LOWORD (lParam), HIWORD (lParam));
		} break;
	default:
		{
			return DefWindowProc (hWnd, Msg, wParam, lParam);
		}
	}

	return 0;
}

WinManager::WinManager () :
	m_hWnd (nullptr),
	m_hInstance (nullptr),
	m_appName (nullptr)
{}

WinManager::~WinManager ()
{
	PostQuitMessage (0);

	if (m_appName != nullptr)
	{
		UnregisterClass (m_appName, m_hInstance);
		DestroyWindow (m_hWnd);
	}
}