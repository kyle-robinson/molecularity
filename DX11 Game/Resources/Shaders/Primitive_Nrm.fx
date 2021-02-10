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
cbuffer PointLightBuffer : register( b1 )
{
    float3 ambientLightColor; // point light
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float dynamicLightStrength;
    
    float3 specularLightColor;
    float specularLightIntensity;
    
    float specularLightPower;
    float3 dynamicLightPosition;
    
    float lightConstant; // attenuation
    float lightLinear;
    float lightQuadratic;
    float useTexture; // miscellaneous
    
    float alphaFactor;
};

cbuffer DirectionalLightBuffer : register( b2 )
{
    float directionalLightStrength;
    float3 directionalLightPosition;
    
    float3 directionalLightColor;
}

cbuffer SpotLightBuffer : register( b3 )
{
    float innerCutoff;
    float3 spotLightPosition;
    
    float outerCutoff;
    float3 spotLightColor;
    
    float3 spotLightDirection;
}

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
    float3 cumulativeColor = { 0.0f, 0.0f, 0.0f };
    
    // DIRECTIONAL LIGHT
    {
        // diffuse calculations
        const float3 toLight = normalize( directionalLightPosition - input.inWorldPos );
        const float distanceToLight = length( toLight );
        const float3 directionToLight = toLight / distanceToLight;
        const float NDotL = dot( directionToLight, input.inNormal );
        const float3 diffuse = directionalLightColor * saturate( NDotL ) * directionalLightStrength;
        
        // specular calculations
        const float3 incidence = input.inNormal * dot( toLight, input.inNormal );
        const float3 reflection = incidence * 2.0f - toLight;
        const float3 specular = specularLightColor * specularLightIntensity *
            pow( max( 0.0f, dot( normalize( -reflection ), normalize( input.inWorldPos ) ) ), specularLightPower );
        
        cumulativeColor += diffuse + specular;
    }
    
    // POINT LIGHT
    {
        // vector calculations
        const float3 vToL = normalize( dynamicLightPosition - input.inWorldPos );
        const float attenuation = 1 / ( lightConstant + lightLinear * vToL + lightQuadratic * pow( vToL, 2 ) );
        
        // ambient calculations
        const float3 ambient = ambientLightColor * ambientLightStrength;

        // diffuse calculations
        const float3 diffuseIntensity = max( dot( vToL, input.inNormal ), 0 ) * attenuation;
        const float3 diffuse = diffuseIntensity * dynamicLightStrength * dynamicLightColor;

        // specular calculations
        const float3 incidence = input.inNormal * dot( vToL, input.inNormal );
        const float3 reflection = incidence * 2.0f - vToL;
        const float3 specular = specularLightColor * specularLightIntensity * attenuation *
            pow( max( 0.0f, dot( normalize( -reflection ), normalize( input.inWorldPos ) ) ), specularLightPower );
        
        cumulativeColor += ambient + diffuse + specular;
    }
    
    // SPOT LIGHT
    {
        // diffuse calculations
        float vToL = normalize( spotLightPosition - input.inWorldPos );
        float3 diffuse = max( dot( vToL, input.inNormal ), 0.0f ) * spotLightColor;

        // specular calculations
        float3 reflectDirection = reflect( -vToL, input.inNormal );
        const float3 specular = pow( max( 0.0f, dot( spotLightDirection, reflectDirection ) ), specularLightPower );

        // attenuation calculations
        const float spotLightDistance = distance( spotLightPosition, input.inWorldPos );
        const float attenuation = 1 / ( lightConstant + lightLinear * vToL + lightQuadratic * pow( spotLightDistance, 2 ) );
        
        // cutoff calculations
        const float lightAngle = ( acos( dot( -vToL, spotLightDirection ) ) * 180.0f ) / 3.141592f;
        const float lightCutoffAmount = 1.0f - smoothstep( innerCutoff, outerCutoff, lightAngle );
        
        cumulativeColor += ( diffuse + specular ) * attenuation * lightCutoffAmount;
    }

    // output colour
    float3 finalColor = saturate( cumulativeColor );
    finalColor *= ( useTexture == 1.0f ? objTexture.Sample( samplerState, input.inTexCoord ) : 1.0f );
    return float4( finalColor, alphaFactor );
}