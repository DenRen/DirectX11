#pragma once

#include <map>

#include "Shaders.h"
#include "Texture.h"

class ResMgr
{
public:

	ResMgr ();

	bool Initialize (ID3D11Device *device, HWND hWnd);

	Texture		 *LoadTexture (const char *path);
	VertexShader *LoadVertexShader (const char *path, const char *funcName,
									D3D11_INPUT_ELEMENT_DESC *layout, int numElements);
	PixelShader  *LoadPixelShader  (const char *path, const char *funcName);

	Texture		 *GetTexture (const char *path);
	VertexShader *GetVertexShader (const char *path, const char *funcName);
	PixelShader  *GetPixelShader  (const char *path, const char *funcName);


	static ResMgr *GetResMgr ();

private:
	ID3D11Device *m_device;
	HWND m_hWnd;

	std::map <std::string, Texture *> m_textures;
	std::map <std::pair <std::string, std::string>, PixelShader  *> m_pixelShaders;
	std::map <std::pair <std::string, std::string>, VertexShader *> m_vertexShaders;
};

