#include "ResourceManager.h"
#include "AddFunc.h"

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::~ResourceManager ()
{
	const int quantityTextures = m_texture.size ();
	for (int i = 0; i < quantityTextures; i++)
		if (m_texture[i] != nullptr)
			delete m_texture[i];
		
	const int quantityShaders = m_shader.size ();
	for (int i = 0; i < quantityShaders; i++)
		if (m_shader[i] != nullptr)
			delete m_shader[i];
}

void ResourceManager::LoadShaderResource (ID3D11Device *device, HWND hWnd, const char *shaderFileName, const char *vertexFuncName, const char *pixelFuncName)
{

	ResourceShader *resourceShader = new ResourceShader ();
	if (!resourceShader->Load (device, hWnd, shaderFileName, vertexFuncName, pixelFuncName))
	{
		delete resourceShader;

		RETURN_VOID_FALSE;
	}
	
	m_shader.push_back (resourceShader);

#ifdef _MY_DEBUG
	printf ("Shader %s loaded\n", shaderFileName);
#endif
}

void ResourceManager::LoadShaderResource (Shader *shader)
{
	if (shader == nullptr)
	{
		RETURN_VOID_FALSE;
	}

	ResourceShader *resourceShader = new ResourceShader ();
	if (resourceShader->Load (shader))
	{
		delete resourceShader;

		RETURN_VOID_FALSE;
	}

	m_shader.push_back (resourceShader);

#ifdef _MY_DEBUG
	printf ("Shader %s loaded\n", shader->GetName ());
#endif
}

void ResourceManager::LoadTextureResource (ID3D11Device * device, const char *textureFileName)
{
	ResourceTexture *resourceTexture = new ResourceTexture ();
	if (!resourceTexture->Load (device, textureFileName))
	{
		delete resourceTexture;

		RETURN_VOID_FALSE;
	}

	m_texture.push_back (resourceTexture);

#ifdef _MY_DEBUG
	printf ("Texture %s loaded\n", textureFileName);
#endif
}

Shader *ResourceManager::GetShaderByName (const char *shaderName)
{
	const int quantityShaders = m_shader.size ();
	for (int i = 0; i < quantityShaders; i++)
	{
		std::string resourceShaderName = m_shader[i]->GetName ();
		if (!strcmp (resourceShaderName.c_str (), shaderName))
		{
			return m_shader[i]->GetShader ();
		}
	}

	return nullptr;
}

Texture *ResourceManager::GetTextureByName (const char *textureName)
{
	const int quantityTextures = m_texture.size ();
	for (int i = 0; i < quantityTextures; i++)
	{
		std::string resourceTextureName = m_texture[i]->GetName ();
		if (!strcmp (resourceTextureName.c_str (), textureName))
		{
			return m_texture[i]->GetTexture ();
		}
	}

	return nullptr;
}

ResourceManager *ResourceManager::GetInstance ()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager ();
	}

	return m_instance;
}
