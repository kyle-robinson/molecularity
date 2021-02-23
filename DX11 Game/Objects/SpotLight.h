#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

/// <summary>
/// Creates a simple spot light.
/// Provides functions to automatically update constant buffer and spawn ImGui control window.
/// </summary>

class Camera;
#include "Light.h"
#include "GraphicsResource.h"

class SpotLight : public Light, public GraphicsResource
{
public:
	bool Initialize( GraphicsContainer& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
	void UpdateConstantBuffer( GraphicsContainer& gfx, std::unique_ptr<Camera>& camera );
	void UpdateModelPosition( std::unique_ptr<Camera>& camera );
	void SpawnControlWindow();
private:
	BOOL enable = TRUE;
	float cone = 20.0f;
	float range = 1000.0f;
	float strength = 1.0f;

	XMFLOAT3 color = { 1.0f, 0.2f, 1.0f };
	XMFLOAT3 direction = { 0.0f, 0.0f, 1.0f };

	ConstantBuffer<CB_PS_spot> cb_ps_spot;
};

#endif