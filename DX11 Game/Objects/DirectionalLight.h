#pragma once
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"
class Camera;

class DirectionalLight : public Light
{
public:
	inline const DirectX::XMFLOAT3& const GetLightPosition() const noexcept { return directionalLightPosition; };
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_directional>& cb_ps_directional );
	void SpawnControlWindow();
private:
	DirectX::XMFLOAT3 directionalLightPosition = { 10.0f, 20.0f, 10.0f };
	DirectX::XMFLOAT3 directionalLightColor = { 0.4f, 1.0f, 0.1f };
	float directionalLightStrength = 1.0f;
};

#endif