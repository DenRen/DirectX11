#include "Shader.h"
#include "AddFunc.h"

Shader::Shader (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName) :
	m_initialized  (Initialize (device, hWnd, shaderFileName, vertexFuncName, pixelFuncName))
{
	return;
}

Shader::~Shader ()
{
	RELEASE (m_matrixBuffer);
	RELEASE (m_layout);
	RELEASE (m_pixelShader);
	RELEASE (m_vertexShader);

	if (m_name != nullptr)
	{
		delete [] m_name;
		m_name = nullptr;
	}
}

void Shader::Begin (ID3D11DeviceContext *deviceContext, int indexContext)
{
	// Set the vertex input layout
	deviceContext->IASetInputLayout (m_layout);

	// Set the vertex and pixel shaders that will used to render
	deviceContext->VSSetShader (m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader (m_pixelShader, nullptr, 0);

	// Render
	deviceContext->DrawIndexed (indexContext, 0, 0);

}

void Shader::End (ID3D11DeviceContext *deviceContext)
{
	deviceContext->IASetInputLayout (nullptr);
	deviceContext->VSSetShader (nullptr, nullptr, 0);
	deviceContext->PSSetShader (nullptr, nullptr, 0);
}

bool Shader::SetShaderParameters (ID3D11DeviceContext *deviceContext, ID3D11ShaderResourceView *texture)
{
	deviceContext->PSSetShaderResources (0, 1, &texture);

	return true;
}

bool Shader::SetShaderParameters (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr = nullptr;

	// Transpose all matrices
	D3DXMatrixTranspose (&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose (&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose (&projectionMatrix, &projectionMatrix);

	// Lock buffer
	result = deviceContext->Map (m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CHECK_FAILED (result);

	dataPtr = (MatrixBufferType *) mappedResource.pData;		// Why??
	dataPtr->worldMatrix = worldMatrix;							// TODO
	dataPtr->viewMatrix = viewMatrix;							// NOT USE
	dataPtr->projectionMatrix = projectionMatrix;				// 

	// Unlock buffer
	deviceContext->Unmap (m_matrixBuffer, 0);

	// Update the values in the shader
	deviceContext->VSSetConstantBuffers (0, 1, &m_matrixBuffer);

	return true;
}

char *Shader::GetName ()
{
	return m_name;
}

bool Shader::IsInitialized ()
{
	return m_initialized;
}

bool Shader::Initialize (ID3D11Device *device, HWND hWnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	m_name = new char[strlen (shaderFileName) + 1];
	memcpy (m_name, shaderFileName, sizeof (shaderFileName) + 1);

	char psFileName[128] = "";
	sprintf_s (psFileName, "%s.ps", shaderFileName);

	char vsFileName[128] = "";
	sprintf_s (vsFileName, "%s.vs", shaderFileName);

	// Initialize the vertex and pixel shaders
	return InitializeShader (device, hWnd, vsFileName, psFileName, vertexFuncName, pixelFuncName);
}

bool Shader::InitializeShader (ID3D11Device *device, HWND hWnd, LPCSTR vsFileName, LPCSTR psFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	HRESULT result = S_OK;
	ID3D10Blob *errorMessage = nullptr;
	ID3D10Blob *vertexShaderBuffer = nullptr;
	ID3D10Blob *pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = {0};
	unsigned int numElements = 0;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Compile vertex shader
	result = D3DX11CompileFromFile (vsFileName, nullptr, nullptr, vertexFuncName, "vs_5_0",
									D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr,
									&vertexShaderBuffer, &errorMessage, nullptr);
	if (FAILED (result))
	{
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage (errorMessage, hWnd, vsFileName);
		}
		else
		{
			MessageBox (hWnd, vsFileName, "Error in VS Shader File", MB_OK);
		}

		RETURN_FALSE;
	}

	// Compile pixel shader
	result = D3DX11CompileFromFile (psFileName, nullptr, nullptr, pixelFuncName, "ps_5_0",
									D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr,
									&pixelShaderBuffer, &errorMessage, nullptr);
	if (FAILED (result))
	{
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage (errorMessage, hWnd, psFileName);
		}
		else
		{
			MessageBox (hWnd, psFileName, "Error in PS Shader File", MB_OK);
		}
	}

	// Create vertex shader buffer
	result = device->CreateVertexShader (vertexShaderBuffer->GetBufferPointer (),
										 vertexShaderBuffer->GetBufferSize (), nullptr,
										 &m_vertexShader);
	CHECK_FAILED (result);

	// Create pixel shader buffer
	result = device->CreatePixelShader (pixelShaderBuffer->GetBufferPointer (),
										pixelShaderBuffer->GetBufferSize (), nullptr,
										&m_pixelShader);
	CHECK_FAILED (result);

	// Setup the layout of the data that goes into the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof (polygonLayout) / sizeof (polygonLayout[0]);

	// Create the vertex input layout
	result = device->CreateInputLayout (polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer (), // 
										vertexShaderBuffer->GetBufferSize (), &m_layout);
	CHECK_FAILED (result);

	// Release shader
	RELEASE (vertexShaderBuffer);
	RELEASE (pixelShaderBuffer);

	// Setup the matrix biffer description
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof (MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer
	result = device->CreateBuffer (&matrixBufferDesc, nullptr, &m_matrixBuffer);
	CHECK_FAILED (result);

	return true;
}

void Shader::OutputShaderErrorMessage (ID3D10Blob *errorMessage, HWND hWnd, LPCSTR shaderFileName)
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
