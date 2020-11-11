#include <cassert>

#include "ResourceManager.h"
#include "DebugFuncs.h"

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::~ResourceManager ()
{
	const int quantityTextures = m_texture.size ();
	for (int i = 0; i < quantityTextures; i++)
		if (m_texture[i] != nullptr)
			delete m_texture[i];
		
	const int quantityPixelShaders = m_pixelShader.size ();
	for (int i = 0; i < quantityPixelShaders; i++)
		if (m_pixelShader[i] != nullptr)
			delete m_pixelShader[i];

	const int quantityVertexShaders = m_vertexShader.size ();
	for (int i = 0; i < quantityVertexShaders; i++)
		if (m_vertexShader[i] != nullptr)
			delete m_vertexShader[i];
}

bool ResourceManager::AddVertexShader (ID3D11Device *device, HWND hWnd, LPCSTR fileName, LPCSTR funcName,
									   VertexShader **vertexShader)
{
	CHECK_NULLPTR (vertexShader);
	*vertexShader = new VertexShader ();
	if (!(*vertexShader)->Initialize (device, hWnd, fileName, funcName))
	{
		delete vertexShader;

		RETURN_FALSE;
	}

	m_vertexShader.push_back (*vertexShader);

#ifdef TURN_DEBUG
	printf ("Vertex shader %s loaded\n", fileName);
#endif

	return true;
}

bool ResourceManager::AddVertexShader (VertexShader *vertexShader)
{
	if (vertexShader == nullptr)
	{
		RETURN_FALSE;
	}

	m_vertexShader.push_back (vertexShader);

	return true;
}

bool ResourceManager::AddPixelShader (ID3D11Device *device, HWND hWnd, LPCSTR fileName, LPCSTR funcName,
									  PixelShader **pixelShader)
{
	CHECK_NULLPTR (pixelShader);
	*pixelShader = new PixelShader ();
	if (!(*pixelShader)->Initialize (device, hWnd, fileName, funcName))
	{
		delete pixelShader;

		RETURN_FALSE;
	}

	m_pixelShader.push_back (*pixelShader);

#ifdef TURN_DEBUG
	printf ("Pixel shader %s loaded\n", fileName);
#endif

	return true;
}

bool ResourceManager::AddPixelShader (PixelShader *pixelShader)
{
	if (pixelShader == nullptr)
	{
		RETURN_FALSE;
	}

	m_pixelShader.push_back (pixelShader);

	return true;
}

bool ResourceManager::AddTexture (ID3D11Device *device, LPCSTR textureFileName, Texture **texture)
{
	CHECK_NULLPTR (texture);

	*texture = new Texture ();
	if (!(*texture)->Initialize (device, textureFileName))
	{
		delete texture;

		RETURN_FALSE;
	}

	m_texture.push_back (*texture);
	m_textureFileName.push_back (textureFileName);

#ifdef TURN_DEBUG
	printf ("Texture %s loaded\n", textureFileName);
#endif

	return true;
}

bool ResourceManager::AddTexture (Texture *texture, LPCSTR textureFileName)
{
	if (texture == nullptr)
	{
		RETURN_FALSE;
	}

	m_texture.push_back (texture);
	m_textureFileName.push_back (textureFileName);

	return true;
}

PixelShader *ResourceManager::GetPixelShader (int number)
{
	return m_pixelShader[number];
}

VertexShader *ResourceManager::GetVertexShader (int number)
{
	return m_vertexShader[number];
}

Texture *ResourceManager::GetTexture (int number)
{
	return m_texture[number];
}


Texture *ResourceManager::GetTextureByName (LPCSTR textureFileName)
{
	if (textureFileName == nullptr)
	{
		return nullptr;
	}

	const int quantityTextures = m_textureFileName.size ();
	for (int i = 0; i < quantityTextures; i++)
	{
		if (strcmp (textureFileName, m_textureFileName[i]) == 0)
		{
			return m_texture[i];
		}
	}

	return nullptr;
}

void ResourceManager::Init ()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager ();
	}
}

ResourceManager *ResourceManager::GetInstance ()
{
	assert (m_instance);

	return m_instance;
}