#pragma once

#include "DXManager.h"

class Graphics
{
public:
	Graphics ();
	~Graphics ();

	bool InitializeDX (HWND hWnd);
	void BeginScene (float r, float g, float b, float a);
	void EndScene ();

	void EnableAlphaBlending (bool enable);
	void EnableZBuffer (bool enable);

	DXManager *GetDXManager ();
	HWND GethWnd ();
	ID3D11Device *GetDevice ();
	ID3D11DeviceContext *GetDeviceContext ();

	void SethWnd (HWND hWnd);

private:
	DXManager *m_dxManager;
	HWND m_hWnd;
};

