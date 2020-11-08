cbuffer ConstantBuffer : register (b0)
{
    matrix World;
    matrix View;
    matrix Projection;

    float4 vLightDir[2];
    float4 vLightColor[2];
    float4 vOutputColor;
}

struct VS_INPUT
{
    float4 pos   : POSITION;
    float3 norm  : NORMAL;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos   : SV_POSITION;
    float3 norm  : TEXCOORD0;
    float4 color : COLOR0;
};

PS_INPUT VS (VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;

    output.pos = mul (input.pos,  World);
    output.pos = mul (output.pos, View);
    output.pos = mul (output.pos, Projection);
    
    output.color = input.color;
    output.norm = mul (input.norm, World);

    return output;
}

float4 PS (PS_INPUT input) : SV_Target
{
    float4 finalColor = input.color;
    for (int i = 0; i < 2; i++)
    {
        finalColor = saturate (finalColor + dot ((float3) vLightDir, input.norm) 
                                * vLightColor[i] * 0.4);
    }

    finalColor.a = 1;

    return finalColor;
}

float4 PSSolid (PS_INPUT input) : SV_Target
{
    return vOutputColor;
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