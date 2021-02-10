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
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 color = { 0.4f, 1.0f, 0.1f };
	float strength = 1.0f;
};

#endif