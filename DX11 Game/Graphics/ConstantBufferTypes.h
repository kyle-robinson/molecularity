#pragma once
#ifndef CONSTANTBUFFERTYPES_H
#define CONSTANTBUFFERTYPES_H

#include <DirectXMath.h>

struct CB_VS_matrix
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

struct CB_PS_point
{
	DirectX::XMFLOAT3 ambientLightColor; // dynamic point light
	float ambientLightStrength;
	
	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicLightStrength;
	
	DirectX::XMFLOAT3 specularLightColor;
	float specularLightStrength;
	
	float specularLightPower;
	DirectX::XMFLOAT3 dynamicLightPosition;
	
	/*float directionalLightStrength; // directional light
	DirectX::XMFLOAT3 directionalLightPosition;

	DirectX::XMFLOAT3 directionalLightColor;*/
	float lightConstant; // attenuation

	float lightLinear;
	float lightQuadratic;
	float useTexture; // miscellaneous
	float alphaFactor;
};

struct CB_PS_directional
{
	float directionalLightStrength;
	DirectX::XMFLOAT3 directionalLightPosition;

	DirectX::XMFLOAT3 directionalLightColor;
};

struct CB_PS_attenuation
{
	float lightConstant;
	float lightLinear;
	float lightQuadratic;
};

struct CB_PS_misc
{
	float useTexture;
	float alphaFactor;
};

struct CB_PS_outline
{
	DirectX::XMFLOAT3 outlineColor;
};

#endif