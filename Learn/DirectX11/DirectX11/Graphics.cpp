#include <cstdio>

#include "Graphics.h"
#include "AddFunc.h"

Graphics::Graphics () :
	m_dxManager (nullptr)
{}
Graphics::~Graphics ()
{
	if (m_dxManager != nullptr)
	{
		delete m_dxManager;
		m_dxManager = nullptr;
	}
}

bool Graphics::InitializeDX (HWND hWnd)
{
	m_dxManager = new DXManager ();
	if (!m_dxManager->Initilize ((int) SCREEN_WIDTH, (int) SCREEN_HEIGHT, VSYNC_ENABLED, hWnd, FULL_SCREEN))
	{											
		RETURN_FALSE;
	}

	return true;
}

void Graphics::Initialize ()
{
	
}

void Graphics::BeginScene (float r, float g, float b, float a)
{
	m_dxManager->BeginScene (r, g, b, a);
}

void Graphics::EndScene ()
{
	m_dxManager->EndScene ();
}

void Graphics::EnableAlphaBlending (bool enable)
{
	m_dxManager->EnableAplhaBlending (enable);
}

void Graphics::EnableZBuffer (bool enable)
{
	m_dxManager->EnableZBuffer (enable);
}

void Graphics::SethWnd (HWND hWnd)
{
	m_hWnd = hWnd;
}

DXManager *Graphics::GetDXManager ()
{
	return m_dxManager;
}

HWND Graphics::GethWnd ()
{
	return m_hWnd;
}

ID3D11Device *Graphics::GetDevice ()
{
	return m_dxManager->GetDevice ();
}

ID3D11DeviceContext *Graphics::GetDeviceContext ()
{
	return m_dxManager->GetDeviceContext ();
}
