#pragma once

#include <D3DX11async.h>
#include <fstream>

#include "InputLayout.h"

class VertexShader
{
	friend class Shader;

public:
	VertexShader ();
	~VertexShader ();
	
	bool Initialize (ID3D11Device *device, HWND hWnd,
					 LPCSTR fileName, LPCSTR funcName,
					 D3D11_INPUT_ELEMENT_DESC *layout, int numElementes);

	ID3D11VertexShader *GetVertexShader ();
	ID3D11InputLayout  *GetInputLayout ();

private:
	ID3D11VertexShader *m_vertexShader;
	ID3D11InputLayout  *m_inputLayout;
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