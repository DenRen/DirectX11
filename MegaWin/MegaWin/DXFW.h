#pragma once

#include "Engine.h"

class DXFW
{
public:
	DXFW ();
	~DXFW ();

	bool Initialize (GameComponent *gameComponent);
	void Run ();

private:
	bool CreateDXWindow (const char *Title, int x, int y, int width, int height);

	char *m_applicationName;
	HINSTANCE m_hInstance;
};

