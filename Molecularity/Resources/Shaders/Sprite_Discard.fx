struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register( t0 );
SamplerState samplerState : SAMPLER : register( s0 );

void PS( PS_INPUT input )
{
    float4 sampleColor = objTexture.Sample( samplerState, input.inTexCoord );
    if ( sampleColor.a < 0.25 )
        discard;
}