#include "DXManager.h"
#include <cstdio>

#include "DXManager.h"
#include "DebugFunc.h"
#include "AddFunc.h"

ID3D11Device *DXManager::m_device = nullptr;
ID3D11DeviceContext *DXManager::m_deviceContext = nullptr;
ID3D11RenderTargetView *DXManager::m_renderTargetView = nullptr;
DXManager::DXManager () :
    m_vsync_enabled (false),
    m_videoCardDescription (""),
    m_videoCardMemory (0),
    m_swapChain (nullptr),
    //m_renderTargetView (nullptr),
    m_depthStencilBuffer (nullptr),
    m_rasterState (nullptr),
    m_depthStencilState (nullptr),
    m_depthStencilView (nullptr),
    m_alphaEnableBlendingState (nullptr),
    m_alphaDisableBlendingState (nullptr),
    m_depthDisabledStencilState (nullptr)
{}

DXManager::~DXManager ()
{

    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState (false, nullptr);
    }

    RELEASE (m_alphaEnableBlendingState);
    RELEASE (m_alphaDisableBlendingState);
    RELEASE (m_depthDisabledStencilState);
    RELEASE (m_depthStencilView);
    RELEASE (m_rasterState);
    RELEASE (m_depthStencilState);
    RELEASE (m_depthStencilBuffer);
    RELEASE (m_renderTargetView);
    RELEASE (m_deviceContext);
    RELEASE (m_device);
    RELEASE (m_swapChain);

}


bool DXManager::GetNumDenum (int Width, int Height, unsigned *numerator, unsigned *denominator,
                             bool saveAdapterDesc)
{
    CHECK_NULLPTR (numerator);
    CHECK_NULLPTR (denominator);

    IDXGIFactory *factory = nullptr;
    IDXGIAdapter *adapter = nullptr;
    IDXGIOutput *adapterOutput = nullptr;
    unsigned int numModes = 0, stringLength = 0;

    HRESULT result = S_OK;
    result = CreateDXGIFactory (__uuidof (IDXGIFactory), (void **)&factory);
    CHECK_FAILED (result);

    result = factory->EnumAdapters (0, &adapter);
    CHECK_FAILED (result);

    result = adapter->EnumOutputs (0, &adapterOutput);
    CHECK_FAILED (result);

    result = adapterOutput->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    CHECK_FAILED (result);

    auto displayModeList = new DXGI_MODE_DESC[numModes];
    result = adapterOutput->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_ENUM_MODES_INTERLACED, &numModes,
        displayModeList);
    CHECK_FAILED (result);

    for (int i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == Width)
        {
            if (displayModeList[i].Height == Height)
            {
                *numerator = displayModeList[i].RefreshRate.Numerator;
                *denominator = displayModeList[i].RefreshRate.Denominator;

                break;
            }
        }
    }

    delete[] displayModeList;
    displayModeList = nullptr;

    if (*numerator == 0 && *denominator == 0)
    {
        RETURN_FALSE;
    }

    DXGI_ADAPTER_DESC adapterDesc = { 0 };
    if (saveAdapterDesc)
    {
        result = adapter->GetDesc (&adapterDesc);
        CHECK_FAILED (result);

        m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

        int error = wcstombs_s (&stringLength, m_videoCardDescription, 128,
            adapterDesc.Description, 128);
        if (error != 0)
        {
            RETURN_FALSE;
        }
    }

    RELEASE (adapterOutput);
    RELEASE (adapter);
    RELEASE (factory);

    return true;
}

