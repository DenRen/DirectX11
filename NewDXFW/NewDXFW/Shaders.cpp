#include <d3dcompiler.h>

#include "Shaders.h"
#include "DebugFunc.h"

HRESULT CompileShaderFromFile (LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
							   ID3DBlob **ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob *pErrorBlob = nullptr;

	hr = D3DX11CompileFromFile (szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
								dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
	if (FAILED (hr))
	{
		if (pErrorBlob != nullptr)
		{
			OutputDebugStringA ((char *) pErrorBlob->GetBufferPointer ());
		}
		else
		{
			RELEASE (pErrorBlob);
		}

		return hr;
	}

	RELEASE (pErrorBlob);

	return S_OK;
}

void OutputShaderErrorMessage (ID3D10Blob *errorMessage, HWND hWnd, LPCSTR shaderFileName)
{
	char *compileErrors = (char *) errorMessage->GetBufferPointer ();
	unsigned long bufferSize = errorMessage->GetBufferSize ();

	std::ofstream fout;
	fout.open ("shader_error.txt");
	fout.write (compileErrors, bufferSize);
	fout.close ();

	RELEASE (errorMessage);

	MessageBox (hWnd, "Error compiling shader. Check shader-error.txt for message", shaderFileName, MB_OK);

	return;
}

// ---------------------------------------------------------------------------------------

VertexShader::VertexShader () :
	m_vertexShader (nullptr),
	m_inputLayout  (nullptr)
{}

VertexShader::~VertexShader ()
{
	RELEASE (m_vertexShader);
	RELEASE (m_inputLayout);
}

bool VertexShader::Initialize (ID3D11Device *device, HWND hWnd,
							   LPCSTR fileName, LPCSTR funcName,
							   D3D11_INPUT_ELEMENT_DESC *layout, int numElements)
{
	HRESULT result = S_OK;
	ID3DBlob *errorMessage = nullptr;
	ID3DBlob *vertexShaderBuffer = nullptr;

	result = CompileShaderFromFile (fileName, funcName, "vs_5_0", &vertexShaderBuffer);
	if (FAILED (result))
	{
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage (errorMessage, hWnd, fileName);
		}

		RETURN_FALSE;
	}
	
	result = device->CreateVertexShader (vertexShaderBuffer->GetBufferPointer (),
										 vertexShaderBuffer->GetBufferSize (), NULL,
										 &m_vertexShader);
	CHECK_FAILED (result);

	result = device->CreateInputLayout (layout, numElements,
										vertexShaderBuffer->GetBufferPointer (),
										vertexShaderBuffer->GetBufferSize (),
										&m_inputLayout);
	vertexShaderBuffer->Release ();
	CHECK_FAILED (result);
	
	return true;
}

ID3D11VertexShader *VertexShader::GetVertexShader ()
{
	return m_vertexShader;
}

ID3D11InputLayout *VertexShader::GetInputLayout ()
{
	return m_inputLayout;
}

// ---------------------------------------------------------------------------------------

PixelShader::PixelShader () :
	m_pixelShader (nullptr)
{}

bool PixelShader::Initialize (ID3D11Device *device, HWND hWnd,
							  LPCSTR fileName, LPCSTR funcName)
{
	HRESULT result = S_OK;
	ID3DBlob *errorMessage = nullptr;
	ID3DBlob *pixelShaderBuffer = nullptr;

	result = CompileShaderFromFile (fileName, funcName, "ps_5_0", &pixelShaderBuffer);
	if (FAILED (result))
	{
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage (errorMessage, hWnd, fileName);
		}

		RETURN_FALSE;
	}

	result = device->CreatePixelShader (pixelShaderBuffer->GetBufferPointer (),
										pixelShaderBuffer->GetBufferSize (), NULL,
										&m_pixelShader);
	CHECK_FAILED (result);

	RELEASE (pixelShaderBuffer);

	return true;
}

ID3D11PixelShader *PixelShader::GetPixelShader ()
{
	return m_pixelShader;
}