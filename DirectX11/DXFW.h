#pragma once

#include <Windows.h>

class DXFW
{
public:
	DXFW ();
	~DXFW ();

	bool Initialize ();
	void Run ();

private:
	bool CreateDXWindow (const char *Title, int x, int y, int width, int height);

	char *m_applicationName;
	HINSTANCE m_hInstance;
};

