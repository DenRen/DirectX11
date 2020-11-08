#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>   // Добавились новые заголовки
#include <xnamath.h>
//#include "resource.h"
#include "AddFunc.h"

const float _size = 1;
const int MX_SETWORLD = 0x101;

struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 norm;
};

struct ConstantBufferMatrixes
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

struct ConstantBufferLights
{
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
};

HINSTANCE				g_hInstance = nullptr;
HWND					g_hWnd		= nullptr;

D3D_DRIVER_TYPE			g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL		g_featureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*			g_device			= nullptr;
ID3D11DeviceContext*	g_deviceContext		= nullptr;
IDXGISwapChain*			g_pSwapChain		= nullptr;
ID3D11RenderTargetView*	g_pRenderTargetView = nullptr;
ID3D11Texture2D*		g_pDepthStencil		= nullptr;
ID3D11DepthStencilView* g_pDepthStencilView = nullptr;

ID3D11VertexShader*		g_pVertexShader		= nullptr;
ID3D11PixelShader*		g_pPixelShader		= nullptr;
ID3D11PixelShader*		g_pPixelShaderSolid = nullptr;
ID3D11InputLayout*		g_pVertexLayout		= nullptr;
ID3D11Buffer*			g_pVertexBuffer		= nullptr;
ID3D11Buffer*			g_pIndexBuffer		= nullptr;
ID3D11Buffer*			g_pCBMatrixes		= nullptr;
ID3D11Buffer*			g_pCBLight			= nullptr;

XMMATRIX g_World;
XMMATRIX g_View;
XMMATRIX g_Projection;
float myTime = 0.0f;

XMFLOAT4 vLightDirs[2];
XMFLOAT4 vLightColors[2];

ID3D11ShaderResourceView* g_pTextureRV	   = nullptr;
ID3D11SamplerState*		  g_pSamplerLinear = nullptr;

HRESULT InitWindow (HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice ();
HRESULT InitGeometry ();
HRESULT InitMatrixes ();

void UpdateTime ();

void UpdateLight ();
void UpdateMatrix (UINT nLightIndex);
void SetMatrixes (float fAngle);
void CleanupDevice ();
void Render ();
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT CompileShaderFromFile (LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
							   ID3DBlob **ppBlobOut);

int WINAPI wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER (hInstance);
	UNREFERENCED_PARAMETER (hPrevInstance);

	if (FAILED (InitWindow (hInstance, nCmdShow)))
	{
		PRINT_DEBUG_INFO;
		return 0;
	}

	if (FAILED (InitDevice ()))
	{
		PRINT_DEBUG_INFO;
		CleanupDevice ();
		return 0;
	}
	
	if (FAILED (InitGeometry ()))
	{
		PRINT_DEBUG_INFO;
		CleanupDevice ();
		return 0;
	}

	if (FAILED (InitMatrixes ()))
	{
		CleanupDevice ();
		PRINT_DEBUG_INFO;
		return 0;
	}

	MSG msg = {0};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage (&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage	 (&msg);
		}
		else
		{
			UpdateTime ();
			Render ();
		}
	}

	CleanupDevice ();

	return (int) msg.wParam;
}

