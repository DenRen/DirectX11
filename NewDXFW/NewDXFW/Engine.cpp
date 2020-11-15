#include <cassert>

#include "Engine.h"
#include "DebugFunc.h"

deTimer *Engine::m_timer = new deTimer ();

Engine::Engine () :
    m_graphics          (nullptr),
    m_shader            (nullptr),
    m_texture           (nullptr),
    m_rect              (nullptr)
    //m_camera            (nullptr)
{}

Engine::~Engine ()
{
    delete m_graphics;
    delete m_shader;
    delete m_texture;
    delete m_rect;
}

bool Engine::InitializeGraphics (HWND hWnd)
{
    m_graphics = new Graphics ();

    if (!m_graphics->InitializeDX (hWnd))
    {
        RETURN_FALSE;
    }
    
    m_hWnd = hWnd;
    m_graphics->SethWnd (hWnd);

    return true;
}

bool Engine::Initialize (HINSTANCE hInstance, HWND hWnd)
{
    auto device = DXManager::GetDevice ();
    auto deviceContext = DXManager::GetDeviceContext ();

    // Init camera ---------------------------------------------------------------------------
    
    XMMATRIX mWorld = XMMatrixIdentity ();

    XMVECTOR Eye = XMVectorSet (0.0f, 0.0f, -5.0f, 0.0f);
    XMVECTOR At  = XMVectorSet (0.0f, 0.0f,  1.0f, 0.0f);
    XMVECTOR Up  = XMVectorSet (0.0f, 1.0f,  0.0f, 0.0f);
    XMMATRIX mView = XMMatrixLookAtLH (Eye, At, Up);

    XMMATRIX mProjection = XMMatrixPerspectiveFovLH (XM_PIDIV4, 16.0f / 9.0f, 0.01f, 100.0f);

    ConstantBufferMatrixes matrix (mWorld, mView, mProjection);

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof (ConstantBufferMatrixes);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = &matrix;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    HRESULT result = device->CreateBuffer (&bd, &initData, &m_CBMatrixes);
    CHECK_FAILED (result);

    deviceContext->VSSetConstantBuffers (0, 1, &m_CBMatrixes);
    deviceContext->PSSetConstantBuffers (0, 1, &m_CBMatrixes);

    //m_camera = new Camera (device, mWorld, mView, mProjection);
    
    // Init shaders and texture --------------------------------------------------------------

    auto vertexShader = new VertexShader ();
    if (!vertexShader->Initialize (device, hWnd, "Shader\\texture.fx", "VS",
                                   VertexPosTex::GetLayout (), VertexPosTex::GetNumElements ()))
    {
        RETURN_FALSE;
    }

    auto pixelShader = new PixelShader ();
    if (!pixelShader->Initialize (device, hWnd, "Shader\\texture.fx", "PS"))
    {
        RETURN_FALSE;
    }

    auto shader = new Shader (vertexShader, pixelShader);

    auto texture = new Texture ();
    if (!texture->Initialize (device, "Texture\\Plazma.jpg"))
    {
        RETURN_FALSE;
    }

    // Init rectangle ------------------------------------------------------------------------

    m_rect = new RectTex (-1.0f, 0.0f, 1.0f, 1.0f, shader, texture, m_CBMatrixes);
    
    return true;
}

void Engine::Run ()
{
    Update ();  // timer
    Render ();
}

void Engine::Update ()
{
    m_timer->updateTimer ();
}

void Engine::Render ()
{
    m_graphics->BeginScene (0, 0, 0, 1);

    m_rect->Draw ();
    
    m_graphics->EndScene ();

    /*
    m_sprite->Render (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);

    m_graphics->EnableAlphaBlending (true);

    m_graphics->EnableAlphaBlending (true);

    shader->SetShaderParameters (m_graphics->GetDeviceContext (), texture->GetTexture ());
    shader->SetShaderParameters (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);

    vertexBuffer->Render (m_graphics->GetDeviceContext ());

    m_graphics->EndScene ();*/
}

Graphics *Engine::GetGraphics ()
{
    return m_graphics;
}

Engine *Engine::GetEngine ()
{
    static Engine engine;
    return &engine;
}

double Engine::GetDeltaTime ()
{
    return m_timer->getTimeInterval ();
}