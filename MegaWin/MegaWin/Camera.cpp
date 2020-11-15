#include "Camera.h"
#include <stdexcept>

ConstantBufferMatrixes::ConstantBufferMatrixes (XMMATRIX world, XMMATRIX view, XMMATRIX projection) :
    m_World (world),
    m_View (view),
    m_Projection (projection)
{}

Camera::Camera (ID3D11Device *device) :
    Camera (device, XMMatrixIdentity (), XMMatrixIdentity (), XMMatrixIdentity ())
{}

Camera::Camera (ID3D11Device *device, XMMATRIX world) :
    Camera (device, world, XMMatrixIdentity (), XMMatrixIdentity ())
{}

Camera::Camera (ID3D11Device *device, XMMATRIX world, XMMATRIX view) :
    Camera (device, world, view, XMMatrixIdentity ())
{}

Camera::Camera (ID3D11Device *device, XMMATRIX world, XMMATRIX view, XMMATRIX projection) :
    m_matrix (world, view, projection)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof (ConstantBufferMatrixes);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    HRESULT result = device->CreateBuffer (&bd, nullptr, &m_CBMatrixes);

    if (FAILED (result))
    {
        throw std::runtime_error ("");
    }
}

XMMATRIX &Camera::GetWorld ()
{
    return m_matrix.m_World;
}

XMMATRIX &Camera::GetView ()
{
    return m_matrix.m_View;
}

XMMATRIX &Camera::GetProjection ()
{
    return m_matrix.m_Projection;
}

void Camera::Render (ID3D11DeviceContext *deviceContext)
{
    deviceContext->UpdateSubresource (m_CBMatrixes, 0, nullptr, &m_matrix, 0, 0);
    deviceContext->VSSetConstantBuffers (0, 1, &m_CBMatrixes);
    deviceContext->PSSetConstantBuffers (0, 1, &m_CBMatrixes);
}

void Camera::SetWorld (XMMATRIX &world)
{
    m_matrix.m_World = world;
}

void Camera::SetView (XMMATRIX &view)
{
    m_matrix.m_View = view;
}

void Camera::SetProjection (XMMATRIX & projection)
{
    m_matrix.m_Projection = projection;
}