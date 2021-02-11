#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
class Camera;

class SpotLight : public Light
{
public:
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_spot>& cb_ps_spot, std::unique_ptr<Camera>& camera );
	void SpawnControlWindow();
private:
	float cone = 20.0f;
	float range = 1000.0f;
	float strength = 1.0f;
	DirectX::XMFLOAT3 color = { 1.0f, 0.2f, 1.0f };
	DirectX::XMFLOAT3 direction = { 0.0f, 0.0f, 1.0f };
};

#endif