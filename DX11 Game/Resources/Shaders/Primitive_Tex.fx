#pragma pack_matrix( row_major )

// VERTEX SHADER
cbuffer ObjectBuffer : register( b0 )
{
	float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
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
    output.outPos = mul( output.outPos, viewMatrix );
    output.outPos = mul( output.outPos, projectionMatrix );
    output.outTex = input.inTex;
    return output;
}

// PIXEL SHADER
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