HRESULT InitWindow (HINSTANCE hInstance, int nCmdShow)
{
	const char *pIcon = "Resources\\Plazma-3.jpg";

	WNDCLASSEX wcex = {0};
	ZeroMemory (&wcex, sizeof (wcex));
	wcex.cbSize = sizeof (WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon (hInstance, (LPCTSTR) pIcon);
	wcex.hCursor = LoadCursor (hInstance, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "MyWindowClass";
	wcex.hIconSm = LoadIcon (wcex.hInstance, (LPCTSTR) pIcon);

	if (!RegisterClassEx (&wcex))
	{
		PRINT_DEBUG_INFO;
		return E_FAIL;
	}
	g_hInstance = hInstance;
	RECT rect = {0, 0, 1920 / _size, 1080 / _size};
	AdjustWindowRect (&rect, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindow ("MyWindowClass", "Покоряю DX11",
						   WS_OVERLAPPEDWINDOW,
						   CW_USEDEFAULT, CW_USEDEFAULT, 
						   rect.right - rect.left, rect.bottom - rect.top, 
						   nullptr, nullptr, hInstance, nullptr);
	if (g_hWnd == nullptr)
	{
		PRINT_DEBUG_INFO;
		return E_FAIL;
	}

	ShowWindow (g_hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {0};
	HDC hDC = nullptr;

	switch (message)
	{
		case WM_PAINT:
			hDC = BeginPaint (hWnd, &ps);
			EndPaint (hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
		default:
			return DefWindowProc (hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT InitDevice ()
{
	HRESULT hr = S_OK;

	RECT rect = {0};
	GetClientRect (g_hWnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;
	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = ARRAYSIZE (driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE (featureLevels);

	DXGI_SWAP_CHAIN_DESC sd = {0};
	ZeroMemory (&sd, sizeof (sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 75;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;           
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain (nullptr, g_driverType, nullptr, createDeviceFlags,
											featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
											&sd, &g_pSwapChain, &g_device, &g_featureLevel,
											&g_deviceContext);
		if (SUCCEEDED (hr))
		{
			break;
		}
	}
	
	if (FAILED (hr))
	{
		PRINT_DEBUG_INFO;
		return hr;
	}

	// RenderTargetOutput - передний буфер
	// RenderTargetView   - задний буфер

	ID3D11Texture2D *pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), (LPVOID *) &pBackBuffer);
	if (FAILED (hr))
	{
		PRINT_DEBUG_INFO;
		return hr;
	}

	hr = g_device->CreateRenderTargetView (pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release ();
	if (FAILED (hr))
	{
		PRINT_DEBUG_INFO;
		return hr;
	}
	
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory (&descDepth, sizeof (descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = g_device->CreateTexture2D (&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED (hr))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory (&descDSV, sizeof (descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = g_device->CreateDepthStencilView (g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED (hr))
	{
		return hr;
	}

	g_deviceContext->OMSetRenderTargets (1, &g_pRenderTargetView, g_pDepthStencilView);
	
	D3D11_VIEWPORT viewport = {0};
	ZeroMemory (&viewport, sizeof (viewport));

	viewport.Width    = (float) width;
	viewport.Height   = (float) height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_deviceContext->RSSetViewports (1, &viewport);

	return S_OK;
}

void Render ()
{
	float ClearColor[4] = {0, 0, 0, 1};
	g_deviceContext->ClearRenderTargetView (g_pRenderTargetView, ClearColor);
	g_deviceContext->ClearDepthStencilView (g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	UpdateLight ();

	g_deviceContext->VSSetShader (g_pVertexShader, nullptr, 0);
	g_deviceContext->VSSetConstantBuffers (0, 1, &g_pCBMatrixes);
	g_deviceContext->VSSetConstantBuffers (1, 1, &g_pCBLight);

	g_deviceContext->PSSetConstantBuffers (0, 1, &g_pCBMatrixes);
	g_deviceContext->PSSetConstantBuffers (1, 1, &g_pCBLight);
	g_deviceContext->PSSetShaderResources (0, 1, &g_pTextureRV);
	g_deviceContext->PSSetSamplers (0, 1, &g_pSamplerLinear);

	g_deviceContext->PSSetShader (g_pPixelShaderSolid, nullptr, 0);
	for (int i = 0; i < 2; i++)
	{
		UpdateMatrix (i);
		g_deviceContext->DrawIndexed (36, 0, 0);
	}

	UpdateMatrix (MX_SETWORLD);
	g_deviceContext->PSSetShader (g_pPixelShader, nullptr, 0);
	g_deviceContext->DrawIndexed (36, 0, 0);

	g_pSwapChain->Present (0, 0);
}

HRESULT CompileShaderFromFile (LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
							   ID3DBlob **ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob *pErrorBlob = nullptr;
	
	hr = D3DX11CompileFromFile (szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
								dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
	if (FAILED (hr))
	{
		if (pErrorBlob != nullptr)
		{
			OutputDebugStringA ((char *) pErrorBlob->GetBufferPointer ());
		}
		else
		{
			RELEASE (pErrorBlob);
		}

		return hr;
	}

	if (pErrorBlob != nullptr)
	{
		pErrorBlob->Release ();
	}

	return S_OK;
}

HRESULT InitGeometry ()
{

	HRESULT hr = S_OK;

	LPCSTR fileName = "Textures\\texture.fx";

	ID3DBlob *pVSBlob = nullptr;
	hr = CompileShaderFromFile (fileName, "VS", "vs_4_0", &pVSBlob);
	if (FAILED (hr))
	{
		MessageBox (nullptr, "Failed to create FX (VS) file.", "Error", MB_OK);
		return hr;
	}

	hr = g_device->CreateVertexShader (pVSBlob->GetBufferPointer (), pVSBlob->GetBufferSize (),
									   nullptr, &g_pVertexShader);
	if (FAILED (hr))
	{
		pVSBlob->Release ();
		return hr;
	}

	// ---------------------------------------------------------------------------------------
	
	UINT size = 0;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, size += 0,							D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, size += sizeof (SimpleVertex::pos),	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, size += sizeof (SimpleVertex::tex),	D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE (layout);

	hr = g_device->CreateInputLayout (layout, numElements, pVSBlob->GetBufferPointer (),
									  pVSBlob->GetBufferSize (), &g_pVertexLayout);
	if (FAILED (hr))
	{
		return hr;
	}

	g_deviceContext->IASetInputLayout (g_pVertexLayout);

	// ------------------------------------------------------------------------------------------

	ID3DBlob *pPSBlob = nullptr;
	hr = CompileShaderFromFile (fileName, "PS", "ps_4_0", &pPSBlob);
	if (FAILED (hr))
	{
		MessageBox (nullptr, "Failed to create FX (PS) file.", "Error", MB_OK);
		return hr;
	}

	hr = g_device->CreatePixelShader (pPSBlob->GetBufferPointer (), pPSBlob->GetBufferSize (),
									  nullptr, &g_pPixelShader);
	pPSBlob->Release ();

	if (FAILED (hr))
	{
		return hr;
	}

	// ------------------------------------------------------------------------------------------

	pPSBlob = nullptr;
	hr = CompileShaderFromFile (fileName, "PSSolid", "ps_4_0", &pPSBlob);
	if (FAILED (hr))
	{
		MessageBox (nullptr, "Failed to create FX (PSSolid) file.", "Error", MB_OK);
		return hr;
	}

	hr = g_device->CreatePixelShader (pPSBlob->GetBufferPointer (), pPSBlob->GetBufferSize (),
									  nullptr, &g_pPixelShaderSolid);
	if (FAILED (hr))
	{
		return hr;
	}

	// ------------------------------------------------------------------------------------------
	
	SimpleVertex vert[] =
	{ /* координаты X, Y, Z              координаты текстры tu, tv   нормаль X, Y, Z        */
		{XMFLOAT3 (-1.0f,  1.0f, -1.0f), XMFLOAT2 (0.0f, 0.0f), XMFLOAT3 ( 0.0f,  1.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f,  1.0f, -1.0f), XMFLOAT2 (0.3f, 0.0f), XMFLOAT3 ( 0.0f,  1.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f,  1.0f,  1.0f), XMFLOAT2 (0.3f, 1.0f), XMFLOAT3 ( 0.0f,  1.0f,  0.0f)},
		{XMFLOAT3 (-1.0f,  1.0f,  1.0f), XMFLOAT2 (0.0f, 1.0f), XMFLOAT3 ( 0.0f,  1.0f,  0.0f)},

		{XMFLOAT3 (-1.0f, -1.0f, -1.0f), XMFLOAT2 (0.0f, 0.0f), XMFLOAT3 ( 0.0f, -1.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f, -1.0f, -1.0f), XMFLOAT2 (0.3f, 0.0f), XMFLOAT3 ( 0.0f, -1.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f, -1.0f,  1.0f), XMFLOAT2 (0.3f, 1.0f), XMFLOAT3 ( 0.0f, -1.0f,  0.0f)},
		{XMFLOAT3 (-1.0f, -1.0f,  1.0f), XMFLOAT2 (0.0f, 1.0f), XMFLOAT3 ( 0.0f, -1.0f,  0.0f)},

		{XMFLOAT3 (-1.0f, -1.0f,  1.0f), XMFLOAT2 (0.0f, 0.0f), XMFLOAT3 (-1.0f,  0.0f,  0.0f)},
		{XMFLOAT3 (-1.0f, -1.0f, -1.0f), XMFLOAT2 (0.3f, 0.0f), XMFLOAT3 (-1.0f,  0.0f,  0.0f)},
		{XMFLOAT3 (-1.0f,  1.0f, -1.0f), XMFLOAT2 (0.3f, 1.0f), XMFLOAT3 (-1.0f,  0.0f,  0.0f)},
		{XMFLOAT3 (-1.0f,  1.0f,  1.0f), XMFLOAT2 (0.0f, 1.0f), XMFLOAT3 (-1.0f,  0.0f,  0.0f)},

		{XMFLOAT3 ( 1.0f, -1.0f,  1.0f), XMFLOAT2 (0.0f, 0.0f), XMFLOAT3 ( 1.0f,  0.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f, -1.0f, -1.0f), XMFLOAT2 (0.3f, 0.0f), XMFLOAT3 ( 1.0f,  0.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f,  1.0f, -1.0f), XMFLOAT2 (0.3f, 1.0f), XMFLOAT3 ( 1.0f,  0.0f,  0.0f)},
		{XMFLOAT3 ( 1.0f,  1.0f,  1.0f), XMFLOAT2 (0.0f, 1.0f), XMFLOAT3 ( 1.0f,  0.0f,  0.0f)},

		{XMFLOAT3 (-1.0f, -1.0f, -1.0f), XMFLOAT2 (0.0f, 0.0f), XMFLOAT3 ( 0.0f,  0.0f, -1.0f)},
		{XMFLOAT3 ( 1.0f, -1.0f, -1.0f), XMFLOAT2 (0.3f, 0.0f), XMFLOAT3 ( 0.0f,  0.0f, -1.0f)},
		{XMFLOAT3 ( 1.0f,  1.0f, -1.0f), XMFLOAT2 (0.3f, 1.0f), XMFLOAT3 ( 0.0f,  0.0f, -1.0f)},
		{XMFLOAT3 (-1.0f,  1.0f, -1.0f), XMFLOAT2 (0.0f, 1.0f), XMFLOAT3 ( 0.0f,  0.0f, -1.0f)},

		{XMFLOAT3 (-1.0f, -1.0f,  1.0f), XMFLOAT2 (0.0f, 0.0f), XMFLOAT3 ( 0.0f,  0.0f,  1.0f)},
		{XMFLOAT3 ( 1.0f, -1.0f,  1.0f), XMFLOAT2 (0.3f, 0.0f), XMFLOAT3 ( 0.0f,  0.0f,  1.0f)},
		{XMFLOAT3 ( 1.0f,  1.0f,  1.0f), XMFLOAT2 (0.3f, 1.0f), XMFLOAT3 ( 0.0f,  0.0f,  1.0f)},
		{XMFLOAT3 (-1.0f,  1.0f,  1.0f), XMFLOAT2 (0.0f, 1.0f), XMFLOAT3 ( 0.0f,  0.0f,  1.0f)},
	};

	const UINT numVertex = ARRAYSIZE (vert);

	D3D11_BUFFER_DESC bd = {0};
	ZeroMemory (&bd, sizeof (bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof (SimpleVertex) * numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {0};
	ZeroMemory (&InitData, sizeof (InitData));
	InitData.pSysMem = vert;

	hr = g_device->CreateBuffer (&bd, &InitData, &g_pVertexBuffer);
	if (FAILED (hr))
	{
		return hr;
	}
	
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	ZeroMemory (&bd, sizeof (bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof (indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = g_device->CreateBuffer (&bd, &InitData, &g_pIndexBuffer);
	if (FAILED (hr))
	{
		return hr;
	}

	// ---------------------------------------------------------------------------------------

	UINT stride = sizeof (SimpleVertex);
	UINT offset = 0;

	g_deviceContext->IASetVertexBuffers (0, 1, &g_pVertexBuffer, &stride, &offset);
	g_deviceContext->IASetIndexBuffer (g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_deviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// ---------------------------------------------------------------------------------------

	ZeroMemory (&bd, sizeof (bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof (ConstantBufferMatrixes);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_device->CreateBuffer (&bd, nullptr, &g_pCBMatrixes);
	if (FAILED (hr))
	{
		return hr;
	}

	ZeroMemory (&bd, sizeof (bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof (ConstantBufferLights);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_device->CreateBuffer (&bd, nullptr, &g_pCBLight);
	if (FAILED (hr))
	{
		return hr;
	}

	// ------------------------------------------------------------------------------------------

	LPCSTR texFileName = "Resources\\metall.dds";
	hr = D3DX11CreateShaderResourceViewFromFile (g_device, texFileName, nullptr, nullptr,
												 &g_pTextureRV, nullptr);
	if (FAILED (hr))
	{
		MessageBox (nullptr, "Failed to load metall.dds", "Error", MB_OK);
		return hr;
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory (&sampDesc, sizeof (sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_device->CreateSamplerState (&sampDesc, &g_pSamplerLinear);
	if (FAILED (hr))
	{
		return hr;
	}

	return S_OK;
}

void CleanupDevice ()
{
	if (g_deviceContext != nullptr)
	{
		g_deviceContext->ClearState ();
	}

	RELEASE (g_pSamplerLinear);
	RELEASE (g_pTextureRV);
	RELEASE (g_pCBMatrixes);
	RELEASE (g_pCBLight);

	RELEASE (g_pVertexBuffer);
	RELEASE (g_pIndexBuffer);
	RELEASE (g_pPixelShader);
	RELEASE (g_pVertexLayout);
	RELEASE (g_pVertexShader);
	RELEASE (g_pDepthStencil);
	RELEASE (g_pDepthStencilView);

	RELEASE (g_pRenderTargetView);
	RELEASE (g_pSwapChain);
	RELEASE (g_deviceContext);
	RELEASE (g_device);
}

HRESULT InitMatrixes ()
{
	RECT rect;
	GetClientRect (g_hWnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;

	g_World = XMMatrixIdentity ();

	XMVECTOR Eye = XMVectorSet (0.0f, 4.0f, -10.0f, 0.0f);
	XMVECTOR At  = XMVectorSet (1.5f, 1.0f,  0.0f, 0.0f);
	XMVECTOR Up  = XMVectorSet (0.0f, 1.0f,  0.0f, 0.0f);
	g_View = XMMatrixLookAtLH (Eye, At, Up);

	g_Projection = XMMatrixPerspectiveFovLH (XM_PIDIV4, width / (float) height, 0.01f, 100.0f);

	return S_OK;
}

void UpdateTime ()
{
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		myTime += (float) XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount ();
		if (dwTimeStart == 0)
		{
			dwTimeStart = dwTimeCur;
		}

		myTime = (dwTimeCur - dwTimeStart) / 1000.0f;
	}
}

void UpdateLight ()
{
	vLightDirs[0] = XMFLOAT4 (-0.577f, 0.577f, -0.577f, 1.0f);
	vLightDirs[1] = XMFLOAT4 (0.0f, 0.0f, -1.0f, 1.0f);

	vLightColors[0] = XMFLOAT4 (1.0f, 0.0f, 0.0f, 1.0f);
	vLightColors[1] = XMFLOAT4 (0.0f, 0.0f, 1.0f, 1.0f);

	XMMATRIX mRotate = XMMatrixRotationY (-3.0f * myTime); // 201
	XMVECTOR vLightDir = XMLoadFloat4 (&vLightDirs[1]);
	vLightDir = XMVector3Transform (vLightDir, mRotate);
	XMStoreFloat4 (&vLightDirs[1], vLightDir);

	mRotate = XMMatrixRotationY (0.5f * myTime);
	vLightDir = XMLoadFloat4 (&vLightDirs[0]);
	vLightDir = XMVector3Transform (vLightDir, mRotate);
	XMStoreFloat4 (&vLightDirs[0], vLightDir);
}

void UpdateMatrix (UINT nLightIndex)
{
	switch (nLightIndex)
	{
		case MX_SETWORLD:
			{
				g_World = XMMatrixRotationAxis (XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f), 0.25 * myTime);
				nLightIndex = 0;
			}
			break;
		default:
			{
				XMMATRIX mLightScale = XMMatrixScaling (0.2f, 0.2f, 0.2f);
				g_World = mLightScale;
				g_World *= XMMatrixTranslationFromVector (4.0f * XMLoadFloat4 (&vLightDirs[nLightIndex]));
				
				nLightIndex = 0;
			}
	}

	ConstantBufferMatrixes constBufMatrix;
	ConstantBufferLights constBufLight;
	constBufMatrix.mWorld		= XMMatrixTranspose (g_World);
	constBufMatrix.mView		= XMMatrixTranspose (g_View);
	constBufMatrix.mProjection	= XMMatrixTranspose (g_Projection);

	constBufLight.vLightColor[0] = vLightColors[0];
	constBufLight.vLightColor[1] = vLightColors[1];
	constBufLight.vLightDir[0]	 = vLightDirs[0];
	constBufLight.vLightDir[1]	 = vLightDirs[1];
	constBufLight.vOutputColor	 = XMFLOAT4 (1.0f, 1.0f, 1.0f, 1.0f);

	g_deviceContext->UpdateSubresource (g_pCBMatrixes, 0, nullptr, &constBufMatrix, 0, 0);
	g_deviceContext->UpdateSubresource (g_pCBLight, 0, nullptr, &constBufLight, 0, 0);
}

void SetMatrixes (float fAngle)
{
	ConstantBufferMatrixes cb;
	cb.mWorld		= XMMatrixTranspose (g_World);
	cb.mView		= XMMatrixTranspose (g_View);
	cb.mProjection	= XMMatrixTranspose (g_Projection);

	g_deviceContext->UpdateSubresource (g_pCBMatrixes, 0, nullptr, &cb, 0, 0);
}