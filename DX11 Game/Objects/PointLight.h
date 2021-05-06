#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

class Camera;
#include "Light.h"
#include "GraphicsResource.h"

/// <summary>
/// Creates a dynamic point light.
/// Provides functions to automatically update constant buffer and spawn ImGui control window.
/// </summary>
class PointLight : public Light, public GraphicsResource
{
public:
	bool Initialize( Graphics& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
	void UpdateConstantBuffer( Graphics& gfx );
	void SpawnControlWindow();
	inline const XMFLOAT3 GetLightPosition() const noexcept { return position; }
	inline ConstantBuffer<CB_PS_point>& const GetConstantBuffer() noexcept { return cb_ps_point; }
private:
	BOOL enable = TRUE;
	float constant = 1.0f;
	float linear = 0.045f;
	float quadratic = 0.0075f;

	float ambientStrength = 0.1f;
	float diffuseStrength = 0.2f;
	float specularPower = 10.0f;
	float specularStrength = 0.2f;

	XMFLOAT3 position = { -5.0f, 9.0f, -10.0f };
	XMFLOAT3 ambientColor = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 diffuseColor = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 specularColor = { 1.0f, 1.0f, 1.0f };

	ConstantBuffer<CB_PS_point> cb_ps_point;
};

#endif