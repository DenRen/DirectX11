#pragma once

#include <d3d11.h>
#include <xnamath.h>

struct ConstantBufferMatrixes
{
	ConstantBufferMatrixes (XMMATRIX world, XMMATRIX view, XMMATRIX projection);

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Projection;
};

class Camera
{
public:
	Camera (ID3D11Device *device);
	Camera (ID3D11Device *device, XMMATRIX world);
	Camera (ID3D11Device *device, XMMATRIX world, XMMATRIX view);
	Camera (ID3D11Device *device, XMMATRIX world, XMMATRIX view, XMMATRIX projection);

	XMMATRIX &GetWorld ();
	XMMATRIX &GetView ();
	XMMATRIX &GetProjection ();

	void Render (ID3D11DeviceContext *deviceContext);

	void SetWorld	   (XMMATRIX &world);
	void SetView	   (XMMATRIX &view);
	void SetProjection (XMMATRIX &projection);
	ID3D11Buffer *m_CBMatrixes;
private:
	ConstantBufferMatrixes m_matrix;
	
};

