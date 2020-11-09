#pragma once

#include <vector>
#include "ResourceTexture.h"
#include "ResourceShader.h"

class ResourceManager
{
public:
	~ResourceManager ();

	void LoadShaderResource (ID3D11Device *device, HWND hWnd, const char *shaderFileName,
							 const char *vertexFuncName, const char *pixelFuncName);
	void LoadShaderResource (Shader *shader);
	void LoadTextureResource (ID3D11Device *device, const char *textureFileName);

	Shader *GetShaderByName (const char *shaderName);
	Texture *GetTextureByName (const char *textureName);

	static ResourceManager *GetInstance ();

private:
	ResourceManager () = default;

	std::vector <ResourceTexture *> m_texture;
	std::vector <ResourceShader *> m_shader;

	static ResourceManager *m_instance;
};

