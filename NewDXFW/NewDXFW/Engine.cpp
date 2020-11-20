#include <cassert>
// _XM_NO_INTRINSICS_
#include "Engine.h"
#include "DebugFunc.h"
#include "Config.h"
#include "NewsQueue.h"

deTimer *Engine::m_timer = new deTimer ();

Engine::Engine () :
    m_graphics          (nullptr),
    m_shader            (nullptr),
    m_texture           (nullptr),
    m_rect              (nullptr),
    m_font              (nullptr),
    text1               (nullptr),
    text2               (nullptr),
    text3               (nullptr)
    //m_camera            (nullptr)
{}

Engine::~Engine ()
{
    delete m_graphics;
    delete m_shader;
    delete m_texture;
    delete m_rect;
}

bool Engine::IntializePrelaunchParams ()
{
    NewsQueue::SetCapacity (1024);
    
    return true;
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
 
    // ---------------------------------------------------------------------------------------

    HRESULT result = S_OK;

    //m_input = new InputDX ();
    // if (!m_input->Initialize (m_hInstance, m_hWnd, WndCnf::WIDTH, WndCnf::HEIGHT)) RETURN_FALSE;

    auto resMgr = ResMgr::GetResMgr ();
    if (!resMgr->Initialize (m_device, m_hWnd)) RETURN_FALSE;
   
    LoadResources ();

    if (!CreateConstatntBufferMatrixes (m_device, &m_CBWVPMatrixes)) RETURN_FALSE;

    if (!InitializeCamera ())                                        RETURN_FALSE;

    // Initialize default values -------------------------------------------------------------

    auto textureWidget = resMgr->GetTexture ("Texture\\H\\WidgetWait.png");
    auto vertexShader = resMgr->GetVertexShader ("Shader\\texture.fx", "VS");
    auto pixelShader  = resMgr->GetPixelShader  ("Shader\\texture.fx", "PS");

    m_shader = new Shader (vertexShader, pixelShader);

    RectTex::SetDefaultValue (textureWidget, m_shader, m_CBWVPMatrixes, &m_WVPMatrixes);

    // Initialize scene objects --------------------------------------------------------------

    m_windowManager = new WindowManager ();
    if (!m_windowManager->Initialize ())                               RETURN_FALSE;

    return true;
}

void Engine::Run ()
{
    Update ();
    Render ();
}

void Engine::LoadResources ()
{
    auto resMgr = ResMgr::GetResMgr ();


    resMgr->LoadVertexShader ("Shader\\texture.fx", "VS",
                              VertexPosTex::GetLayout (),
                              VertexPosTex::GetNumElements ());

    resMgr->LoadPixelShader ("Shader\\texture.fx", "PS");

    resMgr->LoadTexture ("Texture\\Desktop.png");


    // ----------------------------------------------------------------------

    resMgr->LoadTexture ("Texture\\H\\WidgetWait.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetFocused.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetClicked.png");

    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerArrowLeft_Wait.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerArrowLeft_Focused.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerArrowLeft_Clicked.png");

    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerArrowRight_Wait.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerArrowRight_Focused.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerArrowRight_Clicked.png");

    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerSlider_Wait.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerSlider_Focused.png");
    resMgr->LoadTexture ("Texture\\H\\WidgetScrollerSlider_Clicked.png");

    // ----------------------------------------------------------------------

    resMgr->LoadTexture ("Texture\\V\\WidgetWait.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetFocused.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetClicked.png");

    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerArrowLeft_Wait.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerArrowLeft_Focused.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerArrowLeft_Clicked.png");

    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerArrowRight_Wait.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerArrowRight_Focused.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerArrowRight_Clicked.png");

    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerSlider_Wait.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerSlider_Focused.png");
    resMgr->LoadTexture ("Texture\\V\\WidgetScrollerSlider_Clicked.png");

    // ----------------------------------------------------------------------

    resMgr->LoadTexture ("Texture\\WidgetDarkTheme_Wait.png");
    resMgr->LoadTexture ("Texture\\WidgetDarkTheme_Focused.png");
    resMgr->LoadTexture ("Texture\\WidgetDarkTheme_Clicked.png");
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
    m_windowManager->Update ();
}

void Engine::Render ()
{
    static float dt = 0;
    dt = m_timer->getTimeInterval ();

    m_camera->Render (m_deviceContext);

    m_graphics->BeginScene (0, 0, 0, 1);

    m_windowManager->Draw ();

    m_graphics->EnableAlphaBlending (true);

    //text1->Render (1.0, 1.0, 0.0, 0, 0);
    //text2->Render (1.0, 0.0, 1.0, 0.3, 0.4);
    //text3->Render (0.0, 1.0, 1.0, 100, 180);

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