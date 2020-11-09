#include "ResourceShader.h"
#include "DebugFuncs.h"

ResourceShader::ResourceShader () :
	m_shader (nullptr)
{}

ResourceShader::~ResourceShader ()
{
	if (m_shader != nullptr)
	{
		delete m_shader;
	}
}

bool ResourceShader::Load (ID3D11Device *device, HWND hWnd, const char *shaderFileName, const char *vertexFuncName, const char *pixelFuncName)
{
	if (m_shader != nullptr)
	{
		delete m_shader;
	}

	m_shader = new Shader (device, hWnd, shaderFileName, vertexFuncName, pixelFuncName);
	if (!m_shader->IsInitialized ())
	{
		delete m_shader;
		m_shader = nullptr;

		RETURN_FALSE;
	}

	return true;
}

bool ResourceShader::Load (Shader *shader)
{
	if (shader == nullptr)
	{
		return false;
	}

	if (!shader->IsInitialized ())
	{
		delete shader;
		m_shader = nullptr;

		return false;
	}

	m_shader = shader;

	return true;
}

Shader *ResourceShader::GetShader ()
{
	return m_shader;
}

std::string ResourceShader::GetName ()
{
	return m_shader->GetName ();
}
