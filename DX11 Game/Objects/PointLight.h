#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"
class Camera;

class PointLight : public Light
{
public:
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_point>& cb_ps_point, std::unique_ptr<Camera>& camera );
	void SpawnControlWindow();
private:
	DirectX::XMFLOAT3 lightPosition = { -5.0f, 9.0f, -10.0f };

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
};

#endif