bool DXManager::Initilize (HWND hWnd, int Width, int Height, bool fullScreen, bool vsync)
{
   /* HRESULT result = S_OK;
    unsigned numerator = 0, denominator = 0;

    m_vsync_enabled = vsync;

    if (!GetNumDenum (Width, Height, &numerator, &denominator, true))   RETURN_FALSE;
    if (!InitializeSwapChain (hWnd, fullScreen, Width, Height,
                              numerator, denominator))                  RETURN_FALSE;

    if (!InitializeRenderTargetView ())         RETURN_FALSE;*/
    //if (!InitializeDepthBuffer (Width, Height)) RETURN_FALSE;
    //if (!InitializeDepthStencilBuffer ())       RETURN_FALSE;
    //if (!InitializeStencilView ())              RETURN_FALSE;

    // Bind the render target view and depth stencil buffer to the output render pipeline 
    //m_deviceContext->OMSetRenderTargets (1, &m_renderTargetView, m_depthStencilView);
    HRESULT hr = S_OK;
    RECT rc;

    GetClientRect (hWnd, &rc);
    UINT width = rc.right - rc.left;           // получаем ширину
    UINT height = rc.bottom - rc.top;   // и высоту окна
    UINT createDeviceFlags = 0;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE (driverTypes);

    // Тут мы создаем список поддерживаемых версий DirectX

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    UINT numFeatureLevels = ARRAYSIZE (featureLevels);


    DXGI_SWAP_CHAIN_DESC sd;                   // Структура, описывающая цепь связи (Swap Chain)

    ZeroMemory (&sd, sizeof (sd));    // очищаем ее
    sd.BufferCount = 1;                 // у нас один буфер
    sd.BufferDesc.Width = width;        // ширина буфера
    sd.BufferDesc.Height = height;             // высота буфера
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // формат пикселя в буфере
    sd.BufferDesc.RefreshRate.Numerator = 75;         // частота обновления экрана
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера - задний буфер
    sd.OutputWindow = hWnd;                               // привязываем к нашему окну
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;                               // не полноэкранный режим

    D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain (NULL, g_driverType, NULL, createDeviceFlags, 
            featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, 
            &m_swapChain, &m_device, &g_featureLevel, &m_deviceContext);

        if (SUCCEEDED (hr))  // Если устройства созданы успешно, то выходим из цикла
            break;

    }

    if (FAILED (hr)) return hr;

    ID3D11Texture2D *pBackBuffer = NULL;
    hr = m_swapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
    if (FAILED (hr)) return hr;

    hr = m_device->CreateRenderTargetView (pBackBuffer, NULL, &m_renderTargetView);
    pBackBuffer->Release ();

    if (FAILED (hr)) return hr;

    m_deviceContext->OMSetRenderTargets (1, &m_renderTargetView, NULL);

    //if (!InitializeRasterizerState ())  RETURN_FALSE;
    InitializeViewport (Width, Height);
    //if (!InitializeAlphaBlending ())    RETURN_FALSE;
    //if (!InitializeZBuffer ())          RETURN_FALSE;

    return true;
}

