#include <cassert>
// _XM_NO_INTRINSICS_
#include "Engine.h"
#include "DebugFunc.h"
#include "Config.h"

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

bool Engine::InitializeGraphics (HWND hWnd, HINSTANCE hInstance)
{
    m_graphics = new Graphics ();

    if (!m_graphics->InitializeDX (hWnd))
    {
        RETURN_FALSE;
    }
    
    m_hWnd = hWnd;
    m_hInstance = hInstance;

    m_graphics->SethWnd (hWnd);
    
    m_device = m_graphics->GetDevice ();
    m_deviceContext = m_graphics->GetDeviceContext ();

    return true;
}

bool Engine::Initialize (HINSTANCE hInstance, HWND hWnd)
{
    HRESULT result = S_OK;

    m_input = new Input ();
   // if (!m_input->Initialize (m_hInstance, m_hWnd, WndCnf::WIDTH, WndCnf::HEIGHT)) RETURN_FALSE;

    auto vertexShader = new VertexShader ();
    if (!vertexShader->Initialize (m_device, hWnd, "Shader\\texture.fx", "VS",
                                   VertexPosTex::GetLayout (),
                                   VertexPosTex::GetNumElements ()))            RETURN_FALSE;

    auto pixelShader = new PixelShader ();
    if (!pixelShader->Initialize (m_device, hWnd, "Shader\\texture.fx", "PS"))  RETURN_FALSE;

    m_shader = new Shader (vertexShader, pixelShader);
    
    m_texture = new Texture ();
    if (!m_texture->Initialize (m_device, "Texture\\Plazma.jpg"))     RETURN_FALSE;

    /*
    VertexPosTex vert[4] = {{XMFLOAT3 (-0.5f,  0.5f, 0.5f), XMFLOAT2 (0.0f, 0.0f)},
                            {XMFLOAT3 ( 0.5f,  0.5f, 0.5f), XMFLOAT2 (1.0f, 0.0f)},
                            {XMFLOAT3 ( 0.5f, -0.5f, 0.5f), XMFLOAT2 (1.0f, 1.0f)},
                            {XMFLOAT3 (-0.5f, -0.5f, 0.5f), XMFLOAT2 (0.0f, 1.0f)}};
    
    char indeces[6] = {0, 1, 2,
                       0, 2, 3};

    m_vertexBuffer = new VertexBuffer <VertexPosTex, char> ();
    m_vertexBuffer->Initialize (m_device, vert, 4, indeces, 6);
   */
    // -----------
    
    if (!CreateConstatntBufferMatrixes (m_device, &m_CBWVPMatrixes))   RETURN_FALSE;

    XMVECTOR Eye = XMVectorSet (0.0f, 0.0f, -2.0f, 0.0f);
    XMVECTOR At  = XMVectorSet (0.0f, 0.0f,  1.0f, 0.0f);
    XMVECTOR Up  = XMVectorSet (0.0f, 1.0f,  0.0f, 0.0f);
    m_WVPMatrixes.m_View = XMMatrixLookAtLH (Eye, At, Up);

    m_WVPMatrixes.m_Projection = XMMatrixPerspectiveFovLH (XM_PIDIV4, 16.0f / 9.0f, 0.01f, 100.0f);

    m_camera = new Camera (m_WVPMatrixes.m_View, m_WVPMatrixes.m_Projection,
                           &m_WVPMatrixes, m_CBWVPMatrixes);
    
    RectTex::SetDefaultValue (m_texture, m_shader, m_CBWVPMatrixes, &m_WVPMatrixes);

    m_rect = new RectTex (0.5, 0.3, 0.7, 0.2);

    return true;
}

void Engine::Run ()
{
    Update ();  // timer
    Render ();
}

void Engine::Update ()
{/*
    if (!m_input->Update ())
    {
        DUMP_DEBUG_INFO;
        DebugEndMain ();
        throw std::runtime_error ("");
    }*/
    m_timer->updateTimer ();
}

void Engine::Render ()
{
    int mouseX = 0, mouseY = 0;
    //m_input->GetMousePosition (mouseX, mouseY);
    //printf ("%d %d\n", mouseX, mouseY);

    static float dt = 0;
    dt = m_timer->getTimeInterval ();

    m_camera->Render (m_deviceContext);

    //m_WVPMatrixes.UpdateSubresource (m_deviceContext, m_CBWVPMatrixes);

    m_graphics->BeginScene (0, 0, 0, 1);

    m_rect->RotateY (1 * dt);
    m_rect->RotateX (2 * dt);
    m_rect->RotateZ (3 * dt);
    m_rect->Draw ();

    //m_shader->Render (m_deviceContext);
    //m_deviceContext->VSSetConstantBuffers (0, 1, &m_CBWVPMatrixes);
    //m_texture->Render (m_deviceContext);
    //m_vertexBuffer->Render (m_deviceContext);

    /*
    auto devCont = DXManager::GetDeviceContext ();

    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал
    devCont->ClearRenderTargetView (DXManager::m_renderTargetView, ClearColor);

    devCont->VSSetShader (m_shader->m_vertexShader->GetVertexShader (), NULL, 0);
    devCont->PSSetShader (m_shader->m_pixelShader->GetPixelShader (),   NULL, 0);
    
    devCont->Draw (3, 0);
    //m_rect->Draw ();
    
    m_graphics->EndScene ();
    */
    /*
    m_sprite->Render (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);

    m_graphics->EnableAlphaBlending (true);

    m_graphics->EnableAlphaBlending (true);

    shader->SetShaderParameters (m_graphics->GetDeviceContext (), texture->GetTexture ());
    shader->SetShaderParameters (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);

    vertexBuffer->Render (m_graphics->GetDeviceContext ());
    */

    m_graphics->EndScene ();
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