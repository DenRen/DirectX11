
Texture2D txDiffuse : register (t0);
SamplerState samLinear : register (s0);

cbuffer ConstantBufferMatrixes : register (b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_INPUT              
{
    float4 pos : POSITION;   
    float2 tex : TEXCOORD0; 
};
 
struct PS_INPUT              
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PS_INPUT VS (VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    output.pos = mul (input.pos,  World);
    output.pos = mul (output.pos, View);
    output.pos = mul (output.pos, Projection);
    
    output.tex = input.tex;   

    return output;
}

float4 PS (PS_INPUT input) : SV_Target
{
    float4 finalColor = txDiffuse.Sample (samLinear, input.tex) * 2;
    finalColor.a = 1.0f;

    return float4 (1,1,1,1);
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