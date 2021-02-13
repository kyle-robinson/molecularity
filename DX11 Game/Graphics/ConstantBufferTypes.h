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

struct CB_VS_matrix_2D
{
	DirectX::XMMATRIX wvpMatrix;
};

struct CB_PS_outline
{
	DirectX::XMFLOAT3 outlineColor;
};

struct CB_PS_scene
{
	float useTexture;
	float alphaFactor;
};

struct CB_PS_point
{
	DirectX::XMFLOAT3 pointAmbientColor;
	float pointAmbientStrength;
	
	DirectX::XMFLOAT3 pointDiffuseColor;
	float pointDiffuseStrength;
	
	DirectX::XMFLOAT3 pointSpecularColor;
	float pointSpecularStrength;
	
	float pointSpecularPower;
	DirectX::XMFLOAT3 pointPosition;

	float pointConstant;
	float pointLinear;
	float pointQuadratic;
	float pointEnable;
};

struct CB_PS_directional
{
	DirectX::XMFLOAT3 directionalPosition;
	float directionalDiffuseStrength;

	DirectX::XMFLOAT3 directionalDiffuseColor;
	float directionalSpecularStrength;

	DirectX::XMFLOAT3 directionalSpecularColor;
	float directionalSpecularPower;

	float directionalEnable;
};

struct CB_PS_spot
{
	float spotRange;
	DirectX::XMFLOAT3 spotPosition;

	float spotCone;
	DirectX::XMFLOAT3 spotDirection;

	float spotDiffuseStrength;
	DirectX::XMFLOAT3 spotDiffuseColor;

	float spotEnable;
};

#endif