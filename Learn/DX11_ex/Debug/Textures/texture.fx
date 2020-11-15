Texture2D txDiffuse : register (t0);
SamplerState samLinear : register (s0);

cbuffer ConstantBufferMatrixes : register (b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}
 
cbuffer ConstantBufferLight : register (b1)
{
    float4 vLightDir[2];
    float4 vLightColor[2];
    float4 vOutputColor;
}

struct VS_INPUT              
{
    float4 pos  : POSITION;   
    float2 tex  : TEXCOORD0;  
    float3 norm : NORMAL;    
};
 
struct PS_INPUT              
{
    float4 pos  : SV_POSITION;
    float2 tex  : TEXCOORD0;  
    float3 norm : TEXCOORD1; 
};

PS_INPUT VS (VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.pos = mul (input.pos,  World);
    output.pos = mul (output.pos, View);
    output.pos = mul (output.pos, Projection );
    output.norm = mul (input.norm, World);
    output.tex = input.tex;   

    return output;
}

float4 PS (PS_INPUT input) : SV_Target
{
    float4 finalColor = 0;   

    for (int i = 0; i < 2; i++)
    {
        finalColor += saturate (dot ((float3) vLightDir[i], input.norm) * 
                                vLightColor[i]);
    }

    finalColor *= txDiffuse.Sample (samLinear, input.tex) * 2;
    finalColor.a = 1.0f;

    return finalColor;

}

float4 PSSolid( PS_INPUT input) : SV_Target
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