#pragma once
#ifndef CONSTANTBUFFERTYPES_H
#define CONSTANTBUFFERTYPES_H

#include <DirectXMath.h>

struct CB_VS_matrix
{
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
};

struct CB_VS_matrix_2D
{
	XMMATRIX wvpMatrix;
};

struct CB_PS_outline
{
	XMFLOAT3 outlineColor;
};

struct CB_PS_scene
{
	float useTexture;
	float alphaFactor;
	float useNormalMap;
};

struct CB_PS_point
{
	XMFLOAT3 pointAmbientColor;
	float pointAmbientStrength;
	
	XMFLOAT3 pointDiffuseColor;
	float pointDiffuseStrength;
	
	XMFLOAT3 pointSpecularColor;
	float pointSpecularStrength;
	
	float pointSpecularPower;
	XMFLOAT3 pointPosition;

	float pointConstant;
	float pointLinear;
	float pointQuadratic;
	float pointEnable;
};

struct CB_PS_directional
{
	XMFLOAT3 directionalPosition;
	float directionalDiffuseStrength;

	XMFLOAT3 directionalDiffuseColor;
	float directionalSpecularStrength;

	XMFLOAT3 directionalSpecularColor;
	float directionalSpecularPower;

	float directionalEnable;
};

struct CB_PS_spot
{
	float spotRange;
	XMFLOAT3 spotPosition;

	float spotCone;
	XMFLOAT3 spotDirection;

	float spotDiffuseStrength;
	XMFLOAT3 spotDiffuseColor;

	float spotEnable;
};

#endif