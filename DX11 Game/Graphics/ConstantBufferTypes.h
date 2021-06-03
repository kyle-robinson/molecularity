#pragma once
#ifndef CONSTANTBUFFERTYPES_H
#define CONSTANTBUFFERTYPES_H

/// <summary>
/// Contains a list of structs which can be templated on by a ConstantBuffer.
/// Must adhere to 16-byte alignment packing.
/// </summary>

#include <DirectXMath.h>
using namespace DirectX;

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

struct CB_VS_fog
{
	XMFLOAT3 fogColor;
	float fogStart;

	float fogEnd;
	BOOL fogEnable;
};

struct CB_VS_fullscreen
{
	BOOL multiView;
};

struct CB_PS_outline
{
	XMFLOAT3 outlineColor;
};

struct CB_PS_scene
{
	BOOL useTexture;
	float alphaFactor;
	BOOL useNormalMap;	
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
	BOOL pointEnable;
};

struct CB_PS_directional
{
	XMFLOAT3 directionalPosition;
	float directionalDiffuseStrength;

	XMFLOAT3 directionalDiffuseColor;
	float directionalSpecularStrength;

	XMFLOAT3 directionalSpecularColor;
	float directionalSpecularPower;

	BOOL directionalEnable;
};

struct CB_PS_spot
{
	float spotRange;
	XMFLOAT3 spotPosition;

	float spotCone;
	XMFLOAT3 spotDirection;

	float spotDiffuseStrength;
	XMFLOAT3 spotDiffuseColor;

	BOOL spotEnable;
};

#endif