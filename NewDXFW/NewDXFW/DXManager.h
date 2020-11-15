#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

class DXManager
{
public:
	DXManager ();
	~DXManager ();

	bool Initilize (HWND hWnd, int Width, int Height, bool fullScreen, bool vsync);
	void BeginScene (float r, float g, float b, float a);
	void EndScene ();

	void EnableAplhaBlending (bool enable);
	void EnableZBuffer (bool enable);

	static ID3D11Device *GetDevice ();
	static ID3D11DeviceContext *GetDeviceContext ();

	int m_videoCardMemory;
private:
	bool InitializeSwapChain (HWND hWnd, bool fullScreen,
		int screenWidth, int screenHeight,
		unsigned numerator, unsigned denominator);
	bool InitializeRenderTargetView ();
	bool InitializeDepthBuffer (int Width, int Height);
	bool InitializeDepthStencilBuffer ();
	bool InitializeStencilView ();
	bool InitializeRasterizerState ();
	void InitializeViewport (int Width, int Height);
	bool InitializeAlphaBlending ();
	bool InitializeZBuffer ();

	bool GetNumDenum (int Width, int Height, unsigned *numerator, unsigned *denominator,
		bool saveAdapterDesc = true);
	bool m_vsync_enabled;
	char m_videoCardDescription[128];
	IDXGISwapChain *m_swapChain;
	ID3D11RenderTargetView *m_renderTargetView;
	ID3D11Texture2D *m_depthStencilBuffer;
	ID3D11DepthStencilState *m_depthStencilState;
	ID3D11DepthStencilView *m_depthStencilView;
	ID3D11RasterizerState *m_rasterState;
	ID3D11BlendState *m_alphaEnableBlendingState;
	ID3D11BlendState *m_alphaDisableBlendingState;
	ID3D11DepthStencilState *m_depthDisabledStencilState;
	ID3D11SamplerState *m_samplerState;

	static ID3D11Device *m_device;
	static ID3D11DeviceContext *m_deviceContext;
};

