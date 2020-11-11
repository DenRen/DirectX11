#include "GameScene.h"
#include "TextureShader.h"
#include "DebugFuncs.h"

GameScene::GameScene () :
    m_player (nullptr)
{}

GameScene::~GameScene ()
{
    delete m_player;
}

bool GameScene::Initialize ()
{
    TextureShader *shader = (TextureShader *) ResourceManager::GetInstance ()->GetShaderByName ("Shader\\texture");
    if (shader == nullptr)
    {
        RETURN_FALSE;
    }

    m_player = new Player ();
    auto graphic = Engine::GetEngine ()->GetGraphics ();
    m_player->Initialize (graphic->GetDevice (), graphic->GetDeviceContext (), shader);

    return true;
}

void GameScene::Update ()
{
    m_player->Update ();
}

void GameScene::Render (ID3D11DeviceContext *deviceContext,
                        D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
    
}
