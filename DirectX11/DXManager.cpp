#include "DXManager.h"
#include <cstdio>

#define RELEASE(dev)        \
    if (dev != nullptr)     \
    {                       \
        dev->Release ();    \
        dev = nullptr;      \
    }

#define CHECK_FAILED(hr)    \
    if (FAILED (hr))        \
    {                       \
        return false;       \
    }
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
    HRESULT result = 0;
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

                return false;
            }
        }
    }
    
    if (numerator == 0 && denominator == 0)
    {
        return false;
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

    return true;
}

void DXManager::BeginScene (float r, float g, float b, float a)
{

}

void DXManager::EndScene ()
{

}

void DXManager::EnableAplhaBlending (bool enable)
{

}

void DXManager::EnableZBuffer (bool enable)
{

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
    return false;
}

bool DXManager::InitializeDepthBuffer (int screenWidth, int screenHeight)
{
    return false;
}

bool DXManager::InitializeDepthStencilBuffer ()
{
    return false;
}

bool DXManager::InitializeStencilView ()
{
    return false;
}

bool DXManager::InitializeRasterizerState ()
{
    return false;
}

void DXManager::InitializeViewport (int screenWidth, int screenHeight)
{

}

bool DXManager::InitializeAlphaBlendig ()
{
    return false;
}

bool DXManager::InitializeZBuffer ()
{
    return false;
}