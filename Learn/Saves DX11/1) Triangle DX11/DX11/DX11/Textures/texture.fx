
cbuffer ConstantBuffer : register (b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_OUTPUT
{
    float4 pos   : SV_POSITION;
    float4 color : COLOR0;
};


VS_OUTPUT VS (float4 pos : POSITION, float4 color : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.pos = mul (pos,  World);
    output.pos = mul (output.pos, View);
    output.pos = mul (output.pos, Projection);
    
    output.color = color;

    return output;
}

float4 PS (VS_OUTPUT input) : SV_Target
{
    return input.color;
}

/*
float4 PS (float4 pos : POSITION) : SV_Target
{
    float coef = 10;
    pos *= coef * sqrt (abs (pos)) * 20;
    return float4 (sin (pos.x) * sin (pos.y),
                   cos (pos.x) * cos (pos.y),
                   sin (pos.x) * cos (pos.y), 1.0f);
}
*/