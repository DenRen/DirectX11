#pragma once

#include "Graphics.h"

class Engine
{
public:
	~Engine ();

	bool InitializeGraphics (HWND hWnd);
	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	void Run ();
	
	void Release ();

	Graphics *GetGraphics ();
	static Engine *GetEngine ();

private:
	Engine ();

	void Update ();
	void Render ();

	Graphics *m_Graphics;
	static Engine *m_Instance;
};

