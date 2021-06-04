#pragma once
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

class Camera;
#include "Light.h"
#include "GraphicsResource.h"

/// <summary>
/// Creates a simple directional light.
/// Provides functions to automatically update constant buffer and spawn ImGui control window.
/// </summary>
class DirectionalLight : public Light, public GraphicsResource
{
public:
	bool Initialize( Graphics& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
	void UpdateConstantBuffer( Graphics& gfx );
	void SpawnControlWindow();
	inline const XMFLOAT3 GetLightPosition() const noexcept { return position; };
private:
	BOOL enable = TRUE;
	float diffuseStrength = 0.2f;
	float specularPower = 10.0f;
	float specularStrength = 0.2f;

	XMFLOAT3 position = { 10.0f, 20.0f, 10.0f };
	XMFLOAT3 diffuseColor = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 specularColor = { 1.0f, 1.0f, 1.0f };

	ConstantBuffer<CB_PS_directional> cb_ps_directional;
};

#endif