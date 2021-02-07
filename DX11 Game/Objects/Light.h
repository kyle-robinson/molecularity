#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "RenderableGameObject.h"

class Camera;
class Light : public RenderableGameObject
{
public:
	bool Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
		ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader );
	void SpawnControlWindow();
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_light>& cb_ps_light, std::unique_ptr<Camera>& camera );
private:
	// Ambient
	DirectX::XMFLOAT3 ambientColor = { 1.0f, 1.0f, 1.0f };
	float ambientStrength = 0.1f;

	// Diffuse
	DirectX::XMFLOAT3 lightColor = { 1.0f, 1.0f, 1.0f };
	float lightStrength = 1.0f;

	// Specular
	DirectX::XMFLOAT3 specularColor = { 1.0f, 1.0f, 1.0f };
	float specularStrength = 1.0f;
	float specularPower = 10.0f;

	// Attenuation
	float constant = 1.0f;
	float linear = 0.045f;
	float quadratic = 0.0075f;

	// Directional
	DirectX::XMFLOAT3 directionalLightPosition = { 10.0f, 20.0f, 10.0f };
	DirectX::XMFLOAT3 directionalLightColor = { 0.4f, 1.0f, 0.1f };
	float directionalLightStrength = 1.0f;
};

#endif