#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include <string>

class Shader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};

public:
	Shader (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName, 
			LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	
	virtual ~Shader ();

	virtual void Begin (ID3D11DeviceContext *deviceContext, int indexContext);
	virtual void End (ID3D11DeviceContext *deviceContext);

	bool SetShaderParameters (ID3D11DeviceContext *deviceContext, ID3D11ShaderResourceView *texture);
	bool SetShaderParameters (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix, 
							  D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	std::string GetName ();
	bool IsInitialized ();

protected:
	virtual bool Initialize (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName, 
							 LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	bool m_initialized;

private:
	bool InitializeShader (ID3D11Device *device, HWND hWnd, LPCSTR vsFileName, LPCSTR psFileName,
						   LPCSTR vertexFuncName, LPCSTR pixelDuncName);
	void OutputShaderErrorMessage (ID3D10Blob *errorMessage, HWND hWnd, LPCSTR shaderFileName);

	ID3D11VertexShader *m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3D11InputLayout *m_layout;
	ID3D11Buffer *m_matrixBuffer;
	std::string m_name;
};