#pragma once

#include "VertexBuffer.h"
#include "Texture.h"
#include "DXManager.h"
#include "DebugFunc.h"

struct ConstantBufferMatrixes
{
	ConstantBufferMatrixes (XMMATRIX world, XMMATRIX view, XMMATRIX projection);

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Projection;
};

template <typename VertexT, typename IndexT>
class Sprite
{
public:
	Sprite ();
	Sprite (Shader *shader, Texture *texture,
			ID3D11Buffer *CBMatrix, XMMATRIX World = XMMatrixIdentity ());
	virtual ~Sprite ();

	bool Initialize (Shader *shader, Texture *texture);

	void Render (ID3D11DeviceContext *deviceContext);	// Automaticly draw

	void Move (float x, float y, float z);
	void RotateX (float angle);
	void RotateY (float angle);
	void RotateZ (float angle);
	void ScaleUp (float ScaleUpX, float ScaleUpY, float ScaleUpZ);

protected:
	VertexBuffer <VertexT, IndexT> m_vertexBuffer;
	Texture *m_texture;
	Shader  *m_shader;

	XMMATRIX m_worldMatrix;
	ID3D11Buffer *m_CBMatrixes;

private:
	void SetCBMatrix ();
};

template <typename VertexT, typename IndexT>
Sprite <VertexT, IndexT>::Sprite () :
	Sprite (nullptr, nullptr, nullptr)
{}

template<typename VertexT, typename IndexT>
inline Sprite<VertexT, IndexT>::Sprite (Shader *shader, Texture *texture,
										ID3D11Buffer *CBMatrix, XMMATRIX World) :
	m_shader (shader),
	m_texture (texture),
	m_CBMatrixes (CBMatrix),
	m_worldMatrix (World)
{}

template <typename VertexT, typename IndexT>
Sprite <VertexT, IndexT>::~Sprite ()
{

}

template <typename VertexT, typename IndexT>
bool Sprite <VertexT, IndexT>::Initialize (Shader *shader, Texture *texture)
{
	CHECK_NULLPTR (shader);
	CHECK_NULLPTR (texture);

	m_shader = shader;
	m_texture = texture;

	return true;
}

template <typename VertexT, typename IndexT>
void Sprite <VertexT, IndexT>::Render (ID3D11DeviceContext *deviceContext)
{
	m_texture->Render (deviceContext);
	m_shader->Render (deviceContext);
	m_vertexBuffer.Render (deviceContext);

	//SetCBMatrix ();
}

template <typename VertexT, typename IndexT>
void Sprite <VertexT, IndexT>::SetCBMatrix ()
{
	ID3D11DeviceContext *deviceContext = DXManager::GetDeviceContext ();

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT result = deviceContext->Map (m_CBMatrixes, 0, D3D11_MAP_WRITE_DISCARD,
										 0, &mappedResource);
	if (FAILED (result))
	{
		DUMP_DEBUG_INFO;
		DebugEndMain ();
		throw std::runtime_error ("");
	}

	auto dataPtr = (ConstantBufferMatrixes *) mappedResource.pData;
	dataPtr->m_World = m_worldMatrix;

	deviceContext->Unmap (m_CBMatrixes, 0);

	deviceContext->VSSetConstantBuffers (0, 1, &m_CBMatrixes);
}

template <typename VertexT, typename IndexT>
void Sprite <VertexT, IndexT>::Move (float x, float y, float z)
{
	m_worldMatrix *= XMMatrixTranslation (x, y, z);
}

template<typename VertexT, typename IndexT>
inline void Sprite<VertexT, IndexT>::RotateX (float angle)
{
	m_worldMatrix *= XMMatrixRotationX (angle);
}

template<typename VertexT, typename IndexT>
inline void Sprite<VertexT, IndexT>::RotateY (float angle)
{
	m_worldMatrix *= XMMatrixRotationY (angle);
}

template<typename VertexT, typename IndexT>
inline void Sprite<VertexT, IndexT>::RotateZ (float angle)
{
	m_worldMatrix *= XMMatrixRotationZ (angle);
}

template<typename VertexT, typename IndexT>
inline void Sprite <VertexT, IndexT>::ScaleUp (float ScaleUpX, float ScaleUpY, float ScaleUpZ)
{
	m_worldMatrix *= XMMatrixScaling (ScaleUpX, ScaleUpY, ScaleUpZ);
}
