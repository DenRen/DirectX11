#include "Camera.h"
#include "DebugFunc.h"
#include <stdexcept>

bool CreateConstatntBufferMatrixes (ID3D11Device *device, ID3D11Buffer **CBMatrixes)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof (WVPMatrixes);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    HRESULT result = device->CreateBuffer (&bd, nullptr, CBMatrixes);

    if (FAILED (result))
    {   
        RETURN_FALSE;
    }

    return true;
}

Camera::Camera (XMMATRIX view, XMMATRIX proj, WVPMatrixes *WVPMatrixes, ID3D11Buffer *CBMatrixes) :
    m_view (view),
    m_proj (proj),
    m_WVPMatrixes (WVPMatrixes),
    m_CBWVPMatrixes (CBMatrixes)
{}

void Camera::Render (ID3D11DeviceContext *deviceContext)
{
    m_WVPMatrixes->m_View = m_view;
    m_WVPMatrixes->m_Projection = m_proj;

    m_WVPMatrixes->UpdateSubresource (deviceContext, m_CBWVPMatrixes);
    
    //deviceContext->UpdateSubresource (m_CBWVPMatrixes, 0, nullptr, m_WVPMatrixes, 0, 0);
    // deviceContext->VSSetConstantBuffers (0, 1, &m_CBWVPMatrixes);
    // deviceContext->PSSetConstantBuffers (0, 1, &m_CBWVPMatrixes);
}