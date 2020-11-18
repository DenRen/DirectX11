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

    //m_input = new Input ();
    // if (!m_input->Initialize (m_hInstance, m_hWnd, WndCnf::WIDTH, WndCnf::HEIGHT)) RETURN_FALSE;

    auto resMgr = ResMgr::GetResMgr ();

    if (!resMgr->Initialize (m_device, m_hWnd)) RETURN_FALSE;
   
    LoadResources ();

    if (!CreateConstatntBufferMatrixes (m_device, &m_CBWVPMatrixes)) RETURN_FALSE;

    if (!InitializeCamera ())                                        RETURN_FALSE;
    
    // Initialize default values -------------------------------------------------------------

    auto vertexShader = resMgr->GetVertexShader ("Shader\\texture.fx", "VS");
    auto pixelShader  = resMgr->GetPixelShader  ("Shader\\texture.fx", "PS");

    m_shader = new Shader (vertexShader, pixelShader);

    m_texture = resMgr->GetTexture ("Texture\\metall.dds");

    RectTex::SetDefaultValue (m_texture, m_shader, m_CBWVPMatrixes, &m_WVPMatrixes);

    m_rect  = new RectTex ( 0.0, 0.3 * 9.0 / 16.0, 0.7, 0.2, "Texture\\ninja.png");

    m_rect1 = new RectTex (-0.5,       9.0 / 16.0, 0.7, 0.2, "Texture\\metall.dds");

    return true;
}

void Engine::Run ()
{
    Update ();  // timer
    Render ();
}

void Engine::LoadResources ()
{
    auto resMgr = ResMgr::GetResMgr ();

    resMgr->LoadVertexShader ("Shader\\texture.fx", "VS",
                              VertexPosTex::GetLayout (),
                              VertexPosTex::GetNumElements ());

    resMgr->LoadPixelShader ("Shader\\texture.fx", "PS");

    resMgr->LoadTexture ("Texture\\metall.dds");
    resMgr->LoadTexture ("Texture\\ninja.png");
}

bool Engine::InitializeCamera ()
{
    XMVECTOR Eye = XMVectorSet (0.0f, 0.0f, -2.0f / 1.5f, 0.0f);
    XMVECTOR At  = XMVectorSet (0.0f, 0.0f, 1.0f,         0.0f);
    XMVECTOR Up  = XMVectorSet (0.0f, 1.0f, 0.0f,         0.0f);
    m_WVPMatrixes.m_View = XMMatrixLookAtLH (Eye, At, Up);

    m_WVPMatrixes.m_Projection = XMMatrixPerspectiveFovLH (XM_PIDIV4, 16.0f / 9.0f, 0.01f, 100.0f);

    m_camera = new Camera (m_WVPMatrixes.m_View, m_WVPMatrixes.m_Projection,
                           &m_WVPMatrixes, m_CBWVPMatrixes);

    return true;
}

void Engine::Update ()
{
    m_timer->updateTimer ();
}

void Engine::Render ()
{

    static float dt = 0;
    dt = m_timer->getTimeInterval ();

    m_camera->Render (m_deviceContext);

    m_graphics->BeginScene (0, 0, 0, 1);

    m_rect->Draw ();
    m_rect1->Draw ();

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