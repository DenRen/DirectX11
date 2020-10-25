#pragma once

#include "Shader.h"

class TextureShader : public Shader
{
public:
	TextureShader (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName,
				   LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	virtual ~TextureShader ();

	void Begin (ID3D11DeviceContext *deviceContext, int indexContext);
	void End (ID3D11DeviceContext *deviceContext);

protected:
	bool Initialize (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName,
					 LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	bool InitializeSamplerState (ID3D11Device *device);
private:
	ID3D11SamplerState *m_samplerState;
};

