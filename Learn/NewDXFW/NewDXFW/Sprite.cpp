#include "Sprite.h"

ConstantBufferMatrixes::ConstantBufferMatrixes (XMMATRIX world, XMMATRIX view, XMMATRIX projection) :
    m_World (world),
    m_View (view),
    m_Projection (projection)
{}