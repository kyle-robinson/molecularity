#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

class SpotLight : public Light
{
public:
	inline const DirectX::XMFLOAT3& const GetLightPosition() const noexcept { return spotLightPosition; };
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_spot>& cb_ps_spot );
	void SpawnControlWindow();
private:
	float innerCutoff = 1.0f;
	float outerCutoff = 2.0f;
	DirectX::XMFLOAT3 spotLightColor = { 1.0f, 0.5f, 1.0f };
	DirectX::XMFLOAT3 spotLightPosition = { -5.0f, 9.0f, -10.0f };
	DirectX::XMFLOAT3 spotLightDirection = { 1.0f, 0.0f, 1.0f };
};

#endif