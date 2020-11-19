Texture2D txDiffuse : register (t0);        // Буфер текстуры
SamplerState samLinear : register (s0);     // Буфер образца
// Буфер с информацией о матрицах
cbuffer ConstantBufferMatrixes : register (b0)
{
    matrix World;            // Матрица мира
    matrix View;             // Матрица вида
    matrix Projection;       // Матрица проекции
}

//--------------------------------------------------------------------------------------

struct VS_INPUT                   // Входящие данные вершинного шейдера
{
    float4 Pos : POSITION;        // Позиция по X, Y, Z
    float2 Tex : TEXCOORD0;       // Координаты текстуры по tu, tv
};

struct PS_INPUT                   // Входящие данные пиксельного шейдера
{
    float4 Pos : SV_POSITION;     // Позиция пикселя в проекции (экранная)
    float2 Tex : TEXCOORD0;       // Координаты текстуры по tu, tv
};

PS_INPUT VS (VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    output.Pos = mul (input.Pos,  World);
    output.Pos = mul (output.Pos, View);
    output.Pos = mul (output.Pos, Projection);

    output.Tex = input.Tex;

    return output;
}

float4 PS (PS_INPUT input) : SV_Target
{
    float4 finalColor = txDiffuse.Sample (samLinear, input.Tex);
    finalColor.a = 1.0f;   

    return finalColor;
}

/*Texture2D txDiffuse : register (t0);  // Буфер текстуры
SamplerState samLinear : register (s0); // Буфер образца

cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_INPUT                   // Входящие данные вершинного шейдера
{
    float4 Pos : POSITION;        // Позиция по X, Y, Z
    float2 Tex : TEXCOORD0;       // Координаты текстуры по tu, tv
};

struct PS_INPUT                   // Входящие данные пиксельного шейдера
{
    float4 Pos : SV_POSITION;     // Позиция пикселя в проекции (экранная)
    float2 Tex : TEXCOORD0;       // Координаты текстуры по tu, tv
};

PS_INPUT VS (VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = input.Pos;
    //output.Pos = mul (input.Pos, World);
    //output.Pos = mul (output.Pos, View);
    //output.Pos = mul (output.Pos, Projection);

    output.Tex = input.Tex;   

    return output;

}

float4 PS (PS_INPUT input)
{ 
    float4 finalColor = txDiffuse.Sample (samLinear, input.Tex);
    finalColor.a = 1.0f;

    return finalColor;
}*/