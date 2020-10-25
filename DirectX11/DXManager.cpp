#include <cstdio>

#include "DXManager.h"
#include "AddFunc.h"

DXManager::DXManager () :
	m_vsync_enabled             (false),
    m_videoCardMemory           (0),
    m_swapChain                 (nullptr),
    m_device                    (nullptr),
    m_deviceContext             (nullptr),
    m_renderTargetView          (nullptr),
    m_depthStencilBuffer        (nullptr),
    m_rasterState               (nullptr),
    m_depthStencilState         (nullptr),
    m_depthStencilView          (nullptr),
    m_alphaEnableBlendingState  (nullptr),
    m_alphaDisableBlendingState (nullptr),
    m_depthDisabledStencilState (nullptr)
{
	 
}

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

bool DXManager::Initilize (int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullScreen)
{
    HRESULT result = S_OK;
    IDXGIFactory *factory = nullptr;
    IDXGIAdapter *adapter = nullptr;
    IDXGIOutput *adapterOutput = nullptr;
    unsigned int numModes = 0, numerator = 0, denominator = 0, stringLength = 0;
    DXGI_MODE_DESC *displayModeList = nullptr;
    DXGI_ADAPTER_DESC adapterDesc = {0};
    int error = 0;
    ID3D11Texture2D *backBufferPtr = nullptr;

    // Store vsync setting
    m_vsync_enabled = vsync;

    // Create a DirectX graphic interface factory
    result = CreateDXGIFactory (__uuidof (IDXGIFactory), (void **) &factory);
    CHECK_FAILED (result);

    // Use tha factory to create an adapter for the primary graphics interface
    result = factory->EnumAdapters (0, &adapter);
    CHECK_FAILED (result);

    // Enumerate the primary adapter output
    result = adapter->EnumOutputs (0, &adapterOutput);
    CHECK_FAILED (result);

    // Get number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format the adapter
    result = adapterOutput->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM, 
                                                DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    CHECK_FAILED (result);

    // Create a list to hold all the possible modes for this monitor/video card combination
    displayModeList = new DXGI_MODE_DESC[numModes];

    // Fill the display mode list
    result = adapterOutput->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM,
                                                DXGI_ENUM_MODES_INTERLACED, &numModes, 
                                                displayModeList);
    CHECK_FAILED (result);

    for (int i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == (unsigned int) screenWidth)
        {
            if (displayModeList[i].Height == (unsigned int) screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;

                break;
            }
        }
    }
    
    if (numerator == 0 && denominator == 0)
    {
        RETURN_FALSE;
    }

    // Get the adapter description
    result = adapter->GetDesc (&adapterDesc);
    CHECK_FAILED (result);

    // Store the video card memory in mbs
    m_videoCardMemory = (int) (adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a character array
    error = wcstombs_s (&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        return false;
    }

    // Release memory
    delete [] displayModeList;
    displayModeList = nullptr;

    RELEASE (adapterOutput);
    RELEASE (adapter);
    RELEASE (factory);

    if (!InitializeSwapChain (hWnd, fullScreen, screenWidth, screenHeight, numerator, denominator))
    {
        RETURN_FALSE;
    }

    // Get the pointer to the back buffer
    result = m_swapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), (LPVOID *) &backBufferPtr);
    CHECK_FAILED (result);

    // Create the render target
    result = m_device->CreateRenderTargetView (backBufferPtr, NULL, &m_renderTargetView);
    CHECK_FAILED (result);

    // Release back buffer
    backBufferPtr->Release ();
    backBufferPtr = nullptr;

    if (!InitializeDepthBuffer (screenWidth, screenHeight))
    {
        RETURN_FALSE;
    }

    if (!InitializeDepthStencilBuffer ())
    {
        RETURN_FALSE;
    }

    if (!InitializeStencilView ())
    {
        RETURN_FALSE;
    }

    // Bind the render target view and depth stencil buffer to the output render pipeline 
    m_deviceContext->OMSetRenderTargets (1, &m_renderTargetView, m_depthStencilView);

    if (!InitializeRasterizerState ())
    {
        RETURN_FALSE;
    }

    InitializeViewport (screenWidth, screenHeight);

    if (!InitializeAlphaBlending ())
    {
        RETURN_FALSE;
    }

    if (!InitializeZBuffer ())
    {
        RETURN_FALSE;
    }

    return true;
}

