// Vertex Shader
cbuffer ViewBuffer : register( b0 )
{
    bool multiView;
};

struct VS_INPUT
{
    float2 inPos : POSITION;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float2 outTex : TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input )
{   
    VS_OUTPUT output;
    output.outPos = float4( input.inPos, 0.0f, 1.0f );
    
    if ( multiView )
    {
        output.outTex.x = ( input.inPos.x + 1 );
        output.outTex.y = -( input.inPos.y - 1 );
    }
    else
    {
        output.outTex = float2( ( input.inPos.x + 1 ) / 2.0f, -( input.inPos.y - 1 ) / 2.0f );
    }
    
    return output;
}

// Pixel Shader
struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float2 inTex : TEXCOORD;
};

Texture2D quadTexture : TEXTURE : register( t0 );
SamplerState samplerState : SAMPLER : register( s0 );

float4 PS( PS_INPUT input ) : SV_TARGET
{
    return saturate( quadTexture.Sample( samplerState, input.inTex ).rgba );
}