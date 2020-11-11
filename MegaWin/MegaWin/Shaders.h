#pragma once

#include <D3DX11async.h>
#include <fstream>

class VertexShader
{
	friend class Shader;

public:
	VertexShader ();

	bool Initialize (ID3D11Device *device, HWND hWnd,
					 LPCSTR fileName, LPCSTR funcName);

	ID3D11VertexShader *GetVertexShader ();

private:
	ID3D11VertexShader *m_vertexShader;
};

class PixelShader
{
	friend class Shader;

public:
	PixelShader ();

	bool Initialize (ID3D11Device *device, HWND hWnd,
					 LPCSTR fileName, LPCSTR funcName);

	ID3D11PixelShader *GetPixelShader ();

private:
	ID3D11PixelShader *m_pixelShader;
};
