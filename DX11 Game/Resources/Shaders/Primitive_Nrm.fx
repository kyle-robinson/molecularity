#pragma pack_matrix( row_major )

// vertex shader
cbuffer ObjectBuffer : register( b0 )
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POSITION;
};

VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;
    output.outPosition = mul( float4( input.inPosition, 1.0f ), worldMatrix );
    output.outPosition = mul( output.outPosition, viewMatrix );
    output.outPosition = mul( output.outPosition, projectionMatrix );
    output.outTexCoord = input.inTexCoord;
    output.outNormal = normalize( mul( float4( input.inNormal, 0.0f ), worldMatrix ) );
    output.outWorldPos = mul( float4( input.inPosition, 1.0f ), worldMatrix );
    return output;
}

// pixel shader
cbuffer LightBuffer : register( b1 )
{
    float3 ambientLightColor;
    float ambientLightStrength;
    float3 dynamicLightColor;
    float dynamicLightStrength;
    float3 specularLightColor;
    float specularLightIntensity;
    float specularLightPower;
    float3 dynamicLightPosition;
    float lightConstant;
    float lightLinear;
    float lightQuadratic;
    bool useTexture;
    float alphaFactor;
};

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register( t0 );
SamplerState samplerState : SAMPLER : register( s0 );

float4 PS( PS_INPUT input ) : SV_TARGET
{   
    float3 ambient = ambientLightColor * ambientLightStrength;
    float3 sampleColor = objTexture.Sample( samplerState, input.inTexCoord );
    
    float3 vToL = normalize( dynamicLightPosition - input.inWorldPos );
    float attenuation = 1 / ( lightConstant + lightLinear * vToL + lightQuadratic * pow( vToL, 2 ) );
    
    // diffuse calculations
    float3 diffuseIntensity = max( dot( vToL, input.inNormal ), 0 );
    diffuseIntensity *= attenuation;
    float3 diffuse = diffuseIntensity * dynamicLightStrength * dynamicLightColor;
    
    // specular calculations
    float3 incidence = input.inNormal * dot( vToL, input.inNormal );
    float3 reflection = incidence * 2.0f - vToL;
    float3 specular = specularLightColor * specularLightIntensity * attenuation *
        pow( max( 0.0f, dot( normalize( -reflection ), normalize( input.inWorldPos ) ) ), specularLightPower );
    
    float3 finalColor = saturate( ambient + diffuse + specular ) * ( sampleColor = ( useTexture == true ) ? sampleColor : 1 );
    return float4( finalColor, alphaFactor );
}