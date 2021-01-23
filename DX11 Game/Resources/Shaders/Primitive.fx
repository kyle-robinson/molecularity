#pragma pack_matrix( row_major )

// Vertex Shader
cbuffer ConstantBuffer : register( b0 )
{
	float4x4 worldMatrix;
}

struct VS_INPUT
{
    float3 inPos : POSITION;
    float4 inCol : COLOR;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float4 outCol : COLOR;
};

VS_OUTPUT VS( VS_INPUT input )
{    
    VS_OUTPUT output;
    output.outPos = mul( float4( input.inPos, 1.0f ), worldMatrix );
    output.outCol = input.inCol;
    return output;
}

// Pixel Shader
struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float4 inCol : COLOR;
};

float4 PS( PS_INPUT input ) : SV_TARGET
{
    return input.inCol;
}