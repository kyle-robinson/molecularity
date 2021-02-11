#pragma once
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"
class Camera;

class DirectionalLight : public Light
{
public:
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_directional>& cb_ps_directional );
	void SpawnControlWindow();
private:
	float enable = true;
	float diffuseStrength = 0.5f;
	float specularPower = 10.0f;
	float specularStrength = 0.3f;

	DirectX::XMFLOAT3 position = { 10.0f, 20.0f, 10.0f };
	DirectX::XMFLOAT3 diffuseColor = { 0.4f, 1.0f, 0.1f };
	DirectX::XMFLOAT3 specularColor = { 0.4f, 1.0f, 0.1f };
};

#endif