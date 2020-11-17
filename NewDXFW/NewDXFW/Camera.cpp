#include "Camera.h"
#include "DebugFunc.h"
#include <stdexcept>

bool CreateConstatntBufferMatrixes (ID3D11Device *device, ID3D11Buffer **CBMatrixes)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof (ConstantBufferMatrixes);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT result = device->CreateBuffer (&bd, nullptr, CBMatrixes);

    if (FAILED (result))
    {   
        RETURN_FALSE;
    }

    return true;
}

Camera::Camera (XMMATRIX view, XMMATRIX projection, ID3D11Buffer *CBMatrixes) :
    m_view (view),
    m_proj (projection),
    m_CBMatrixes (CBMatrixes)
{}

void Camera::Render (ID3D11DeviceContext *deviceContext)
{

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    HRESULT result = S_OK;

    result = deviceContext->Map (m_CBMatrixes, 0, D3D11_MAP_WRITE_DISCARD, 0,
                                 &mappedResource);
    if (FAILED (result))
    {
        DUMP_DEBUG_INFO;
        DebugEndMain ();
        throw std::runtime_error ("");
    }

    auto data = (ConstantBufferMatrixes *)mappedResource.pData;
    data->m_View       = XMMatrixTranspose (m_view);
    data->m_Projection = XMMatrixTranspose (m_proj);
    
    deviceContext->Unmap (m_CBMatrixes, 0);

    //deviceContext->UpdateSubresource (m_CBMatrixes, 0, nullptr, &m_matrix, 0, 0);
    deviceContext->VSSetConstantBuffers (0, 1, &m_CBMatrixes);
    deviceContext->PSSetConstantBuffers (0, 1, &m_CBMatrixes);
}