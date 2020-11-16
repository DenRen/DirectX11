// Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// Typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex shader
PixelInputType TextureVertexShader (VertexInputType input)
{
	PixelInputType output;
	output.position = input.position;
/*
	// Change the position vector to be 4 units for proper matrix calculations
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view and projection matrix
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position, viewMatrix);
	output.position = mul (output.position, projectionMatrix);

	output.tex = input.tex;

	return output;*/
	return output;
}