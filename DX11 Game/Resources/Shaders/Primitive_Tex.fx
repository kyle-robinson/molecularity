#pragma pack_matrix( row_major )

// Vertex Shader
cbuffer ConstantBuffer : register( b0 )
{
	float4x4 worldMatrix;
}

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float2 outTex : TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input )
{    
    VS_OUTPUT output;
    output.outPos = mul( float4( input.inPos, 1.0f ), worldMatrix );
    output.outTex = input.inTex;
    return output;
}

// Pixel Shader
struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float2 inTex : TEXCOORD;
};

Texture2D diffuseTexture : DIFFUSE_TEXTURE : register( t0 );
SamplerState samplerState : SAMPLER : register( s0 );

float4 PS( PS_INPUT input ) : SV_TARGET
{
    float3 textureSample = diffuseTexture.Sample( samplerState, input.inTex );
    return float4( saturate( textureSample ), 1.0f );
}