#include <iostream>
#include <d3dcompiler.h>

#include "Shader.h"
#include "DebugFuncs.h"


Shader::Shader (VertexShader *vertexShader, PixelShader *pixelShader) :
	m_vertexShader (vertexShader),
	m_pixelShader (pixelShader)
{}

void Shader::SetShader (ID3D11DeviceContext *deviceContext)
{
	deviceContext->VSSetShader (m_vertexShader->m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader (m_pixelShader->m_pixelShader,   nullptr, 0);
}

void Shader::RemShader (ID3D11DeviceContext *deviceContext)
{
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
