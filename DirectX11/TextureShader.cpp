#include "TextureShader.h"
#include "AddFunc.h"

TextureShader::TextureShader (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName) :
	Shader (device, hWnd, shaderFileName, vertexFuncName, pixelFuncName)
{}

TextureShader::~TextureShader ()
{
	if (m_samplerState != nullptr)
	{
		RELEASE (m_samplerState);
	}
}

bool TextureShader::Initialize (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	if (!Shader::Initialize (device, hWnd, shaderFileName, vertexFuncName, pixelFuncName))
	{
		RETURN_FALSE;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT result = S_OK;

	ZeroMemory (&samplerDesc, sizeof (samplerDesc));

	// Create texture sampler description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create sampler state
	result = device->CreateSamplerState (&samplerDesc, &m_samplerState);
	CHECK_FAILED (result);
}

void TextureShader::Begin (ID3D11DeviceContext * deviceContext, int indexContext)
{
	deviceContext->PSSetSamplers (0, 1, &m_samplerState);
	Shader::Begin (deviceContext, indexContext);
}

void TextureShader::End (ID3D11DeviceContext *deviceContext)
{
	deviceContext->PSSetSamplers (0, 0, nullptr);
	Shader::End (deviceContext);
}