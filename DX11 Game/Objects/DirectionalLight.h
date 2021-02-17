#pragma once
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

class Camera;
#include "Light.h"
#include "GraphicsResource.h"

class DirectionalLight : public Light, public GraphicsResource
{
public:
	bool Initialize( GraphicsContainer& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
	void UpdateConstantBuffer( GraphicsContainer& gfx );
	void SpawnControlWindow();
	inline const XMFLOAT3 GetLightPosition() const noexcept { return position; };
private:
	BOOL enable = TRUE;
	float diffuseStrength = 0.5f;
	float specularPower = 10.0f;
	float specularStrength = 0.3f;

	XMFLOAT3 position = { 10.0f, 20.0f, 10.0f };
	XMFLOAT3 diffuseColor = { 0.4f, 1.0f, 0.1f };
	XMFLOAT3 specularColor = { 0.4f, 1.0f, 0.1f };

	ConstantBuffer<CB_PS_directional> cb_ps_directional;
};

#endif