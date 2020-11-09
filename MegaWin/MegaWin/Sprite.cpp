#include <cassert>
#include "Sprite.h"
#include "DebugFuncs.h"
#include "ResourceManager.h"

Sprite::Sprite (float size) :
	m_size (size),
	m_vertexBuffer (nullptr),
	m_texture (nullptr),
	m_shader (nullptr)
{}

Sprite::~Sprite ()
{
	if (m_vertexBuffer != nullptr)
	{
		delete m_vertexBuffer;
		m_vertexBuffer = nullptr;
	}
}

bool Sprite::Initialize (ID3D11Device *device, Shader *shader, LPCTSTR textureFileName,
						 bool isWriteAble)
{
	assert (textureFileName != nullptr);

	m_shader = shader;

	m_vertexBuffer = new VertexBuffer ();
	m_vertexBuffer->Initialize (device, shader, m_size, isWriteAble);

	if (textureFileName != nullptr)
	{
		m_texture = ResourceManager::GetInstance ()->GetTextureByName (textureFileName);
	}
	else
	{
		RETURN_FALSE;
	}

	return true;
}

void Sprite::Render (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (m_texture != nullptr)
	{
		m_shader->SetShaderParameters (deviceContext, m_texture->GetTexture ());
		m_shader->SetShaderParameters (deviceContext, worldMatrix, viewMatrix, projectionMatrix);

		m_vertexBuffer->Render (deviceContext);
	}
}