void DXManager::BeginScene (float r, float g, float b, float a)
{
    float color[4] = { 0.0f };
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;

    m_deviceContext->ClearRenderTargetView (m_renderTargetView, color);
    //m_deviceContext->ClearDepthStencilView (m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DXManager::EndScene ()
{
    if (m_vsync_enabled)
    {
        // Lock to screen refresh rate
        m_swapChain->Present (1, 0);
    }
    else
    {
        // Present as fast as possible
        m_swapChain->Present (0, 0);
    }
}

void DXManager::EnableAplhaBlending (bool enable)
{
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (enable)
    {
        m_deviceContext->OMSetBlendState (m_alphaEnableBlendingState, blendFactor, 0xffffffff);
    }
    else
    {
        m_deviceContext->OMSetBlendState (m_alphaDisableBlendingState, blendFactor, 0xffffffff);
    }
}

void DXManager::EnableZBuffer (bool enable)
{
    if (enable)
    {
        m_deviceContext->OMSetDepthStencilState (m_depthStencilState, 1);
    }
    else
    {
        m_deviceContext->OMSetDepthStencilState (m_depthDisabledStencilState, 1);
    }
}

ID3D11Device *DXManager::GetDevice ()
{
    return m_device;
}

ID3D11DeviceContext *DXManager::GetDeviceContext ()
{
    return m_deviceContext;
}

bool DXManager::InitializeSwapChain (HWND hWnd, bool fullScreen,
                                     int screenWidth, int screenHeight,
                                     unsigned numerator, unsigned denominator)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
    SET_IN_ZERO (swapChainDesc);

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (m_vsync_enabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 75;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1; // Turn multisampling off
    swapChainDesc.SampleDesc.Quality = 0; //
    swapChainDesc.Windowed = !fullScreen;

    // Set the scan line ordering and scailing to unspecified
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    // Set the feature level to DirectX11
    auto featureLevel = D3D_FEATURE_LEVEL_11_0;

    HRESULT result = S_OK;
    result = D3D11CreateDeviceAndSwapChain (NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel,
        1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain,
        &m_device, NULL, &m_deviceContext);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeRenderTargetView ()
{
    HRESULT result = S_OK;
    ID3D11Texture2D *pBackBuf = nullptr;

    result = m_swapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), (LPVOID *) &pBackBuf);
    CHECK_FAILED (result);

    result = m_device->CreateRenderTargetView (pBackBuf, NULL, &m_renderTargetView);
    CHECK_FAILED (result);

    RELEASE (pBackBuf);

    return true;
}

bool DXManager::InitializeDepthBuffer (int Width, int Height)
{
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    SET_IN_ZERO (depthBufferDesc);

    // Set up depth buffer desc
    depthBufferDesc.Width = Width;
    depthBufferDesc.Height = Height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    HRESULT result = S_OK;
    result = m_device->CreateTexture2D (&depthBufferDesc, NULL, &m_depthStencilBuffer);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeDepthStencilBuffer ()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    SET_IN_ZERO (depthStencilDesc);

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask  = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    HRESULT result = S_OK;
    result = m_device->CreateDepthStencilState (&depthStencilDesc, &m_depthStencilState);
    CHECK_FAILED (result);

    m_deviceContext->OMSetDepthStencilState (m_depthStencilState, 1);

    return true;
}

bool DXManager::InitializeStencilView ()
{
    HRESULT result = S_OK;

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    SET_IN_ZERO (depthStencilViewDesc);

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    result = m_device->CreateDepthStencilView (m_depthStencilBuffer, &depthStencilViewDesc,
                                               &m_depthStencilView);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeRasterizerState ()
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    SET_IN_ZERO (rasterDesc);

    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    HRESULT result = S_OK;
    result = m_device->CreateRasterizerState (&rasterDesc, &m_rasterState);
    CHECK_FAILED (result);

    m_deviceContext->RSSetState (m_rasterState);

    return true;
}

void DXManager::InitializeViewport (int Width, int Height)
{
    D3D11_VIEWPORT viewport = { 0 };
    SET_IN_ZERO (viewport);

    viewport.Width = (float)Width;
    viewport.Height = (float)Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    m_deviceContext->RSSetViewports (1, &viewport);
}

bool DXManager::InitializeAlphaBlending ()
{
    D3D11_BLEND_DESC blendStateDesc = { 0 };
    SET_IN_ZERO (blendStateDesc);

    blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    HRESULT result = S_OK;
    result = m_device->CreateBlendState (&blendStateDesc, &m_alphaEnableBlendingState);
    CHECK_FAILED (result);

    // Modify to create disable alpha blend state
    blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

    result = m_device->CreateBlendState (&blendStateDesc, &m_alphaDisableBlendingState);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeZBuffer ()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
    SET_IN_ZERO (depthStencilDesc);

    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask  = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    HRESULT result = S_OK;
    result = m_device->CreateDepthStencilState (&depthStencilDesc, &m_depthDisabledStencilState);
    CHECK_FAILED (result);

    return true;
}