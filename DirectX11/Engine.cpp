#include "Engine.h"

Engine *Engine::m_Instance = nullptr;

Engine::Engine () :
    m_Graphics (nullptr)
{}

Engine::~Engine ()
{
    if (m_Graphics != nullptr)
    {
        delete m_Graphics;
        m_Graphics = nullptr;
    }
}

bool Engine::InitializeGraphics (HWND hWnd)
{
    m_Graphics = new Graphics ();

    return m_Graphics->InitializeDX (hWnd);
}

bool Engine::Initialize (HINSTANCE hInstance, HWND hWnd)
{
    m_Graphics->Initialize ();

    return true;
}

void Engine::Run ()
{
    Update ();
    Render ();
}

void Engine::Update ()
{}

void Engine::Render ()
{
    m_Graphics->BeginScene (0, 0, 0, 1);

    // Render stuff goes here

    m_Graphics->EndScene ();
}

Graphics *Engine::GetGraphics ()
{
    return m_Graphics;
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