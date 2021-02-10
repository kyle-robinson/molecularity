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
	DirectX::XMFLOAT3 pointAmbientColor; // point light
	float pointAmbientStrength;
	
	DirectX::XMFLOAT3 pointDiffuseColor;
	float pointDiffuseStrength;
	
	DirectX::XMFLOAT3 pointSpecularColor;
	float pointSpecularStrength;
	
	float pointSpecularPower;
	DirectX::XMFLOAT3 pointPosition;

	float lightConstant; // attenuation
	float lightLinear;
	float lightQuadratic;
	float useTexture; // miscellaneous

	float alphaFactor;
};

struct CB_PS_directional
{
	float directionalStrength;
	DirectX::XMFLOAT3 directionalPosition;

	DirectX::XMFLOAT3 directionalColor;
};

struct CB_PS_spot
{
	float innerCutoff;
	DirectX::XMFLOAT3 spotPosition;

	float outerCutoff;
	DirectX::XMFLOAT3 spotColor;

	DirectX::XMFLOAT3 spotDirection;
};

struct CB_PS_outline
{
	DirectX::XMFLOAT3 outlineColor;
};

#endif