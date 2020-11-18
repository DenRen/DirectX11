#include "ResourceManager.h"
#include "DebugFunc.h"

ResMgr::ResMgr () :
    m_device (nullptr),
    m_hWnd (nullptr)
{}

bool ResMgr::Initialize (ID3D11Device *device, HWND hWnd)
{
    if (device != nullptr && hWnd != nullptr)
    {
        m_device = device;
        m_hWnd = hWnd;
        
        return true;
    }

    return false;
}

Texture *ResMgr::LoadTexture (const char *path)
{
    std::string key = path;

    auto iter = m_textures.find (key);
    if (iter != m_textures.end ())
    {
        return iter->second;
    }

    Texture *newTexture = new Texture ();

    if (!newTexture->Initialize (m_device, path))
    {
        RETURN_THROW;
    }
    
    return m_textures[key] = newTexture;
}

VertexShader *ResMgr::LoadVertexShader (const char *path, const char *funcName,
                                        D3D11_INPUT_ELEMENT_DESC *layout, int numElements)
{
    auto key = std::make_pair (std::string (path), std::string (funcName));

    auto iter = m_vertexShaders.find (key);
    if (iter != m_vertexShaders.end ())
    {
        return iter->second;
    }

    auto *newVertexShader = new VertexShader ();

    if (!newVertexShader->Initialize (m_device, m_hWnd, path, funcName,
                                      layout, numElements))
    {
        RETURN_THROW;
    }

    return m_vertexShaders[key] = newVertexShader;
}

PixelShader *ResMgr::LoadPixelShader (const char *path, const char *funcName)
{
    auto key = std::make_pair (std::string (path), std::string (funcName));

    auto iter = m_pixelShaders.find (key);
    if (iter != m_pixelShaders.end ())
    {
        return iter->second;
    }

   auto *newPixelShader = new PixelShader ();

    if (!newPixelShader->Initialize (m_device, m_hWnd, path, funcName))
    {
        RETURN_THROW;
    }

    return m_pixelShaders[key] = newPixelShader;
}

Texture *ResMgr::GetTexture (const char *path)
{
    auto iter = m_textures.find (std::string (path));
    
    if (iter == m_textures.end ())
    {
        return nullptr;
    }

    return iter->second;
}

VertexShader *ResMgr::GetVertexShader (const char *path, const char *funcName)
{
    auto key = std::make_pair (std::string (path), std::string (funcName));
    auto iter = m_vertexShaders.find (key);

    if (iter == m_vertexShaders.end ())
    {
        return nullptr;
    }

    return iter->second;
}

PixelShader *ResMgr::GetPixelShader (const char *path, const char *funcName)
{
    auto key = std::make_pair (std::string (path), std::string (funcName));
    auto iter = m_pixelShaders.find (key);

    if (iter == m_pixelShaders.end ())
    {
        return nullptr;
    }

    return iter->second;
}

ResMgr *ResMgr::GetResMgr ()
{
    static ResMgr resMgr;
    return &resMgr;
}
