#include "Engine.h"
#include "AddFunc.h"

Engine *Engine::m_Instance = nullptr;

Engine::Engine () :
    m_graphics (nullptr),
    vertexBuffer (nullptr),
    shader (nullptr),
    texture (nullptr),
    m_resourceManager (nullptr)
{}

Engine::~Engine ()
{
    if (m_graphics != nullptr)
    {
        delete m_graphics;
    }

    if (m_sprite != nullptr)
    {
        delete m_sprite;
    }

    if (m_resourceManager == nullptr)
    {
        delete m_resourceManager;
    }

    delete vertexBuffer;
    delete shader;
    delete texture;
}

bool Engine::InitializeGraphics (HWND hWnd)
{
    m_graphics = new Graphics ();

    return m_graphics->InitializeDX (hWnd);
}

bool Engine::Initialize (HINSTANCE hInstance, HWND hWnd)
{
    m_graphics->Initialize ();

    m_resourceManager = ResourceManager::GetInstance ();
    m_resourceManager->LoadTextureResource (m_graphics->GetDevice (), "Texture\\ninja.png");
    m_resourceManager->LoadShaderResource (m_graphics->GetDevice (), hWnd, "Shader\\texture",
                                           "TextureVertexShader", "TexturePixelShader");

    shader = (TextureShader *) m_resourceManager->GetShaderByName ("Shader\\texture");

    /*shader = new TextureShader (m_graphics->GetDevice (), hWnd, "Shader\\texture",
                                "TextureVertexShader", "TexturePixelShader");
    if (!shader->IsInitialized ())
    {
        RETURN_FALSE;
    }*/

    m_animSprite = new AnimatedSprite (320.0f, 25.0f);
    m_animSprite->Initialize (m_graphics->GetDevice (), m_graphics->GetDeviceContext (), 
                              shader, shader->GetName ().c_str ());

    /*
    m_sprite = new Sprite (3200.0f);
    m_sprite->Initialize (m_graphics->GetDevice (), shader, "Texture\\sonic.png");
    */
    /*
    shader = new TextureShader (m_graphics->GetDevice (), hWnd, "Shader\\texture",
                                       "TextureVertexShader", "TexturePixelShader");
    if (!shader->IsInitialized ())
    {
        RETURN_FALSE;
    }

    texture = new Texture ();
    bool result = texture->Initialize (m_graphics->GetDevice (), "Texture\\sonic.png");
    if (!result)
    {
        RETURN_FALSE;
    }

    vertexBuffer = new VertexBuffer ();
    result = vertexBuffer->Initialize (m_graphics->GetDevice (), shader, 3200.0f, false);
    if (!result)
    {
        RETURN_FALSE;
    }
    */
    return true;
}

void Engine::Run ()
{
    Update ();
    Render ();
}

void Engine::Update ()
{
    m_animSprite->Update ();
}

void Engine::Render ()
{
    m_graphics->BeginScene (0, 0, 0, 1);

    // Render stuff goes here

    D3DXMATRIX viewMatrix;
    D3DXMATRIX projectionMatrix;
    D3DXMATRIX worldMatrix;

    D3DXVECTOR3 position (0, 0, -200.0f);
    D3DXVECTOR3 up (0, 1.0f, 0.0f);
    D3DXVECTOR3 lookAt (0.0f, 0.0f, 1.0f);

    D3DXMatrixLookAtLH (&viewMatrix, &position, &lookAt, &up);
    D3DXMatrixOrthoLH (&projectionMatrix, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.0f);
    D3DXMatrixIdentity (&worldMatrix);
    
    m_animSprite->Render (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);
    
    /*
    m_sprite->Render (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);

    m_graphics->EnableAlphaBlending (true);*/
    /*
    m_graphics->EnableAlphaBlending (true);

    shader->SetShaderParameters (m_graphics->GetDeviceContext (), texture->GetTexture ());
    shader->SetShaderParameters (m_graphics->GetDeviceContext (), worldMatrix, viewMatrix, projectionMatrix);

    vertexBuffer->Render (m_graphics->GetDeviceContext ());
    */
    /*
    m_graphics->EndScene ();*/

    m_graphics->EndScene ();
}

Graphics *Engine::GetGraphics ()
{
    return m_graphics;
}

Engine *Engine::GetEngine ()
{
    if (m_Instance == nullptr)
    {
        m_Instance = new Engine ();
    }

    return m_Instance;
}

void Engine::Release ()
{
    if (m_Instance != nullptr)
    {
        delete m_Instance;
        m_Instance = nullptr;
    }
}