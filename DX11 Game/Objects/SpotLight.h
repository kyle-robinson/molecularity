#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

class Camera;
#include "Light.h"
#include "GraphicsResource.h"

/// <summary>
/// Creates a simple spot light.
/// Provides functions to automatically update constant buffer and spawn ImGui control window.
/// </summary>
class SpotLight : public Light, public GraphicsResource, public Listener
{
public:
	SpotLight();
	~SpotLight();

	bool Initialize( Graphics& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
	void UpdateConstantBuffer( Graphics& gfx, std::unique_ptr<Camera>& camera );
	void UpdateModelPosition( std::unique_ptr<Camera>& camera );
	void SpawnControlWindow();
	void HandleEvent( Event* event );
private:
	BOOL enable = TRUE;
	float cone = 20.0f;
	float range = 1000.0f;
	float strength = 0.6f;

	XMFLOAT3 color = { 0.45f, 0.72f, 1.0f };
	XMFLOAT3 direction = { 0.0f, 0.0f, 1.0f };

	std::vector<Model> gunModels;
	ConstantBuffer<CB_PS_spot> cb_ps_spot;
};

#endif