void DXManager::BeginScene (float r, float g, float b, float a)
{
    float color[4] = {0.0f};
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;

    // Clear the back buffer
    m_deviceContext->ClearRenderTargetView (m_renderTargetView, color);

    // Clear the depth buffer
    m_deviceContext->ClearDepthStencilView (m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
    float blendFactor[4] = {0.0f};

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
    return nullptr;
}

ID3D11DeviceContext *DXManager::GetDeviceContext ()
{
    return nullptr;
}

bool DXManager::InitializeSwapChain (HWND hWnd, bool fullScreen, int screenWidth, int screenHeight, unsigned int numerator, unsigned int denominator)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT result = S_OK;

    // Initialize swap chain
    ZeroMemory (&swapChainDesc, sizeof (swapChainDesc));

    // Set to a single back buffer
    swapChainDesc.BufferCount = 1;

    // Set the width and height of the back buffer
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

    // Set regular 32-bit back buffer
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set the refresh rate of the back buffer
    if (m_vsync_enabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    // Set the usage of the back buffer
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // Set the handle for the window
    swapChainDesc.OutputWindow = hWnd;

    // Turn multisampling off
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Set to full screen or windowed mode
    swapChainDesc.Windowed = fullScreen;

    // Set the scan line ordering and scailing to unspecified
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discord the back buffer contents after scaling after presenting
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    swapChainDesc.Flags = 0;

    // Set the feature level to DirectX11
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    // Create swap chain, device and device context 
    result = D3D11CreateDeviceAndSwapChain (NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel,
                                            1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain,
                                            &m_device, NULL, &m_deviceContext);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeDepthBuffer (int screenWidth, int screenHeight)
{
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    HRESULT result = S_OK;

    // Initialize depth buffer desc
    ZeroMemory (&depthBufferDesc, sizeof (depthBufferDesc));

    // Set up depth buffer desc
    depthBufferDesc.Width = screenWidth;
    depthBufferDesc.Height = screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer
    result = m_device->CreateTexture2D (&depthBufferDesc, NULL, &m_depthStencilBuffer);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeDepthStencilBuffer ()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    HRESULT result = S_OK;
    
    // Initialize depth stencil desc
    ZeroMemory (&depthStencilDesc, sizeof (depthStencilDesc));

    // Set up description
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    result = m_device->CreateDepthStencilState (&depthStencilDesc, &m_depthStencilState);
    CHECK_FAILED (result);

    // Set depth stencil state
    m_deviceContext->OMSetDepthStencilState (m_depthStencilState, 1);

    return true;
}

bool DXManager::InitializeStencilView ()
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    HRESULT result = S_OK;

    // Initilize description
    ZeroMemory (&depthStencilViewDesc, sizeof (depthStencilViewDesc));

    // Set up description
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create depth stencil view
    result = m_device->CreateDepthStencilView (m_depthStencilBuffer, &depthStencilViewDesc,
                                               &m_depthStencilView);
    CHECK_FAILED (result);
    
    return true;
}

bool DXManager::InitializeRasterizerState ()
{
    D3D11_RASTERIZER_DESC rasterDesc;
    HRESULT result = S_OK;

    // Set up description
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterize state
    result = m_device->CreateRasterizerState (&rasterDesc, &m_rasterState);
    CHECK_FAILED (result);

    // Set the rasterizes
    m_deviceContext->RSSetState (m_rasterState);

    return true;
}

void DXManager::InitializeViewport (int screenWidth, int screenHeight)
{
    D3D11_VIEWPORT viewport;

    // Setup
    viewport.Width = (float) screenWidth;
    viewport.Height = (float) screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // Create viewport
    m_deviceContext->RSSetViewports (1, &viewport);
}

bool DXManager::InitializeAlphaBlending ()
{
    D3D11_BLEND_DESC blendStateDesc;
    HRESULT result = S_OK;

    // Clear the blend state description
    ZeroMemory (&blendStateDesc, sizeof (blendStateDesc));

    // Initialize / clear description
    blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    // Create the enabled blend state
    result = m_device->CreateBlendState (&blendStateDesc, &m_alphaEnableBlendingState);
    CHECK_FAILED (result);

    // Modify to create disable alpha blend state
    blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

    // create the disabled state
    result = m_device->CreateBlendState (&blendStateDesc, &m_alphaDisableBlendingState);
    CHECK_FAILED (result);

    return true;
}

bool DXManager::InitializeZBuffer ()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    HRESULT result = S_OK;

    // Clear the description
    ZeroMemory (&depthStencilDesc, sizeof (depthStencilDesc));

    // Set up description
    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create disabled state
    result = m_device->CreateDepthStencilState (&depthStencilDesc, &m_depthDisabledStencilState);
    CHECK_FAILED (result);

    return true;
}