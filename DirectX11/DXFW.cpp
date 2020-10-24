#include <cstdio>

#include "DXFW.h"
#include "SystemDefs.h"
#include "AddFunc.h"

LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

DXFW::DXFW ()
{

}

DXFW::~DXFW ()
{
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings (nullptr, 0);
	}

	Engine::GetEngine ()->Release ();

	UnregisterClass (m_applicationName, m_hInstance);
	m_hInstance = nullptr;
}

bool DXFW::Initialize ()
{
	if (!CreateDXWindow ("DirectX11 - force", WINDOW_POSX, WINDOW_POSY, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		RETURN_FALSE;
	}

	if (!Engine::GetEngine ()->Initialize (m_hInstance, Engine::GetEngine ()->GetGraphics ()->GethWnd ()))
	{
		RETURN_FALSE;
	}

	return true;
}

void DXFW::Run ()
{
	MSG msg = {0};
	ZeroMemory (&msg, sizeof (msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		else
		{
			Engine::GetEngine ()->Run ();
		}
	}
}

bool DXFW::CreateDXWindow (const char *Title, int x, int y, int width, int height)
{
	HWND hWnd = nullptr;
	WNDCLASSEX wc = {0};

	m_applicationName = const_cast <char *> (Title);
	m_hInstance = GetModuleHandle (nullptr);

	wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = m_hInstance;
	wc.hIcon		 = LoadIcon (nullptr, IDI_WINLOGO);
	wc.hIconSm		 = wc.hIcon;
	wc.hCursor		 = LoadCursor (nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = m_applicationName;
	wc.cbSize		 = sizeof (WNDCLASSEX);

	if (!RegisterClassEx (&wc))
	{
		MessageBox (nullptr, "RegisterClassEx () failed", "Error", MB_OK);
		RETURN_FALSE;
	}

	int screenWidth	 = -1;
	int screenHeight = -1;

	if (FULL_SCREEN)
	{
		screenWidth  = GetSystemMetrics (SM_CXSCREEN);
		screenHeight = GetSystemMetrics (SM_CYSCREEN);

		DEVMODE dmScreenSettings = {0};
		ZeroMemory (&dmScreenSettings, sizeof (dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof (dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (unsigned long) screenWidth;
		dmScreenSettings.dmPelsHeight	= (unsigned long) screenHeight;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		screenWidth  = width;
		screenHeight = height;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;	
	hWnd = CreateWindowEx (WS_EX_APPWINDOW, Title, Title, nStyle,
						   x, y, screenWidth, screenHeight, nullptr, nullptr, m_hInstance, nullptr);

	if (hWnd == nullptr)
	{
		MessageBox (nullptr, "CreateWindowEx () failed", "Error", MB_OK);
		Engine::GetEngine ()->Release ();
		PostQuitMessage (0);

		RETURN_FALSE;
	}

	if (!Engine::GetEngine ()->InitializeGraphics (hWnd))
	{
		MessageBox (hWnd, "Could't initialize DirectX 11", "Error", MB_OK);
		Engine::GetEngine ()->Release ();
		PostQuitMessage (0);

		UnregisterClass (m_applicationName, m_hInstance);
		m_hInstance = nullptr;
		DestroyWindow (hWnd);

		RETURN_FALSE;
	}

	Engine::GetEngine ()->GetGraphics ()->SethWnd (hWnd);

	ShowWindow (hWnd, SW_SHOW);
	SetForegroundWindow (hWnd);
	SetFocus (hWnd);

	return true;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {0};
	HDC hdc = {0};

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
		default:
			{
				return DefWindowProc (hWnd, Msg, wParam, lParam);
			}
	}
	
	return 0;
}