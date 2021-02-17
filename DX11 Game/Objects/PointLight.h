#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

class Camera;
#include "Light.h"
#include "GraphicsResource.h"

class PointLight : public Light, public GraphicsResource
{
public:
	bool Initialize( GraphicsContainer& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_point>& cb_ps_point );
	void SpawnControlWindow();
	inline const XMFLOAT3 GetLightPosition() const noexcept { return position; };
private:
	BOOL enable = TRUE;
	float constant = 1.0f;
	float linear = 0.045f;
	float quadratic = 0.0075f;

	float ambientStrength = 0.1f;
	float diffuseStrength = 0.6f;
	float specularPower = 10.0f;
	float specularStrength = 0.6f;

	XMFLOAT3 position = { -5.0f, 9.0f, -10.0f };
	XMFLOAT3 ambientColor = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 diffuseColor = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 specularColor = { 1.0f, 1.0f, 1.0f };
};

#endif