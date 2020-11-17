#include "Structure_Data.h"

WVPMatrixes::WVPMatrixes (XMMATRIX world, XMMATRIX view, XMMATRIX projection) :
    m_World (world),
    m_View (view),
    m_Projection (projection)
{}

void WVPMatrixes::UpdateSubresource (ID3D11DeviceContext *deviceContext, ID3D11Buffer *CBWVPMatrixes)
{
    WVPMatrixes temp (XMMatrixTranspose (m_World),
                      XMMatrixTranspose (m_View),
                      XMMatrixTranspose (m_Projection));

    deviceContext->UpdateSubresource (CBWVPMatrixes, 0, nullptr, &temp, 0, 0);
}
