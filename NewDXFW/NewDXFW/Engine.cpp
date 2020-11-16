#include <cassert>

#include "Engine.h"
#include "DebugFunc.h"

struct ConstantBuffer
{
    XMMATRIX mWorld;        // Матрица мира
    XMMATRIX mView;         // Матрица вида
    XMMATRIX mProjection;   // Матрица проекции

    ConstantBuffer (XMMATRIX mWorld, XMMATRIX mView, XMMATRIX mProjection) :
        mWorld (mWorld),
        mView (mView),
        mProjection (mProjection)
    {}
};

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
    m_device = m_graphics->GetDevice ();
    m_deviceContext = m_graphics->GetDeviceContext ();

    return true;
}

bool Engine::Initialize (HINSTANCE hInstance, HWND hWnd)
{
    HRESULT result = S_OK;

    auto vertexShader = new VertexShader ();
    if (!vertexShader->Initialize (m_device, hWnd, "Shader\\texture.fx", "VS",
                                   VertexPosTex::GetLayout (),
                                   VertexPosTex::GetNumElements ()))            RETURN_FALSE;

    auto pixelShader = new PixelShader ();
    if (!pixelShader->Initialize (m_device, hWnd, "Shader\\texture.fx", "PS"))  RETURN_FALSE;

    m_shader = new Shader (vertexShader, pixelShader);
    
    m_texture = new Texture ();
    if (!m_texture->Initialize (m_device, "Texture\\Plazma.jpg"))     RETURN_FALSE;

    VertexPosTex vert[4] = {{XMFLOAT3 (-0.5f,  0.5f, 0.5f), XMFLOAT2 (0.0f, 0.0f)},
                            {XMFLOAT3 ( 0.5f,  0.5f, 0.5f), XMFLOAT2 (1.0f, 0.0f)},
                            {XMFLOAT3 ( 0.5f, -0.5f, 0.5f), XMFLOAT2 (1.0f, 1.0f)},
                            {XMFLOAT3 (-0.5f, -0.5f, 0.5f), XMFLOAT2 (0.0f, 1.0f)}};
    
    printf ("size: %d\n", sizeof (VertexPosTex));
    fflush (stdout);

    /*
    SimpleVertex vert[4] = {{XMFLOAT3 (-0.5f,  0.5f, 0.5f)},
                            {XMFLOAT3 ( 0.5f,  0.5f, 0.5f)},
                            {XMFLOAT3 ( 0.5f, -0.5f, 0.5f)},
                            {XMFLOAT3 (-0.5f, -0.5f, 0.5f)}};
    */
    char indeces[6] = {0, 1, 2,
                       0, 2, 3};

    m_vertexBuffer = new VertexBuffer <VertexPosTex, char> ();
    m_vertexBuffer->Initialize (m_device, vert, 4, indeces, 6);
    /*
    D3D11_BUFFER_DESC bufDesc = {};
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.ByteWidth = sizeof (ConstantBuffer);    // размер буфера = размеру структуры
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // тип - константный буфер
    bufDesc.CPUAccessFlags = 0;
    result = m_device->CreateBuffer (&bufDesc, NULL, &m_CBMatrixes);
    CHECK_FAILED (result);

    XMMATRIX g_World = XMMatrixIdentity ();

    XMVECTOR Eye = XMVectorSet (0.0f, 1.0f, -5.0f, 0.0f);
    XMVECTOR At = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX g_View = XMMatrixLookAtLH (Eye, At, Up);

    XMMATRIX g_Projection = XMMatrixPerspectiveFovLH (XM_PIDIV4, 16.0f / 9.0f, 0.01f, 100.0f);

    ConstantBuffer cb (g_World, g_View, g_Projection);

    m_deviceContext->UpdateSubresource (m_CBMatrixes, 0, nullptr, &cb, 0, 0);
    m_deviceContext->VSSetConstantBuffers (0, 1, &m_CBMatrixes);

    m_texture = new Texture ();
    if (!m_texture->Initialize (m_device, "Texture\\Plazma.jpg"))     RETURN_FALSE;

    m_rect = new RectTex (0, 0, 0, 0, m_shader, m_texture, m_CBMatrixes);
    */
    /*
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof (SimpleVertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vert;

    m_device->CreateBuffer (&bd, &InitData, &g_pVertexBuffer);

    UINT stride = sizeof (SimpleVertex);
    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers (0, 1, &g_pVertexBuffer, &stride, &offset);
    m_deviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    */

    /*
    auto device = DXManager::GetDevice ();
    auto deviceContext = DXManager::GetDeviceContext ();

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

    m_shader = new Shader (vertexShader, pixelShader);

    SimpleVertex vertices[3];

    vertices[0].Pos.x =  0.0f;  vertices[0].Pos.y =  0.5f;  vertices[0].Pos.z = 0.5f;
    vertices[1].Pos.x =  0.5f;  vertices[1].Pos.y = -0.5f;  vertices[1].Pos.z = 0.5f;
    vertices[2].Pos.x = -0.5f;  vertices[2].Pos.y = -0.5f;  vertices[2].Pos.z = 0.5f;

    D3D11_BUFFER_DESC bd;  // Структура, описывающая создаваемый буфер

    ZeroMemory (&bd, sizeof (bd));                    // очищаем ее
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof (SimpleVertex) * 3; // размер буфера = размер одной вершины * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;          // тип буфера - буфер вершин
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData; // Структура, содержащая данные буфера
    ZeroMemory (&InitData, sizeof (InitData)); // очищаем ее
    InitData.pSysMem = vertices;               // указатель на наши 3 вершины

    // Вызов метода g_pd3dDevice создаст объект буфера вершин ID3D11Buffer
    device->CreateBuffer (&bd, &InitData, &m_pVertexBuffer);

    UINT stride = sizeof (SimpleVertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers (0, 1, &m_pVertexBuffer, &stride, &offset);
    deviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/
    /*
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
    */
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

    m_shader->Render (m_deviceContext);
    m_vertexBuffer->Render (m_deviceContext);

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