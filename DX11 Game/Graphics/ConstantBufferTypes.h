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

struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength;
	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicLightStrength;
	DirectX::XMFLOAT3 specularLightColor;
	float specularLightStrength;
	float specularLightPower;
	DirectX::XMFLOAT3 dynamicLightPosition;
	float lightConstant;
	float lightLinear;
	float lightQuadratic;
	bool useTexture;
	float alphaFactor;
};

struct CB_PS_outline
{
	alignas(16) DirectX::XMFLOAT3 outlineColor;
};

#endif