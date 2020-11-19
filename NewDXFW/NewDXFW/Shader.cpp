#include <iostream>
#include <d3dcompiler.h>

#include "Shader.h"
#include "DebugFunc.h"

Shader::Shader (VertexShader *vertexShader, PixelShader *pixelShader) :
	m_vertexShader (vertexShader),
	m_pixelShader  (pixelShader)
{}

void Shader::Render (ID3D11DeviceContext *deviceContext)
{
	deviceContext->IASetInputLayout (m_vertexShader->m_inputLayout);
	deviceContext->VSSetShader		(m_vertexShader->m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader		(m_pixelShader->m_pixelShader,   nullptr, 0);

}

void Shader::RemShader (ID3D11DeviceContext *deviceContext)
{
	deviceContext->IASetInputLayout (nullptr);
	deviceContext->VSSetShader (nullptr, nullptr, 0);
	deviceContext->PSSetShader (nullptr, nullptr, 0);
}

bool Shader::IsInitialized ()
{
	if (m_vertexShader == nullptr || m_vertexShader->GetVertexShader () == nullptr ||
		m_pixelShader  == nullptr || m_pixelShader->GetPixelShader ()   == nullptr)
	{
		return false;
	}

	return true;
}