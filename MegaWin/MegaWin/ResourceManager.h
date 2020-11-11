#pragma once

#include <vector>
#include "ResourceTexture.h"
#include "ResourceShader.h"
#include "Shaders.h"

class ResourceManager
{
public:
	~ResourceManager ();

	bool AddVertexShader (ID3D11Device *device, HWND hWnd, LPCSTR fileName, LPCSTR funcName,
						  VertexShader **vertexShader);
	bool AddVertexShader (VertexShader *vertexShader);
	
	bool AddPixelShader (ID3D11Device *device, HWND hWnd, LPCSTR fileName, LPCSTR funcName,
						 PixelShader **pixelShader);
	bool AddPixelShader (PixelShader  *pixelShader);
	
	bool AddTexture (ID3D11Device *device, LPCSTR textureFileName, Texture **texture);
	bool AddTexture (Texture *texture, LPCSTR textureFileName);

	PixelShader  *GetPixelShader  (int number);
	VertexShader *GetVertexShader (int number);
	Texture *GetTexture (int number);

	Texture *GetTextureByName (LPCSTR textureFileName);

	static void Init ();
	static ResourceManager *GetInstance ();

private:
	ResourceManager () = default;

	std::vector <Texture *> m_texture;
	std::vector <LPCSTR> m_textureFileName;

	std::vector <PixelShader *>  m_pixelShader;
	std::vector <VertexShader *> m_vertexShader;

	static ResourceManager *m_instance;
};

