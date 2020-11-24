#include <cassert>

#include "Engine.h"
#include "DebugFuncs.h"

deTimer *Engine::m_timer = new deTimer ();

Engine::Engine () :
    m_graphics          (nullptr),
    m_shader            (nullptr),
    m_texture           (nullptr),
    m_camera            (nullptr)
{}

Engine::~Engine ()
{
    delete m_graphics;
    delete m_shader;
    delete m_texture;
}

bool Engine::InitializeGraphics (HWND hWnd)
{
    m_graphics = new Graphics ();

    if (!m_graphics->InitializeDX (hWnd))
    {
        RETURN_FALSE;
    }
    
    m_graphics->SethWnd (hWnd);

    return true;
}

bool Engine::Initialize (HINSTANCE hInstance, HWND hWnd)
{
    m_graphics->Initialize ();

    auto device = m_graphics->GetDevice ();
    auto deviceContext = m_graphics->GetDeviceContext ();

    // Init camera ---------------------------------------------------------------------------
    
    XMMATRIX mWorld = XMMatrixIdentity ();

    XMVECTOR Eye = XMVectorSet (0.0f, 0.0f, -5.0f, 5.0f);
    XMVECTOR At  = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up  = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX mView = XMMatrixLookAtLH (Eye, At, Up);

    XMMATRIX mProjection = XMMatrixPerspectiveFovLH (XM_PIDIV4, 16.0f / 9.0f, 0.01f, 100.0f);

    m_camera = new Camera (device, mWorld, mView, mProjection);
    
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

    m_rect = new RectTex (device, deviceContext, -1.0f, 0.0f, 1, 1, shader, texture);

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
    
    auto deviceContext = m_graphics->GetDeviceContext ();

    //m_camera->Render (deviceContext);

    deviceContext->VSSetConstantBuffers (0, 1, &(m_camera->m_CBMatrixes));

    XMMATRIX mWorld = XMMatrixTranslation (0.5, 0, 0);
    mWorld *= XMMatrixRotationY (m_timer->getTimeInterval () / 100);

    XMVECTOR Eye = XMVectorSet (0.0f, 4.0f, -10.0f, 0.0f);
    XMVECTOR At  = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up  = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX mView = XMMatrixLookAtLH (Eye, At, Up);

    XMMATRIX mProjection = XMMatrixPerspectiveFovLH (XM_PIDIV4, 16.0f / 9.0f, 0.01f, 100.0f);

    ConstantBufferMatrixes cb (mWorld, mView, mProjection);
    deviceContext->UpdateSubresource (m_camera->m_CBMatrixes, 0, nullptr, 
                                      &cb, 0, 0);

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

double Engine::getDeltaTime ()
{
    return m_timer->getTimeInterval ();
}